
uniform sampler2D u_Texture;
uniform float u_Time;

varying vec2 v_UV;

void main()
{
    gl_FragColor = texture2D( u_Texture, v_UV );

	float totalTime = 0.0f;
	totalTime += u_Time;

	vec4 flashCycle = vec4(sin(u_Time), sin(u_Time), sin(u_Time), 1.0f);

	if (totalTime >= 1.0f)
	{
		gl_FragColor.xyz -= flashCycle.xyz;
	}

	if (totalTime <= 0.0f)
	{
		gl_FragColor.xyz += flashCycle.xyz;
	}
	
	if (totalTime >= 2.0f)
	{
		totalTime = 0.0f;
	}

}
