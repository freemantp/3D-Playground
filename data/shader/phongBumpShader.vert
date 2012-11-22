#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;	
layout (location = 2) in vec4 VertexTangent;	
layout (location = 3) in vec2 VertexTexCoord;

uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec3 light_vector;
out vec3 normal_vector;
out vec3 halfway_vector;
out vec2 texture_coord;

void main()
{
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	vec3 lightPosEyeSP = vec3(0,1,1);
	
	vec3 norm = normalize(NormalMatrix * VertexNormal);
	vec3 tang = normalize(NormalMatrix * VertexTangent.xyz);
	vec3 bitang = normalize( cross(norm, tang) ) * VertexTangent.w;
 
    mat3 tbni = inverse(mat3(tang, bitang, norm));
 
    vec4 v  = ModelMatrix * vec4(VertexPosition, 1.0);                  // vertex in model space
    light_vector  = tbni * (inverse(ModelMatrix) * vec4(lightPosEyeSP - v.xyz, 0)).xyz;  // light vector in tangent space
 
    v = ViewMatrix * ViewMatrix * vec4(VertexPosition, 1.0);                       // vertex in eye space
    vec4 light_vector_eye  = normalize((ViewMatrix * vec4(lightPosEyeSP, 1.0)) - v); // light vector in eye space
    vec4 viewer_vector_eye = normalize(-v);                     // view vector in eye space
 
    halfway_vector = tbni * (inverse(ViewMatrix * ModelMatrix) * vec4((light_vector_eye.xyz + viewer_vector_eye.xyz), 0.0)).xyz;
                                            // halfway vector in tangent space
    normal_vector = inverse(transpose(tbni)) * VertexNormal;
 
    texture_coord = VertexTexCoord.xy;
}