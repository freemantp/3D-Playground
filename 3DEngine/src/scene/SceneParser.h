#pragma once

#include <tinyxml2.h>
#include <map>
#include <string>
#include "Scene.h"

using tinyxml2::XMLElement;

class ShaderBase;
class Camera;
class InputHandlerFactory;

class SceneParser
{
public:

	SceneParser(InputHandlerFactory& factory);

	bool parse(const char* xmlDocument);
	std::shared_ptr<Scene> getScene();

protected:

	
	inline bool getIntAttrib(XMLElement* element, const char* attribName, int& value);
	inline bool getFloatAttrib(XMLElement* element, const char* attribName, float& value);
	bool getVector3(XMLElement* element, glm::vec3& vec);
	bool getColorVector3(XMLElement* element, glm::vec3& vec);

	bool parseMaterials(XMLElement* materialElement);
	bool parseObjects(XMLElement* materialElement);
	bool parseSkybox(XMLElement* skybox);
	bool parseCamera(Camera** cam, XMLElement* materialElement);
	bool parseLights(XMLElement* materialElement);
	bool parseTransforms(glm::mat4& tMatrix, XMLElement* transformElem);

	std::map<std::string, ShaderBase*> shaders;
	std::shared_ptr<Scene> generatedScene;

	InputHandlerFactory& factory;

};

