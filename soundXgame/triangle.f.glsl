// FRAGMENT SHADER

varying vec3 f_color;

void main(void) 
{			
	gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);
	
	/*
	gl_FragColor[0] = 1.0;		// R
	gl_FragColor[1] = 0.0;		// G
	gl_FragColor[2] = 1.0;		// B
	*/
}