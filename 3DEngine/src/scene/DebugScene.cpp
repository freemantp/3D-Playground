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
#include <glm/gtc/matrix_transform.hpp>

SceneOverlay2D::SceneOverlay2D(Scene_ptr scene)
: Scene(std::shared_ptr<OrthogonalCamera>(new OrthogonalCamera()),false)
{
	renderBoundingBoxes = false;
	renderLightRepresentation = false;

	if (scene)
	{
		lightModel->directionalLight = DirectionalLight::Create(glm::vec3(0, 0, -1), false);

		const int spacing = 50;
		const float width = 400.f;

		float current_x = width / 2 + spacing;

		auto add_shadowmap_box = [&](Shadow_ptr shadow)
		{
			if(DepthTexture_ptr dt = shadow->ShadowMap())
			{
				auto dim = dt->Dimensions();

				float height = width / dim.x * dim.y;

				auto canvas_box = Util::CreateBox();
				DepthMapMaterial_ptr mat = DepthMapMaterial::Create();
				mat->depthTexture = dt;
				mat->nearPlane = shadow->NearPlane();
				mat->farPlane = shadow->FarPlane();
				mat->perspective = (shadow->Type() == Shadow::ProjectionType::Perspective);

				canvas_box->SetMaterial(mat);

				glm::mat4 t = glm::translate(glm::mat4(1), glm::vec3(current_x, height / 2 + spacing, 0));
				t = glm::scale(t, glm::vec3(width, height, 1));
				t = glm::rotate(t, -glm::half_pi<float>(), glm::vec3(0, 0, 1));
				canvas_box->SetWorldTransform(t);

				AddShape(canvas_box);

				current_x += width + spacing;
			}
		};

		for (auto& sl : scene->lightModel->spotLights)
		{
			add_shadowmap_box(sl->Shadow());	
		}

		if(scene->lightModel->directionalLight)
			add_shadowmap_box(scene->lightModel->directionalLight->Shadow());
	}
}


SceneOverlay2D::~SceneOverlay2D()
{
}

SceneOverlay2D_ptr SceneOverlay2D::Create(Scene_ptr scene)
{
	return SceneOverlay2D_ptr(new SceneOverlay2D(scene), [](SceneOverlay2D* d) {delete d;});
}

void SceneOverlay2D::TimeUpdate(double time)
{
}
