#include "stdafx.h"
#include "ObjLoader.h"
#include <fstream>
#include <regex>
#include <sstream>

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::ivec3;

void ObjLoader::parseIdx(string& s,ivec3& indices)
{
	string::const_iterator sit;
	stringstream ss;

	int idxPtr = 0;
	int number;
	bool lastWasSlash = false;

	for(sit = s.cbegin(); sit != s.cend(); sit++)
	{
		char c = *sit;
		
		if(c == '/') {
			if( lastWasSlash)			
				indices[idxPtr++] = -1;
			else {
				ss >> number;
				indices[idxPtr++] = number - 1;
				ss.str("");
				ss.clear();
			}				
	
			lastWasSlash = true;
		}
		else {
			ss << c;
			lastWasSlash = false;
		}
	}

	ss >> number;
	indices[idxPtr++] = number - 1;
}

void ObjLoader::loadObj(istream& istr)
{	
	int lineCount = 0;

	std::string line;
	while ( istr.good() )
	{
		getline (istr,line);
		lineCount++;

		if(line.substr(0,2) == "v ")
		{
			istringstream s(line.substr(2));
			float x, y,z;
			s >> x; s >> y; s >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if(line.substr(0,3) == "vn ")
		{
			istringstream s(line.substr(3));
			glm::vec3 n; 
			s >> n.x; s >> n.y; s >> n.z;
			normals.push_back(n);
		}
		else if(line.substr(0,3) == "vt ")
		{
			istringstream s(line.substr(3));
			glm::vec2 tc; 
			s >> tc.x; s >> tc.y;
			texCoords.push_back(tc);
		}
		else if(line.substr(0,2) == "f ")
		{
			
			ivec3 v1,v2,v3;
			string s1,s2,s3;
			istringstream data(line.substr(3));
			data >> s1; data >> s2; data >> s3;

			parseIdx(s1,v1);
			parseIdx(s2,v2);
			parseIdx(s3,v3);

			triangles.push_back(Tri(v1,v2,v3));

		}
		
	}
	cout << "Mesh loaded, f=" << triangles.size() <<  " , v=" << vertices.size() / 3 << " , n=" << normals.size() << " ,tex coords=" << texCoords.size() << endl;
	prepareOpenGLarrays();
}

void ObjLoader::prepareOpenGLarrays(void) 
{
	vector<float> normalRaw;
	normalRaw.reserve(vertices.size());

	for(int i = 0; i < triangles.size(); i++)
	{
		vec3 n1 = normals[triangles[i].v1.z];
		vec3 n2 = normals[triangles[i].v2.z];
		vec3 n3 = normals[triangles[i].v3.z];
		
	}

}

void ObjLoader::loadObjFile(const string& path)
{
	ifstream myfile;
	myfile.open(path);

	std::string line;
	if (myfile.is_open())
	{
		loadObj(myfile);
		myfile.close();
	}
	else 
		cerr << "Unable to open file" << endl;; 
}