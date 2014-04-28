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
	mat4 ShadowMatrix;
};

layout (std140) uniform Lights
{
	PointLight PointLights[4];
	SpotLight  SpotLights[4];
} pLights;

uniform sampler2DShadow ShadowMapArray[4];
uniform sampler3D PCFDataOffsets;
uniform ivec3 PCFDataOffsetsSize;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform samplerCube EnvMapTex;
uniform float EnvReflection; //[0,1]

//Subroutine declaration
subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

//Uniforms
//uniform LightInfo Light;
uniform MaterialInfo Material;
subroutine uniform shadeModelType shadeModel;

//input from previous stage
in vec3 Position;
in vec4 PositionModel;
in vec3 Normal;
in vec3 ReflectDir;

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


float random(vec4 seed) {
  float dot_product = dot(seed, vec4(12.9898,78.233, 45.164, 94.673));
  return fract(sin(dot_product) * 43758.5453);
}

float getShadow(int sl_i)
{
	if(sl_i < NumSpotLights)
	{
		vec4 ShadowCoord = pLights.SpotLights[sl_i].ShadowMatrix * PositionModel;

		ivec3 offsetCoords;
		offsetCoords.xy = ivec2( mod(gl_FragCoord.xy,  PCFDataOffsetsSize.xy) );

		int Radius = 5;
		vec4 sc = ShadowCoord;
		float sum = 0;

		//First test the four outermost offsets
		for (int i=0; i<4; i++)
		{
			//offsetCoords.z = i;
			//vec4 offsets = texelFetch(PCFDataOffsets,offsetCoords,0) * Radius * ShadowCoord.w;

			ivec2 offsets;

			offsets.x = i < 2 ? - 1 : 1;
			offsets.y = mod(i,2) == 0 ? 1 : -1;

			//sc.xy =  ShadowCoord.xy  + offsets.xy;
			//sum += textureProj(ShadowMapArray[sl_i],sc);
			sum += textureProjOffset(ShadowMapArray[sl_i],ShadowCoord,offsets);
			//sc.xy =  ShadowCoord.xy  + offsets.zw;
			//sum += textureProj(ShadowMapArray[sl_i],sc);
		}

		float shadow = sum /4.0;



		//Only continue with inner offsets if outer ones are not completely shadowed or illuminated
		//if(shadow != 1.0 && shadow != 0.0)
		//{
			
		//	int numSamples = int(PCFDataOffsetsSize.z);

		//	for (int i=4; i<numSamples; i++)
		//	{
		//		offsetCoords.z = i;
		//		vec4 offsets = texelFetch(PCFDataOffsets,offsetCoords,0) * Radius * ShadowCoord.w;

		//		sc.xy =  ShadowCoord.xy  + offsets.xy;
		//		sum += textureProj(ShadowMapArray[sl_i],sc);
		//		sc.xy =  ShadowCoord.xy  + offsets.zw;
		//		sum += textureProj(ShadowMapArray[sl_i],sc);
		//	}

		//	shadow = sum / float(numSamples * 2.0);
		//}

		return shadow;
	}
	else
		return 1.0f;
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
			float shadow = getShadow(i);

			ambient  += spotFactor * ambientCurrent  * light.Color;
			diffuse  += spotFactor * diffuseCurrent  * light.Color * shadow;
			specular += spotFactor * specularCurrent * light.Color * shadow;
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

	FragColor = vec4(ambient + (diffuse + specular) , Material.Opacity);


}