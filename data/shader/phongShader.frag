#version 400

uniform vec4 LightPosition; //in eye coords
uniform vec3 MaterialColor;	//diffuse reflectivity
uniform vec3 LightColor;	//light source intensity

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

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

vec3 phongModel(const in vec3 position,const in vec3 normal)
{
	vec3 s = normalize( vec3(Light.Position) - position);
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

	FragColor = vec4(phongModel(Position,Normal), 1.0);
}