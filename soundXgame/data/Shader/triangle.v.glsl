// VERTEX SHADER

// input variables for vertex
attribute vec2 coord2d;
attribute vec4 v_color;

// varying for fragment
varying vec4 f_color;					

void main(void) 
{								
	// resultierende Screenposition. built-in Outputvariable
	gl_Position = vec4(coord2d, 0.0, 1.0);
	f_color = v_color;
}