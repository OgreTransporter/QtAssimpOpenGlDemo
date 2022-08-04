#include "qtkmodelmesh.h"
#include "qtkscene.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

namespace qtk
{
    ModelMesh::ModelMesh(Vertices vertices, Indices indices, Textures textures, qtk::Shaders shaders)
        : mProgram(new QOpenGLShaderProgram)
        , mVAO(new QOpenGLVertexArrayObject)
        , mVBO(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
        , mEBO(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
        , mVertices(std::move(vertices))
        , mIndices(std::move(indices))
        , mTextures(std::move(textures))
    {
        initMesh(shaders);
    }

    ModelMesh::~ModelMesh()
    {
    }

    void ModelMesh::draw()
    {
        draw(*mProgram);
    }

    void ModelMesh::draw(QOpenGLShaderProgram& shader)
    {
        mVAO->bind();
        // Bind shader
        shader.bind();

        // Set Model View Projection values
        shader.setUniformValue("uModel", mTransform.toMatrix());
        shader.setUniformValue("uView", Scene::View());
        shader.setUniformValue("uProjection", Scene::Projection());

        GLuint diffuseCount = 1;
        GLuint specularCount = 1;
        GLuint normalCount = 1;
        for (GLuint i = 0; i < mTextures.size(); i++)
        {
            // Activate the current texture index by adding offset to GL_TEXTURE0
            glActiveTexture(GL_TEXTURE0 + i);
            mTextures[i].mTexture->bind();

            // Get a name for the texture using a known convention -
            // Diffuse:   material.texture_diffuse1, material.texture_diffuse2, ...
            // Specular:   material.texture_specular1, material.texture_specular2, ...
            QString number;
            QString name = mTextures[i].mType;
            if (name == "texture_diffuse") number = QString::number(diffuseCount++);
            if (name == "texture_specular") number = QString::number(specularCount++);
            if (name == "texture_normal") number = QString::number(normalCount++);

            // Set the uniform to track this texture ID using our naming convention
            shader.setUniformValue((name + number).toStdString().c_str(), i);
        }

        // Draw the mesh
        glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, mIndices.data());

        // Release shader, textures
        for (const auto& texture : mTextures)
        {
            texture.mTexture->release();
        }
        shader.release();
        mVAO->release();
        glActiveTexture(GL_TEXTURE0);
    }

    void ModelMesh::initMesh(qtk::Shaders shaders)
    {
        initializeOpenGLFunctions();

        // Create VAO, VBO, EBO
        mVAO->create();
        mVBO->create();
        mEBO->create();

        mVAO->bind();

        // Allocate VBO
        mVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
        mVBO->bind();

        mVBO->allocate(mVertices.data(), (int)(mVertices.size() * sizeof(mVertices[0])));

        // Allocate EBO
        mEBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
        mEBO->bind();
        mEBO->allocate(mIndices.data(), (int)(mIndices.size() * sizeof(mIndices[0])));
        mEBO->release();

        // Load and link shaders
        mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, shaders.mVertexShaderText);
        mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, shaders.mFragmentShaderText);
        mProgram->link();
        mProgram->bind();

        // Positions
        mProgram->enableAttributeArray(0);
        mProgram->setAttributeBuffer(0, GL_FLOAT, offsetof(ModelVertex, mPosition), 3, sizeof(ModelVertex));

        // Normals
        mProgram->enableAttributeArray(1);
        mProgram->setAttributeBuffer(1, GL_FLOAT, offsetof(ModelVertex, mNormal), 3, sizeof(ModelVertex));

        // Texture Coordinates
        mProgram->enableAttributeArray(2);
        mProgram->setAttributeBuffer(2, GL_FLOAT, offsetof(ModelVertex, mTextureCoord), 2, sizeof(ModelVertex));

        // Vertex tangents
        mProgram->enableAttributeArray(3);
        mProgram->setAttributeBuffer(3, GL_FLOAT, offsetof(ModelVertex, mTangent), 3, sizeof(ModelVertex));

        // Vertex bitangents
        mProgram->enableAttributeArray(4);
        mProgram->setAttributeBuffer(4, GL_FLOAT, offsetof(ModelVertex, mBitangent), 3, sizeof(ModelVertex));

        mProgram->release();
        mVBO->release();
        mVAO->release();
    }
}
