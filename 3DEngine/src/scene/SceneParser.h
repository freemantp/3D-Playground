#pragma once

#include "../util/SharedPointer.h"

#include <tinyxml2.h>
#include <map>
#include <string>
#include <glm/glm.hpp>

class InputHandlerFactory;

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Material);


class SceneParser
{
public:
	SceneParser();

	bool Parse(const std::string& xml_document);
	Scene_ptr Scene();

protected:

	inline bool GetIntAttrib(tinyxml2::XMLElement* element, const char* attrib_name, int& value);
	inline bool GetFloatAttrib(tinyxml2::XMLElement* element, const char* attrib_name, float& value);
	inline bool GetVector3(tinyxml2::XMLElement* element, glm::vec3& vec);
	inline bool GetColorVector3(tinyxml2::XMLElement* element, glm::vec3& vec);

	bool ParseMaterials(tinyxml2::XMLElement* material_element);
	bool ParseObjects(tinyxml2::XMLElement* material_element);
	bool ParseSkybox(tinyxml2::XMLElement* skybox);
	bool ParseCamera(Camera_ptr& cam, tinyxml2::XMLElement* material_element);
	bool ParseLights(tinyxml2::XMLElement* material_element);
	bool ParseTransforms(glm::mat4& tMatrix, tinyxml2::XMLElement* transform_elem);

	std::map<std::string, Material_ptr> materials;
	Scene_ptr generated_scene;
};

