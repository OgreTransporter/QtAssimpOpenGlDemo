#ifndef QTKMODEL_H
#define QTKMODEL_H
#include "qtkapi.h"
#include "qtkmodelmesh.h"
#include "qtkshaders.h"
#include <QObject>
#include <QOpenGLShaderProgram>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace qtk
{
    class QTKAPI Model : public QObject
    {
        Q_OBJECT
    public:
        typedef QHash<QString, Model*> ModelManager;

    public:
        Model(QString name, QString path, Shaders shaders, QObject* parent = nullptr);
        ~Model();

        void draw();
        void draw(QOpenGLShaderProgram& shader);

        void flipTexture(const QString& fileName, bool flipX = false, bool flipY = true);

        template <typename T>
        void setUniform(QString location, T value)
        {
            for (auto& mesh : mMeshes)
            {
                mesh.mProgram->bind();
                mesh.mProgram->setUniformValue(location.toStdString().c_str(), value);
                mesh.mProgram->release();
            }
        }

        static Model* getInstance(QString name);

    private:
        /**
         * Loads a model in .obj, .fbx, .gltf, and other formats
         * For a full list of formats see assimp documentation:
         *  https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
         *
         * Models should not be loaded into Qt resource system
         * Instead pass an *absolute* path to this function
         * Relative paths will break if Qtk is executed from different locations
         *
         * Models can also be loaded from the `qtk/resource` directory using qrc format
         *  loadModel(":/models/backpack/backpack.obj")
         * See resourcemanager.h for more information
         *
         * @param path Absolute path to a model .obj or other format accepted by assimp
         */
        void loadModel(const QString& path);
        void processNode(aiNode* node, const aiScene* scene);
        ModelMesh processMesh(aiMesh* mesh, const aiScene* scene);
        ModelMesh::Textures loadMaterialTextures(aiMaterial* mat, aiTextureType type, const QString& typeName);
        void sortModels();

    public:
        Transform3D mTransform;

    private:
        static ModelManager mManager;
        ModelMesh::Textures mTexturesLoaded;
        std::vector<ModelMesh> mMeshes;
        QString mDirectory;
        Shaders mShaders;
        QString mName;
    };
}
#endif // QTKMODEL_H
