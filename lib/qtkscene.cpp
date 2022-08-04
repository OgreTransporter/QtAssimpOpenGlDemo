#include "qtkscene.h"

namespace qtk
{
	Camera3D Scene::mCamera;
	QMatrix4x4 Scene::mProjection;

	Scene::Scene()
		: mInit(false)
		, mSkybox(nullptr)
	{
	}

	Scene::~Scene()
	{
		for (auto& mesh : mMeshes) delete mesh;
		for (auto& model : mModels) delete model;
		if (mSkybox != nullptr) delete mSkybox;
	}

	Camera3D& Scene::Camera()
	{
		return mCamera;
	}

	QMatrix4x4 Scene::View()
	{
		return mCamera.toMatrix();
	}

	QMatrix4x4& Scene::Projection()
	{
		return mProjection;
	}

	Skybox* Scene::skybox()
	{
		return mSkybox;
	}

	void Scene::setSkybox(Skybox* skybox)
	{
		mSkybox = skybox;
	}

	void Scene::privDraw()
	{
		if (!mInit)
		{
			initializeOpenGLFunctions();
			init();
			mInit = true;
		}
		if (mSkybox != nullptr) mSkybox->draw();
		for (auto& model : mModels) model->draw();
		for (const auto& mesh : mMeshes) mesh->draw();
		draw();
	}

	void Scene::privUpdate()
	{
		update();
	}
}
