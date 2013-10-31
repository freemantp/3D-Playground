#include "stdafx.h"
#include "SceneParser.h"

#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "../shader/PhongShader.h"
#include "../shader/PhongTextureShader.h"
#include "../shader/ColorShader.h"
#include "../shader/ConstShader.h"
#include "../shader/SHDiffuseShader.h"
#include "../texture/CubeMapTexture.h"

#include "../util/Util.h"
#include "../camera/PerspectiveCamera.h"
#include "../scene/Scene.h"

#include "../shape/Mesh.h"
#include "../shape/Box.h"
#include "../shape/Skybox.h"

#include "../light/SpotLight.h"
#include "../config.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using std::string;
using std::vector;

typedef std::pair<string,ShaderBase_ptr> ShaderKeyVal;

SceneParser::SceneParser(InputHandlerFactory& factory) 
	: factory(factory)
{

}

Scene_ptr SceneParser::getScene()
{
	return generatedScene;
}

bool SceneParser::parse(const char* xmlDocument)
{
	bool parseOk = true;
	
	tinyxml2::XMLDocument doc;
	if(doc.Parse(xmlDocument) == tinyxml2::XML_NO_ERROR)
	{	
		XMLElement* root = doc.RootElement();

		if( strcmp( root->Name(), "scene" ) == 0 )
		{
			string sceneName(root->Attribute("name"));
			std::cout << "Parsing scene \"" << sceneName << "\":" << std::endl;

			//Camera
			XMLElement* cameraElement;

			if(cameraElement = root->FirstChildElement("camera"))
			{
				Camera_ptr cam;
				parseOk &= parseCamera(cam,cameraElement);
				try
				{
					generatedScene = Scene::Create(factory,cam);
					generatedScene->name = sceneName;
				}
				catch (std::exception* e)
				{
					Error(e->what());
					return false;
				}		
			}
			else
			{
				return false;
			}

			//Materials
			XMLElement* materialsElement = root->FirstChildElement("materials");
			if( cameraElement != nullptr) 
			{
				if( ! parseMaterials(materialsElement) )
					return false;
			}

			//Lights
			XMLElement* lightsElement = root->FirstChildElement("lights");
			if( lightsElement != nullptr)
			{
				if( ! parseLights(lightsElement) )
					return false;
			}

			//Skybox
			XMLElement* skyboxElement = root->FirstChildElement("skybox");
			if( skyboxElement != nullptr)
			{
				if( ! parseSkybox(skyboxElement) )
					return false;
			}

			//Objects
			if( XMLElement* objectsElement = root->FirstChildElement("objects"))
			{
				if( ! parseObjects(objectsElement) )
					return false;
			}

		}
		else 
		{
			Error("Not a valid scene file");
			parseOk = false;
		}
	}
	else
	{
		Error("File could not be parsed" );
		parseOk = false;
	}

	return parseOk;
}

bool SceneParser::parseMaterials(XMLElement* materialsGroupElement)
{
	XMLElement* materialElement = materialsGroupElement->FirstChildElement("material");

	do
	{
		string shaderName = materialElement->Attribute("shader");

		ShaderBase_ptr shader;
		
		if(shaderName == "phong")
		{			
			PhongShader_ptr ps;
			XMLElement* subElem;

			//Load textured version if available
			if(subElem = materialElement->FirstChildElement("texture"))
			{
				string texFile(subElem->Attribute("file"));
			
				auto pbs = PhongTextureShader::Create(texFile);

				//bump mapping		
				if(subElem = materialElement->FirstChildElement("bumpMap"))
				{
					string bumpMapFile(subElem->Attribute("file"));
					string type(subElem->Attribute("type"));
					pbs->setBumpMap(bumpMapFile, type == "normal");
				}

				//specular mapping				
				if(subElem = materialElement->FirstChildElement("specularMap"))
				{
					string specMapFile(subElem->Attribute("file"));
					pbs->setSpecularMap(specMapFile);
				}
				ps = pbs;
			}
			else
			{
				ps = PhongShader::Create();
			}
			
			//Load common phong attributes
			if ( subElem = materialElement->FirstChildElement("ambientReflect") )
				getColorVector3(subElem,ps->ambientReflection);

			if ( subElem = materialElement->FirstChildElement("diffuseReflect") )
				getColorVector3(subElem,ps->diffuseReflection);

			if ( subElem = materialElement->FirstChildElement("glossyReflect"))
				getColorVector3(subElem,ps->glossyReflection);

			if ( subElem = materialElement->FirstChildElement("shininess") )
				getIntAttrib(subElem,"value",ps->shininess);

			shader = ps;
		}
		else if(shaderName == "color")
		{
			shader.reset(new ColorShader());
		}
		else if(shaderName == "const")
		{
			ConstShader_ptr cshader(new ConstShader(vec3(1.0)));
			
			XMLElement* subElem;
			if ( subElem = materialElement->FirstChildElement("color") )
				getColorVector3(subElem,cshader->color);

			shader = cshader;
		}
		else if (shaderName == "diffuseSH")
		{
			shader = ShDiffuseShader::Create();
		}
		else
		{
			Error("Shader " + shaderName + " not supported");
			return false;
		}

		if( ! shader->isLinked() )
				return false;

		shaders.insert(ShaderKeyVal(materialElement->Attribute("name"),shader));

	} 
	while (materialElement = materialElement->NextSiblingElement("material"));
	

	return true;
}

bool SceneParser::parseSkybox(XMLElement* skyboxElem)
{
	CubeMapTexture_ptr texture;

	const char* path = skyboxElem->Attribute("cubeMapFile");
	if( path = skyboxElem->Attribute("cubeMapFile"))
	{
		//Single file cube map
		texture.reset(new CubeMapTexture(Config::TEXTURE_BASE_PATH + path));
	}
	else if( path = skyboxElem->Attribute("cubeMapFolder") )
	{
		//Cube map consisting of 6 files
		string type = skyboxElem->Attribute("type");
		texture.reset(new CubeMapTexture(Config::TEXTURE_BASE_PATH + path, type));
	}
	else
	{
		return false;
	}

	Skybox_ptr sb = Skybox_ptr(new Skybox(texture));
	sb->init();
	generatedScene->setSkybox(sb);

	return true;
}

bool SceneParser::parseObjects(XMLElement* objects)
{
	//Parse children of 'objects'
	if(XMLElement* objeElem = objects->FirstChildElement())
	{
		do
		{
			string type = objeElem->Name();
			Shape_ptr shape;

			if(type == "mesh")
			{
				string file = objeElem->Attribute("file");
				shape = Util::loadModel("../data/models/"+file);
				if(!shape)
					return false;
			}
			else if(type == "box")
			{
				shape = Box_ptr(new Box());
			}
			else 
			{
				Error("Object of type " + type + " not supported");
				continue;
			}

			shape->init();

			//See if a material is specified
			if(const char* materialName = objeElem->Attribute("material"))
			{
				if(shape->getShader())
				{
					Warn("The shape already has a material assigned, ignoring specified material");	
				}
				else
				{
					if(ShaderBase_ptr material = shaders[materialName])		
						shape->setShader(material);
					else
						Warn("The specified material " + string(materialName) +" is not defined");	
				}
			}

			// Parse transform node
			if(XMLElement* transformsElem = objeElem->FirstChildElement("transform"))
			{
				mat4 tMatrix;
				parseTransforms(tMatrix,transformsElem);
				shape->worldTransform = tMatrix;
			}

			generatedScene->addShape(shape);		
		} 
		while (objeElem = objeElem->NextSiblingElement());
	}
	
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

bool SceneParser::parseCamera(Camera_ptr& cam, tinyxml2::XMLElement* camElement)
{
	bool success = true;
	string type = camElement->Attribute("type");

	if(type == "perspective")
	{
		float fov;

		if( ! ( success &= getFloatAttrib(camElement,"fov",fov) )  )
			return false;

		cam.reset(new PerspectiveCamera(fov));
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

	cam->SetPosition(pos);
	cam->SetTarget(target);
	cam->SetUpVector(up);

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
			PointLight_ptr plight(new PointLight());
			vec3 pos, color;

			getVector3(lightElem->FirstChildElement("position"),pos);
			getColorVector3(lightElem->FirstChildElement("color"),color);
			
			plight->SetPosition(vec4(pos,1.0));
			plight->SetColor(color);

			generatedScene->addLight(plight);
			
		}
		else if(lightType == "spot")
		{
			
			vec3 pos, color, direction;
			float cutoff, exponent = 5;
			getVector3(lightElem->FirstChildElement("position"),pos);
			getVector3(lightElem->FirstChildElement("direction"),direction);
			getColorVector3(lightElem->FirstChildElement("color"),color);

			getFloatAttrib(lightElem,"cutoff",cutoff);
			getFloatAttrib(lightElem,"exponent",exponent);

			SpotLight_ptr slight(new SpotLight(direction,cutoff,exponent));

			slight->SetPosition(vec4(pos,1.0));
			slight->SetColor(color);

			generatedScene->addLight(slight);
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
	if(element == nullptr)
		return false;
	
	bool success = true;

	success &= getFloatAttrib(element,"x",vec.x);
	success &= getFloatAttrib(element,"y",vec.y);
	success &= getFloatAttrib(element,"z",vec.z);

	return success;
}

bool SceneParser::getColorVector3(XMLElement* element, vec3& vec)
{
	if(element == nullptr)
		return false;
	
	bool success = true;

	success &= getFloatAttrib(element,"r",vec.x);
	success &= getFloatAttrib(element,"g",vec.y);
	success &= getFloatAttrib(element,"b",vec.z);

	return success;
}

bool SceneParser::getFloatAttrib(XMLElement* element, const char* attribName, float& value)
{
	if(element == nullptr)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}

bool SceneParser::getIntAttrib(XMLElement* element, const char* attribName, int& value)
{
	if(element == nullptr)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}