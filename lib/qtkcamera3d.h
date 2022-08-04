#ifndef QTKCAMERA3D_H
#define QTKCAMERA3D_H
#include "qtkapi.h"
#include "qtktransform3d.h"

namespace qtk
{
	class QTKAPI Camera3D
	{
	public:
		// Constants
		static const QVector3D LocalForward;
		static const QVector3D LocalUp;
		static const QVector3D LocalRight;

		// Accessors
		inline Transform3D& transform() { return mTransform; }
		inline const QVector3D& translation() const
		{
			return mTransform.translation();
		}
		inline const QQuaternion& rotation() const
		{
			return mTransform.rotation();
		}
		const QMatrix4x4& toMatrix();

		// Queries
		inline QVector3D forward() const
		{
			return mTransform.rotation().rotatedVector(LocalForward);
		}
		inline QVector3D right() const
		{
			return mTransform.rotation().rotatedVector(LocalRight);
		}
		inline QVector3D up() const
		{
			return mTransform.rotation().rotatedVector(LocalUp);
		}

	private:
		Transform3D mTransform;
		QMatrix4x4 mWorld;

#ifndef QT_NO_DATASTREAM
		friend QDataStream& operator<<(QDataStream& out, Camera3D& transform);
		friend QDataStream& operator>>(QDataStream& in, Camera3D& transform);
#endif
	};

	Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);

	// Qt Streams
#ifndef QT_NO_DATASTREAM
	QDataStream& operator<<(QDataStream& out, const Camera3D& transform);
	QDataStream& operator>>(QDataStream& in, Camera3D& transform);
#endif

#ifndef QT_NO_DEBUG_STREAM
	QDebug operator<<(QDebug dbg, const Camera3D& transform);
#endif
}

#endif // QTKCAMERA3D_H
