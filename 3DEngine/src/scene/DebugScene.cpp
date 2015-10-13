#include "stdafx.h"
#include "DebugScene.h"

#include "../materials/Material.h"
#include  "../util/Util.h"
#include  "../util/TimeManager.h"
#include "../camera/OrthogonalCamera.h"
#include  "../shape/RenderMesh.h"
#include  "../light/SpotLight.h"
#include  "../light/DirectionalLight.h"
#include  "../light/Shadow.h"
#include  "../texture/DepthTexture.h"
#include  "../texture/Texture2D.h"

#include <memory>

DebugScene::DebugScene(Scene_ptr scene)
: Scene(std::shared_ptr<OrthogonalCamera>(new OrthogonalCamera()),false)
{
	if (scene)
	{
		auto dirLight = DirectionalLight::Create(false);
		dirLight->SetDirection(glm::vec3(0, 0, -1));

		lightModel->directionalLight = dirLight;

		const int spacing = 50;
		const float width = 400.f;

		float current_x = width / 2 + spacing;

		auto add_shadowmap_box = [&](Shadow_ptr shadow)
		{
			if(auto dt = shadow->ShadowMap())
			{
				auto dim = dt->Dimensions();

				float height = width / dim.x * dim.y;

				auto box = Util::CreateBox();
				DepthMapMaterial_ptr mat = DepthMapMaterial::Create();
				mat->depthTexture = dt;
				mat->nearPlane = shadow->NearPlane();
				mat->farPlane = shadow->FarPlane();
				mat->perspective = shadow->Type() == Shadow::ProjectionType::Perspective;

				box->SetMaterial(mat);

				box->worldTransform = glm::translate(glm::mat4(1), glm::vec3(current_x, height / 2 + spacing, 0));
				box->worldTransform = glm::scale(box->worldTransform, glm::vec3(width, height, 1));
				box->worldTransform = glm::rotate(box->worldTransform, -glm::half_pi<float>(), glm::vec3(0, 0, 1));

				AddShape(box);

				current_x += width + spacing;
			}
		};

		for (auto sl : scene->lightModel->spotLights)
		{
			add_shadowmap_box(sl->Shadow());	
		}

		if(scene->lightModel->directionalLight)
			add_shadowmap_box(scene->lightModel->directionalLight->Shadow());
	}
}


DebugScene::~DebugScene()
{
}

DebugScene_ptr DebugScene::Create(Scene_ptr scene)
{
	return DebugScene_ptr(new DebugScene(scene), [](DebugScene* d) {delete d;});
}
