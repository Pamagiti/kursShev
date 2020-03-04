#ifndef MATHCLASS_H
#define MATHCLASS_H

#include <QPointF>
#include <QVector3D>
#include <QMatrix4x4>
#include <loadsaveobj.h>
#include <camera.h>
#include <object.h>
#include <transform.h>
#include <viewport.h>


namespace mathClasses {

//class mathClass
//{
//public:
//    mathClass();

    enum FrustumPlane {FrustumPlane_Near = -1,
                      FrustumPlane_Far = 1};
    QPointF screenToNormalizedDeviceCoord(const QPoint &screenCoord, const QSize &screenSize);
    QVector3D projectScreenPointToFrustumPlane(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrixInv, FrustumPlane frustumPlane = FrustumPlane_Far);
    QVector<QVector3D> projectScreenPointsToFrustumPlane(const QVector<QPointF> &screenPointsNdc, const QMatrix4x4 &projectionMatrixInv, mathClasses::FrustumPlane frustumPlane);
    void screenPointToRay(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrix, const QMatrix4x4 &modelViewMatrix, QVector3D &rayOrigin, QVector3D &rayDirection);
    bool fuzzyEqual(const QVector3D &a, const QVector3D &b, double delta = 1e-6);
//};

#endif // MATHCLASS_H
//#ifndef SELECTIONTOOLS_H
//#define SELECTIONTOOLS_H

//#include <QPointF>
//#include <QVector3D>
//#include <QMatrix4x4>

//namespace SelectionTools
//{

//enum FrustumPlane {FrustumPlane_Near = -1,
//                  FrustumPlane_Far = 1};
//QPointF screenToNormalizedDeviceCoord(const QPoint &screenCoord, const QSize &screenSize);
//QVector3D projectScreenPointToFrustumPlane(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrixInv, FrustumPlane frustumPlane = FrustumPlane_Far);
//QVector<QVector3D> projectScreenPointsToFrustumPlane(const QVector<QPointF> &screenPointsNdc, const QMatrix4x4 &projectionMatrixInv, SelectionTools::FrustumPlane frustumPlane);
//void screenPointToRay(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrix, const QMatrix4x4 &modelViewMatrix, QVector3D &rayOrigin, QVector3D &rayDirection);
//bool fuzzyEqual(const QVector3D &a, const QVector3D &b, double delta = 1e-6);

//}

//#endif // SELECTIONTOOLS_H
