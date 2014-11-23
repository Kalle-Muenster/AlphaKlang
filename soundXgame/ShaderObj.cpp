#include "ShaderObj.h"
#include "projectMacros.h"

ShaderObj::ShaderObj(void)
{
	
	this->InitializeObject();
	
	this->LoadTexture("X-512.jpg");
	this->transform.position = Vector3(1,2,3);
	this->IsGrounded = true;

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
	

}

void
ShaderObj::draw(void)
{
	//this->drawBegin();

	// TEST ... JUST A CUBE - 3
	IMeshObject::draw();

	//this->drawEnd();

}