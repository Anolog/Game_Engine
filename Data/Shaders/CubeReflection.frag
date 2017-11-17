
uniform samplerCube u_TextureCube;

varying vec2 v_UV;
varying vec3 v_Reflection;

void main()
{
	gl_FragColor = textureCube( u_TextureCube, v_Reflection );
}
