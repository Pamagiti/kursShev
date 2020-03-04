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
    //selectPoint = points;
    //добавить selectpoint
}

void viewport::resizeGL(int w, int h)
{
    float aspect = w / (h ? (float)h : 1);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45, aspect, 0.001f, 1000.0f);
    //QMatrix4x4 testP = m_projectionMatrix;
    //qDebug() << "ProjMM" << testP;
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
        QMatrix4x4 test1 = m_projectionMatrix * m_camera->getViewMatrix() * m_object->getModalMatrix();
//        qDebug() << test;
//        QVector4D ee(obj.list_vertices.at(0).x, obj.list_vertices.at(0).y, obj.list_vertices.at(0).z, 1);
//        QVector3D test2 = (ee * test1.inverted()).toVector3D();
//        qDebug() << test2;
//        QVector3D test = screenCToWorldC(m_mousePosition, obj.list_vertices.at(0).x, obj.list_vertices.at(0).y, obj.list_vertices.at(0).z);
//        qDebug() << test;

//        QPointF Viewport3D::pixelPosToViewPos(const QPointF &pos)
//        {
//            return QPointF(2.0 * float(pos.x()) / width() - 1.0,
//                           1.0 - 2.0 * float(pos.y()) / height());
//        }


    }
//    else if (event->button() == Qt::MiddleButton){
//        int res = selectingPoint(event->x(), event->y(), points);
//        qDebug() << res;
//    }
    event->accept();
}

void viewport::mouseMoveEvent(QMouseEvent *event)
{
//    if (event->button() == Qt::LeftButton){
    QVector2D diff = QVector2D(event->localPos()) - m_mousePosition;//Просчет разницы между текущим положением и старым(направление)
    m_mousePosition = QVector2D(event->localPos());//Текущее положение

    float angle = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0); //Нормаль к вектору направления

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

//    angleX = diff.x() / 2.0f;
//    angleY = diff.y() / 2.0f;

//    m_camera->rotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));
//    m_camera->rotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));
//    }
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
//    QString dir = QFileDialog::getExistingDirectory(nullptr, "", "");

    //QString path = QFileDialog::getOpenFileName(this, "Open Dialog", "", "*.obj",  nullptr, QFileDialog::DontUseNativeDialog);
//    QString path;

//    foreach(QString str, pathToFiles) path.append(str);
//    QDir(dir).absolutePath();

    obj.loadObj(":/testmodels/cube.obj");

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

        points << QVector3D(xV, yV, zV);
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
    foreach (QList<int> face, obj.FACE)
            for (int i = 0; i < face.count(); i++)
            {
                GLint coord = face.at(i);
                //qDebug() << coord;
                vertexes.append(VertexData(coords[coord], texCoords[0], normal[0]));
                indexes.append(indexes.size());
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

//    if (!m_programSelect.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/select.vsh"))
//        close();//Шейдер вершин

//    if (!m_programSelect.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/select.fsh"))
//        close();//Шейдер пиксельный

//    if (!m_programSelect.link()) //link - объединяет все шейдеры
//            close();
}

void viewport::showPoints(QList<QVector3D> &points, float size)
{
    this->points = points;
    this->pointSize = size;
}

void viewport::drawPoints()
{
    showPoints(points, 5.0f);
    glColor3d(0.0f, 0.0f, 0.0f);
    glPointSize(this->pointSize);
    glBegin(GL_POINTS);
    foreach(QVector3D point, this->points) {
        glVertex3d(point.x(),point.y(),point.z());
    }
    glEnd();
}

QVector3D viewport::screenCToWorldC(const QVector2D &m_mousePosition, float x, float y, float z)
{
    QVector4D tmp(2.0f * m_mousePosition.x() / width() - 1.0f, -2.0f * m_mousePosition.y() / height() + 1, -1.0f, 1.0f);
    QVector4D iTmp((m_projectionMatrix.inverted() * tmp).toVector2D(), -1.0f, 0.0f);
    QVector3D Direction((m_camera->getViewMatrix().inverted() * iTmp).toVector3D().normalized());
    QVector3D camPos((m_camera->getViewMatrix().inverted() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f)).toVector3D());

//    Ax + By + Cz + D = 0
//    norm(A, B, C)
//    P * N - P0 * N = 0
//    O + D * t
//    (O + D * t) * N - P0 * N = 0
//    t = (P0 * N - O * N) / (D * N)
//    result = O + D * t

    QVector3D N(0.0f, 1.0f, 0.0f);
    QVector3D P0(x, y, z);
//    float t = -QVector3D::dotProduct(camPos, N) / QVector3D::dotProduct(Direction, N);
    float t = (QVector3D::dotProduct(P0, N) - QVector3D::dotProduct(camPos, N)) / QVector3D::dotProduct(Direction, N);
    QVector3D result = camPos + Direction * t;
    return result;
}

int viewport::ColorPicking(int x, int y, QList<QVector3D> &startPoint)
{
        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, width(), height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_programSelect.bind();
        m_programSelect.setUniformValue("u_projectionMatrix", m_projectionMatrix);
        m_camera->draw(&m_programSelect);

        //отрисовать точки - нужна модельная матрица
        //for(start, count, i++){
        // m_programSelect.setUniformValue("u_code", i + 1)
        //point.draw();
        //
        for (int i = 0; i < startPoint.count(); i++){
            m_programSelect.setUniformValue("u_code", (float)i + 1);
            object.draw(&m_programSelect, context()->functions());
            m_programSelect.setUniformValue("u_modalMatrix", object.modalMatrix);
        }
        m_programSelect.release();

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        unsigned char res[4];
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);


        glDisable(GL_DEPTH_TEST);

        return res[0];
}

