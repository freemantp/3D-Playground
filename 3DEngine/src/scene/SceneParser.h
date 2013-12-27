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
	Scene_ptr getScene();

protected:

	
	inline bool getIntAttrib(XMLElement* element, const char* attribName, int& value);
	inline bool getFloatAttrib(XMLElement* element, const char* attribName, float& value);
	bool getVector3(XMLElement* element, glm::vec3& vec);
	bool getColorVector3(XMLElement* element, glm::vec3& vec);

	bool parseMaterials(XMLElement* materialElement);
	bool parseObjects(XMLElement* materialElement);
	bool parseSkybox(XMLElement* skybox);
	bool parseCamera(Camera_ptr& cam, XMLElement* materialElement);
	bool parseLights(XMLElement* materialElement);
	bool parseTransforms(glm::mat4& tMatrix, XMLElement* transformElem);

	std::map<std::string, ShaderBase_ptr> shaders;
	Scene_ptr generatedScene;

	InputHandlerFactory& factory;

};

