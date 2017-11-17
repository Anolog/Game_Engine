
uniform sampler2D u_Texture;
uniform vec4 u_LightColor[3];
uniform vec3 u_LightPosition[3];

varying vec2 v_UV;
varying vec4 v_WSVertPos;

vec4 CalculateLight(vec4 lightColor, vec3 lightPos)
{
	vec4 result = gl_FragColor * lightColor;

	vec4 tempLightPos = vec4(lightPos.x, lightPos.y, lightPos.z, 1);

	vec4 dist = tempLightPos - v_WSVertPos;
	dist.w = 1;
	dist = normalize(dist);

	float dotproduct;
	dotproduct = dot(dist, tempLightPos);

	vec4 diff = gl_FragColor * lightColor * dotproduct;
	diff.w = gl_FragColor.w;

	float atten = dotproduct/0.5 + dist * 0.5 + ((dist * dist) + 0.5);

	return diff * atten; 
}

void main()
{
	vec4 totals[3];

	vec4 wsVertPos = v_WSVertPos;

    gl_FragColor = texture2D( u_Texture, v_UV );

	for (int i = 0; i < 3; i++)
	{
		totals[i] = CalculateLight(u_LightColor[i], u_LightPosition[i]);
	}

	gl_FragColor = totals[0] + totals[1] + totals[2];

}

