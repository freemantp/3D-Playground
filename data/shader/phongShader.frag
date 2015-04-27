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

const int numLights = 4;

struct AmbientLight
{
	vec3 Color;
};

struct PointLight
{
	vec4 Position;
	vec3 Color;
};

struct DirectionalLight
{
	vec3 Direction;
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

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
	DirectionalLight DirectionalLight0;
	AmbientLight	 AmbientLight0;
} sceneLights;

uniform sampler2DShadow ShadowMapArray[4];
uniform sampler3D PCFDataOffsets;
uniform ivec3 PCFDataOffsetsSize;
uniform float PCFBlurRadius;
uniform bool UseShadows;
uniform bool PcfShadows;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform bool HasDirectionalLight;
uniform bool HasAmbientLight;
uniform bool HasEnvMap;
uniform samplerCube EnvMapTex;
uniform float EnvReflection; //[0,1]

//Subroutine declaration
subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

//Uniforms
//uniform LightInfo Light;
uniform MaterialInfo Material;
subroutine uniform shadeModelType shadeModel;

//input from previous stage
in vec3 PositionEye;
in vec4 PositionModel;
in vec3 NormalEye;
in vec3 ReflectDir;

//Blinn-Phong model
subroutine( shadeModelType )
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( clamp( dot(h,normal), 0.0, 1.0), Material.Shininess ) ;
}

//Phong model
subroutine( shadeModelType )
float phong(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 r = reflect( -s , normal);
	return pow( clamp( dot(r,v), 0.0, 1.0), Material.Shininess );
}

void shade(const in vec3 eyePosition, const in vec3 normal, const in vec3 lightDir, 
		   inout vec3 ambient, inout vec3 diffuse, inout vec3 specular  )
{	
	vec3 v = normalize(-eyePosition);
	
	float sDotN = clamp(dot(lightDir,normal), 0.0 , 1.0);

	ambient = Material.AmbientReflectivity;
	diffuse = Material.DiffuseReflectivity * sDotN;
	specular = vec3(0);

	if(sDotN > 0)
	{
		specular = Material.SpecularReflectivity * shadeModel(lightDir,v,normal) ;
	}
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

	vec3 ambient = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	vec3 ambientCurrent, diffuseCurrent, specularCurrent;

	//Ambient light
	if(HasAmbientLight)
	{
		vec3 ambientColor = sceneLights.AmbientLight0.Color;
		ambient += Material.AmbientReflectivity * ambientColor;
	}

	//Directional light
	if(HasDirectionalLight)
	{
		vec3 dirDirection = normalize(sceneLights.DirectionalLight0.Direction);
		vec3 dirColor = sceneLights.DirectionalLight0.Color;
		
		float sDotN = max(dot(dirDirection,normal), 0.0);

		shade(PositionEye,normal,dirDirection,ambientCurrent, diffuseCurrent, specularCurrent);

		diffuse += Material.DiffuseReflectivity * diffuseCurrent * dirColor;
		specular += Material.SpecularReflectivity * specularCurrent * dirColor;
	}

	//Point lights
	for(int i=0; i < NumPointLights; i++)
	{	
		PointLight light = sceneLights.PointLights[i];

		vec3 lightVec =  vec3(light.Position) - PositionEye;
		vec3 lightVecNormalized = normalize(lightVec);

		shade(PositionEye,normal,lightVecNormalized,ambientCurrent, diffuseCurrent, specularCurrent);

		float distance = length(lightVec);
		float k = 0.2;
		float distAttenuation = 1 / ( 1 + k*distance*distance);

		diffuse  += diffuseCurrent  * light.Color * distAttenuation;
		specular += specularCurrent * light.Color * distAttenuation;
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
			shade(PositionEye,normal,lightVecNormalized,ambientCurrent, diffuseCurrent, specularCurrent);

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
		
			diffuse  += distAttenuation * borderFadeFacor * diffuseCurrent  * light.Color * shadow;
			specular += distAttenuation * borderFadeFacor * specularCurrent * light.Color * shadow;
		}
	}

	//Environment mapping
	if(HasEnvMap && Material.Shininess > 0.0) 
	{
		float reflectionRatio = clamp(Material.Shininess / 30.0, 0.0, 1.0);
		vec3 cubeMapColor = texture(EnvMapTex, ReflectDir).xyz;

		//diffuse  +=  ??
		specular += Material.SpecularReflectivity * cubeMapColor * reflectionRatio;
	}

	vec3 lightColor = vec3(ambient + diffuse + specular);

	FragColor = vec4(lightColor , Material.Opacity);

}