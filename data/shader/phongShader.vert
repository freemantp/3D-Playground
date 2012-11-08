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

void main()
{
	vec3 normal = normalize(NormalMatrix * VertexNormal);
	vec4 eyeCoords = ModelViewMatrix * vec4( VertexPosition, 1.0);

	vec3 s = normalize( vec3(Light.Position - eyeCoords) );
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect( -s , normal);

	float sDotN = max(dot(s,normal), 0.0);

	vec3 ambient = Light.AmbientIntensity * Material.AmbientReflectivity;
	vec3 diffuse = Light.DiffuseIntensity * Material.DiffuseReflectivity * sDotN;
	vec3 specular = vec3(0.0);

	if(sDotN > 0)
	{
		specular = Light.SpecularIntensity * Material.SpecularReflectivity * pow( max( dot(r,v), 0.0), Material.Shininess ) ;
	}

	//vec3 ambient = Light.AmbientIntensity * Material.AmbientReflectivity;

	LightIntensity = ambient + diffuse + specular;

    gl_Position = MVP * vec4( VertexPosition, 1.0 );	
}
