#include "Shader.h"


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
		//std::cerr << filename;

		// Shader Objekt zerstören
		glDeleteShader(res);
		return 0;
	}
	return res;
}
