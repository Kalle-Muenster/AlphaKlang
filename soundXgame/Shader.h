#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#include "Connectable.h"
#include <math.h>


class Shader :
	public IConnectable
{
private:
	GLuint program;
	// Attribute + Uniforms für Vertices und Farbe
	GLint attribute_coord2d, attribute_v_color;
	GLint uniform_alpha;
	// Buffer für Vertices und Farben
	GLuint vbo_triangle, vbo_triangle_colors;
	
	char* readFile(const char* filename);
	GLuint createShader(const char* filename, GLenum type);
	bool initResources(const char* filenameVertex, const char* filenameFragment);
	void freeResources();
protected:
	void drawShader(void);
public:
	Shader(void);
	~Shader(void);


};

#endif