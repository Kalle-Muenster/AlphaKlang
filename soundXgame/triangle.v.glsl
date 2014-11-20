// VERTEX SHADER

// Aktueller Vertex, Input-Variable
attribute vec2 coord2d;

// time


// wird ohne Verwendung nicht funktionieren
attribute vec3 v_color;		

varying vec3 f_color;					

void main(void) 
{								
	// resultierende Screenposition. built-in Outputvariable
	gl_Position = vec4(coord2d, 0.0, 1.0);
	f_color = v_color;
}