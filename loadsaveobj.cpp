#include "loadsaveobj.h"

LoadSaveObj::LoadSaveObj()
{

}

void LoadSaveObj::loadObj(QString filename)
{
    QFile object(filename);

    if(!object.open(QIODevice::ReadOnly))
        return;

    while (!object.atEnd())
        record(object.readLine());

    object.close();
}

void LoadSaveObj::record(const QString &line)
{
    if (line.at(0) == 'v' && line.at(1) == ' ')
        readVerticex(line);
    else if (line.at(0) == 'v' && line.at(1) == 't')
        readTextureCord(line);
    else if (line.at(0) == 'v' && line.at(1) == 'n')
        readNormal(line);
    else if (line.at(0) == 'f')
        readFaceTwo(line);
    QVector<QList<int>> TF = triungFaces(n_Face);
    foreach (QList<int> t, TF)
        if (t.size() != 0)
            FACE << t;

}

void LoadSaveObj::readVerticex(const QString &line)
{
    QStringList list;

    list = line.split(QRegExp(" "));

    if (list.at(1) != ""){
        vert.x = list.at(1).toFloat();
        vert.y = list.at(2).toFloat();
        vert.z = list.at(3).toFloat();
    }
    else if (list.at(1) == ""){
        vert.x = list.at(2).toFloat();
        vert.y = list.at(3).toFloat();
        vert.z = list.at(4).toFloat();
    }
    list_vertices.push_back(vert);
}

void LoadSaveObj::readTextureCord(const QString &line)
{
    m_texture_coord = true;

    QStringList list;
    list = line.split(QRegExp(" "));

    tc.u = list.at(1).toFloat();
    tc.v = list.at(2).toFloat();

    list_texture.push_back(tc);
}

void LoadSaveObj::readNormal(const QString &line)
{
    m_normal = true;

    QStringList list;

    list = line.split(QRegExp(" "));

    norm.x = list.at(1).toFloat();
    norm.y = list.at(2).toFloat();
    norm.z = list.at(3).toFloat();

    list_normal.push_back(norm);
}

//void LoadSaveObj::readFace(const QString &line)
//{
//    QStringList list;
//    Face f;

//    // f.id_mtl = usemtl.size();

//    list = line.split(QRegExp(" "));

//    if (list.last() == "\r\n"){
//        countFace = list.size() - 2;
//        list.removeAt(list.indexOf(list.last()));} else countFace = list.size() - 1;
//    QStringList list_value;
//    if (m_texture_coord && !m_normal)
//    {
//        if (countFace == 4)
//        {
//            // f 1/1 2/2 3/3
//            for(int i = 1; i < 4; ++i)
//            {
//                list_value = list.at(i).split(QRegExp("/"));
//                //qDebug() << list_value;
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = 0;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//                face.push_back(f);
//            }
//            for(int i = 2; i < 5; ++i)
//            {
//                list_value = list.at(i).split(QRegExp("/"));
//                //qDebug() << list_value;
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = 0;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//            }
//        }
//        else
//        {
//            for(int i = 1; i < countFace + 1; ++i)
//            {
//                list_value = list.at(i).split(QRegExp("/"));
//                //qDebug() << list_value;
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = 0;
//                    count++;
//                }
//                face.push_back(f);
//            }
//        }
//    } else if (!m_texture_coord && m_normal)
//    {
//        if (countFace == 4)
//        {
//            for(int i = 1; i < 4; ++i)
//            {

//                // f 1//1 2//2 3//3
//                list_value = list.at(i).split(QRegExp("/"));
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = 0;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//                face.push_back(f);
//            }
//            for(int i = 2; i < 5; ++i)
//            {

//                // f 1//1 2//2 3//3
//                list_value = list.at(i).split(QRegExp("/"));
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = 0;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//                face.push_back(f);
//            }
//        } else
//        {
//            for(int i = 1; i < countFace + 1; ++i)
//            {

//                // f 1//1 2//2 3//3
//                list_value = list.at(i).split(QRegExp("/"));
//                if (list_value.size() != 0)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = 0;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                face.push_back(f);
//            }
//        }

//    } else if (m_texture_coord && m_normal)
//    {
//        if (countFace == 4)
//        {
//            for(int i = 1; i < 4; ++i)
//            {
//                // f 1/1/1 2/2/2 3/3/3
//                //qDebug() << "ну вот все в сборе";
//                list_value = list.at(i).split(QRegExp("/"));

//                if (list_value.size() != 1)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//                face.push_back(f);
//            }
//            for(int i = 2; i < 5; ++i)
//            {
//                // f 1/1/1 2/2/2 3/3/3
//                //qDebug() << "ну вот все в сборе";
//                list_value = list.at(i).split(QRegExp("/"));

//                if (list_value.size() != 1)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                triangles.push_back(f.id_vertices);
//                face.push_back(f);
//            }
//        } else
//        {
//            for(int i = 1; i < countFace + 1; ++i)
//            {
//                // f 1/1/1 2/2/2 3/3/3
//                //qDebug() << "ну вот все в сборе";
//                list_value = list.at(i).split(QRegExp("/"));

//                if (list_value.size() != 1)
//                {
//                    f.id_vertices = list_value.at(0).toInt() - 1;
//                    f.id_textur_coordinat = list_value.at(1).toInt() - 1;
//                    f.id_normal = list_value.at(2).toInt() - 1;
//                    count++;
//                }
//                face.push_back(f);
//            }
//        }
//    }
//}

void LoadSaveObj::readFace(const QString &line)
{
    QStringList list;
    Face f;

    // f.id_mtl = usemtl.size();

    list = line.split(QRegExp(" "));
    //qDebug() << list;

    if (list.last() == "\r\n"){
        countFace = list.size() - 2;
        list.removeAt(list.indexOf(list.last()));} else countFace = list.size() - 1;
    //qDebug() << countFace;
    QStringList list_value;
    if (m_texture_coord && !m_normal)
    {

        // f 1/1 2/2 3/3
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

void LoadSaveObj::readFaceTwo(const QString &line)
{
    QStringList list;
    QList<int> faces;
    list = line.split(QRegExp(" "));
    if (list.last() == "\r\n"){
        countFace = list.size() - 2;
        list.removeAt(list.indexOf(list.last()));} else countFace = list.size() - 1;
    //qDebug() << countFace;
    QStringList list_value;

    for(int i = 1; i < countFace + 1; ++i)
    {
        list_value = list.at(i).split(QRegExp("/"));

        if (list_value.size() != 1)
        {
            faces << list_value.at(0).toInt() - 1;
        }
        //qDebug() << "Faces" << faces;
    }

    n_Face << faces;

}

QVector<QList<int>> LoadSaveObj::triungFaces(const QVector<QList<int> > &faces)
{
    int nTriangulatedFaces = 0;
    foreach (QList<int> face, faces)
        nTriangulatedFaces += face.count() - 2;

    QVector<QList<int>> triFaces;
    triFaces.resize(nTriangulatedFaces);

    int triFaceid = 0;

    foreach (QList<int> face, faces){
        QVector<QList<int>> triangl = FaceToTri(face);
        foreach (QList<int> tri, triangl)
            triFaces[triFaceid] << tri;
            triFaceid++;
    }
    return triFaces;
}

QVector<QList<int>> LoadSaveObj::FaceToTri(const QList<int> &face)
{
    int nVert = face.count();
    int nTri = nVert - 2;
    QVector<QList<int>> tri;
    tri.resize(nTri);

    int triID = 0;
    for (int vertID = 2; vertID < nVert; vertID++)
    {
        tri[triID] << face[0];
        tri[triID] << face[vertID - 1];
        tri[triID] << face[vertID];
        triID++;

    }
    return tri;
}
