#ifndef QTKMESHRENDERER_H
#define QTKMESHRENDERER_H
#include "qtkapi.h"
#include "qtkobject.h"
#include "qtkmesh.h"

namespace qtk
{
    class QTKAPI MeshRenderer : public Object
    {
    public:
        // Static QHash of all mesh objects within the scene
        typedef QHash<QString, MeshRenderer*> MeshManager;

    public:
        // Delegate constructors
        MeshRenderer(QString name, Vertices vertices, Indices indices, const QString& vert, const QString& frag, DrawMode mode = QTK_DRAW_ARRAYS);
        MeshRenderer(QString name, const QString& vert, const QString& frag);
        // Constructor
        MeshRenderer(QString name, const ShapeBase& shape, const QString& vert, const QString& frag);
        ~MeshRenderer();

        // Retrieve a mesh by name stored within a static QHash
        static MeshRenderer* getInstance(const QString& name);

        void init();
        void draw();

        // Draw types like GL_TRIANGLES, GL_POINTS, GL_LINES, etc
        void setDrawType(int drawType);

        // Shader settings
        void setShaderVertex(const QString& vert);
        void setShaderFragment(const QString& frag);
        void setShaders(const QString& vert, const QString& frag);

        template <typename T>
        inline void setUniform(int location, T value)
        {
            mProgram.setUniformValue(location, value);
        }

        template <typename T>
        inline void setUniform(const char* location, T value)
        {
            mProgram.setUniformValue(location, value);
        }

        // Set MVP matrix using this Object's transform
        // + View and projection provided by MainWidget static members
        void setUniformMVP(QString model = "uModel", QString view = "uView",
            QString projection = "uProjection");

        // Sets the texture to the image at the given path
        // + Sets mHasTexture to enable texture binding in draw()
        void setTexture(QString path);
        void setTexture(QOpenGLTexture* texture);

        // These functions modify data stored in a VBO
        // + After calling them, the VBO will need to be reallocated
        void setShape(const Shape& value) override;
        void setColor(const QVector3D& color);

    private:
        static MeshManager sInstances;

        int mDrawType;
        bool mHasTexture;
        QString mVertexShader;
        QString mFragmentShader;
    };
}
#endif // QTKMESHRENDERER_H
