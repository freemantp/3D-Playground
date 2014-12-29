#include "stdafx.h"
#include "SceneParser.h"

#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "../shader/PhongShader.h"
#include "../shader/PhongTextureShader.h"
#include "../shader/IntrinsicColorShader.h"
#include "../shader/SkyboxShader.h"
#include "../shader/ConstColorShader.h"
#include "../shader/SHDiffuseShader.h"
#include "../shader/ShaderLibrary.h"

#include "../texture/Texture2D.h"
#include "../texture/CubeMapTexture.h"

#include "../materials/Material.h"

#include "../util/Util.h"
#include "../util/MeshRaw.h"
#include "../util/ShCoeffParser.h"

#include "../camera/PerspectiveCamera.h"
#include "../scene/Scene.h"

#include "../shape/Mesh.h"
#include "../shape/Box.h"
#include "../shape/Skybox.h"

#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../config.h"

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using std::string;
using std::vector;

typedef std::pair<string,Material_ptr> ShaderKeyVal;

SceneParser::SceneParser(InputHandlerFactory& factory) 
	: factory(factory)
{

}

Scene_ptr SceneParser::GetScene()
{
	return generatedScene;
}

bool SceneParser::parse(const std::string& xmlDocument)
{
	bool parseOk = true;
	
	tinyxml2::XMLDocument doc;
	if(doc.Parse(xmlDocument.c_str()) == tinyxml2::XML_NO_ERROR)
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
	if (XMLElement* materialElement = materialsGroupElement->FirstChildElement("material"))
	{
		ShaderLibrary_ptr sl = ShaderLibrary::GetInstance();

		do
		{
			string materialType = materialElement->Attribute("shader");
			string materialName = materialElement->Attribute("name");

			Material_ptr material;

			if (materialType == "phong")
			{
				PhongMaterial_ptr phongMat;
				XMLElement* subElem;

				//Load textured version if available
				if (subElem = materialElement->FirstChildElement("texture"))
				{					
					string texFile(subElem->Attribute("file"));
					Texture2D_ptr albedoTex = Texture2D::Create(Config::TEXTURE_BASE_PATH + texFile);

					TextureMaterial_ptr tm = TextureMaterial::Create();
					tm->albedoTexture = albedoTex;

					//bump mapping		
					if (subElem = materialElement->FirstChildElement("bumpMap"))
					{
						string bumpMapFile(subElem->Attribute("file"));
						string type(subElem->Attribute("type"));

						tm->bumpTexture = Texture2D::Create(Config::TEXTURE_BASE_PATH + bumpMapFile);
						tm->bumpIsNormalMap = (type == "normal");
					}

					//specular mapping				
					if (subElem = materialElement->FirstChildElement("specularMap"))
					{
						string specMapFile(subElem->Attribute("file"));

						tm->specularTexture = Texture2D::Create(Config::TEXTURE_BASE_PATH + specMapFile);
					}

					phongMat = tm;
				}
				else
				{
					phongMat = PhongMaterial::Create();
				}

				//Load common phong attributes
				if (subElem = materialElement->FirstChildElement("ambientReflect"))
					GetColorVector3(subElem, phongMat->ambientReflection);

				if (subElem = materialElement->FirstChildElement("diffuseReflect"))
					GetColorVector3(subElem, phongMat->diffuseReflection);

				if (subElem = materialElement->FirstChildElement("glossyReflect"))
					GetColorVector3(subElem, phongMat->glossyReflection);

				if (subElem = materialElement->FirstChildElement("shininess"))
					GetIntAttrib(subElem, "value", phongMat->shininess);

				material = phongMat;
			}
			else if (materialType == "color")
			{
				material = IntrinsicColorMaterial::Create();
			}
			else if (materialType == "const")
			{
				ConstantColorMaterial_ptr mat = ConstantColorMaterial::Create();
				
				XMLElement* subElem;
				if (subElem = materialElement->FirstChildElement("color"))
					GetColorVector3(subElem, mat->color);

				material = mat;
			}
			else if (materialType == "diffuseSH")
			{
				auto shMaterial = ShDiffuseMaterial::Create();
				auto data = Util::LoadTextFile(Config::DATA_BASE_PATH + "sh/grace.xml");
				shMaterial->shCoeffs = ShCoeffParser::Parse(data);

				material = shMaterial;
			}
			else
			{
				Error("Material " + materialType + " not supported");
				return false;
			}

			material->SetName(materialName);
			materials.insert(ShaderKeyVal(materialElement->Attribute("name"), material));

		} while (materialElement = materialElement->NextSiblingElement("material"));
	}
	

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

	SkyboxMaterial_ptr sbMat = SkyboxMaterial::Create();
	sbMat->texture = texture;

	Skybox_ptr sb = Skybox_ptr(new Skybox(sbMat));
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

			std::string modelPath = "../data/models/";

			if(type == "mesh")
			{
				string file = objeElem->Attribute("file");

				modelPath += file;
				shape = Util::LoadModel(modelPath);
				if (!shape)
					return false;
			}
			else if(type == "box")
			{
				shape = Box::Create();
			}
			else 
			{
				Error("Object of type " + type + " not supported");
				continue;
			}

			shape->Init();

			//See if a material is specified, if yes override mtllib in case of mesh
			if(const char* materialName = objeElem->Attribute("material"))
			{
				if(shape->GetMaterial())
				{
					Warn("The shape already has a material assigned, ignoring specified material");	
				}
				else
				{
					if(Material_ptr material = materials[materialName])		
						shape->SetMaterial(material);
					else
						Warn("The specified material " + string(materialName) +" is not defined");	
				}
			}

			// Parse transform node
			if(XMLElement* transformsElem = objeElem->FirstChildElement("transform"))
			{
				glm::mat4 tMatrix;
				ParseTransforms(tMatrix,transformsElem);
				shape->worldTransform = tMatrix;
			}

			generatedScene->AddShape(shape);		
		} 
		while (objeElem = objeElem->NextSiblingElement());
	}
	
	return true;
}

bool SceneParser::ParseTransforms(glm::mat4& tMatrix, tinyxml2::XMLElement* transformElem)
{
	bool success = true;
	
	XMLElement* transform = transformElem->FirstChildElement();
	do
	{
		std::string type = transform->Name();

		if(type == "translate")
		{
			glm::vec3 transl;
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
			glm::vec3 axis;
			float angle;
			if( GetVector3(transform,axis) && GetFloatAttrib(transform, "angle",angle) )
			{
				tMatrix = glm::rotate(tMatrix,glm::radians(angle),axis);
			}
			else
			{
				Error("Could not parse rotation");
				success = false;
			}
		}
		else if(type == "scale")
		{
			glm::vec3 factors;
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
	std::string type = camElement->Attribute("type");

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

	glm::vec3 pos, target, up;
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
	if(XMLElement* lightElem = lightsGroupElement->FirstChildElement("light"))
	{
		do
		{
			string lightType = lightElem->Attribute("type");

			if (lightType == "point")
			{
				PointLight_ptr plight = PointLight::Create();
				glm::vec3 pos, color;

				GetVector3(lightElem->FirstChildElement("position"), pos);
				GetColorVector3(lightElem->FirstChildElement("color"), color);

				plight->SetPosition(glm::vec4(pos, 1.0));
				plight->SetColor(color);

				generatedScene->AddLight(plight);

			}
			else if (lightType == "spot")
			{

				glm::vec3 pos, color, direction;
				float cutoff, exponent = 5;
				GetVector3(lightElem->FirstChildElement("position"), pos);
				GetVector3(lightElem->FirstChildElement("direction"), direction);
				GetColorVector3(lightElem->FirstChildElement("color"), color);

				GetFloatAttrib(lightElem, "cutoff", cutoff);
				GetFloatAttrib(lightElem, "exponent", exponent);
				SpotLight_ptr slight = SpotLight::Create(direction, cutoff, exponent);

				slight->SetPosition(glm::vec4(pos, 1.0));
				slight->SetColor(color);

				generatedScene->AddLight(slight);
			}
			else if (lightType == "directional")
			{
				if (generatedScene->lightModel->directionalLight)
					Warn("Multiple directional lights, in the scene. Only one is suported");

				glm::vec3 color, direction;
				GetColorVector3(lightElem->FirstChildElement("color"), color);
				GetVector3(lightElem->FirstChildElement("direction"), direction);

				DirectionalLight_ptr dirLight = DirectionalLight::Create();
				dirLight->SetColor(color);
				dirLight->SetDirection(glm::normalize(direction));
				generatedScene->SetLight(dirLight);

			}
			else
			{
				Error("Light " + lightType + " not supported");
				return false;
			}
		} while (lightElem = lightElem->NextSiblingElement("light"));
	}
	
	return true;
}

bool SceneParser::GetVector3(XMLElement* element, glm::vec3& vec)
{
	if(element == nullptr)
		return false;
	
	bool success = true;

	success &= GetFloatAttrib(element,"x",vec.x);
	success &= GetFloatAttrib(element,"y",vec.y);
	success &= GetFloatAttrib(element,"z",vec.z);

	return success;
}

bool SceneParser::GetColorVector3(XMLElement* element, glm::vec3& vec)
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