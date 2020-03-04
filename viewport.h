#ifndef VIEWPORT_H
#define VIEWPORT_H


#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include "loadsaveobj.h"
#include <object.h>


class Object;
class transform;
class camera;

class viewport : public QOpenGLWidget
{
    Q_OBJECT

public:
    viewport(QWidget *parent = nullptr);
    ~viewport();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void loadObj();

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);

    void initShaders();
    void showPoints(QList<QVector3D> &points, float size);
    void drawPoints();
    QVector3D screenCToWorldC(const QVector2D &m_mousePosition, float x, float y, float z);

    int ColorPicking(int x, int y, QList<QVector3D> &startPoint);

    QList<QVector3D> selectPoint;
    QList<QVector3D> startPoint;

    float pointSize;
    QList<QVector3D> points;
    QColor pointsColor;

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSelect;
    QVector2D m_mousePosition;

    Object *m_object;
    Object object;
    transform *m_transform;

    QVector<Object *> m_objects;//класс для описания 3д объектов
    QVector<transform *> m_TransformObjects;

//    float angleO;
//    float angleM;

    float angleY;
    float angleX;

    camera *m_camera;
    LoadSaveObj obj;

};

#endif // VIEWPORT_H
