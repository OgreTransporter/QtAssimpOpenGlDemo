#ifndef QTKOBJECT_H
#define QTKOBJECT_H
#include "qtkapi.h"
#include "qtkmesh.h"
#include "qtktransform3d.h"
#include <QObject>
#include <QString>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

namespace qtk
{
    class QTKAPI Object : public QObject
    {
        Q_OBJECT

    public:
        friend MeshRenderer;
        // Initialize an object with no shape data assigned
        Object(QString name)
            : mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mTexture(nullptr)
        { }
        // Initialize an object with shape data assigned
        Object(QString name, const ShapeBase& shape)
            : mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mShape(shape), mTexture(nullptr)
        { }

        ~Object() {}

        inline const Vertices& vertices() { return mShape.mVertices; }
        inline const Indices& indices() { return mShape.mIndices; }
        inline const Colors& colors() { return mShape.mColors; }
        inline const TexCoords& texCoords() { return mShape.mTexCoords; }
        inline const Normals& normals() { return mShape.mNormals; }
        inline QOpenGLTexture& texture() const { return *mTexture; }

        virtual inline void setVertices(const Vertices& value) { mShape.mVertices = value; }
        virtual inline void setIndices(const Indices& value) { mShape.mIndices = value; }
        virtual inline void setColors(const Colors& value) { mShape.mColors = value; }
        virtual inline void setTexCoords(const TexCoords& value) { mShape.mTexCoords = value; }
        virtual inline void setNormals(const Normals& value) { mShape.mNormals = value; }
        virtual inline void setShape(const Shape& value) { mShape = value; }

        QOpenGLBuffer mVBO, mNBO;
        QOpenGLVertexArrayObject mVAO;
        QOpenGLShaderProgram mProgram;

        Transform3D mTransform;
        Shape mShape;

        QString mName;
    private:
        QOpenGLTexture* mTexture;
    };
}
#endif // QTKOBJECT_H
