#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition; //in eye coords
uniform vec3 MaterialColor;	//diffuse reflectivity
uniform vec3 LightColor;	//light source intensity

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

void main()
{
	vec3 normal = normalize(NormalMatrix * VertexNormal);

	vec4 eyeCoords = ModelViewMatrix * vec4( VertexPosition, 1.0f);
	vec3 s = normalize( vec3(LightPosition - eyeCoords) );

	LightIntensity = LightColor * MaterialColor * max( dot( s, normal ), 0.0 );

    gl_Position = MVP * vec4( VertexPosition, 1.0 );	
}
