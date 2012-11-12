#pragma once

#include <tinyxml2.h>
#include "Scene.h"
#include <map>

using tinyxml2::XMLElement;

class SceneParser
{
public:

	SceneParser();

	bool parse(const char* xmlDocument);
	Scene* getScene();

protected:

	inline bool getFloatAttrib(XMLElement* element, const char* attribName, float& value);
	bool getVector3(XMLElement* element, vec3& vec);
	bool getColorVector3(XMLElement* element, vec3& vec);

	bool parseMaterials(XMLElement* materialElement);
	bool parseObjects(XMLElement* materialElement);
	bool parseCamera(Camera** cam, XMLElement* materialElement);
	bool parseLights(XMLElement* materialElement);
	bool parseTransforms(mat4& tMatrix, tinyxml2::XMLElement* transformElem);

	std::map<string, ShaderBase*> shaders;
	Scene* generatedScene;

};

