
uniform samplerCube u_TextureCube;

varying vec2 v_UV;
varying vec4 v_ObjectPos;

void main()
{
    //if(v_ObjectPos.z>=1)
	gl_FragColor = textureCube( u_TextureCube, v_ObjectPos );
}
