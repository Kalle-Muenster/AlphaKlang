#include "Knob.h"
#include "Utility.h"
#include <math.h>

bool const
	Knob::canHaveMultipleInstances = true;

#define INPUT InputManager::getInstance()

Knob::Knob(void)
{
	TypeHashCode = (unsigned)typeid(Knob).hash_code();
	float  step=1.0f;
	Value.SetUp(0,360,0,step,Value.Clamp);
	Value.ControllerActive=true;


	lastMouse = VectorF(0,0);

	IsUnderControll=false;

	texture.w = 64;
	texture.h = 64;
	texture.format =  GL_RGBA;

	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("knobA_64x64.png");

	//glGenTextures(1,&texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	//glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.w, texture.h, 0, texture.format, GL_BYTE, texture.pData);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);


	verts[0] = glm::vec3(0,0,0);
	verts[1] = glm::vec3(1,0,0);
	verts[2] = glm::vec3(1,-1,0);
	verts[3] = glm::vec3(0,-1,0);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);



	uvs[0]=glm::vec2(0,0.5);
	uvs[1]=glm::vec2(0.5,0.5);
	uvs[2]=glm::vec2(0.5,0);
	uvs[3]=glm::vec2(0,0);
	glGenBuffers(1, &frameUVBuffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[0], GL_STATIC_DRAW);

	uvs[4]=glm::vec2(0.5,0.5);
	uvs[5]=glm::vec2(1,0.5);
	uvs[6]=glm::vec2(1,0);
	uvs[7]=glm::vec2(0.5,0);
	glGenBuffers(1, &frameUVBuffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[4], GL_STATIC_DRAW);

	uvs[8]=glm::vec2(0,1);
	uvs[9]=glm::vec2(0.5,1);
	uvs[10]=glm::vec2(0.5,0.5);
	uvs[11]=glm::vec2(0,0.5);

	glGenBuffers(1, &frameUVBuffers[2]);
	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[8], GL_STATIC_DRAW);

	uvs[12]=glm::vec2(0.5,1);
	uvs[13]=glm::vec2(1,1);
	uvs[14]=glm::vec2(1,0.5);
	uvs[15]=glm::vec2(0.5,0.5);

	glGenBuffers(1, &frameUVBuffers[3]);
	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[12], GL_STATIC_DRAW);



	INPUT->attachMouseClick(this);
}


Knob::~Knob(void)
{
	INPUT->DetachMouseClick(this);
}

bool Knob::Initialize(void)
{
	Area = *ProjectMappe::Rectangle::Zero;

//	GuiManager::getInstance()->Add(this);

	PositionOnPanel = Panel.GetHalbSize();
	SizeScaledPanel.x = 0.25;
	SizeScaledPanel.y = 0.25;
	this->angle = 0;
	GetArea();
	return true;
}





void 
Knob::DoUpdate(void)
{
	GetArea();
	if(IsUnderControll)
	{
		if(INPUT->Mouse.LEFT.RELEASE)
			this->IsUnderControll = false;
	}
}

void 
Knob::mouseMotion(int x, int y)
{	
	if(lastMouse.x==x &&lastMouse.y==y )
		return;

if(IsUnderControll)
{
	lastMouse.x = x-lastMouse.x;
	lastMouse.y = y-lastMouse.y;

	Value = Value + lastMouse.y;

	lastMouse.x = x;
	lastMouse.y = y;
}
else
	INPUT->DetachMouseMove(this);
}

void 
Knob::mouseClicks(int button,bool IsPressed,VectorF position)
{
	GetArea();
	if((button!=1) &&  Area.Containes(position))
	{
		if(IsPressed)
			INPUT->attachMouseMove(this);


		IsUnderControll=button==0?IsPressed:IsUnderControll;
		//lastMouse = VectorF(left + (Value * (right-left)),Area.GetCenter().y);
		//glutWarpPointer(lastMouse.x,lastMouse.y);
	}
}

void 
Knob::draw(void)
{
	if(!vertexBufferID)
		return;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	_DrawBackground();
	_DrawBar(Value);
	_DrawForeground();	

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

void 
Knob::_DrawBackground(void)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// Translation:
		VectorF values = (Panel.GetPosition() + PositionOnPanel);

		glTranslatef(values.x, values.y+Area.GetSize().y, 0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		values = GetArea().GetSize();
		glScalef(values.x,values.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);

}

void 
Knob::_DrawBar(float position)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// rotating the chart:
		VectorF pos = GetArea().GetHalbSize() / 2;
		pos.y = -pos.y;
		glTranslatef(pos.x,pos.y,0);
		glRotatef(position,0,0,-1);
		glTranslatef(-pos.x,-pos.y,0);

		//translate:
		pos = ( Panel.GetPosition() + PositionOnPanel);
		glTranslatef(pos.x, pos.y+Area.GetSize().y, 0);

		// Rotate:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		pos.x = right-left;
		pos.y = bottom-top;
		glScalef(Value * pos.x * 0.99,pos.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);

	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void 
Knob::_DrawForeground(void)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[3]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// Translation:
		VectorF values = (Panel.GetPosition() + PositionOnPanel);
		glTranslatef(values.x, values.y+Area.GetSize().y, 0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		values = Area.GetSize();
		glScalef(values.x,values.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);
}


