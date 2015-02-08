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
	Value.SetUp(0,360,0,step,Value.Cycle);
	Value.ControllerActive=false;


	lastMouse = VectorF(0,0);

	IsUnderControll=false;

	texture.w = 64;
	texture.h = 64;
	texture.format =  GL_RGBA;

	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("GUI/knobA_64x64.png");

	//glGenTextures(1,&texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	//glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.w, texture.h, 0, texture.format, GL_BYTE, texture.pData);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);


	verts[0] = glm::vec3(-0.5,0.5,0);
	verts[1] = glm::vec3(0.5,0.5,0);
	verts[2] = glm::vec3(0.5,-0.5,0);
	verts[3] = glm::vec3(-0.5,-0.5,0);

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
	bool ok = ControllElement::Initialize();
	PositionOnPanel = Panel.GetHalbSize();
	SizeScaledPanel.x = 0.1;
	SizeScaledPanel.y = 0.1;
	return ok;
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

	if(Label[0]!='\0')
		GuiManager::getInstance()->Write(&Label[0],Area.GetPosition(),color);
}

ProjectMappe::Rectangle
Knob::GetArea(void)
{
	VectorF	vec = Panel.GetSize();
	vec.x *= SizeScaledPanel.x;
	vec.y *= SizeScaledPanel.x/2;
	Area.SetSize(vec);

	vec = (Panel.GetPosition() + PositionOnPanel );
	Area.SetPosition(vec);
	
	left=vec.x;
	top=vec.y;

	vec = Area.GetCenter() + Area.GetHalbSize();

	right=vec.x;
	bottom=vec.y;

	return Area;
}


void 
Knob::mouseMotion(int x, int y)
{	
	if(lastMouse.x == x && lastMouse.y == y )
		return;

	if(IsUnderControll)
	{
		lastMouse.x = x-lastMouse.x;
		lastMouse.y = y-lastMouse.y;

		Value = Value + lastMouse.y;

		sprintf(&Label[0],"%f",(float)Value);

		lastMouse.x = x;
		lastMouse.y = y;

		color.u32 = 0xffffff00;
	}
	else
	{
		INPUT->DetachMouseMove(this);
		color.u32 = 0xffffffff;
	}
}

void 
Knob::mouseClicks(int button,bool IsPressed,VectorF position)
{
	GetArea();


	if(button==0)
	{
		if(Area.Containes(position))
		{
			IsUnderControll = IsPressed;
			
			if(IsUnderControll)
				INPUT->attachMouseMove(this);

			//lastMouse = VectorF(left + (Value * (right-left)),Area.GetCenter().y);
			//glutWarpPointer(lastMouse.x,lastMouse.y);
		}
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
		VectorF values = GetArea().GetCenter();

		glTranslatef(values.x, values.y, 0);

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
		//translate:
		VectorF pos = Area.GetCenter();
		glTranslatef(pos.x, pos.y, 0);

		// Rotate:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle + position, 0, 0, -1);

		// Scaling:
		pos.x = right-left;
		pos.y = bottom-top;
		glScalef(pos.x,pos.y,0);

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

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// Translation:
		VectorF values = Area.GetCenter();
		glTranslatef(values.x, values.y, 0);

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


