#include "qtkcamera3d.h"

namespace qtk
{
    const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
    const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
    const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);


    /*******************************************************************************
     * Accessors
     ******************************************************************************/

     // Produces worldToView matrix
    const QMatrix4x4& Camera3D::toMatrix()
    {
        mWorld.setToIdentity();
        // Qt6 renamed QMatrix4x4::conjugate() to conjugated()
        mWorld.rotate(mTransform.rotation().conjugated());
        mWorld.translate(-mTransform.translation());
        return mWorld;
    }


    /*******************************************************************************
     * Qt Streams
     ******************************************************************************/

#ifndef QT_NO_DATASTREAM
    QDataStream& operator<<(QDataStream& out, Camera3D& transform)
    {
        out << transform.transform();
        return out;
    }

    QDataStream& operator>>(QDataStream& in, Camera3D& transform)
    {
        in >> transform.transform();
        return in;
    }
#endif

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug dbg, const Camera3D& transform)
    {
        dbg << "Camera3D\n{\n";
        dbg << "Position: <" << transform.translation().x() << ", "
            << transform.translation().y() << ", "
            << transform.translation().z() << ">\n";
        dbg << "Rotation: <" << transform.rotation().x() << ", "
            << transform.rotation().y() << ", "
            << transform.rotation().z() << " | "
            << transform.rotation().scalar() << ">\n}";
        return dbg;
    }
#endif
}
