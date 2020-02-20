#include "object.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Object::Object():
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    m_scale = 1.0f;
}

Object::Object(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes):
    m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_texture(0)
{
    m_scale = 1.0f;
    init(vertData, indexes);
}

Object::~Object()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
    if(m_texture != 0){
        if(m_texture->isCreated())
            m_texture->destroy();
    }
}

void Object::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes)
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof (VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof (GLuint));
    m_indexBuffer.release();

//    m_texture = new QOpenGLTexture(texture.mirrored());

//    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);//?

//    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);//?

//    m_texture->setWrapMode(QOpenGLTexture::Repeat);//?
}

void Object::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;
//    m_texture->bind(0);
//    program->setUniformValue("u_texture", 0);
    QMatrix4x4 modalMatrix;
    modalMatrix.setToIdentity();

    modalMatrix.translate(m_translate);
    modalMatrix.rotate(m_rotate);
    modalMatrix.scale(m_scale);
    modalMatrix = m_globalTransform * modalMatrix;

    QMatrix4x4 testM = modalMatrix;
    qDebug() << "Modal M" << testM;
    program->setUniformValue("u_modalMatrix", modalMatrix);
    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof (VertexData));
//qDebug() << "11";
//    offset += sizeof(QVector3D);

//    int textLoc = program->attributeLocation("a_textcoord");
//    program->enableAttributeArray(textLoc);
//    program->setAttributeBuffer(textLoc, GL_FLOAT, 2, sizeof (VertexData));
//qDebug() << "12";
//    offset += sizeof(QVector2D);

//    int normLoc = program->attributeLocation("a_normal");
//    program->enableAttributeArray(normLoc);
//    program->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));
    m_indexBuffer.bind();

    functions->glDrawElements(GL_QUADS, m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
    m_vertexBuffer.release();
    m_indexBuffer.release();
//    m_texture->release();
// GL_QUADS GL_TRIANGLES
}

void Object::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void Object::translate(const QVector3D &t)
{
    m_translate += t;
}

void Object::scale(const float &s)
{
    m_scale *= s;
}

void Object::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
}
