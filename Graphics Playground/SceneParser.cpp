#include "stdafx.h"
#include "SceneParser.h"

#include <string>

using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;
using std::string;

void SceneParser::parse(const char* xmlDocument)
{
	tinyxml2::XMLDocument doc;
	if(doc.Parse(xmlDocument) == tinyxml2::XML_NO_ERROR)
	{

		XMLElement* root = doc.RootElement();

		if(strcmp(root->Name(),"scene") == 0)
		{
			string sceneName(root->Attribute("name"));
			std::cout << "Parsing scene \"" << sceneName << "\":" << std::endl;

			XMLNode* fChild = root->FirstChild();

			do
			{
				XMLElement* child = fChild->ToElement();
				string childName = child->Name();
				std::cout << childName << std::endl;

				if(childName == "materials")
				{
					parseMaterials(child);
				}
				else if(childName == "objects")
				{
					parseObjects(child);
				}

			} 
			while (fChild = fChild->NextSibling());


		}
		else 
		{
			Error("Not a valid scene file");
		}
	}
	else
	{
		Error("File could not be parsed");
	}
}

void SceneParser::parseMaterials(XMLElement* materialElement)
{
	XMLNode* fChild = materialElement->FirstChild();

	do
	{
		XMLElement* child = fChild->ToElement();
		//string childName = child->Name();
		//std::cout << childName << std::endl;

		std::cout << "\t" << child->Attribute("name") << std::endl;

	} 
	while (fChild = fChild->NextSibling());
}

void SceneParser::parseObjects(XMLElement* materialElement)
{
	XMLNode* fChild = materialElement->FirstChild();

	do
	{
		XMLElement* child = fChild->ToElement();
		string childName = child->Name();
		//std::cout << childName << std::endl;

		std::cout << "\t" << childName << std::endl;

	} 
	while (fChild = fChild->NextSibling());
}


