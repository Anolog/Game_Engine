
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;
attribute vec4 a_Normal;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform vec3 u_CameraPosition;

varying vec2 v_UV;
varying vec3 v_Reflection;

void main()
{
	vec4 objectpos = vec4(a_Position, 1);
	vec4 worldpos = u_WorldMatrix * objectpos;
	vec4 viewpos = u_ViewMatrix * worldpos;
	vec4 clippos = u_ProjMatrix * viewpos;
	gl_Position = clippos;
	
	vec3 eyesurface = -u_CameraPosition;
    vec3 normal = normalize(objectpos);
    vec3 reflectDir = reflect(eyesurface, normal);

	v_UV = a_UV;
	v_Reflection=reflectDir;

}
