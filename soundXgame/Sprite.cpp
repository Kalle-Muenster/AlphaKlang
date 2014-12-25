#include "Sprite.h"
#include "TargetGrabber.h"




Sprite::Sprite(void)
{
	
}

Sprite::Sprite(Texture particle,bool addToScene)
{
	verts.push_back(Vector3(-1,1,0));
	verts.push_back(Vector3(1,1,0));
	verts.push_back(Vector3(1,-1,0));
	verts.push_back(Vector3(-1,-1,0));

	uvs.push_back(glm::vec2(0,0));
	uvs.push_back(glm::vec2(1,0));
	uvs.push_back(glm::vec2(1,1));
	uvs.push_back(glm::vec2(0,1));

	FaceShape=GL_QUADS;

	glm::vec3 temp1 = glm::vec3(0,0,-1);
	glm::vec3 temp2 = glm::vec3(1,0,0);
	glm::vec3 temp3 = glm::vec3(0,1,0);

	transform.forward = Vector3(temp1.x,temp1.y,temp1.z);
	transform.right = temp2;
	transform.up = temp3;



	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	IGObject::InitializeObject(addToScene);
	
	SetUp(particle,true);
	SetName("particle");
	if(!addToScene)
		LockID();

	SomeValue = 0;
}


Sprite::Sprite(char* filename,bool addToScene)
{
	verts.push_back(Vector3(-1,1,0));
	verts.push_back(Vector3(1,1,0));
	verts.push_back(Vector3(1,-1,0));
	verts.push_back(Vector3(-1,-1,0));

	uvs.push_back(glm::vec2(0,0));
	uvs.push_back(glm::vec2(1,0));
	uvs.push_back(glm::vec2(1,1));
	uvs.push_back(glm::vec2(0,1));

	FaceShape=GL_QUADS;

	glm::vec3 temp1 = glm::vec3(0,0,-1);
	glm::vec3 temp2 = glm::vec3(1,0,0);
	glm::vec3 temp3 = glm::vec3(0,1,0);

	transform.forward = temp1;
	transform.right = temp2;
	transform.up = temp3;



	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	IGObject::InitializeObject(addToScene);
	
	SetUp(filename,true);
	SetName(filename);
	if(!addToScene)
		LockID();

	SomeValue = 0;
}


Sprite::~Sprite(void)
{

}

Texture*
Sprite::GetTexture(void)
{
	return &texture;
}

void 
Sprite::SetTexture(Texture tex)
{
	texture=tex;
}

void 
Sprite::SetUp(string textureFile,bool backFace)
{	
	LoadTexture(textureFile);
	this->NoBackfaceCulling = backFace;

	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
}

void 
Sprite::SetUp(Texture texture,bool backFace)
{
	this->texture = texture;
	this->NoBackfaceCulling = backFace;

	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
}

void
Sprite::SetSomeValue(float value)
{
	this->SomeValue = value;
}

void 
Sprite::draw(void)
{
	Transform temp = *getTransform();
	for(int i = 0;i<3;i++)
	{
		IMeshObject::draw();
		getTransform()->position.x-=2;
	}
	getTransform()->position = temp.position;
}