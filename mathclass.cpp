#include "mathclass.h"

//mathClass::mathClass()
//{

//}

//#include "selectiontools.h"

using namespace mathClasses;

QPointF mathClasses::screenToNormalizedDeviceCoord(const QPoint &screenCoord, const QSize &screenSize)
{
    return QPointF(screenCoord.x() / double(screenSize.width()) * 2 - 1,
                   1 - screenCoord.y() / double(screenSize.height()) * 2);
}

QVector3D mathClasses::projectScreenPointToFrustumPlane(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrixInv, mathClasses::FrustumPlane frustumPlane)
{
    // formula is taken from http://www.opengl.org/sdk/docs/man2/xhtml/gluUnProject.xml
    return projectionMatrixInv * QVector3D(screenPointNdc.x(),screenPointNdc.y(),frustumPlane);
}

QVector<QVector3D> mathClasses::projectScreenPointsToFrustumPlane(const QVector<QPointF> &screenPointsNdc, const QMatrix4x4 &projectionMatrixInv, FrustumPlane frustumPlane)
{
    QVector<QVector3D> points(screenPointsNdc.count());
    for (int pointInd = 0; pointInd < screenPointsNdc.count(); ++pointInd)
        points[pointInd] = projectScreenPointToFrustumPlane(screenPointsNdc[pointInd],projectionMatrixInv,frustumPlane);
    return points;
}

void mathClasses::screenPointToRay(const QPointF &screenPointNdc, const QMatrix4x4 &projectionMatrix, const QMatrix4x4 &modelViewMatrix, QVector3D &rayOrigin, QVector3D &rayDirection)
{
    bool modelViewInverted = true;
    const QMatrix4x4 modelViewMatrixInv = modelViewMatrix.inverted(&modelViewInverted);
    if (!modelViewInverted) {
        qDebug() << "Failed to invert model view matrix";
        return;
    }
    const QVector3D cameraOriginEye = QVector3D(0,0,0);
    const QVector3D cameraOriginWorld = modelViewMatrixInv * cameraOriginEye;
    bool projectionMatrixInverted = true;
    const QMatrix4x4 projectionMatrixInv = projectionMatrix.inverted(&projectionMatrixInverted);
    if (!projectionMatrixInverted) {
        qDebug() << "Failed to invert projection matrix";
        return;
    }
    const QVector3D pointOnFarPlaneEye = projectScreenPointToFrustumPlane(screenPointNdc,projectionMatrixInv,FrustumPlane_Far);
    const QVector3D pointOnFarPlaneWorld = modelViewMatrixInv * pointOnFarPlaneEye;
    rayOrigin = cameraOriginWorld;
    rayDirection = pointOnFarPlaneWorld - cameraOriginWorld;
}

bool mathClasses::fuzzyEqual(const QVector3D &a, const QVector3D &b, double delta)
{
    return (qAbs(a.x() - b.x()) < delta) && (qAbs(a.y() - b.y()) < delta) && (qAbs(a.z() - b.z()) < delta);
}
