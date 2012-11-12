#include "stdafx.h"
#include "SceneParser.h"

#include <string>
#include "shader/PhongShader.h"
#include "shader/DiffusePerVertexShader.h"
#include "Util.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using std::string;

typedef std::pair<string,ShaderBase*> ShaderKeyVal;

SceneParser::SceneParser()
{

}

Scene* SceneParser::getScene()
{
	return generatedScene;
}

bool SceneParser::parse(const char* xmlDocument)
{

	bool parseOk = true;
	
	tinyxml2::XMLDocument doc;
	if(doc.Parse(xmlDocument) == tinyxml2::XML_NO_ERROR)
	{
		vector<Shape*> shapes;
		vector<ShaderBase*> materials;
		vector<Camera*> cameras;
		vector<Light*> lights;
		
		XMLElement* root = doc.RootElement();

		if( strcmp( root->Name(), "scene" ) == 0 )
		{
			string sceneName(root->Attribute("name"));
			std::cout << "Parsing scene \"" << sceneName << "\":" << std::endl;

			//Camera
			XMLElement* cameraElement = root->FirstChildElement("camera");

			if( cameraElement == NULL)
				return false;
			
			Camera* cam = NULL;
			parseOk &= parseCamera(&cam,cameraElement);
			generatedScene = new Scene(cam);

			//Materials
			XMLElement* materialsElement = root->FirstChildElement("materials");
			if( cameraElement != NULL)
				parseMaterials(materialsElement);

			//Lights
			XMLElement* lightsElement = root->FirstChildElement("lights");
			if( lightsElement != NULL)
				parseLights(lightsElement);

			//Objects
			XMLElement* objectsElement = root->FirstChildElement("objects");
			if( objectsElement != NULL)
				parseObjects(objectsElement);
		}
		else 
		{
			Error("Not a valid scene file");
			parseOk = false;
		}
	}
	else
	{
		Error("File could not be parsed");
		parseOk = false;
	}

	return true;
}

bool SceneParser::parseMaterials(XMLElement* materialsGroupElement)
{
	XMLElement* materialElement = materialsGroupElement->FirstChildElement("material");

	do
	{
		string shaderName = materialElement->Attribute("shader");

		ShaderBase* shader;

		if(shaderName == "PhongShader")
		{
			shader = new PhongShader();
		}
		else if(shaderName == "DiffusePerVertexShader")
		{
			shader = new DiffusePerVertexShader();
		}
		else
		{
			Error("Shader " + shaderName + " not supported");
			return false;
		}

		shaders.insert(ShaderKeyVal(materialElement->Attribute("name"),shader));

	} 
	while (materialElement = materialElement->NextSiblingElement("material"));
	

	return true;
}

bool SceneParser::parseObjects(XMLElement* objects)
{
	//Meshes
	XMLElement* meshElem = objects->FirstChildElement("mesh");
	do
	{
		string file = meshElem->Attribute("file");
		Mesh* model = Util::loadModel("../data/models/"+file);
		ShaderBase* material = shaders[meshElem->Attribute("material")];

		if(material != NULL)		
		{
			model->setShader(material);
		}
		int a = 0;

		//TODO: parse transforms
		generatedScene->addShape(model);

		
	} 
	while (meshElem = meshElem->NextSiblingElement("mesh"));
	
	return true;
}


bool SceneParser::parseCamera(Camera** cam, tinyxml2::XMLElement* camElement)
{
	bool success = true;
	string type = camElement->Attribute("type");

	if(type == "perspective")
	{
		float fov;

		if( ! ( success &= getFloatAttrib(camElement,"fov",fov) )  )
			return false;

		*cam = new PerspectiveCamera(fov);
	}
	else //unknown camera type
	{
		Error("Camera type " + type + " not supported");
		return false;
	}

	vec3 pos, target, up;
	success &= getVector3( camElement->FirstChildElement("position"),pos);
	success &= getVector3( camElement->FirstChildElement("target"),target);
	success &= getVector3( camElement->FirstChildElement("up"),up);

	(*cam)->setPosition(pos);
	(*cam)->setTarget(target);
	(*cam)->setUpVector(up);

	return success;

}

bool SceneParser::parseLights(tinyxml2::XMLElement* lightsGroupElement)
{
	XMLElement* lightElem = lightsGroupElement->FirstChildElement("light");

	do
	{
		string lightType = lightElem->Attribute("type");

		if(lightType == "point")
		{
			PointLight* plight = new PointLight();
			vec3 pos, color;
			getVector3(lightElem->FirstChildElement("position"),pos);
			getColorVector3(lightElem->FirstChildElement("color"),color);

			plight->position = vec4(pos,1.0);
			plight->color = color;

			generatedScene->addLight(plight);
			
		}
		else
		{
			Error("Light " + lightType + " not supported");
			return false;
		}
	} 
	while (lightElem = lightElem->NextSiblingElement("light"));
	
	return true;
}

bool SceneParser::getVector3(XMLElement* element, vec3& vec)
{
	if(element == NULL)
		return false;
	
	bool success = true;

	success &= getFloatAttrib(element,"x",vec.x);
	success &= getFloatAttrib(element,"y",vec.y);
	success &= getFloatAttrib(element,"z",vec.z);

	return success;
}

bool SceneParser::getColorVector3(XMLElement* element, vec3& vec)
{
	if(element == NULL)
		return false;
	
	bool success = true;

	success &= getFloatAttrib(element,"r",vec.x);
	success &= getFloatAttrib(element,"g",vec.y);
	success &= getFloatAttrib(element,"b",vec.z);

	return success;
}


bool SceneParser::getFloatAttrib(XMLElement* element, const char* attribName, float& value)
{
	if(element == NULL)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}