#ifndef LOADSAVEOBJ_H
#define LOADSAVEOBJ_H
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

class LoadSaveObj
{
public:
    LoadSaveObj();
    void loadObj(QString filename);

    lVertices vert;
    TexturCord tc;
    Normal norm;
    Face ind;

    int countFace = 0;
    GLint count = 0;

    bool m_texture_coord = false; // is vt?
    bool m_normal = false; // is vn?

    QVector<lVertices> list_vertices; //v
    QVector<TexturCord> list_texture; //vt
    QVector<Normal> list_normal; //vn
    QVector<Face> face; //f
    QVector<int> triangles;
private:
    void record(const QString &line);
    void readVerticex(const QString &line);
    void readTextureCord(const QString &line);
    void readNormal(const QString &line);
    void readFace(const QString &line);

    QString get_corret_line(const QString &line);
    bool isblank(const QChar &ch);
};

#endif // LOADSAVEOBJ_H
