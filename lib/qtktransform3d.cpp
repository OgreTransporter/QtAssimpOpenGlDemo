#include "qtktransform3d.h"

namespace qtk
{
	const QVector3D Transform3D::LocalForward(0.0f, 0.0f, 1.0f);
	const QVector3D Transform3D::LocalUp(0.0f, 1.0f, 0.0f);
	const QVector3D Transform3D::LocalRight(1.0f, 0.0f, 0.0f);

    /*******************************************************************************
     * Transformations
     ******************************************************************************/

    void Transform3D::translate(const QVector3D& dt)
    {
        m_dirty = true;
        mTranslation += dt;
    }

    void Transform3D::scale(const QVector3D& ds)
    {
        m_dirty = true;
        mScale *= ds;
    }

    void Transform3D::rotate(const QQuaternion& dr)
    {
        m_dirty = true;
        mRotation = dr * mRotation;
    }

    void Transform3D::grow(const QVector3D& ds)
    {
        m_dirty = true;
        mScale += ds;
    }


    /*******************************************************************************
     * Setters
     ******************************************************************************/

    void Transform3D::setTranslation(const QVector3D& t)
    {
        m_dirty = true;
        mTranslation = t;
    }

    void Transform3D::setScale(const QVector3D& s)
    {
        m_dirty = true;
        mScale = s;
    }

    void Transform3D::setRotation(const QQuaternion& r)
    {
        m_dirty = true;
        mRotation = r;
    }


    /*******************************************************************************
     * Accessors
     ******************************************************************************/

     // Produces modelToWorld matrix using current set of transformations
     // Transformation * rotation * scale = modelToWorld
    const QMatrix4x4& Transform3D::toMatrix()
    {
        if (m_dirty)
        {
            m_dirty = false;
            mWorld.setToIdentity();
            mWorld.translate(mTranslation);
            mWorld.rotate(mRotation);
            mWorld.scale(mScale);
        }
        return mWorld;
    }


    /*******************************************************************************
     * Queries
     ******************************************************************************/

    QVector3D Transform3D::forward() const
    {
        return mRotation.rotatedVector(LocalForward);
    }

    QVector3D Transform3D::up() const
    {
        return mRotation.rotatedVector(LocalUp);
    }

    QVector3D Transform3D::right() const
    {
        return mRotation.rotatedVector(LocalRight);
    }


    /*******************************************************************************
     * QT Streams
     ******************************************************************************/

#ifndef QT_NO_DEBUG_STREAM
    QDebug operator<<(QDebug dbg, const Transform3D& transform)
    {
        dbg << "Transform3D\n{\n";
        dbg << "Position: <" << transform.translation().x() << ", "
            << transform.translation().y() << ", "
            << transform.translation().z() << ">\n";
        dbg << "Scale: <" << transform.scale().x() << ", "
            << transform.scale().y() << ", "
            << transform.scale().z() << ">\n";
        dbg << "Rotation: <" << transform.rotation().x() << ", "
            << transform.rotation().y() << ", "
            << transform.rotation().z() << " | " <<
            transform.rotation().scalar() << ">\n}";
        return dbg;
    }
#endif

#ifndef QT_NO_DATASTREAM
    QDataStream& operator<<(QDataStream& out, const Transform3D& transform)
    {
        out << transform.mTranslation;
        out << transform.mScale;
        out << transform.mRotation;
        return out;
    }

    QDataStream& operator>>(QDataStream& in, Transform3D& transform)
    {
        in >> transform.mTranslation;
        in >> transform.mScale;
        in >> transform.mRotation;
        transform.m_dirty = true;
        return in;
    }
#endif
}
