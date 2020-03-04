#ifndef OBJECT_H
#define OBJECT_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include "transform.h"

class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLFunctions;

struct VertexData //Структура с информацией о вершинах
{
    VertexData()
    {
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n):
        position(p), textCoord(t), normal(n)
    {
    }
    QVector3D position; //позиция
    QVector2D textCoord; //текстурные координаты
    QVector3D normal; //нормали
};

class Object : public transform
{
public:
    Object();
    Object(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes);
    ~Object();

    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void scale(const float &s);
    void translate(const QVector3D &t);
    void setGlobalTransform(const QMatrix4x4 &g);
    const QMatrix4x4 getModalMatrix();
    QMatrix4x4 modalMatrix;
private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture *m_texture;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
};

#endif // OBJECT_H
