#version 400

in vec3 VertexPosition;
in vec3 VertexColor;

uniform mat4 mvp;

out vec3 Color;

void main()
{
  Color = VertexColor;
  gl_Position = mvp * vec4( VertexPosition, 1.0 );
}