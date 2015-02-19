// VERTEX SHADER

// input variables for vertex
attribute vec3 coord3d;
//attribute vec4 v_color;

// varying for fragment
varying vec4 f_color;	

uniform float Scale;				
uniform mat4 mvp;

void main(void) 
{	

	//coord3d.x = coord3d.x * Scale;
	//coord3d.y = coord3d.y * Scale;
	
	//gl_Position = mvp * vec4(coord3d, 1);
	
	// resultierende Screenposition. built-in Outputvariable
	gl_Position = vec4(coord3d, 1.0);
	
	//f_color = v_color;
	f_color = vec4(1.0, 0, 0, 0.3);
}