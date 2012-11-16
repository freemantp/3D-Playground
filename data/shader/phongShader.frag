#version 400

layout (location = 0) out vec4 FragColor;

//Structs declaration
/*struct LightInfo
{
	vec4 Position;
	vec3 AmbientIntensity;
	vec3 DiffuseIntensity;
	vec3 SpecularIntensity;
};*/

struct MaterialInfo
{
	vec3 AmbientReflectivity;
	vec3 DiffuseReflectivity;
	vec3 SpecularReflectivity;
	int Shininess;
};

struct PointLight
{
	vec4 Position;
	vec3 Color;
};

layout (std140) uniform Lights
{
	PointLight PointLights[4];
} pLights;

uniform int NumLights;

//Subroutine declaration
subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

//Uniforms
//uniform LightInfo Light;
uniform MaterialInfo Material;
subroutine uniform shadeModelType shadeModel;

//input from previous stage
in vec3 Position;
in vec3 Normal;

//Blinn-Phong model
subroutine( shadeModelType )
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( max( dot(h,normal), 0.0), Material.Shininess ) ;
}

//Phong model
subroutine( shadeModelType )
float phong(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 r = reflect( -s , normal);
	return pow( max( dot(r,v), 0.0), Material.Shininess );
}

vec3 shade(const in vec3 position,const in vec3 normal)
{	
	
	vec3 ambient = vec3(0);
	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);

	for(int i=0; i < NumLights; i++)
	{
	
		vec3 s = normalize( vec3(pLights.PointLights[i].Position) - position);
		vec3 v = normalize(-position);
	
		float sDotN = max(dot(s,normal), 0.0);

		ambient += pLights.PointLights[i].Color * Material.AmbientReflectivity;
		diffuse += pLights.PointLights[i].Color * Material.DiffuseReflectivity * sDotN;

		if(sDotN > 0)
		{
			specular += pLights.PointLights[i].Color * Material.SpecularReflectivity * shadeModel(s,v,normal) ;
		}

	}

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = normalize(Normal);
	FragColor = vec4(shade(Position,normal), 1.0);
}