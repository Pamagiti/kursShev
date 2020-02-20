#include "model.h"

model::model()
{

}

void model::loadObj(QString filename)
{
    QFile object(filename);

    if(!object.open(QIODevice::ReadOnly))
        return;

    while (!object.atEnd())
        record(object.readLine());

    object.close();
}

void model::record(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == ' ')
        readVerticex(line);
    else if (line.at(0) == 'v' && line.at(1) == 't')
        readTextureCord(line);
    else if (line.at(0) == 'v' && line.at(1) == 'n')
        readNormal(line);
    else if (line.at(0) == 'f')
        readFace(line);
}

void model::readVerticex(const QString &line)
{
    QStringList list;

    list = line.split(QRegExp(" "));

    vert.x = list.at(1).toFloat();
    vert.y = list.at(2).toFloat();
    vert.z = list.at(3).toFloat();

    list_vertices.push_back(vert);
}

void model::readTextureCord(const QString &line)
{
    m_texture_coord = true;

    QStringList list;

    list = line.split(QRegExp(" "));

    tc.u = list.at(1).toFloat();
    tc.v = list.at(2).toFloat();

    list_texture.push_back(tc);
}

void model::readNormal(const QString &line)
{
    m_normal = true;

    QStringList list;

    list = line.split(QRegExp(" "));

    norm.x = list.at(1).toFloat();
    norm.y = list.at(2).toFloat();
    norm.z = list.at(3).toFloat();

    list_normal.push_back(norm);
}

void model::readFace(const QString &line)
{
    QStringList list;
    Face f;

    // f.id_mtl = usemtl.size();

    list = line.split(QRegExp(" "));
    //qDebug() << list;

    //qDebug() << countFace;
    QStringList list_value;
    if (m_texture_coord && !m_normal)
    {
        // f 1/1 2/2 3/3  4/4
        for(int i = 1; i < countFace + 1; ++i)
        {
            list_value = list.at(i).split(QRegExp("/"));
            //qDebug() << list_value;
            if (list_value.size() != 0)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = list_value.at(1).toInt() - 1;
                f.id_normal = 0;
                count++;
            }
            face.push_back(f);
        }
    } else if (!m_texture_coord && m_normal)
    {
        for(int i = 1; i < countFace + 1; ++i)
        {

            // f 1//1 2//2 3//3
            list_value = list.at(i).split(QRegExp("/"));
            if (list_value.size() != 0)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = 0;
                f.id_normal = list_value.at(2).toInt() - 1;
                count++;
            }
            face.push_back(f);
        }
    } else if (m_texture_coord && m_normal)
    {
        for(int i = 1; i < countFace + 1; ++i)
        {
            // f 1/1/1 2/2/2 3/3/3
            //qDebug() << "ну вот все в сборе";
            list_value = list.at(i).split(QRegExp("/"));

            if (list_value.size() != 1)
            {
                f.id_vertices = list_value.at(0).toInt() - 1;
                f.id_textur_coordinat = list_value.at(1).toInt() - 1;
                f.id_normal = list_value.at(2).toInt() - 1;
                count++;
            }
            face.push_back(f);
        }
    }
}
