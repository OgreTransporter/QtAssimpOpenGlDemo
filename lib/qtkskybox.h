#ifndef QTKSKYBOX_H
#define QTKSKYBOX_H
#include "qtkapi.h"
#include "qtkmesh.h"
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
        explicit Skybox(QOpenGLTexture* cubeMap, const QString& name, QString vertexShader, QString fragmentShader);
        Skybox(QString right, QString top, QString front, QString left, QString bottom, QString back, const QString& name, QString vertexShader, QString fragmentShader);
        ~Skybox();

        void draw();

    private:
        void init(QString vertexShader, QString fragmentShader);

        Vertices mVertices;
        Indices mIndices;

        QOpenGLShaderProgram mProgram;
        QOpenGLVertexArrayObject mVAO;
        QOpenGLBuffer mVBO;
        QOpenGLTexture* mCubeMap;
    };
}
#endif // QTKSKYBOX_H
