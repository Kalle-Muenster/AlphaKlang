#include "SliderX.h"
#include "Utility.h"
#include <math.h>

#define INPUT InputManager::getInstance()
SliderX::SliderX(void)
{
	float  step=1.f/128.f;
	ValueX.SetUp(0,1,0,step,ValueX.Clamp);
	ValueX.ControllerActive=true;
	ValueY.SetUp(0,1,0,step,ValueY.Clamp);
	ValueY.ControllerActive=true;

	lastMouse = VectorF(0,0);
	DimensionsSwitched=false;
	YIsUnderControll=XIsUnderControll = false;

    texture.w = 64;
    texture.h = 64;
	texture.format =  GL_RGBA;
	
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("sliderX-2color_64x64.png");
	
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
	


	 	uvs[0]=glm::vec2(0,ValueY.MOVE);
		uvs[1]=glm::vec2(0.5,ValueY.MOVE);
		uvs[2]=glm::vec2(0.5,0);
		uvs[3]=glm::vec2(0,0);
		glGenBuffers(1, &frameUVBuffers[0]);
		glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[0]);
		glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[0], GL_STATIC_DRAW);

		uvs[4]=glm::vec2(0.5,ValueY.MOVE);
		uvs[5]=glm::vec2(1,ValueY.MOVE);
		uvs[6]=glm::vec2(1,0);
		uvs[7]=glm::vec2(0.5,0);
		glGenBuffers(1, &frameUVBuffers[1]);
		glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[1]);
		glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[4], GL_STATIC_DRAW);


	for(int Ypos=0;Ypos<2;Ypos++)
	{
		float p = 1.f/4.f;
		uvs[8+(Ypos)*4]=glm::vec2(0,p*(Ypos+2));
		uvs[9+(Ypos)*4]=glm::vec2(1,p*(Ypos+2));
		uvs[10+(Ypos)*4]=glm::vec2(1,p*(Ypos+2)+p);
		uvs[11+(Ypos)*4]=glm::vec2(0,p*(Ypos+2)+p);
		
		glGenBuffers(1, &frameUVBuffers[Ypos+2]);
		glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[Ypos+2]);
		glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[(Ypos+2)*4], GL_STATIC_DRAW);
	}

	INPUT->attachMouseClick(this);
}


SliderX::~SliderX(void)
{
	INPUT->DetachMouseClick(this);
}

bool SliderX::Initialize(void)
{
		Area = *ProjectMappe::Rectangle::Zero;
		//UpdateManager::getInstance()->SignOutFromUpdate(this);
		//UpdateManager::getInstance()->SignInForEarlyUpdate(this);
		GuiManager::getInstance()->Add(this);
		Panel = ProjectMappe::Rectangle(&this->Connection()->getTransform()->position.x,
										&this->Connection()->getTransform()->position.y,
										&this->Connection()->getTransform()->scale.x,
										&this->Connection()->getTransform()->scale.y );
		PositionOnPanel = Panel.GetHalbSize();
		SizeScaledPanel.x = 0.3;
		SizeScaledPanel.y = SizeScaledPanel.x/4;
		this->angle = 0;
		GetArea();
		return true;
}

	ProjectMappe::Rectangle
	SliderX::GetArea(void)
	{
		VectorF	vec = Panel.GetSize();
		vec.x *= SizeScaledPanel.x;
		vec.y = vec.x/4;
		Area.SetSize(vec);
		vec = (Panel.GetPosition() + PositionOnPanel );
		Area.SetPosition(vec);
		left=Area.GetCenter().x-Area.GetHalbSize().x;
		right=Area.GetCenter().x+Area.GetHalbSize().x;
		top=Area.GetCenter().y-Area.GetHalbSize().y;
		bottom=Area.GetCenter().y+Area.GetHalbSize().y;
		ValueY.MOVE=1/((right-left)+2);
		return Area;
	}

void SliderX::DoUpdate(void)
{
	GetArea();
	if(XIsUnderControll)
	{


	if(INPUT->Mouse.LEFT.DOUBLE )
	{ 
		DimensionsSwitched = !DimensionsSwitched; 
		return;
	}

	if(INPUT->Mouse.LEFT.RELEASE)
		this->XIsUnderControll = false;

	}

	if(YIsUnderControll)
		if(INPUT->Mouse.MIDDLE.RELEASE)
			this->YIsUnderControll = false;
}

void 
SliderX::mouseMotion(int x, int y)
{	if(lastMouse.x==x &&lastMouse.y==y )
		return;
	if(XIsUnderControll||YIsUnderControll)
	{
		lastMouse.x = x-lastMouse.x;
		lastMouse.y = y-lastMouse.y;
		
		//if(INPUT->Mouse.LEFT.DOUBLE)
		//{
		//	DimensionsSwitched = !DimensionsSwitched;
		////	XIsUnderControll = YIsUnderControll = false;
		//}
		
		if(DimensionsSwitched)
		{	
			if(YIsUnderControll)
				ValueX = (ValueX + (lastMouse.y / Area.GetSize().x));
			if(XIsUnderControll)
				ValueY = (ValueY + (lastMouse.x / Area.GetSize().x));
		} 
		else
		{
			if(XIsUnderControll)
				ValueX = ValueX + (lastMouse.x / Area.GetSize().x);
			if(YIsUnderControll)
				ValueY = ValueY + (lastMouse.y / Area.GetSize().x);
		}

		lastMouse.x = x;
		lastMouse.y = y;

	}
	else
		INPUT->DetachMouseMove(this);
}

void 
SliderX::mouseClicks(int button,bool IsPressed,VectorF position)
{
	GetArea();
	 if((button!=1) &&  Area.Containes(position))
	 {
		 if(IsPressed)
			INPUT->attachMouseMove(this);


		 XIsUnderControll=button==0?IsPressed:XIsUnderControll;
		 YIsUnderControll=button==2?IsPressed:YIsUnderControll;
		 lastMouse = VectorF(left + ((DimensionsSwitched?ValueY:ValueX) * (right-left)),Area.GetCenter().y);
		 glutWarpPointer(lastMouse.x,lastMouse.y);
		 
	 }
}

void SliderX::draw(void)
{
	if(DimensionsSwitched)
	{
		GuiManager::getInstance()->Write("Dimensions Switched",GetArea().GetPosition().x,GetArea().GetPosition().y);
	}
	else
		GuiManager::getInstance()->Write("Dimensions NOT Switched",GetArea().GetPosition().x,GetArea().GetPosition().y);

	if(!vertexBufferID)
		return;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	_DrawBackground();
	_DrawBar(DimensionsSwitched?ValueY:ValueX);
	_DrawForeground();	

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

void 
SliderX::_DrawBackground(void)
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
		VectorF values = (Panel.GetPosition() + PositionOnPanel);

		glTranslatef(values.x, values.y+Area.GetSize().y, 0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		values = GetArea().GetSize() * 0.98f;
		glScalef(values.x,values.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);

}

void 
SliderX::_DrawBar(float position)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	short i = DimensionsSwitched ? 4:0;
	uvs[i].y = uvs[i+1].y = (DimensionsSwitched ? ValueX + ValueX.MOVE : ValueY + ValueY.MOVE)/2.f;
	uvs[i+2].y = uvs[i+3].y =  (uvs[i].y + (DimensionsSwitched ? ValueX.MOVE : ValueY.MOVE ))/2.f;



	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[DimensionsSwitched?1:0]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	glPushMatrix();
	{
		// Translation:
		VectorF pos = ( Panel.GetPosition() + PositionOnPanel);

		//glTranslatef((pos.x+(DimensionsSwitched ? ValueY:ValueX )*size.x)-size.x, pos.y, 0);
		glTranslatef(pos.x, pos.y+Area.GetSize().y, 0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		pos.x = right-left;
		pos.y = bottom-top;

		// Scaling:

		glScalef((DimensionsSwitched ? ValueY :ValueX ) * pos.x * 0.99,pos.y,0);

		
		glBegin(GL_QUADS);
		glTexCoord2f(uvs[i].x,uvs[i].y);
		glVertex3f(verts[0].x,verts[0].y,verts[0].z);
		i++;
		glTexCoord2f(uvs[i].x,uvs[i].y);
		glVertex3f(verts[1].x,verts[1].y,verts[1].z);
		i++;
		glTexCoord2f(uvs[i].x,uvs[i].y);
		glVertex3f(verts[2].x,verts[2].y,verts[2].z);
		i++;
		glTexCoord2f(uvs[i].x,uvs[i].y);
		glVertex3f(verts[3].x,verts[3].y,verts[3].z);
		glEnd();

	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void 
SliderX::_DrawForeground(void)
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
		values = GetArea().GetSize();
		glScalef(values.x,values.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);
}