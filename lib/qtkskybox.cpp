#include "qtkskybox.h"
#include "qtkscene.h"
#include "qtktexture.h"

namespace qtk
{
    Skybox::Skybox(QString right, QString top, QString front, QString left, QString bottom, QString back, const QString& name, QString vertexShader, QString fragmentShader)
        : mCubeMap(Texture::initCubeMap(
            QImage(right).mirrored(), QImage(top),
            QImage(front), QImage(left),
            QImage(bottom), QImage(back))),
        mVBO(QOpenGLBuffer::VertexBuffer),
        mVertices(Cube(QTK_DRAW_ELEMENTS).vertices()),
        mIndices(Cube(QTK_DRAW_ELEMENTS).indices())
    {
        init(vertexShader, fragmentShader);
    }

    Skybox::Skybox(QOpenGLTexture* cubeMap, const QString& name, QString vertexShader, QString fragmentShader)
        : mCubeMap(cubeMap)
    {
        init(vertexShader, fragmentShader);
    }

    Skybox::~Skybox()
    {
    }

    /*******************************************************************************
     * Public Member Functions
     ******************************************************************************/

    void Skybox::draw()
    {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        mVAO.bind();
        mProgram.bind();
        mCubeMap->bind();

        mProgram.setUniformValue("uProjectionMatrix", Scene::Projection());
        mProgram.setUniformValue("uViewMatrix", Scene::Camera().toMatrix());
        mProgram.setUniformValue("uTexture", 0);
        glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, mIndices.data());

        mCubeMap->release();
        mProgram.release();
        mVAO.release();

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glActiveTexture(GL_TEXTURE0);
    }


    /*******************************************************************************
     * Private Member Functions
     ******************************************************************************/

    void Skybox::init(QString vertexShader, QString fragmentShader)
    {
        initializeOpenGLFunctions();

        // Set up shader program
        mProgram.create();
        mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader);
        mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader);
        mProgram.link();
        mProgram.bind();

        // Setup VAO
        mVAO.create();
        mVAO.bind();

        // Setup VBO for vertex position data
        mVBO.create();
        mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
        mVBO.bind();
        // Allocate vertex positions into VBO
        mVBO.allocate(mVertices.data(), (int)(mVertices.size() * sizeof(mVertices[0])));

        // Enable attribute array for vertex positions
        mProgram.enableAttributeArray(0);
        mProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
        // Set shader texture unit to 0
        mProgram.setUniformValue("uTexture", 0);

        mVAO.release();
        mVBO.release();
        mProgram.release();
    }
}
