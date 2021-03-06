
attribute vec4 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform vec3 u_CameraPosition;
uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

varying vec2 v_UV;
varying vec4 v_WSVertPos;

void main()
{
    vec4 objectpos = a_Position;
    vec4 worldpos = u_WorldMatrix * objectpos;
    vec4 viewpos = u_ViewMatrix * worldpos;
    vec4 clippos = u_ProjMatrix * viewpos;
	
	v_WSVertPos = worldpos;

    gl_Position = clippos;

    v_UV = a_UV;
}
