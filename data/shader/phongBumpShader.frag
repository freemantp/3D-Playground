#version 400

// ----------------- declarations -----------------

const int numLights = 4;

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

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
} sceneLights;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform sampler2D AlbedoTex;
uniform sampler2D BumpmapTex;
uniform sampler2D SpecularTex;
uniform mat4 ViewMatrix;
uniform bool isNormalMap;
uniform bool hasSpecularMap;
uniform bool hasBumpMap;
uniform int	Shininess;

// ----------------- in / out -----------------

//input from previous stage
in vec2 TexCoord;
in vec3 PositionEYE;
in vec3 NormalEYE;
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

	if(diffuse > 0)
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
			normal = bump_normal(PositionEYE,NormalEYE,bumpVal);
		}
	}
	else
	{
		normal = NormalEYE;
	}
	return normal;
}

// ----------------- main -----------------
void main() 
{
	vec3 normal = getNormal();
	vec3 albedo = texture2D(AlbedoTex,TexCoord).rgb;

	float spread = 5;
	float depth = (albedo.r - (1-1/spread)) * spread;

	//FragColor = vec4(0,0,0,1);	

	//Point lights
	for(int i=0;  i < NumPointLights; i++)
	{
		PointLight light = sceneLights.PointLights[i];
		vec3 lightDir = isNormalMap ? PointlightDirTGT[i] : normalize( vec3(light.Position) - PositionEYE);
		FragColor += vec4(light.Color * albedo * shade(normal,ViewDirection, lightDir,TexCoord),0);
	}

	//Spot lights
	for(int i=0;  i < NumSpotLights; i++)
	{
		SpotLight light = sceneLights.SpotLights[i];
		vec3 lightToCamDir = normalize( vec3(light.Position) - PositionEYE);
		
		float angle = acos( clamp(dot(-lightToCamDir,normalize(light.Direction)),0.0,0.9999) );
		float cutoff = radians( clamp (light.CutoffAngle, 0.0, 90.0) ) ;
		
		if( angle  < cutoff)
		{
			float spotFactor = pow( dot( -lightToCamDir, light.Direction), light.Exponent);
			vec3 lightDir = isNormalMap ? SpotlightDirTGT[i] : normalize( vec3(light.Position) - PositionEYE);
			FragColor += spotFactor * vec4(light.Color * albedo * shade(normal,ViewDirection,lightDir,TexCoord),0);
		}
	}

	FragColor = vec4(depth,depth,depth,1);
	//FragColor = vec4(albedo,1);
	

}
