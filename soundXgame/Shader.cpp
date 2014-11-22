#include "Shader.h"


Shader::Shader(void)
{

}

Shader::~Shader(void)
{
	this->freeResources();
}

bool 
Shader::Initialize(void)
{
	this->initResources("triangle.v.glsl", "triangle.f.glsl");
	
	return true;

}

char* Shader::readFile(const char* filename)
{
	// read file as binar
	FILE* input;
	fopen_s(&input, filename, "rb");

	if (input == NULL)
		return NULL;

	// Params: Zeiger auf Datei, offset vom origin, origin
	// Ist Datei leer?
	if (fseek(input, 0, SEEK_END) == -1)
		return NULL;

	// Indikator des Streams ausgeben -> Länge der Datei
	long size = ftell(input);

	if (size == -1)
		return NULL;

	// Anfang der Datei ausgeben
	// Check of Datei leer
	if (fseek(input, 0, SEEK_SET) == -1)
		return NULL;

	// Bytes im Speicher allokieren/allozieren
	char *content = (char*)malloc((size_t)size + 1);

	// Check ob malloc erfolgreich war
	if (content == NULL)
		return NULL;

	// Daten aus Stream lesen
	// Params: Pointer auf Speicherblock, Größe in Bytes der einzelnen Elemente, Anzahl der Elemente, Pointer auf die Datei
	fread(content, 1, (size_t)size, input);

	// Check ob Streamoperationen Fehler erzeugen
	// z.B. Datei zum lesen öffnen und schreiben wollen
	if (ferror(input))
	{
		free(content);
		return NULL;
	}

	fclose(input);
	content[size] = '\0';
	return content;
}

GLuint Shader::createShader(const char* filename, GLenum type)
{
	const GLchar* source = readFile(filename);

	if (source == NULL)
	{
		fprintf(stderr, "Error opening %s. ", filename);
		//std::cerr << "Error opening " << filename;
		perror("");
		return 0;
	}

	GLuint res = glCreateShader(type);

	const GLchar* sources[2] = {
		#ifdef GL_ES_VERSION_2_0
			"#version 100\n"
			"#define GLES2\n",
		#else
			"#version 120\n",
		#endif
	source };

	// Source Code in Shader ersetzen
	glShaderSource(res, 2, sources, NULL);

	// Pointer auf beliebigen Datentyp
	free((void*)source);

	glCompileShader(res);
	GLint compile_ok = GL_FALSE;

	// Gib Parameter aus Shader Objekt zurück
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

	if (compile_ok == GL_FALSE)
	{
		fprintf(stderr, "%s", filename);

		// Shader Objekt zerstören
		glDeleteShader(res);
		return 0;
	}
	return res;
}



bool
Shader::initResources(const char* filenameVertex, const char* filenameFragment)
{
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;
	GLuint vs, fs;

	filenameVertex = strcat("data/Shader/",filenameVertex);
	filenameFragment = strcat("data/Shader/",filenameFragment);

	if ((vs = Shader::createShader(filenameVertex, GL_VERTEX_SHADER)) == 0)
		return false;
	if ((fs = Shader::createShader(filenameFragment, GL_FRAGMENT_SHADER)) == 0)
		return false;

	// program = Kombination aus Fragment und Vertex Shader
	// arbeiten zusammen und vertex shader kann zusätzliche daten an fragment shader übermitteln
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// Parameter eines Programobjekts zurückgeben
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

	if (!link_ok)
	{
		//std::cerr << "glLinkProgram:";
		fprintf(stderr, "glLinkProgram:");
		return false;
	}

	//vermischen von vertex und farbwerten
	
	const char* attribute_name = "coord2d";

	// Speicherort eines Attributs zurüchgeben
	attribute_coord2d = glGetAttribLocation(program, attribute_name);

	if (attribute_coord2d == -1)
	{
		//std::cerr << "Could not bind attribute %s\n" << attribute_name;
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1)
	{
		fprintf(stderr, "Could not find attribute %s\n", attribute_name);
		return false;
	}
	
	GLfloat triangle_attributes[] = {
		 0.0,  0.8,		1.0, 0.0, 0.0,
		-0.8, -0.8,		0.0, 1.0, 0.0,
		 0.8, -0.8,		0.0, 0.0, 1.0,
	};

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	return true;
}

void
Shader::drawShader(void)
{
	
	// use program for shader
	glUseProgram(program);

	//vertexattributepointer farbe + vertex
	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

	glVertexAttribPointer(
		attribute_coord2d,   // attribute
		2,                   // anzahl der elemente pro vertex
		GL_FLOAT,            // datentyp der elemente
		GL_FALSE,            // werte verwenden wie sie sind
		5 * sizeof(GLfloat), // nächste coord2d erscheint alle 5 elemente
		0                    // offset vom ersten element
	);
	glVertexAttribPointer(
		attribute_v_color,					// attribute
		3,									// anzahl der elemente pro vertex
		GL_FLOAT,							// datentyp der elemente
		GL_FALSE,							// werte verwenden wie sie sind
		5 * sizeof(GLfloat),				// nächste coord2d erscheint alle 5 elemente
		(GLvoid*)(2 * sizeof(GLfloat))		// offset vom ersten element
	);

	// Grafikprimitive mit Arraydaten zeichnen
	// Params: mode, startindex, anzahl der indizes die gerendert werden sollen
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// aktivieren eines vertex attribute arrays
	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

	// end program
	glUseProgram(0);

}

void
Shader::freeResources()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
	glDeleteBuffers(1, &vbo_triangle_colors);
}
