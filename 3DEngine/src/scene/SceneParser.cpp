#include "stdafx.h"
#include "SceneParser.h"

#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "../shader/PhongShader.h"
#include "../shader/PhongTextureShader.h"
#include "../shader/ColorShader.h"
#include "../shader/ConstShader.h"
#include "../shader/SHDiffuseShader.h"
#include "../texture/Texture.h"
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

Scene_ptr SceneParser::GetScene()
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
				parseOk &= ParseCamera(cam,cameraElement);
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
				if( ! ParseMaterials(materialsElement) )
					return false;
			}

			//Lights
			XMLElement* lightsElement = root->FirstChildElement("lights");
			if( lightsElement != nullptr)
			{
				if( ! ParseLights(lightsElement) )
					return false;
			}

			//Skybox
			XMLElement* skyboxElement = root->FirstChildElement("skybox");
			if( skyboxElement != nullptr)
			{
				if( ! ParseSkybox(skyboxElement) )
					return false;
			}

			//Objects
			if( XMLElement* objectsElement = root->FirstChildElement("objects"))
			{
				if( ! ParseObjects(objectsElement) )
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

bool SceneParser::ParseMaterials(XMLElement* materialsGroupElement)
{
	XMLElement* materialElement = materialsGroupElement->FirstChildElement("material");

	do
	{
		string shaderType = materialElement->Attribute("shader");
		string materialName = materialElement->Attribute("name");

		ShaderBase_ptr shader;
		
		if(shaderType == "phong")
		{			
			PhongShader_ptr ps;
			XMLElement* subElem;

			//Load textured version if available
			if(subElem = materialElement->FirstChildElement("texture"))
			{
				string texFile(subElem->Attribute("file"));
			
				Texture_ptr albedo = Texture::Create(Config::TEXTURE_BASE_PATH + texFile);

				auto pbs = PhongTextureShader::Create(albedo);

				//bump mapping		
				if(subElem = materialElement->FirstChildElement("bumpMap"))
				{
					string bumpMapFile(subElem->Attribute("file"));
					string type(subElem->Attribute("type"));

					Texture_ptr bumpMap = Texture::Create(Config::TEXTURE_BASE_PATH + bumpMapFile);
					pbs->SetBumpMap(bumpMap, type == "normal");
				}

				//specular mapping				
				if(subElem = materialElement->FirstChildElement("specularMap"))
				{
					string specMapFile(subElem->Attribute("file"));

					Texture_ptr specularMap = Texture::Create(Config::TEXTURE_BASE_PATH + specMapFile);
					pbs->SetSpecularMap(specularMap);
				}
				ps = pbs;
			}
			else
			{
				ps = PhongShader::Create();
			}
			
			//Load common phong attributes
			if ( subElem = materialElement->FirstChildElement("ambientReflect") )
				GetColorVector3(subElem,ps->ambientReflection);

			if ( subElem = materialElement->FirstChildElement("diffuseReflect") )
				GetColorVector3(subElem,ps->diffuseReflection);

			if ( subElem = materialElement->FirstChildElement("glossyReflect"))
				GetColorVector3(subElem,ps->glossyReflection);

			if ( subElem = materialElement->FirstChildElement("shininess") )
				GetIntAttrib(subElem,"value",ps->shininess);

			shader = ps;
		}
		else if(shaderType == "color")
		{
			shader.reset(new ColorShader());
		}
		else if(shaderType == "const")
		{
			ConstShader_ptr cshader(new ConstShader(vec3(1.0)));
			
			XMLElement* subElem;
			if ( subElem = materialElement->FirstChildElement("color") )
				GetColorVector3(subElem,cshader->color);

			shader = cshader;
		}
		else if (shaderType == "diffuseSH")
		{
			shader = ShDiffuseShader::Create();
		}
		else
		{
			Error("Shader " + shaderType + " not supported");
			return false;
		}

		if( ! shader->isLinked() )
				return false;

		shaders.insert(ShaderKeyVal(materialElement->Attribute("name"),shader));

	} 
	while (materialElement = materialElement->NextSiblingElement("material"));
	

	return true;
}

bool SceneParser::ParseSkybox(XMLElement* skyboxElem)
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
	sb->Init();
	generatedScene->SetSkybox(sb);

	return true;
}

bool SceneParser::ParseObjects(XMLElement* objects)
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

				if (Mesh_ptr loadedMesh = Util::LoadModel("../data/models/" + file))
				{					
					// Mesh has material descriptors
					if (loadedMesh->GetMaterials().size() > 0)
					{
						Info("Mesh has own materials");
					}

					shape = loadedMesh;
				}					
				else
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

			shape->Init();

			//See if a material is specified
			if(const char* materialName = objeElem->Attribute("material"))
			{
				if(shape->GetShader())
				{
					Warn("The shape already has a material assigned, ignoring specified material");	
				}
				else
				{
					if(ShaderBase_ptr material = shaders[materialName])		
						shape->SetShader(material);
					else
						Warn("The specified material " + string(materialName) +" is not defined");	
				}
			}
			else // Look whether 
			{
				shape->GetShader();
			}

			// Parse transform node
			if(XMLElement* transformsElem = objeElem->FirstChildElement("transform"))
			{
				mat4 tMatrix;
				ParseTransforms(tMatrix,transformsElem);
				shape->worldTransform = tMatrix;
			}

			generatedScene->AddShape(shape);		
		} 
		while (objeElem = objeElem->NextSiblingElement());
	}
	
	return true;
}

bool SceneParser::ParseTransforms(mat4& tMatrix, tinyxml2::XMLElement* transformElem)
{
	
	bool success = true;
	
	XMLElement* transform = transformElem->FirstChildElement();
	do
	{
		string type = transform->Name();

		if(type == "translate")
		{
			vec3 transl;
			if( GetVector3(transform,transl) )
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
			if( GetVector3(transform,axis) && GetFloatAttrib(transform, "angle",angle) )
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
			if( GetVector3(transform,factors) )
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

bool SceneParser::ParseCamera(Camera_ptr& cam, tinyxml2::XMLElement* camElement)
{
	bool success = true;
	string type = camElement->Attribute("type");

	if(type == "perspective")
	{
		float fov;

		if( ! ( success &= GetFloatAttrib(camElement,"fov",fov) )  )
			return false;

		cam.reset(new PerspectiveCamera(fov));
	}
	else //unknown camera type
	{
		Error("Camera type " + type + " not supported");
		return false;
	}

	vec3 pos, target, up;
	success &= GetVector3( camElement->FirstChildElement("position"),pos);
	success &= GetVector3( camElement->FirstChildElement("target"),target);
	success &= GetVector3( camElement->FirstChildElement("up"),up);

	cam->SetPosition(pos);
	cam->SetTarget(target);
	cam->SetUpVector(up);

	return success;

}

bool SceneParser::ParseLights(tinyxml2::XMLElement* lightsGroupElement)
{
	XMLElement* lightElem = lightsGroupElement->FirstChildElement("light");

	do
	{
		string lightType = lightElem->Attribute("type");

		if(lightType == "point")
		{
			PointLight_ptr plight(new PointLight());
			vec3 pos, color;

			GetVector3(lightElem->FirstChildElement("position"),pos);
			GetColorVector3(lightElem->FirstChildElement("color"),color);
			
			plight->SetPosition(vec4(pos,1.0));
			plight->SetColor(color);

			generatedScene->AddLight(plight);
			
		}
		else if(lightType == "spot")
		{
			
			vec3 pos, color, direction;
			float cutoff, exponent = 5;
			GetVector3(lightElem->FirstChildElement("position"),pos);
			GetVector3(lightElem->FirstChildElement("direction"),direction);
			GetColorVector3(lightElem->FirstChildElement("color"),color);

			GetFloatAttrib(lightElem,"cutoff",cutoff);
			GetFloatAttrib(lightElem,"exponent",exponent);

			SpotLight_ptr slight(new SpotLight(direction,cutoff,exponent));

			slight->SetPosition(vec4(pos,1.0));
			slight->SetColor(color);

			generatedScene->AddLight(slight);
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

bool SceneParser::GetVector3(XMLElement* element, vec3& vec)
{
	if(element == nullptr)
		return false;
	
	bool success = true;

	success &= GetFloatAttrib(element,"x",vec.x);
	success &= GetFloatAttrib(element,"y",vec.y);
	success &= GetFloatAttrib(element,"z",vec.z);

	return success;
}

bool SceneParser::GetColorVector3(XMLElement* element, vec3& vec)
{
	if(element == nullptr)
		return false;
	
	bool success = true;

	success &= GetFloatAttrib(element,"r",vec.x);
	success &= GetFloatAttrib(element,"g",vec.y);
	success &= GetFloatAttrib(element,"b",vec.z);

	return success;
}

bool SceneParser::GetFloatAttrib(XMLElement* element, const char* attribName, float& value)
{
	if(element == nullptr)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}

bool SceneParser::GetIntAttrib(XMLElement* element, const char* attribName, int& value)
{
	if(element == nullptr)
		return false;
	
	std::istringstream isstr(element->Attribute(attribName));
	return !(isstr >> value).fail();
}