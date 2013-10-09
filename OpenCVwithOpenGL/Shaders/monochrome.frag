uniform sampler2D tex;

void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);

	//float mono = ((color.x) + (color.y) + (color.z))/3.0;

	gl_FragColor =  vec4(color.x,color.y,color.z,0.6);
}