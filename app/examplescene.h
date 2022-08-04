#ifndef EXAMPLESCENE_H
#define EXAMPLESCENE_H
#include <qtkscene.h>
#include <qtkshaders.h>

class ExampleScene : public qtk::Scene
{
public:
    ExampleScene();

    virtual void init();
    virtual void draw();
    virtual void update();

private:
    qtk::Shaders loadShader(QString vert, QString frag);
    qtk::Shaders loadShader(QString name);

private:
    QString mRessourcePath;

    qtk::MeshRenderer* mTestPhong;
    qtk::MeshRenderer* mTestSpecular;
    qtk::MeshRenderer* mTestDiffuse;
    qtk::MeshRenderer* mTestAmbient;

    qtk::Shaders mDefaultMeshRendererShader;
    qtk::Shaders mDefaultModelShader;
};

#endif // EXAMPLESCENE_H
