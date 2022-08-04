#ifndef QTKMODELMESH_H
#define QTKMODELMESH_H
#include "qtkapi.h"
#include "qtktransform3d.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class QOpenGLTexture;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

namespace qtk
{
    struct ModelVertex
    {
        QVector3D mPosition;
        QVector3D mNormal;
        QVector3D mTangent;
        QVector3D mBitangent;
        QVector2D mTextureCoord;
    };

    struct ModelTexture
    {
        GLuint mID;
        QOpenGLTexture* mTexture;
        QString mType;
        QString mPath;
    };

    class QTKAPI ModelMesh : protected QOpenGLFunctions
    {
        friend class Model;
    public:
        typedef std::vector<ModelVertex> Vertices;
        typedef std::vector<GLuint> Indices;
        typedef std::vector<ModelTexture> Textures;

    public:
        // Constructors, Destructors
        ModelMesh(Vertices vertices, Indices indices, Textures textures, QString vertexShader, QString fragmentShader);
        ~ModelMesh();

        void draw();
        void draw(QOpenGLShaderProgram& shader);

    private:
        void initMesh(QString vert, QString frag);

    private:
        // ModelMesh Private Members
        QOpenGLBuffer* mVBO, * mEBO;
        QOpenGLVertexArrayObject* mVAO;
        QOpenGLShaderProgram* mProgram;

    public:
        // ModelMesh Public Members
        Vertices mVertices;
        Indices mIndices;
        Textures mTextures;
        Transform3D mTransform;
    };
}
#endif // QTKMODELMESH_H
