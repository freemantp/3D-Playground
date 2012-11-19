#version 400

layout (location = 0) out vec4 FragColor;

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

//PointLight declaration
struct SpotLight
{
	vec4 Position;
	vec3 Color;
	vec3 Direction;
	float CutoffAngle;
	float Exponent;
};


layout (std140) uniform Lights
{
	PointLight PointLights[4];
	SpotLight  SpotLights[4];
} pLights;

uniform int NumPointLights;
uniform int NumSpotLights;

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

vec3 shade(const in vec3 position, const in vec3 normal, const in vec3 lightDir)
{	
	vec3 v = normalize(-position);
	
	float sDotN = max(dot(lightDir,normal), 0.0);

	vec3 ambient = Material.AmbientReflectivity;
	vec3 diffuse = Material.DiffuseReflectivity * sDotN;
	vec3 specular = vec3(0);

	if(sDotN > 0)
	{
		specular = Material.SpecularReflectivity * shadeModel(lightDir,v,normal) ;
	}

	return ambient + diffuse + specular;
}

void main()
{
	vec3 normal = normalize(Normal);

	//Point lights
	for(int i=0; i < NumPointLights; i++)
	{	
		PointLight light = pLights.PointLights[i];

		vec3 s = normalize( vec3(light.Position) - Position);
		FragColor += vec4( light.Color * shade(Position,normal,s), 1.0  );		
	}

	//Spot lights
	for(int i=0; i < NumSpotLights; i++)
	{	
		SpotLight light = pLights.SpotLights[i];

		vec3 s = normalize( vec3(light.Position) - Position);
		float angle = acos( dot(-s,light.Direction) );
		float cutoff = radians( clamp (light.CutoffAngle, 0.0, 90.0) ) ;
		
		if( angle  < cutoff)
		{		
			float spotFactor = pow( dot( -s , light.Direction), light.Exponent);
			FragColor += spotFactor * vec4( light.Color * shade(Position,normal,s), 1.0  );
		}
	}

}