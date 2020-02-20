#include "viewport.h"
#include <QOpenGLContext>
#include <QtMath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QQuaternion>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <QFileDialog>
#include <QDir>

#include "camera.h"
#include "object.h"
#include "transform.h"


viewport::viewport(QWidget *parent) : QOpenGLWidget(parent)
{
    m_camera = new camera;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}

viewport::~viewport()
{
    delete m_camera;

    for (int i = 0; i < m_objects.size(); ++i)
        delete m_objects[i];

    delete m_object;

    for (int i = 0; i < m_TransformObjects.size(); ++i)
        delete m_TransformObjects[i];

    delete m_transform;
}

void viewport::initializeGL()
{
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    loadObj();
    m_TransformObjects.append(m_objects[m_objects.size() - 1]);
}

void viewport::resizeGL(int w, int h)
{
    float aspect = w / (h ? (float)h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.001f, 1000.0f);
    QMatrix4x4 testP = m_projectionMatrix;
    qDebug() << "ProjMM" << testP;
}

void viewport::paintGL()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     m_program.bind();
     m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
//     m_program.setUniformValue("u_ligthPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
//     m_program.setUniformValue("u_lightPower", 1.0f);
     m_camera->draw(&m_program);
     m_object->draw(&m_program, context()->functions());


//     for (int i = 0; i < m_TransformObjects.size(); i++) {
//         m_TransformObjects[i]->draw(&m_program, context()->functions());
//     }

     m_program.release();
}

void viewport::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){//только на левую кнопку мыши
        m_mousePosition = QVector2D(event->localPos());//Локальная позиция данного окна

        QVector4D tmp(2.0f * m_mousePosition.x() / width() - 1.0f, -2.0f * m_mousePosition.y() / height() + 1, -1.0f, 1.0f);
       // qDebug () << tmp;
        QVector4D iTmp((m_projectionMatrix.inverted() * tmp).toVector2D(), -1.0f, 0.0f);
       // qDebug () << iTmp;
        QVector3D Direction((m_camera->getViewMatrix().inverted() * iTmp).toVector3D().normalized());
        //qDebug () << Direction;
        QVector3D camPos((m_camera->getViewMatrix().inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f)).toVector3D());
        //qDebug () << camPos;
        //Ax + By + Cz + D = 0
        //norm(A, B, C)
        //D - расстояние до центра системы координат
        //P * N + d = 0 где d - расстояние d центра d = -P0 * N любая точка плоскоти умножается на нормаль
        //O + D*t
        //в тетрадь смотри ебать

        QVector3D N(0.0f, 0.0f, 1.0f);
        float t = -QVector3D::dotProduct(camPos, N) / QVector3D::dotProduct(Direction, N);
        QVector3D result = camPos + Direction * t;
        //qDebug () << result;

    }
//    if (event->buttons() == Qt::RightButton)
    event->accept();
}

void viewport::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;//Просчет разницы между текущим положением и старым(направление)
    m_mousePosition = QVector2D(event->localPos());//Текущее положение

    float angle = diff.length() / 2.0f; //Угол поворота

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0); //Нормаль к вектору направления

    //Итоговый поворот
    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

//    angleX = diff.y() / 2.0f;
//    angleY = diff.x() / 2.0f;

//    m_camera->rotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));
//    m_camera->rotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));

    update();
}

void viewport::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0){
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    } else if (event->delta() < 0){
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }
    update();
}

void viewport::loadObj()
{
    QStringList pathToFiles = QFileDialog::getOpenFileNames(nullptr, "Open Dialog", "", "*.obj");
    QString path;
    foreach(QString str, pathToFiles) path.append(str);
    QDir(path).absolutePath();

    obj.loadObj(path);

    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normal;
    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
    //qDebug() << "V";
    for (int i = 0; i < obj.list_vertices.size(); i++)
    {
        GLfloat xV = obj.list_vertices.at(i).x;
        GLfloat yV = obj.list_vertices.at(i).y;
        GLfloat zV = obj.list_vertices.at(i).z;
        //qDebug() << xV << yV << zV;
        coords.append(QVector3D(xV, yV, zV));
       // qDebug() << "Успех вершин";
    }
    //qDebug() << "VT";
    if (!obj.m_texture_coord)
    {
        for (int i = 0; i < obj.count; i++)
        {
            GLfloat uT = 0;
            GLfloat vT = 0;
            //qDebug() << "текстуры" << uT << vT;
            texCoords.append(QVector2D(0, 1));
        }
    } else if (obj.m_texture_coord){
        for (int i = 0; i < obj.list_texture.size(); i++)
        {
            GLfloat uT = obj.list_texture.at(i).u;
            GLfloat vT = obj.list_texture.at(i).v;
            //qDebug()<< uT << vT;
            texCoords.append(QVector2D(0, 1));
        }
    }
    //qDebug() << "старт заполнения нормалей";
    //qDebug() << "VN";
    if (!obj.m_normal)
    {
        for (int i = 0; i < obj.count; i++)
        {
            GLfloat xN = 0;
            GLfloat yN = 0;
            GLfloat zN = 0;
            //qDebug() << xN << yN << zN;
            normal.append(QVector3D(xN, yN, zN));
        }
    } else if (obj.m_normal)
    {
        for (int i = 0; i < obj.list_normal.size(); i++)
        {
            GLfloat xN = obj.list_normal.at(i).x;
            GLfloat yN = obj.list_normal.at(i).y;
            GLfloat zN = obj.list_normal.at(i).z;
            //qDebug() << "нормали" << xN << yN << zN;
            normal.append(QVector3D(xN, yN, zN));
        }
    }
    for (int i = 0; i < obj.face.size(); i++)
    {

        GLint cord1 = obj.face.at(i).id_vertices;
        GLint tcord1 = obj.face.at(i).id_textur_coordinat;
        GLint norma1 = obj.face.at(i).id_normal;
        qDebug() << cord1 + 1 << "/" << tcord1 + 1<< "/" << norma1 + 1;
        vertexes.append(VertexData(coords[cord1], texCoords[tcord1], normal[norma1]));
        int k = indexes.size();
        qDebug() << k;
        indexes.append(indexes.size());

//        GLint cord2 = obj.face.at(i + 1).id_vertices;
//        GLint tcord2 = obj.face.at(i + 1).id_textur_coordinat;
//        GLint norma2 = obj.face.at(i + 1).id_normal;

//        vertexes.append(VertexData(coords[cord2], texCoords[tcord2], normal[norma2]));
//        indexes.append(indexes.size());

//        GLint cord3 = obj.face.at(i + 2).id_vertices;
//        GLint tcord3 = obj.face.at(i + 2).id_textur_coordinat;
//        GLint norma3 = obj.face.at(i + 2).id_normal;

//        vertexes.append(VertexData(coords[cord3], texCoords[tcord3], normal[norma3]));
//        indexes.append(indexes.size());
    }

    m_objects.append(new Object(vertexes, indexes));
    m_object = new Object(vertexes, indexes);
}

void viewport::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertsh.vsh"))
        close();//Шейдер вершин

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragsh.fsh"))
        close();//Шейдер пиксельный

    if (!m_program.link()) //link - объединяет все шейдеры
        close();
//    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertPOINT.vsh"))
//        close();//Шейдер вершин

//    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragPOINT.fsh"))
//        close();//Шейдер пиксельный

//    if (!m_program.link()) //link - объединяет все шейдеры
//        close();
}

