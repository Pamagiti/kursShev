#include "camera.h"
#include <QOpenGLShaderProgram>
#include <QQuaternion>

camera::camera()
{
    m_scale = 1.0f;
    m_globalTransform.setToIdentity();
}

void camera::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if (functions != 0) return;
    QMatrix4x4 testView = m_viewMatrix;
    qDebug() << "testV" << testView;
    program->setUniformValue("u_viewMatrix", m_viewMatrix);
}

void camera::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;

    updateVM();
}

void camera::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;

    updateVM();
}

void camera::rotateY(const QQuaternion &r)
{
    m_rotateX = r * m_rotateY;
    m_rotate = m_rotateY * m_rotateX;

    updateVM();
}

void camera::translate(const QVector3D &t)
{
    m_translate += t;

    updateVM();
}

void camera::scale(const float &s)
{
    m_scale *= s;

    updateVM();
}

void camera::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;

    updateVM();
}

const QMatrix4x4 &camera::getViewMatrix() const
{
    return m_viewMatrix;
}

void camera::updateVM()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix = m_viewMatrix * m_globalTransform.inverted();

}
