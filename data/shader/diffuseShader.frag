#version 400

uniform PointLight
{
	vec4 LPosition;
	vec3 LColor;
};

uniform vec3 MaterialColor;	//diffuse reflectivity

in vec3 Position;
in vec3 Normal;
in vec3 LightIntensity;

layout (location = 0) out vec4 FragColor;

void main()
{
	vec3 s = normalize( vec3(LPosition) - Position );
	vec3 LightIntensity = LColor * MaterialColor * max( dot( s, Normal ), 0.0 );
	FragColor = vec4(LightIntensity.xyz, 1.0);
}