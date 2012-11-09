#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; //in eye coords
uniform vec3 MaterialColor;	//diffuse reflectivity
uniform vec3 LightColor;	//light source intensity

struct LightInfo
{
	vec4 Position;
	vec3 AmbientIntensity;
	vec3 DiffuseIntensity;
	vec3 SpecularIntensity;
};

struct MaterialInfo
{
	vec3 AmbientReflectivity;
	vec3 DiffuseReflectivity;
	vec3 SpecularReflectivity;
	int Shininess;
};

uniform LightInfo Light;
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void getEyeSpace( out vec3 normal, out vec4 position)
{
	normal = normalize(NormalMatrix * VertexNormal);
	position = ModelViewMatrix * vec4( VertexPosition, 1.0);
}

vec3 phongModel(const in vec4 position,const in vec3 normal)
{
	vec3 s = normalize( vec3(Light.Position - position) );
	vec3 v = normalize(-position.xyz);
	vec3 r = reflect( -s , normal);

	float sDotN = max(dot(s,normal), 0.0);

	vec3 ambient = Light.AmbientIntensity * Material.AmbientReflectivity;
	vec3 diffuse = Light.DiffuseIntensity * Material.DiffuseReflectivity * sDotN;
	vec3 specular = vec3(0.0);  

	if(sDotN > 0)
	{
		specular = Light.SpecularIntensity * Material.SpecularReflectivity * pow( max( dot(r,v), 0.0), Material.Shininess ) ;
	}

	return ambient + diffuse + specular;
}

void main()
{
	vec3 eyeNormal;
	vec4 eyeCoords;

	getEyeSpace(eyeNormal, eyeCoords);

	LightIntensity = phongModel(eyeCoords,eyeNormal);

    gl_Position = MVP * vec4( VertexPosition, 1.0 );	
}
