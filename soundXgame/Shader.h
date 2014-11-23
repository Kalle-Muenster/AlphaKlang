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
	
	bool initResources(const char* filenameVertex, const char* filenameFragment);
	GLuint createShader(const char* filename, GLenum type);
	char* readFile(const char* filename);
	void freeResources();
protected:
	void drawBegin(void);
	void drawEnd(void);
public:
	Shader(void);
	~Shader(void);


};

#endif