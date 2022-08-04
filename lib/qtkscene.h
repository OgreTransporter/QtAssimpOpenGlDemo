#ifndef QTKSCENE_H
#define QTKSCENE_H
#include "qtkapi.h"
#include "qtkcamera3d.h"
#include "qtkmeshrenderer.h"
#include "qtkmodel.h"
#include "qtkskybox.h"
#include <QOpenGLFunctions>

namespace qtk
{
    class QTKAPI Scene : protected QOpenGLFunctions
    {
        friend class OpenGLWidget;
    public:
        Scene();
        ~Scene();

        virtual void init() = 0;
        virtual void draw() = 0;
        virtual void update() = 0;

        static Camera3D& Camera();
        static QMatrix4x4 View();
        static QMatrix4x4& Projection();

        Skybox* skybox();
        void setSkybox(Skybox* skybox);

    private:
        void privDraw();
        void privUpdate();

    private:
        static Camera3D mCamera;
        static QMatrix4x4 mProjection;

        bool mInit;

    protected:
        Skybox* mSkybox;
        std::vector<MeshRenderer*> mMeshes;
        std::vector<Model*> mModels;
    };
}
#endif // QTKSCENE_H
