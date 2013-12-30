#pragma once

#include <tinyxml2.h>
#include <map>
#include <string>
#include "../util/SharedPointer.h"
#include <glm/glm.hpp>

using tinyxml2::XMLElement;

class InputHandlerFactory;

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(ShaderBase);

class SceneParser
{
public:

	SceneParser(InputHandlerFactory& factory);

	bool parse(const char* xmlDocument);
	Scene_ptr GetScene();

protected:

	inline bool GetIntAttrib(XMLElement* element, const char* attribName, int& value);
	inline bool GetFloatAttrib(XMLElement* element, const char* attribName, float& value);
	bool GetVector3(XMLElement* element, glm::vec3& vec);
	bool GetColorVector3(XMLElement* element, glm::vec3& vec);

	bool ParseMaterials(XMLElement* materialElement);
	bool ParseObjects(XMLElement* materialElement);
	bool ParseSkybox(XMLElement* skybox);
	bool ParseCamera(Camera_ptr& cam, XMLElement* materialElement);
	bool ParseLights(XMLElement* materialElement);
	bool ParseTransforms(glm::mat4& tMatrix, XMLElement* transformElem);

	std::map<std::string, ShaderBase_ptr> shaders;
	Scene_ptr generatedScene;

	InputHandlerFactory& factory;

};

