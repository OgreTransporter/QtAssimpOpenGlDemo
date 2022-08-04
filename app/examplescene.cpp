#include "examplescene.h"
#include <qtktexture.h>
#include <qtkskybox.h>
#include "resources.h"

ExampleScene::ExampleScene()
    : mRessourcePath(DEMO_RESOURCES_DIR)
{
    mDefaultMeshRendererVert = mRessourcePath + "/shaders/vertex/multi-color.vert";
    mDefaultMeshRendererFrag = mRessourcePath + "/shaders/fragment/multi-color.frag";
    mDefaultModelVert = mRessourcePath + "/shaders/vertex/model-basic.vert";
    mDefaultModelFrag = mRessourcePath + "/shaders/fragment/model-basic.frag";
    Camera().transform().setTranslation(0.0f, 0.0f, 20.0f);
    Camera().transform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

void ExampleScene::init()
{
    qtk::Skybox* sb = new qtk::Skybox(
        mRessourcePath + "/skybox/right.png",
        mRessourcePath + "/skybox/top.png",
        mRessourcePath + "/skybox/front.png",
        mRessourcePath + "/skybox/left.png",
        mRessourcePath + "/skybox/bottom.png",
        mRessourcePath + "/skybox/back.png",
        "Skybox",
        mRessourcePath + "/skybox/skybox.vert",
        mRessourcePath + "/skybox/skybox.frag"
        );
    setSkybox(sb);

    // Initialize Phong example cube
    mTestPhong = new qtk::MeshRenderer("phong", qtk::Cube(), mRessourcePath + "/shaders/vertex/solid-phong.vert", mRessourcePath + "/shaders/fragment/solid-phong.frag");
    mTestPhong->mTransform.setTranslation(3.0f, 0.0f, -2.0f);
    mTestPhong->init();
    mTestPhong->mProgram.bind();
    mTestPhong->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
    mTestPhong->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
    mTestPhong->setUniform("uAmbientStrength", 0.2f);
    mTestPhong->setUniform("uSpecularStrength", 0.50f);
    mTestPhong->setUniform("uSpecularShine", 256);

    mTestPhong->mVAO.bind();
    mTestPhong->mNBO.create();
    mTestPhong->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mTestPhong->mNBO.bind();
    mTestPhong->mNBO.allocate(mTestPhong->normals().data(), mTestPhong->normals().size() * sizeof(mTestPhong->normals()[0]));
    mTestPhong->mProgram.enableAttributeArray(1);
    mTestPhong->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mTestPhong->mNBO.release();
    mTestPhong->mVAO.release();
    mTestPhong->mProgram.release();


    // Initialize Ambient example cube
    mTestAmbient = new qtk::MeshRenderer("ambient", qtk::Cube(), mRessourcePath + "/shaders/vertex/solid-ambient.vert", mRessourcePath + "/shaders/fragment/solid-ambient.frag");
    mTestAmbient->mTransform.setTranslation(7.0f, 0.0f, -2.0f);
    mTestAmbient->init();
    mTestAmbient->mProgram.bind();
    mTestAmbient->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
    mTestAmbient->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
    mTestAmbient->setUniform("uAmbientStrength", 0.2f);

    mTestAmbient->mVAO.bind();
    mTestAmbient->mNBO.create();
    mTestAmbient->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mTestAmbient->mNBO.bind();
    mTestAmbient->mNBO.allocate(mTestAmbient->normals().data(), mTestAmbient->normals().size() * sizeof(mTestAmbient->normals()[0]));
    mTestAmbient->mProgram.enableAttributeArray(1);
    mTestAmbient->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mTestAmbient->mNBO.release();
    mTestAmbient->mVAO.release();
    mTestAmbient->mProgram.release();

    // Initialize Diffuse example cube
    mTestDiffuse = new qtk::MeshRenderer("diffuse", qtk::Cube(), mRessourcePath + "/shaders/vertex/solid-diffuse.vert", mRessourcePath + "/shaders/fragment/solid-diffuse.frag");
    mTestDiffuse->mTransform.setTranslation(9.0f, 0.0f, -2.0f);
    mTestDiffuse->init();
    mTestDiffuse->mProgram.bind();
    mTestDiffuse->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
    mTestDiffuse->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
    mTestDiffuse->setUniform("uAmbientStrength", 0.2f);

    mTestDiffuse->mVAO.bind();
    mTestDiffuse->mNBO.create();
    mTestDiffuse->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mTestDiffuse->mNBO.bind();
    mTestDiffuse->mNBO.allocate(mTestDiffuse->normals().data(), mTestDiffuse->normals().size() * sizeof(mTestDiffuse->normals()[0]));
    mTestDiffuse->mProgram.enableAttributeArray(1);
    mTestDiffuse->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mTestDiffuse->mNBO.release();
    mTestDiffuse->mVAO.release();
    mTestDiffuse->mProgram.release();

    // Initialize Specular example cube
    mTestSpecular = new qtk::MeshRenderer("specular", qtk::Cube(), mRessourcePath + "/shaders/vertex/solid-specular.vert", mRessourcePath + "/shaders/fragment/solid-specular.frag");
    mTestSpecular->mTransform.setTranslation(11.0f, 0.0f, -2.0f);
    mTestSpecular->init();
    mTestSpecular->mProgram.bind();
    mTestSpecular->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
    mTestSpecular->setUniform("uLightColor", QVector3D(1.0f, 1.0f, 1.0f));
    mTestSpecular->setUniform("uAmbientStrength", 0.2f);
    mTestSpecular->setUniform("uSpecularStrength", 0.50f);
    mTestSpecular->setUniform("uSpecularShine", 256);

    mTestSpecular->mVAO.bind();
    mTestSpecular->mNBO.create();
    mTestSpecular->mNBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mTestSpecular->mNBO.bind();
    mTestSpecular->mNBO.allocate(mTestSpecular->normals().data(), mTestSpecular->normals().size() * sizeof(mTestSpecular->normals()[0]));
    mTestSpecular->mProgram.enableAttributeArray(1);
    mTestSpecular->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mTestSpecular->mNBO.release();
    mTestSpecular->mVAO.release();
    mTestSpecular->mProgram.release();

    //
    // Model loading

    mModels.push_back(new qtk::Model("backpack", mRessourcePath + "/models/backpack/backpack.obj", mDefaultModelVert, mDefaultModelFrag));
    // Sometimes model textures need flipped in certain directions
    mModels.back()->flipTexture("diffuse.jpg", false, true);
    mModels.back()->mTransform.setTranslation(0.0f, 0.0f, -10.0f);

    mModels.push_back(new qtk::Model("bird", mRessourcePath + "/models/bird/bird.obj", mDefaultModelVert, mDefaultModelFrag));
    mModels.back()->mTransform.setTranslation(2.0f, 2.0f, -10.0f);
    // Sometimes the models are very large
    mModels.back()->mTransform.scale(0.0025f);
    mModels.back()->mTransform.rotate(-110.0f, 0.0f, 1.0f, 0.0f);

    mModels.push_back(new qtk::Model("lion", mRessourcePath + "/models/lion/lion.obj", mDefaultModelVert, mDefaultModelFrag));
    mModels.back()->mTransform.setTranslation(-3.0f, -1.0f, -10.0f);
    mModels.back()->mTransform.scale(0.15f);

    mModels.push_back(new qtk::Model("alien", mRessourcePath + "/models/alien-hominid/alien.obj", mDefaultModelVert, mDefaultModelFrag));
    mModels.back()->mTransform.setTranslation(2.0f, -1.0f, -5.0f);
    mModels.back()->mTransform.scale(0.15f);

    mModels.push_back(new qtk::Model("scythe", mRessourcePath + "/models/scythe/scythe.obj", mDefaultModelVert, mDefaultModelFrag));
    mModels.back()->mTransform.setTranslation(-6.0f, 0.0f, -10.0f);
    mModels.back()->mTransform.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    mModels.back()->mTransform.rotate(90.0f, 0.0f, 1.0f, 0.0f);

    mModels.push_back(new qtk::Model("masterChief", mRessourcePath + "/models/spartan/spartan.obj", mDefaultModelVert, mDefaultModelFrag));
    mModels.back()->mTransform.setTranslation(-1.5f, 0.5f, -2.0f);


    //
    // Building example mesh objects

    // Render an alien with specular
    // Test alien Model with phong lighting and specular mapping
    mMeshes.push_back(new qtk::MeshRenderer("alienTestLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(4.0f, 1.5f, 10.0f);
    mMeshes.back()->mTransform.scale(0.25f);
    // This function changes values we have allocated in a buffer, so init() after
    mMeshes.back()->setColor(GREEN);
    mMeshes.back()->init();

    mModels.push_back(new qtk::Model("alienTest", mRessourcePath + "/models/alien-hominid/alien.obj", mRessourcePath + "/shaders/vertex/model-specular.vert", mRessourcePath + "/shaders/fragment/model-specular.frag"));
    mModels.back()->mTransform.setTranslation(3.0f, -1.0f, 10.0f);
    mModels.back()->mTransform.scale(0.15f);
    mModels.back()->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.ambientStrength", 0.8f);
    mModels.back()->setUniform("uMaterial.diffuseStrength", 0.8f);
    mModels.back()->setUniform("uMaterial.specularStrength", 1.0f);
    mModels.back()->setUniform("uMaterial.shine", 32.0f);

    mModels.back()->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));


    // Test spartan Model with phong lighting, specular and normal mapping
    mMeshes.push_back(new qtk::MeshRenderer("spartanTestLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(1.0f, 1.5f, 10.0f);
    mMeshes.back()->mTransform.scale(0.25f);
    // This function changes values we have allocated in a buffer, so init() after
    mMeshes.back()->setColor(GREEN);
    mMeshes.back()->init();

    mModels.push_back(new qtk::Model("spartanTest", mRessourcePath + "/models/spartan/spartan.obj", mRessourcePath + "/shaders/vertex/model-normals.vert", mRessourcePath + "/shaders/fragment/model-normals.frag"));
    mModels.back()->mTransform.setTranslation(0.0f, -1.0f, 10.0f);
    mModels.back()->mTransform.scale(2.0f);
    mModels.back()->setUniform("uMaterial.ambient", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uMaterial.ambientStrength", 1.0f);
    mModels.back()->setUniform("uMaterial.diffuseStrength", 1.0f);
    mModels.back()->setUniform("uMaterial.specularStrength", 1.0f);
    mModels.back()->setUniform("uMaterial.shine", 128.0f);
    mModels.back()->setUniform("uLight.ambient", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uLight.diffuse", QVector3D(1.0f, 1.0f, 1.0f));
    mModels.back()->setUniform("uLight.specular", QVector3D(1.0f, 1.0f, 1.0f));


    // Test basic cube with phong.vert and phong.frag shaders
    mMeshes.push_back(new qtk::MeshRenderer("testLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(5.0f, 1.25f, 10.0f);
    mMeshes.back()->mTransform.scale(0.25f);
    mMeshes.back()->setDrawType(GL_LINE_LOOP);
    // This function changes values we have allocated in a buffer, so init() after
    mMeshes.back()->setColor(GREEN);
    mMeshes.back()->init();

    mMeshes.push_back(new qtk::MeshRenderer("testPhong", qtk::Cube(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/phong.vert", mRessourcePath + "/shaders/fragment/phong.frag"));
    mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 10.0f);
    mMeshes.back()->setColor(QVector3D(0.0f, 0.25f, 0.0f));
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();

    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));

    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();

    mMeshes.back()->setUniform("uMaterial.ambient", QVector3D(0.0f, 0.3f, 0.0f));
    mMeshes.back()->setUniform("uMaterial.diffuse", QVector3D(0.0f, 0.2f, 0.0f));

    mMeshes.back()->setUniform("uMaterial.specular", QVector3D(1.0f, 1.0f, 1.0f));
    mMeshes.back()->setUniform("uMaterial.ambientStrength", 1.0f);
    mMeshes.back()->setUniform("uMaterial.diffuseStrength", 1.0f);
    mMeshes.back()->setUniform("uMaterial.specularStrength", 1.0f);
    mMeshes.back()->setUniform("uMaterial.shine", 64.0f);
    mMeshes.back()->setUniform("uLight.ambient", QVector3D(0.25f, 0.2f, 0.075f));
    mMeshes.back()->setUniform("uLight.diffuse", QVector3D(0.75f, 0.6f, 0.22f));
    mMeshes.back()->setUniform("uLight.specular", QVector3D(0.62f, 0.55f, 0.37f));
    mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));

    mMeshes.back()->mProgram.release();

    //
    // Create simple shapes using MeshRenderer class and data in mesh.h

    mMeshes.push_back(new qtk::MeshRenderer("rightTriangle", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(-5.0f, 0.0f, -2.0f);

    mMeshes.push_back(new qtk::MeshRenderer("centerCube", qtk::Cube(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(-7.0f, 0.0f, -2.0f);

    mMeshes.push_back(new qtk::MeshRenderer("leftTriangle", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(-9.0f, 0.0f, -2.0f);
    mMeshes.back()->setDrawType(GL_LINE_LOOP);

    mMeshes.push_back(new qtk::MeshRenderer("topTriangle", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(-7.0f, 2.0f, -2.0f);
    mMeshes.back()->mTransform.scale(0.25f);

    mMeshes.push_back(new qtk::MeshRenderer("bottomTriangle", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(-7.0f, -2.0f, -2.0f);
    mMeshes.back()->mTransform.scale(0.25f);
    mMeshes.back()->setDrawType(GL_LINE_LOOP);
    // This function changes values we have allocated in a buffer, so init() after
    mMeshes.back()->setColor(GREEN);
    mMeshes.back()->init();

    //
    // Testing for normals, texture coordinates

    // RGB Normals cube to show normals are correct with QTK_DRAW_ARRAYS
    mMeshes.push_back(new qtk::MeshRenderer("rgbNormalsCubeArraysTest", qtk::Cube(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/rgb-normals.vert", mRessourcePath + "/shaders/fragment/rgb-normals.frag"));
    mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 4.0f);
    mMeshes.back()->init();
    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));

    mMeshes.back()->mProgram.release();
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();

    // RGB Normals cube to show normals are correct with QTK_DRAW_ELEMENTS_NORMALS
    mMeshes.push_back(new qtk::MeshRenderer("rgbNormalsCubeElementsTest", qtk::Cube(qtk::QTK_DRAW_ELEMENTS_NORMALS), mRessourcePath + "/shaders/vertex/rgb-normals.vert", mRessourcePath + "/shaders/fragment/rgb-normals.frag"));
    mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, 2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));

    mMeshes.back()->mProgram.release();
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();

    // Texturing a cube using texture coordinates and glDrawArrays
    // + Texturing with UVs using glDrawElements requires QTK_DRAW_ELEMENTS_NORMALS
    // + UVs required duplicating element position data from QTK_DRAW_ELEMENTS
    // + This is because the same position must use different UV coordinates
    mMeshes.push_back(new qtk::MeshRenderer("uvCubeArraysTest", qtk::Cube(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/texture2d.vert", mRessourcePath + "/shaders/fragment/texture2d.frag"));
    mMeshes.back()->mTransform.setTranslation(-3.0f, 0.0f, -2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->setTexture(qtk::Texture::initTexture2D(mRessourcePath + "/images/crate.png"));
    mMeshes.back()->setUniform("uTexture", 0);
    mMeshes.back()->texture().bind();

    mMeshes.back()->texture().release();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.destroy();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(), mMeshes.back()->mShape.texCoords().size() * sizeof(mMeshes.back()->mShape.texCoords()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    // Test drawing a cube with texture coordinates using glDrawElements
    mMeshes.push_back(new qtk::MeshRenderer("uvCubeElementsTest", qtk::Cube(qtk::QTK_DRAW_ELEMENTS_NORMALS), mRessourcePath + "/shaders/vertex/texture2d.vert", mRessourcePath + "/shaders/fragment/texture2d.frag"));
    mMeshes.back()->mTransform.setTranslation(-1.7f, 0.0f, -2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mNBO.allocate(mMeshes.back()->texCoords().data(), mMeshes.back()->texCoords().size() * sizeof(mMeshes.back()->texCoords()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mMeshes.back()->setTexture(qtk::Texture::initTexture2D(mRessourcePath + "/images/crate.png"));

    mMeshes.back()->mProgram.setUniformValue("uTexture", 0);
    mMeshes.back()->mProgram.release();
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mTransform.rotate(45.0f, 0.0f, 1.0f, 0.0f);

    // Texturing a cube using a cube map
    // + Cube map texturing works with both QTK_DRAW_ARRAYS and QTK_DRAW_ELEMENTS
    mMeshes.push_back(new qtk::MeshRenderer("testCubeMap", qtk::Cube(qtk::QTK_DRAW_ELEMENTS), mRessourcePath + "/shaders/vertex/texture-cubemap.vert", mRessourcePath + "/shaders/fragment/texture-cubemap.frag"));
    mMeshes.back()->mTransform.setTranslation(-3.0f, 1.0f, -2.0f);
    mMeshes.back()->mTransform.setRotation(45.0f, 0.0f, 1.0f, 0.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->setTexture(qtk::Texture::initCubeMap(mRessourcePath + "/images/crate.png"));
    mMeshes.back()->setUniform("uTexture", 0);

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.destroy();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(), mMeshes.back()->mShape.texCoords().size() * sizeof(mMeshes.back()->mShape.texCoords()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    // Create a cube with custom shaders
    // + Apply RGB normals shader and spin the cube for a neat effect
    mMeshes.push_back(new qtk::MeshRenderer("rgbNormalsCube", qtk::Cube(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/rgb-normals.vert", mRessourcePath + "/shaders/fragment/rgb-normals.frag"));
    mMeshes.back()->mTransform.setTranslation(5.0f, 2.0f, -2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));

    mMeshes.back()->mProgram.release();
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();

    // RGB Normals triangle to show normals are correct with QTK_DRAW_ARRAYS
    mMeshes.push_back(new qtk::MeshRenderer("rgbTriangleArraysTest", qtk::Triangle(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/rgb-normals.vert", mRessourcePath + "/shaders/fragment/rgb-normals.frag"));
    mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    // RGB Normals triangle to show normals are correct with QTK_DRAW_ELEMENTS
    mMeshes.push_back(new qtk::MeshRenderer("rgbTriangleElementsTest", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS_NORMALS), mRessourcePath + "/shaders/vertex/rgb-normals.vert", mRessourcePath + "/shaders/fragment/rgb-normals.frag"));
    mMeshes.back()->mTransform.setTranslation(7.0f, 0.0f, 4.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->normals().data(), mMeshes.back()->normals().size() * sizeof(mMeshes.back()->normals()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    // Test drawing triangle with glDrawArrays with texture coordinates
    mMeshes.push_back(new qtk::MeshRenderer("testTriangleArraysUV", qtk::Triangle(qtk::QTK_DRAW_ARRAYS), mRessourcePath + "/shaders/vertex/texture2d.vert", mRessourcePath + "/shaders/fragment/texture2d.frag"));
    mMeshes.back()->mTransform.setTranslation(-3.0f, 2.0f, -2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->setTexture(qtk::Texture::initTexture2D(mRessourcePath + "/images/crate.png"));
    mMeshes.back()->setUniform("uTexture", 0);
    mMeshes.back()->texture().bind();

    mMeshes.back()->texture().release();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.destroy();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(), mMeshes.back()->mShape.texCoords().size() * sizeof(mMeshes.back()->mShape.texCoords()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    // Test drawing triangle with glDrawElements with texture coordinates
    mMeshes.push_back(new qtk::MeshRenderer("testTriangleElementsUV", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS_NORMALS), mRessourcePath + "/shaders/vertex/texture2d.vert", mRessourcePath + "/shaders/fragment/texture2d.frag"));
    mMeshes.back()->mTransform.setTranslation(-2.5f, 0.0f, -1.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();

    mMeshes.back()->setTexture(qtk::Texture::initTexture2D(mRessourcePath + "/images/crate.png"));
    mMeshes.back()->setUniform("uTexture", 0);
    mMeshes.back()->texture().bind();

    mMeshes.back()->texture().release();

    mMeshes.back()->mVAO.bind();
    mMeshes.back()->mNBO.destroy();
    mMeshes.back()->mNBO.create();
    mMeshes.back()->mNBO.bind();
    mMeshes.back()->mNBO.allocate(mMeshes.back()->mShape.texCoords().data(), mMeshes.back()->mShape.texCoords().size() * sizeof(mMeshes.back()->mShape.texCoords()[0]));
    mMeshes.back()->mProgram.enableAttributeArray(1);
    mMeshes.back()->mProgram.setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
    mMeshes.back()->mNBO.release();
    mMeshes.back()->mVAO.release();
    mMeshes.back()->mProgram.release();

    //
    // Lighting cube examples

    // Example of a cube with no lighting applied
    mMeshes.push_back(new qtk::MeshRenderer("noLight", qtk::Cube(qtk::QTK_DRAW_ELEMENTS), mRessourcePath + "/shaders/vertex/solid-perspective.vert", mRessourcePath + "/shaders/fragment/solid-perspective.frag"));
    mMeshes.back()->mTransform.setTranslation(5.0f, 0.0f, -2.0f);
    mMeshes.back()->init();
    mMeshes.back()->mProgram.bind();
    mMeshes.back()->setUniform("uColor", QVector3D(0.0f, 0.25f, 0.0f));
    mMeshes.back()->mProgram.release();

    // Create objects that represent light sources for lighting examples
    mMeshes.push_back(new qtk::MeshRenderer("phongLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(3.0f, 2.0f, -2.0f);
    mMeshes.back()->mTransform.scale(0.25f);

    mMeshes.push_back(new qtk::MeshRenderer("diffuseLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(9.0f, 2.0f, -2.0f);
    mMeshes.back()->mTransform.scale(0.25f);

    mMeshes.push_back(new qtk::MeshRenderer("specularLight", qtk::Triangle(qtk::QTK_DRAW_ELEMENTS), mDefaultMeshRendererVert, mDefaultMeshRendererFrag));
    mMeshes.back()->mTransform.setTranslation(11.0f, 2.0f, -2.0f);
    mMeshes.back()->mTransform.scale(0.25f);
}

void ExampleScene::draw()
{
    mTestPhong->mProgram.bind();
    mTestPhong->setUniform("uModelInverseTransposed", mTestPhong->mTransform.toMatrix().normalMatrix());
    mTestPhong->setUniform("uLightPosition", qtk::MeshRenderer::getInstance("phongLight")->mTransform.translation());
    mTestPhong->setUniform("uCameraPosition", qtk::Scene::Camera().transform().translation());
    mTestPhong->mProgram.release();
    mTestPhong->draw();

    mTestAmbient->mProgram.bind();
    mTestAmbient->setUniform("uCameraPosition", qtk::Scene::Camera().transform().translation());
    mTestAmbient->mProgram.release();
    mTestAmbient->draw();

    mTestDiffuse->mProgram.bind();
    mTestDiffuse->setUniform("uModelInverseTransposed", mTestDiffuse->mTransform.toMatrix().normalMatrix());
    mTestDiffuse->setUniform("uLightPosition", qtk::MeshRenderer::getInstance("diffuseLight")->mTransform.translation());
    mTestDiffuse->setUniform("uCameraPosition", Scene::Camera().transform().translation());
    mTestDiffuse->mProgram.release();
    mTestDiffuse->draw();

    mTestSpecular->mProgram.bind();
    mTestSpecular->setUniform("uModelInverseTransposed", mTestSpecular->mTransform.toMatrix().normalMatrix());
    mTestSpecular->setUniform("uLightPosition", qtk::MeshRenderer::getInstance("specularLight")->mTransform.translation());
    mTestSpecular->setUniform("uCameraPosition", Scene::Camera().transform().translation());
    mTestSpecular->mProgram.release();
    mTestSpecular->draw();
}

void ExampleScene::update()
{
    auto position = qtk::MeshRenderer::getInstance("alienTestLight")->mTransform.translation();
    qtk::Model::getInstance("alienTest")->setUniform("uLight.position", position);
    qtk::Model::getInstance("alienTest")->setUniform("uCameraPosition", Scene::Camera().transform().translation());
    auto posMatrix = qtk::Model::getInstance("alienTest")->mTransform.toMatrix();
    qtk::Model::getInstance("alienTest")->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    qtk::Model::getInstance("alienTest")->setUniform("uMVP.model", posMatrix);
    qtk::Model::getInstance("alienTest")->setUniform("uMVP.view", Scene::Camera().toMatrix());
    qtk::Model::getInstance("alienTest")->setUniform("uMVP.projection", Scene::Projection());
    qtk::Model::getInstance("alienTest")->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);

    position = qtk::MeshRenderer::getInstance("spartanTestLight")->mTransform.translation();
    qtk::Model::getInstance("spartanTest")->setUniform("uLight.position", position);
    qtk::Model::getInstance("spartanTest")->setUniform("uCameraPosition", Scene::Camera().transform().translation());
    posMatrix = qtk::Model::getInstance("spartanTest")->mTransform.toMatrix();
    qtk::Model::getInstance("spartanTest")->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    qtk::Model::getInstance("spartanTest")->setUniform("uMVP.model", posMatrix);
    qtk::Model::getInstance("spartanTest")->setUniform("uMVP.view", Scene::Camera().toMatrix());
    qtk::Model::getInstance("spartanTest")->setUniform("uMVP.projection", Scene::Projection());
    qtk::Model::getInstance("spartanTest")->mTransform.rotate(0.75f, 0.0f, 1.0f, 0.0f);

    qtk::MeshRenderer::getInstance("testPhong")->mTransform.rotate(0.75f, 1.0f, 0.5f, 0.0f);
    qtk::MeshRenderer::getInstance("testPhong")->mProgram.bind();
    position = qtk::MeshRenderer::getInstance("testLight")->mTransform.translation();
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uLight.position", position);
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uCameraPosition", Scene::Camera().transform().translation());
    posMatrix = qtk::MeshRenderer::getInstance("testPhong")->mTransform.toMatrix();
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uMVP.normalMatrix", posMatrix.normalMatrix());
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uMVP.model", posMatrix);
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uMVP.view", Scene::Camera().toMatrix());
    qtk::MeshRenderer::getInstance("testPhong")->setUniform("uMVP.projection", Scene::Projection());
    qtk::MeshRenderer::getInstance("testPhong")->mProgram.release();

    // Rotate lighting example cubes
    mTestPhong->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
    qtk::MeshRenderer::getInstance("noLight")->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
    mTestAmbient->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
    mTestDiffuse->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);
    mTestSpecular->mTransform.rotate(0.75f, 0.5f, 0.3f, 0.2f);

    // Examples of various translations and rotations

    // Rotate in multiple directions simultaneously
    qtk::MeshRenderer::getInstance("rgbNormalsCube")->mTransform.rotate(0.75f, 0.2f, 0.4f, 0.6f);

    // Pitch forward and roll sideways
    qtk::MeshRenderer::getInstance("leftTriangle")->mTransform.rotate(0.75f, 1.0f, 0.0f, 0.0f);
    qtk::MeshRenderer::getInstance("rightTriangle")->mTransform.rotate(0.75f, 0.0f, 0.0f, 1.0f);

    // Move between two positions over time
    static float translateX = 0.025f;
    float limit = -9.0f; // Origin position.x - 2.0f
    float posX = qtk::MeshRenderer::getInstance("topTriangle")->mTransform.translation().x();
    if (posX < limit || posX > limit + 4.0f)
    {
        translateX = -translateX;
    }
    qtk::MeshRenderer::getInstance("topTriangle")->mTransform.translate(translateX, 0.0f, 0.0f);
    qtk::MeshRenderer::getInstance("bottomTriangle")->mTransform.translate(-translateX, 0.0f, 0.0f);
    // And lets rotate the triangles in two directions at once
    qtk::MeshRenderer::getInstance("topTriangle")->mTransform.rotate(0.75f, 0.2f, 0.0f, 0.4f);
    qtk::MeshRenderer::getInstance("bottomTriangle")->mTransform.rotate(0.75f, 0.0f, 0.2f, 0.4f);
    // And make the bottom triangle green, instead of RGB

    // Rotate center cube in several directions simultaneously
    // + Not subject to gimbal lock since we are using quaternions :)
    qtk::MeshRenderer::getInstance("centerCube")->mTransform.rotate(0.75f, 0.2f, 0.4f, 0.6f);
}
