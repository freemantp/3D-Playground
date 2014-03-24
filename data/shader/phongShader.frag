#version 400

layout (location = 0) out vec4 FragColor;

struct MaterialInfo
{
	vec3 AmbientReflectivity;
	vec3 DiffuseReflectivity;
	vec3 SpecularReflectivity;
	int Shininess;
	float Opacity; //[0,1]
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
uniform samplerCube EnvMapTex;
uniform float EnvReflection; //[0,1]

uniform sampler2D ShadowMap;

//Subroutine declaration
subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

//Uniforms
//uniform LightInfo Light;
uniform MaterialInfo Material;
subroutine uniform shadeModelType shadeModel;

//input from previous stage
in vec3 Position;
in vec3 Normal;
in vec3 ReflectDir;
in vec4 ShadowCoord;

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

void shade(const in vec3 position, const in vec3 normal, const in vec3 lightDir, 
		   inout vec3 ambient, inout vec3 diffuse, inout vec3 specular  )
{	
	vec3 v = normalize(-position);
	
	float sDotN = max(dot(lightDir,normal), 0.0);

	ambient = Material.AmbientReflectivity;
	diffuse = Material.DiffuseReflectivity * sDotN;
	specular = vec3(0);

	if(sDotN > 0)
	{
		specular = Material.SpecularReflectivity * shadeModel(lightDir,v,normal) ;
	}

}

void main()
{
	vec3 normal = normalize(Normal);
	vec4 fragmentColor = vec4(0,0,0,1);

	vec3 ambient = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	vec3 ambientCurrent, diffuseCurrent, specularCurrent;

	//Point lights
	for(int i=0; i < NumPointLights; i++)
	{	
		PointLight light = pLights.PointLights[i];

		vec3 s = normalize( vec3(light.Position) - Position);
		shade(Position,normal,s,ambientCurrent, diffuseCurrent, specularCurrent);

		ambient  += ambientCurrent  * light.Color;
		diffuse  += diffuseCurrent  * light.Color;
		specular += specularCurrent * light.Color;

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
			shade(Position,normal,s,ambientCurrent, diffuseCurrent, specularCurrent);

			float spotFactor = pow( dot( -s , light.Direction), light.Exponent);

			ambient  += spotFactor * ambientCurrent  * light.Color;
			diffuse  += spotFactor * diffuseCurrent  * light.Color;
			specular += spotFactor * specularCurrent * light.Color;
		}
	}

	//Environment mapping
	if(Material.Shininess > 0.0) 
	{
		float reflectionRatio = clamp(Material.Shininess / 30.0, 0.0, 1.0);
		vec3 cubeMapColor = texture(EnvMapTex, ReflectDir).xyz;

		specular *= cubeMapColor;
		ambient  *= cubeMapColor;
	}

	float shadow = textureProj(ShadowMap,ShadowCoord);

	FragColor = vec4(ambient + (diffuse + specular) * shadow , Material.Opacity);

}