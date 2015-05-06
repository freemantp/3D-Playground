#version 400

#include light_common.glsl

struct TextureMaterial
{
	vec3 AmbientReflectivity;
	vec3 DiffuseReflectivity;
	vec3 SpecularReflectivity;
	int Shininess;
	float Opacity; //[0,1]
	sampler2D AlbedoTex;
	sampler2D BumpmapTex;
	sampler2D SpecularTex;
	bool BumpTexIsNormalMap;
	bool HasSpecularMap;
	bool HasBumpMap;
};

struct EnvironmentMap
{
	bool Exists;	
	samplerCube CubeTexture;
};

uniform mat4 ViewMatrix;
uniform TextureMaterial Material;
uniform EnvironmentMap EnvMap;

// ----------------- in / out -----------------

//input from previous stage
in vec3 PositionEye;
in vec3 NormalEye;
in vec3 ReflectDir;
in vec3 LightDirection;
in vec3 ViewDirection;
in vec2 TexCoord;
in vec3 PointlightDirTGT[numLights];
in vec3 SpotlightDirTGT[numLights];

layout (location = 0) out vec4 FragColor;

// ----------------- subroutines -----------------

//Blinn-Phong model
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( clamp( dot(h,normal), 0.0, 1.0), Material.Shininess ) ;
}

void shade(const in vec3 normal, const in vec3 viewDir, const in vec3 lightDir, vec2 texCoord, inout float diffuse, inout float specular)
{		
	diffuse = clamp(dot(lightDir,normal), 0.0 , 1.0);	
	float specularity = Material.HasSpecularMap ? texture(Material.SpecularTex,TexCoord).r : 1.0;

	if(specularity > 0 && Material.Shininess > 0)
		specular = specularity * blinn(lightDir,viewDir,normal) ;
}

//see https://web.archive.org/web/20120307174333/http://athile.net/library/wiki/index.php/Library/Graphics/Bump_Mapping
vec3 getBumpNormal(vec3 fragVertex, vec3 fragNormal, float value)
{
    vec2 dV = vec2(dFdx(value), dFdy(value));
 
    vec3 dPdx = dFdx(fragVertex);
    vec3 dPdy = dFdy(fragVertex);  
 
    vec3 dPdz = normalize(fragNormal);
    dPdy = normalize(cross(dPdz, dPdx));
    dPdx = normalize(cross(dPdy, dPdz));
 
    vec3 N = normalize(-dV.x * dPdx - dV.y * dPdy + dPdz);
    return N;
}

// Returns the normal, depending on the map type
vec3 getNormal()
{
	vec3 normal;
	if(Material.HasBumpMap)
	{
		if(Material.BumpTexIsNormalMap)
		{
			// fetch normal from normal map, expand to the [-1, 1] range, and normalize
			normal = normalize(2.0 * texture (Material.BumpmapTex, TexCoord).rgb - 1.0);
		}
		else
		{
			float bumpVal = texture(Material.BumpmapTex, TexCoord).r;
			normal = getBumpNormal(PositionEye,NormalEye,bumpVal);
		}
	}
	else
	{
		normal = normalize(NormalEye);
	}
	return normal;
}

// ----------------- main -----------------
void main() 
{
	vec3 normal = getNormal();
	vec3 albedo = texture(Material.AlbedoTex,TexCoord).rgb;

	float diffuse = 0, specular = 0;

	FragColor = vec4(0,0,0,1);

	//Ambient light
	if(HasAmbientLight)
	{
		FragColor += vec4(sceneLights.AmbientLight0.Color*albedo,0);
	}

	//Directional light
	if(HasDirectionalLight)
	{
		vec3 dirDirection = sceneLights.DirectionalLight0.Direction;
		vec3 dirColor = sceneLights.DirectionalLight0.Color;
		shade(normal,ViewDirection, dirDirection,TexCoord,diffuse,specular);
		FragColor += vec4(dirColor * albedo * (diffuse + specular),0);
	}

	//Point lights
	for(int i=0;  i < NumPointLights; i++)
	{
		PointLight light = sceneLights.PointLights[i];

		vec3 lightToCamDir = vec3(light.Position) - PositionEye;
		vec3 lightToCamDirNormalized = normalize(lightToCamDir);

		float distance = length(lightToCamDir);
		float k = 0.2;
		float distAttenuation = 1 / ( 1 + k*distance*distance);
		
		vec3 lightDir = Material.BumpTexIsNormalMap ? PointlightDirTGT[i] : lightToCamDirNormalized;
		shade(normal,ViewDirection, lightDir,TexCoord,diffuse,specular);
		FragColor += distAttenuation * vec4(light.Color * albedo * (diffuse + specular),0);
	}

	//Spot lights
	for(int i=0;  i < NumSpotLights; i++)
	{
		SpotLight light = sceneLights.SpotLights[i];
		vec3 lightToCamDir = vec3(light.Position) - PositionEye;
		vec3 lightToCamDirNormalized = normalize(lightToCamDir);
		
		float angle = acos( clamp(dot(-lightToCamDirNormalized,normalize(light.Direction)),0.0,0.9999) );
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

			vec3 lightDir = Material.BumpTexIsNormalMap ? SpotlightDirTGT[i] : normalize( vec3(light.Position) - PositionEye);
			shade(normal,ViewDirection, lightDir,TexCoord,diffuse,specular);
			FragColor +=  distAttenuation * borderFadeFacor * vec4(light.Color * albedo * (diffuse + specular),0);
		}
	}

	if(EnvMap.Exists && length(Material.SpecularReflectivity) > 0.0)
	{
		float reflectionRatio = clamp(Material.Shininess / 100.0, 0.0, 1.0);
		vec3 cubeMapColor = texture(EnvMap.CubeTexture, ReflectDir).xyz;

		//diffuse  +=  ??
		FragColor += vec4(cubeMapColor * Material.SpecularReflectivity,0);
	}

}
