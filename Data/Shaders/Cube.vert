
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

varying vec2 v_UV;
varying vec4 v_ObjectPos;

void main()
{
	vec4 objectpos = vec4(a_Position, 1);
	vec4 worldpos = u_WorldMatrix * objectpos;
	vec4 viewpos = u_ViewMatrix *vec4(objectpos.x,objectpos.y,objectpos.z,0);
	vec4 clippos = u_ProjMatrix *vec4(viewpos.x,viewpos.y,viewpos.z,1);
	gl_Position = clippos;

	v_UV = a_UV;
	v_ObjectPos=objectpos;


}
