#include "ShaderObj.h"
#include "projectMacros.h"

ShaderObj::ShaderObj(void)
{
	
	this->InitializeObject();
	
	this->transform.position = Vector3(1,2,3);
	//this->IsGrounded = true;

	UpdateManager::getInstance()->SignInForUpdate(this);
}

ShaderObj::~ShaderObj(void)
{
}

void
ShaderObj::DoUpdate(void)
{

}

void
ShaderObj::InitializeObject(void)
{
	SetID(SCENE->Add(this));
	LoadMesh("cube_quads.obi");
	LockID();
	this->LoadTexture("X-512.jpg");
}

void
ShaderObj::draw(void)
{
	// HELP for texCoords
	// http://stackoverflow.com/questions/23163683/opengl-vertex-shader-transform-object-disappearing


	// use program for shader
	glUseProgram(program);

	//vertexattributepointer farbe + vertex
	glEnableVertexAttribArray(attribute_coord3d);
	//glEnableVertexAttribArray(attribute_v_color);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(
		attribute_coord3d,   // attribute
		3,                   // anzahl der elemente pro vertex
		GL_FLOAT,            // datentyp der elemente
		GL_FALSE,            // werte normalisieren
		NULL,//3 * sizeof(GLfloat), // nächste coord2d erscheint alle x elemente
		0                    // offset vom ersten element
	);

	/*glVertexAttribPointer(
		attribute_v_color,					// attribute
		3,									// anzahl der elemente pro vertex
		GL_FLOAT,							// datentyp der elemente
		GL_FALSE,							// werte normalisieren
		6 * sizeof(GLfloat),				// nächste v_color erscheint alle 5 elemente
		(GLvoid*)(2 * sizeof(GLfloat))		// offset vom dritten element
	);*/

	//this->drawBegin();

	IMeshObject::draw();

	
	// aktivieren eines vertex attribute arrays
	glDisableVertexAttribArray(attribute_coord3d);
	//glDisableVertexAttribArray(attribute_v_color);
	// end program
	glUseProgram(0);
	//this->drawEnd();

}