// Debug Playground.cpp : Defines the entry point for the console application.
//


#include <util/Util.h>
#include "util/ObjLoader.h"
#include "util/MeshRaw.h"

#include <regex>
#include <string>

int main(int argc, char* argv[])
{

	std::string l1 =  "v -4479.864258 -2150.956055 10633.285156";
	std::string l2 =  "v  -4480.626465 -2191.785156 10581.707031";
	std::string l3 =  "v  -4537.026855 -2182.099854 10623.152344";

	   
	std::regex vertex_rgx("\\s*v((:?\\s+-?\\d+\\.?\\d+){3})");
	   std::regex face_rgx("^\\s*f((:?\\s\\d+/?\\d*/?\\d*){3})$");
       std::smatch result;
       std::regex_search(l1, result, vertex_rgx);


	    if(std::regex_search(l1, result, vertex_rgx))
	   // if(std::regex_search(l4, result, face_rgx))
		{
			std::cout << result.size() << std::endl;
			std::cout << result[1] << std::endl;
		}

	return 0;	

}

