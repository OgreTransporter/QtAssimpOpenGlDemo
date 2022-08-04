#ifndef QTKSKYBOX_H
#define QTKSKYBOX_H
#include "qtkapi.h"
#include "qtkmesh.h"
#include "qtkshaders.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

namespace qtk
{
    class QTKAPI Skybox : protected QOpenGLFunctions
    {
    public:
        explicit Skybox(QOpenGLTexture* cubeMap, const QString& name, qtk::Shaders shaders);
        Skybox(QString right, QString top, QString front, QString left, QString bottom, QString back, const QString& name, qtk::Shaders shaders);
        ~Skybox();

        void draw();

    private:
        void init(qtk::Shaders shaders);

        Vertices mVertices;
        Indices mIndices;

        QOpenGLShaderProgram mProgram;
        QOpenGLVertexArrayObject mVAO;
        QOpenGLBuffer mVBO;
        QOpenGLTexture* mCubeMap;
    };
}
#endif // QTKSKYBOX_H
