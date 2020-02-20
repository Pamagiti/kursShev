#ifndef VIEWPORT_H
#define VIEWPORT_H


#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include "loadsaveobj.h"

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


private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkybox;
    QVector2D m_mousePosition;

    Object *m_object;
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
