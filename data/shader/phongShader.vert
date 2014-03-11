#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform vec3 CameraPosWorld;

uniform mat4 ShadowMatrix;

out vec3 Position;
out vec3 Normal;
out vec3 ReflectDir;
out vec4 ShadowCoord;

void main()
{
	vec4 posHomogenous = vec4(VertexPosition,1.0);

	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * posHomogenous);

	//Transform to world space
	vec3 positionWorld = vec3(ModelMatrix * posHomogenous);
	vec3 normalWorld   = normalize(vec3(ModelMatrix * vec4(VertexNormal, 0.0)));
	vec3 camDirectionWorld = normalize( CameraPosWorld - positionWorld);

	ReflectDir = reflect(camDirectionWorld, normalWorld);
	ReflectDir *= -1; //Why so?

	ShadowCoord = ShadowMatrix * ModelMatrix * posHomogenous;

    gl_Position = MVP * posHomogenous;
}