#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform vec3 CameraPosWorld;

out vec3 Position;
out vec4 PositionModel;
out vec3 Normal;
out vec3 ReflectDir;

void main()
{
	vec4 posHomogenous = vec4(VertexPosition,1.0);

	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * posHomogenous);
	PositionModel = ModelMatrix * posHomogenous;

	//Transform to world space
	vec3 positionWorld = vec3(ModelMatrix * posHomogenous);
	vec3 normalWorld   = normalize(vec3(ModelMatrix * vec4(VertexNormal, 0.0)));
	vec3 camDirectionWorld = normalize( CameraPosWorld - positionWorld);

	ReflectDir = reflect(camDirectionWorld, normalWorld);
	ReflectDir *= -1; //Why so?

    gl_Position = MVP * posHomogenous;
}