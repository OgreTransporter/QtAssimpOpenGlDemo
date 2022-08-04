#include "qtkmodel.h"
#include "qtktexture.h"
#include "qtkscene.h"
#include <QOpenGLTexture>
#include <QFileInfo>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <algorithm>

namespace qtk
{
    Model::ModelManager Model::mManager;

    Model::Model(QString name, QString path, QString vertexShader, QString fragmentShader, QObject* parent)
        : QObject{ parent }
        , mName(name)
        , mVertexShader(vertexShader)
        , mFragmentShader(fragmentShader)
    {
        loadModel(path);
    }

    Model::~Model()
    {
        mManager.remove(mName);
    }

    void Model::draw()
    {
        for (GLuint i = 0; i < mMeshes.size(); i++)
        {
            mMeshes[i].mTransform = mTransform;
            mMeshes[i].draw();
        }
    }

    void Model::draw(QOpenGLShaderProgram& shader)
    {
        for (GLuint i = 0; i < mMeshes.size(); i++)
        {
            mMeshes[i].mTransform = mTransform;
            mMeshes[i].draw(shader);
        }
    }

    void Model::flipTexture(const QString& fileName, bool flipX, bool flipY)
    {
        bool modified = false;
        QString fullPath = mDirectory + '/' + fileName;
        for (auto& texture : mTexturesLoaded)
        {
            if (texture.mPath == fileName)
            {
                texture.mTexture->destroy();
                texture.mTexture->create();
                texture.mTexture->setData(*Texture::initImage(fullPath, flipX, flipY));
                modified = true;
            }
        }
        if (!modified)
        {
            qDebug() << "Attempt to flip texture that doesn't exist: " << fullPath << "\n";
        }
    }

    Model* Model::getInstance(QString name)
    {
        return mManager[name];
    }

    void Model::loadModel(const QString& path)
    {
        Assimp::Importer importer;

        // JIC a relative path was used, get the absolute file path
        QFileInfo info(path);
        info.makeAbsolute();
        mDirectory = info.absoluteFilePath();

        // Import the model, converting non-triangular geometry to triangles
        // + And flipping texture UVs, etc..
        // Assimp options: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        const aiScene* scene =
            importer.ReadFile(mDirectory.toStdString(), aiProcess_Triangulate
                | aiProcess_FlipUVs
                | aiProcess_GenSmoothNormals
                | aiProcess_CalcTangentSpace
                | aiProcess_OptimizeMeshes
                | aiProcess_SplitLargeMeshes
            );


        // If there were errors, print and return
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            qDebug() << "Error::ASSIMP::" << importer.GetErrorString() << "\n";
            return;
        }

        // If there were no errors, find the directory that contains this model
        mDirectory = mDirectory.mid(0, mDirectory.lastIndexOf('/'));

        // Pass the pointers to the root node and the scene to recursive function
        // + Base case breaks when no nodes left to process on model
        processNode(scene->mRootNode, scene);

        // Sort models by their distance from the camera
        // Optimizes drawing so that overlapping objects are not overwritten
        // + Since the topmost object will be drawn first
        sortModels();

        // Object finished loading, insert it into ModelManager
        mManager.insert(mName, this);
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // Process each mesh that is available for this node
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            mMeshes.push_back(processMesh(mesh, scene));
        }

        // Process each child node for this mesh using recursion
        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    ModelMesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        ModelMesh::Vertices vertices;
        ModelMesh::Indices indices;
        ModelMesh::Textures textures;

        // For each vertex in the aiMesh
        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            // Create a local vertex object for positions, normals, and texture coords
            ModelVertex vertex;

            // Reuse this vector to initialize positions and normals
            QVector3D vector3D;

            // Initialize vertex position
            vector3D.setX(mesh->mVertices[i].x);
            vector3D.setY(mesh->mVertices[i].y);
            vector3D.setZ(mesh->mVertices[i].z);
            // Set the position of our local vertex to the local vector object
            vertex.mPosition = vector3D;

            if (mesh->HasNormals())
            {
                // Initialize vertex normal
                vector3D.setX(mesh->mNormals[i].x);
                vector3D.setY(mesh->mNormals[i].y);
                vector3D.setZ(mesh->mNormals[i].z);
                // Set the normals of our local vertex to the local vector object
                vertex.mNormal = vector3D;
            }

            // Initialize texture coordinates, if any are available
            if (mesh->mTextureCoords[0])
            {
                QVector2D vector2D;
                // Texture coordinates
                vector2D.setX(mesh->mTextureCoords[0][i].x);
                vector2D.setY(mesh->mTextureCoords[0][i].y);
                vertex.mTextureCoord = vector2D;

                // Tangents
                vector3D.setX(mesh->mTangents[i].x);
                vector3D.setY(mesh->mTangents[i].y);
                vector3D.setZ(mesh->mTangents[i].z);
                vertex.mTangent = vector3D;

                // Bitangents
                vector3D.setX(mesh->mBitangents[i].x);
                vector3D.setY(mesh->mBitangents[i].y);
                vector3D.setZ(mesh->mBitangents[i].z);
                vertex.mBitangent = vector3D;
            }
            else
            {
                vertex.mTextureCoord = { 0.0f, 0.0f };
            }

            // Add the initialized vertex to our container of vertices
            vertices.push_back(vertex);
        }

        // For each face on the mesh, process its indices
        for (GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; j++)
            {
                // Add the index to out container of indices
                indices.push_back(face.mIndices[j]);
            }
        }

        // Process material
        if (mesh->mMaterialIndex >= 0)
        {
            // Get the material attached to the model using Assimp
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // Get all diffuse textures from the material
            ModelMesh::Textures diffuseMaps =
                loadMaterialTextures(material, aiTextureType_DIFFUSE,
                    "texture_diffuse");
            // Insert all diffuse textures found into our textures container
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // Get all specular textures from the material
            ModelMesh::Textures specularMaps =
                loadMaterialTextures(material, aiTextureType_SPECULAR,
                    "texture_specular");
            // Insert all specular textures found into our textures container
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // Get all normal textures from the material
            ModelMesh::Textures normalMaps =
                loadMaterialTextures(material, aiTextureType_HEIGHT,
                    "texture_normal");
            // Insert all normal maps found into our textures container
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }

        return ModelMesh(vertices, indices, textures, mVertexShader, mFragmentShader);
    }

    ModelMesh::Textures Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const QString& typeName)
    {
        ModelMesh::Textures textures;

        for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            // Call GetTexture to get the name of the texture file to load
            aiString fileName;
            mat->GetTexture(type, i, &fileName);

            // Check if we have already loaded this texture
            bool skip = false;
            for (GLuint j = 0; j < mTexturesLoaded.size(); j++)
            {
                // If the path to the texture already exists in m_texturesLoaded, skip it
                if (std::strcmp(mTexturesLoaded[j].mPath.toStdString().c_str(), fileName.C_Str()) == 0)
                {
                    textures.push_back(mTexturesLoaded[j]);
                    // If we have loaded the texture, do not load it again
                    skip = true;
                    break;
                }
            }

            // If the texture has not yet been loaded
            if (!skip)
            {
                ModelTexture texture;
                texture.mTexture = Texture::initTexture2D(mDirectory + '/' + QString(fileName.C_Str()), false, false);
                texture.mID = texture.mTexture->textureId();
                texture.mType = typeName;
                texture.mPath = fileName.C_Str();
                // Add the texture to the textures container
                textures.push_back(texture);
                // Add the texture to the loaded textures to avoid loading it twice
                mTexturesLoaded.push_back(texture);
            }
        }

        // Return the resulting textures
        return textures;
    }

    void Model::sortModels()
    {
        auto cameraPos = Scene::Camera().transform();
        auto cameraDistance = [&cameraPos](const ModelMesh& a, const ModelMesh& b)
        {
            // Sort by the first vertex position, since all transforms will be the same
            return (cameraPos.translation().distanceToPoint(a.mVertices[0].mPosition)) < (cameraPos.translation().distanceToPoint(b.mVertices[0].mPosition));
        };
        std::sort(mMeshes.begin(), mMeshes.end(), cameraDistance);
    }
}
