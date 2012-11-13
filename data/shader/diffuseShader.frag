#version 400

uniform vec4 LightPosition; //in eye coords
uniform vec3 MaterialColor;	//diffuse reflectivity
uniform vec3 LightColor;	//light source intensity

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

void main()
{
	vec3 s = normalize( vec3(LightPosition) - Position );
	vec3 LightIntensity = LightColor * MaterialColor * max( dot( s, Normal ), 0.0 );
	FragColor = vec4(LightIntensity, 1.0);
}