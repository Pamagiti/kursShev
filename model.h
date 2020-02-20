#ifndef MODEL_H
#define MODEL_H
#include <QFile>
#include <QString>
#include <QStringList>
#include <QtOpenGL/QtOpenGL>
#include <QVector>


struct lVertices
{
    float x;
    float y;
    float z;
};

struct TexturCord
{
    float u = 0;
    float v = 0;
};

struct Normal
{
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Face
{
      int id_vertices;
      int id_textur_coordinat = 0;
      int id_normal = 0;
};


class model
{
public:
    model();

    void loadObj(QString filename);
//  void saveObj();

    lVertices vert;
    TexturCord tc;
    Normal norm;
    Face ind;

    GLint count = 0;

    bool m_texture_coord = false; // is vt?
    bool m_normal = false; // is vn?

    QVector<lVertices> list_vertices; //v
    QVector<TexturCord> list_texture; //vt
    QVector<Normal> list_normal; //vn
    QVector<Face> face; //f
private:
    void record(const QString &line);
    void readVerticex(const QString &line);
    void readTextureCord(const QString &line);
    void readNormal(const QString &line);
    void readFace(const QString &line);
};

#endif // MODEL_H
