#ifndef EXAMPLESCENE_H
#define EXAMPLESCENE_H
#include <qtkscene.h>

class ExampleScene : public qtk::Scene
{
public:
    ExampleScene();

    virtual void init();
    virtual void draw();
    virtual void update();

private:
    QString mRessourcePath;

    qtk::MeshRenderer* mTestPhong;
    qtk::MeshRenderer* mTestSpecular;
    qtk::MeshRenderer* mTestDiffuse;
    qtk::MeshRenderer* mTestAmbient;

    QString mDefaultMeshRendererVert;
    QString mDefaultMeshRendererFrag;
    QString mDefaultModelVert;
    QString mDefaultModelFrag;
};

#endif // EXAMPLESCENE_H
