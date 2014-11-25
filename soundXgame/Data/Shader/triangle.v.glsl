// VERTEX SHADER

// input variables for vertex
attribute vec3 coord3d;
//attribute vec4 v_color;

// varying for fragment
varying vec4 f_color;					

void main(void) 
{								
	// resultierende Screenposition. built-in Outputvariable
	gl_Position = vec4(coord3d, 1.0);
	//f_color = v_color;
	f_color = vec4(1.0, 0, 0, 0.3);
}