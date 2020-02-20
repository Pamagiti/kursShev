#ifndef CAMERA_H
#define CAME

#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>
#include "transform.h"

class camera : public transform
{
public:
    camera();

    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions = 0);

    void rotate(const QQuaternion &r);
    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);

    void translate(const QVector3D &t);

    void scale(const float &s);

    void setGlobalTransform(const QMatrix4x4 &g);

    const QMatrix4x4 &getViewMatrix() const;

    void updateVM();

private:
    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;

    QVector3D m_translate;

    float m_scale;

    QMatrix4x4 m_globalTransform;

    QMatrix4x4 m_viewMatrix;

};

#endif // CAMERA_H
