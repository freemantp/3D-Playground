#version 400


// ----------------- declarations -----------------

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

subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[4];
	SpotLight  SpotLights[4];
} pLights;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform sampler2D AlbedoTex;
uniform sampler2D NormalMapTex;
uniform mat4 ViewMatrix;

uniform int	Shininess;

//Subroutine declaration
subroutine uniform shadeModelType shadeModel;

// ----------------- in / out -----------------

//input from previous stage
in vec3 normal_vector;
in vec3 light_vector;
in vec3 halfway_vector;
in vec2 texture_coord;

layout (location = 0) out vec4 FragColor;


// ----------------- subroutines -----------------

//Blinn-Phong model
subroutine( shadeModelType )
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( max( dot(h,normal), 0.0), Shininess ) ;
}

//Phong model
subroutine( shadeModelType )
float phong(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 r = reflect( -s , normal);
	return pow( max( dot(r,v), 0.0), Shininess );
}


/*vec3 phongModel( vec3 norm, vec3 diffR ) {
    vec3 r = reflect( -LightDir, norm );
    float sDotN = max( dot(LightDir, norm), 0.0 );
    vec3 diffuse = diffR * sDotN;

    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = vec3(0.1) *
               pow( max( dot(r,ViewDir), 0.0 ), Shininess );

    return diffuse + spec;
}*/

void main() {
    // Lookup the normal from the normal map

	bool flag = true;

	vec3 normal1         = normalize(normal_vector);
    vec3 light_vector1   = normalize(light_vector);
    vec3 halfway_vector1 = normalize(halfway_vector);
 
    vec4 c = texture(AlbedoTex, texture_coord);
    vec3 n = normalize((texture(NormalMapTex, texture_coord).xyz-0.5)*2.0);
    normal1 = flag ? normalize(n.xyz+normal1) : normal1;
 
    vec4 emissive_color = vec4(0.0, 1.0, 0.0, 1.0);     // green
    vec4 ambient_color  = vec4(1.0, 1.0, 1.0, 1.0);     // white
    vec4 diffuse_color  = vec4(1.0, 1.0, 1.0, 1.0);     // white
    vec4 specular_color = vec4(1.0, 1.0, 1.0, 1.0);     // white
 
    float emissive_contribution = 0.01;
    float ambient_contribution  = 0.09;
    float diffuse_contribution  = 0.4;
    float specular_contribution = 0.9;
 
    float d = dot(normal1, light_vector1);
    bool facing = d > 0.0;
 
    FragColor = emissive_color * emissive_contribution +
            ambient_color  * ambient_contribution  * c +
            diffuse_color  * diffuse_contribution  * c * max(d, 0) +
                    (facing ?
            specular_color * specular_contribution * c * pow(max(0,dot(normal1, halfway_vector1)), 200.0) :
            vec4(0.0, 0.0, 0.0, 0.0));
    FragColor.a = 1.0;

}
