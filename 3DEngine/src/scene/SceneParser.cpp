#include "stdafx.h"
#include "SceneParser.h"

#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "../shader/PhongShader.h"
#include "../shader/DiffuseShader.h"
#include "../shader/ColorShader.h"
#include "../shader/ConstShader.h"

#include "../util/Util.h"
#include "../camera/PerspectiveCamera.h"
#include "../scene/Scene.h"

#include "../shape/Mesh.h"
#include "../shape/Box.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using std::string;
using std::vector;

typedef std::pair<string,ShaderBase*> ShaderKeyVal;

SceneParser::SceneParser(InputHandlerFactory& factory) : factory(factory)
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
			generatedScene = new Scene(factory,cam);

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
			PhongShader* ps = new PhongShader();

			XMLElement* subElem;

			if ( (subElem = materialElement->FirstChildElement("ambientReflect")) != NULL )
				getColorVector3(subElem,ps->ambientReflection);

			if ( (subElem = materialElement->FirstChildElement("diffuseReflect")) != NULL )
				getColorVector3(subElem,ps->diffuseReflection);

			if ( (subElem = materialElement->FirstChildElement("glossyReflect")) != NULL )
				getColorVector3(subElem,ps->glossyReflection);

			if ( (subElem = materialElement->FirstChildElement("shininess")) != NULL )
				getIntAttrib(subElem,"value",ps->shininess);
			

			shader = ps;
		}
		else if(shaderName == "DiffuseShader")
		{
			DiffuseShader* dpvs = new DiffuseShader();
			XMLElement* colorElem = materialElement->FirstChildElement("color");
			if(colorElem != NULL)
			{
				getColorVector3(colorElem,dpvs->materialColor);
			}

			shader = dpvs;

		}
		else if(shaderName == "ColorShader")
		{
			shader = new ColorShader();
		}
		else if(shaderName == "ConstShader")
		{
			shader = new ConstShader();
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
	XMLElement* objeElem = objects->FirstChildElement();
	
	if(objeElem == NULL)
		return false;

	do
	{
		string type = objeElem->Name();
		Shape* s;

		if(type == "mesh")
		{
			string file = objeElem->Attribute("file");
			s = Util::loadModel("../data/models/"+file);		
		}
		else if(type == "box")
		{
			s = new Box();		
		}
		else 
		{
			Error("Object of type " + type + " not supported");
			continue;
		}

		s->init();

		const char* shaderName = objeElem->Attribute("material");

		if(shaderName != NULL)
		{
			ShaderBase* material = shaders[shaderName];

			if(material != NULL)		
			{
				s->setShader(material);
			}
		}


		XMLElement* transformsElem = objeElem->FirstChildElement("transform");

		if(transformsElem != NULL)
		{
			mat4 tMatrix;
			parseTransforms(tMatrix,transformsElem);
			s->worldTransform = tMatrix;
		}

		generatedScene->addShape(s);

		
	} 
	while (objeElem = objeElem->NextSiblingElement());
	
	return true;
}

bool SceneParser::parseTransforms(mat4& tMatrix, tinyxml2::XMLElement* transformElem)
{
	
	bool success = true;
	
	XMLElement* transform = transformElem->FirstChildElement();
	do
	{
		string type = transform->Name();

		if(type == "translate")
		{
			vec3 transl;
			if( getVector3(transform,transl) )
			{
				tMatrix = glm::translate(tMatrix,transl);
			}
			else
			{
				Error("Could not parse translation");
				success = false;
			}
		}
		else if(type == "rotate")
		{
			vec3 axis;
			float angle;
			if( getVector3(transform,axis) && getFloatAttrib(transform, "angle",angle) )
			{
				tMatrix = glm::rotate(tMatrix,angle,axis);
			}
			else
			{
				Error("Could not parse rotation");
				success = false;
			}
		}
		else if(type == "scale")
		{
			vec3 factors;
			if( getVector3(transform,factors) )
			{
				tMatrix = glm::scale(tMatrix,factors);
			}
			else
			{
				Error("Could not parse scale");
				success = false;
			}
		}

	} 
	while (transform = transform->NextSiblingElement());

	return success;

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

			plight->setPosition(vec4(pos,1.0));
			plight->setColor(color);

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

bool SceneParser::getIntAttrib(XMLElement* element, const char* attribName, int& value)
{
	if(element == NULL)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}