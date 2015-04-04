#version 400

// ----------------- declarations -----------------

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

uniform int NumPointLights;
uniform int NumSpotLights;
uniform bool HasDirectionalLight;
uniform bool HasAmbientLight;
uniform sampler2D AlbedoTex;
uniform sampler2D BumpmapTex;
uniform sampler2D SpecularTex;
uniform mat4 ViewMatrix;
uniform bool isNormalMap;
uniform bool hasSpecularMap;
uniform bool hasBumpMap;
uniform bool HasEnvMap;
uniform int	Shininess;

// ----------------- in / out -----------------

//input from previous stage
in vec2 TexCoord;
in vec3 PositionEye;
in vec3 NormalEye;
in vec3 LightDirection;
in vec3 ViewDirection;
in vec3 PointlightDirTGT[numLights];
in vec3 SpotlightDirTGT[numLights];

layout (location = 0) out vec4 FragColor;


// ----------------- subroutines -----------------

//Blinn-Phong model
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( max( dot(h,normal), 0.0), Shininess ) ;
}

float shade(const in vec3 normal, const in vec3 viewDir, const in vec3 lightDir, vec2 texCoord)
{		
	float diffuse = max( dot(lightDir,normal), 0.0 );	
	float specular = 0;

	float specularity = hasSpecularMap ? texture(SpecularTex,TexCoord).r : 1.0;

	if(specularity > 0)
		specular = blinn(lightDir,viewDir,normal) ;

	return diffuse + specularity * specular;
}

//see http://athile.net/library/wiki/index.php/Library/Graphics/Bump_Mapping
vec3 bump_normal(vec3 fragVertex, vec3 fragNormal, float value)
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
	if(hasBumpMap)
	{
		if(isNormalMap)
		{
			// fetch normal from normal map, expand to the [-1, 1] range, and normalize
			normal = normalize(2.0 * texture (BumpmapTex, TexCoord).rgb - 1.0);
		}
		else
		{
			float bumpVal = texture(BumpmapTex, TexCoord).r;
			normal = bump_normal(PositionEye,NormalEye,bumpVal);
		}
	}
	else
	{
		normal = NormalEye;
	}
	return normal;
}

// ----------------- main -----------------
void main() 
{
	vec3 normal = getNormal();
	vec3 albedo = texture(AlbedoTex,TexCoord).rgb;

	FragColor = vec4(0,0,0,1);

	//Ambient light
	if(HasAmbientLight)
	{
		vec3 ambientColor = sceneLights.AmbientLight0.Color;
		FragColor += vec4(ambientColor,0);
	}

	//Directional light
	if(HasDirectionalLight)
	{
		vec3 dirDirection = sceneLights.DirectionalLight0.Direction;
		vec3 dirColor = sceneLights.DirectionalLight0.Color;
		FragColor += vec4(dirColor * albedo * shade(normal,ViewDirection, dirDirection,TexCoord),0);
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
		
		vec3 lightDir = isNormalMap ? PointlightDirTGT[i] : lightToCamDirNormalized;
		FragColor += distAttenuation * vec4(light.Color * albedo * shade(normal,ViewDirection, lightDir,TexCoord),0);
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

			vec3 lightDir = isNormalMap ? SpotlightDirTGT[i] : normalize( vec3(light.Position) - PositionEye);
			FragColor +=  distAttenuation * borderFadeFacor * vec4(light.Color * albedo * shade(normal,ViewDirection,lightDir,TexCoord),0);
		}
	}

	if(HasEnvMap)
	{
		//TODO: add env mapping
		FragColor += vec4(1e-10,1e-10,1e-10,0);
	}

}
