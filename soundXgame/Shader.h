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

	int initResourcesShader();
	void drawShader();
	void freeResources();
public:
	Shader(void);
	~Shader(void);
	virtual bool Initialize(void);
	static char* readFile(const char* filename);
	static GLuint createShader(const char* filename, GLenum type);

};

#endif