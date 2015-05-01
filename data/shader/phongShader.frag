#version 400

#include light_common.glsl

struct MaterialInfo
{
	vec3 AmbientReflectivity;
	vec3 DiffuseReflectivity;
	vec3 SpecularReflectivity;
	int Shininess;
	float Opacity; //[0,1]
};

uniform sampler2DShadow ShadowMapArray[4];
uniform sampler3D PCFDataOffsets;
uniform ivec3 PCFDataOffsetsSize;
uniform float PCFBlurRadius;
uniform bool UseShadows;
uniform bool PcfShadows;

uniform bool HasEnvMap;
uniform samplerCube EnvMapTex;
uniform float EnvReflection; //[0,1]

//uniform LightInfo Light;
uniform MaterialInfo Material;

//input from previous stage
in vec3 PositionEye;
in vec4 PositionModel;
in vec3 NormalEye;
in vec3 ReflectDir;
in vec3 ViewDirection;

layout (location = 0) out vec4 FragColor;

// ----------------- functions -----------------

//Blinn-Phong model
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( clamp( dot(h,normal), 0.0, 1.0), Material.Shininess ) ;
}

float shade(const in vec3 normal, const in vec3 viewDir, const in vec3 lightDir)
{			
	float diffuse = clamp(dot(lightDir,normal), 0.0 , 1.0);
	float specular = 0;
	float specularity = length(Material.SpecularReflectivity);

	if(specularity > 0)
	{
		specular = blinn(lightDir,viewDir,normal);	
	}

	return diffuse + specular * specularity;
}

float getShadow(int sl_i)
{
	if(UseShadows && sl_i < NumSpotLights)
	{
		vec4 ShadowCoord = sceneLights.SpotLights[sl_i].ShadowMatrix * PositionModel;

		ivec3 offsetCoords;
		offsetCoords.xy = ivec2( mod(gl_FragCoord.xy,  PCFDataOffsetsSize.xy) );       

		if(PcfShadows)
		{	
			vec4 sc = ShadowCoord;
			float sum = 0.0;

			//First test the four outermost offsets
			for (int i=0; i<4; i++)
			{
				offsetCoords.z = i;
				vec4 offsets = texelFetch(PCFDataOffsets,offsetCoords,0) * PCFBlurRadius * ShadowCoord.w;

				sc.xy =  ShadowCoord.xy  + offsets.xy;
				sum += textureProj(ShadowMapArray[sl_i], sc);
				sc.xy =  ShadowCoord.xy  + offsets.zw;
				sum += textureProj(ShadowMapArray[sl_i], sc);

			}

			float shadow = sum /8.0f;

			//Only continue with inner offsets if outer ones are not completely shadowed or illuminated
			if(shadow != 1.0 && shadow != 0.0)
			{			
				int numSamples = int(PCFDataOffsetsSize.z);

				for (int i=4; i<numSamples; i++)
				{
					offsetCoords.z = i;
					vec4 offsets = texelFetch(PCFDataOffsets,offsetCoords,0) * PCFBlurRadius * ShadowCoord.w;

					sc.xy =  ShadowCoord.xy  + offsets.xy;
					sum += textureProj(ShadowMapArray[sl_i], sc);
					sc.xy =  ShadowCoord.xy  + offsets.zw;
					sum += textureProj(ShadowMapArray[sl_i], sc);
				}

				shadow = sum / float(numSamples * 2.0);
			}
			return shadow;
		}
		else
		{
			return textureProj(ShadowMapArray[sl_i],ShadowCoord);
		}		
	}
	else
		return 1.0f;
}

void main()
{
	vec3 normal = normalize(NormalEye);
	vec3 albedo = Material.DiffuseReflectivity;

	FragColor = vec4(0,0,0,1);

	//Ambient light
	if(HasAmbientLight)
	{
		FragColor += vec4(Material.AmbientReflectivity * sceneLights.AmbientLight0.Color,0);
	}

	//Directional light
	if(HasDirectionalLight)
	{
		vec3 dirDirection = normalize(sceneLights.DirectionalLight0.Direction);
		vec3 dirColor = sceneLights.DirectionalLight0.Color;		
		float shade_val = shade(normal,ViewDirection,dirDirection);
		FragColor += vec4(dirColor * albedo * shade_val,0);
	}

	//Point lights
	for(int i=0; i < NumPointLights; i++)
	{	
		PointLight light = sceneLights.PointLights[i];

		vec3 lightVec = vec3(light.Position) - PositionEye;
		vec3 lightVecNormalized = normalize(lightVec);		

		float distance = length(lightVec);
		float k = 0.2;
		float distAttenuation = 1 / ( 1 + k*distance*distance);

		float shade_val = shade(normal,ViewDirection,lightVecNormalized);
		FragColor += distAttenuation * vec4(light.Color * albedo * shade_val,0);
	}

	//Spot lights
	for(int i=0; i < NumSpotLights; i++)
	{	
		SpotLight light = sceneLights.SpotLights[i];
		vec3 lightToCamDir =  vec3(light.Position) - PositionEye;
		vec3 lightVecNormalized = normalize(lightToCamDir);

		float angle = acos( dot(-lightVecNormalized,light.Direction) );
		float cutoff = radians( clamp (light.CutoffAngle, 0.0, 90.0) ) ;
		
		if( angle  < cutoff)
		{		
			// Quadratic falloff towards borders starting at p percent of the angle
			float p = 0.6;
			
			float angleRatio =  angle / cutoff;
			float a = -1/(1-2*p+p*p);
			float borderFadeFacor = angleRatio <=p ? 1: a * pow(angleRatio-p,2)+1;
			
			//Quadratic distance attenuation with factor k 
			float dist = length(lightToCamDir);
			float k = 0.2;
			float distAttenuation = 1 / ( 1 + k*dist*dist);

			float shadow = getShadow(i);

			float shade_val = shade(normal,ViewDirection,lightVecNormalized);
		
			FragColor += shadow * distAttenuation * borderFadeFacor * vec4(light.Color * albedo * shade_val, 0);
		}
	}

	//Environment mapping
	if(HasEnvMap && Material.Shininess > 0.0) 
	{
		float reflectionRatio = clamp(Material.Shininess / 30.0, 0.0, 1.0);
		vec3 cubeMapColor = texture(EnvMapTex, ReflectDir).xyz;

		//diffuse  +=  ??
		FragColor += vec4(Material.SpecularReflectivity * cubeMapColor * reflectionRatio,0);
	}

	FragColor.a = Material.Opacity;

}