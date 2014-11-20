#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>


class Shader
{
public:
	Shader(void);
	~Shader(void);
	static char* readFile(const char* filename);
	static GLuint createShader(const char* filename, GLenum type);

};

#endif