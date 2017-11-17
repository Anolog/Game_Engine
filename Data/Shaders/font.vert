
attribute vec4 a_Position;
attribute vec2 a_UV;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

varying vec2 v_UV;

void main()
{
	vec4 objectpos = a_Position;

	vec4 worldpos = u_WorldMatrix * objectpos;
	vec4 viewpos = u_ViewMatrix * worldpos;
	vec4 clippos = u_ProjMatrix * viewpos;

	gl_Position = clippos;

	v_UV = a_UV * u_UVScale + u_UVOffset;
}
