#include "ButtonControl.h"
#include "projectMacros.h"


bool const
ButtonControl::canHaveMultipleInstances = true;

 const char*
ButtonControl::States[] = {"DISABLED","NORMAL","HOVER","PRESSED"};

ButtonControl::ButtonControl(void)
{
	TypeHashCode = (unsigned)typeid(ButtonControl).hash_code();
	btnState = ButtonState::NORMAL;
	SetColor(0,0,0,255);

    texture.w = 64;
    texture.h = 64;
	texture.format =  GL_RGBA;
	
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("GUI/buttons_256x256.png");
	
	//glGenTextures(1,&texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	//glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.w, texture.h, 0, texture.format, GL_BYTE, texture.pData);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);
	

	verts[0] = glm::vec3(-0.5, 0.5,0);
	verts[1] = glm::vec3( 0.5, 0.5,0);
	verts[2] = glm::vec3( 0.5,-0.5,0);
	verts[3] = glm::vec3(-0.5,-0.5,0);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	
	
	for(int Ypos=0;Ypos<4;Ypos++)
	{
		float p = 1.f/4.f;
		uvs.push_back(glm::vec2(0,p*Ypos));
		uvs.push_back(glm::vec2(1,p*Ypos));
		uvs.push_back(glm::vec2(1,p*Ypos+p));
		uvs.push_back(glm::vec2(0,p*Ypos+p));
		
		glGenBuffers(1, &frameUVBuffers[Ypos]);
		glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[Ypos]);
		glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[Ypos*4], GL_STATIC_DRAW);
	}
}


ButtonControl::~ButtonControl(void)
{
}

char bufX[32];
char bufY[32];
VectorF mPosition;

void
ButtonControl::DoUpdate(void)
{
	GetArea();

	if(GetState()==DISABLED)
		return;

	if(GetArea().Containes(INPUT->Mouse.Position))
	{
		SetState(HOVER);
		if(INPUT->Mouse.LEFT.CLICK||INPUT->Mouse.LEFT.HOLD)
			SetState(PRESSED);

		if(INPUT->Mouse.LEFT.RELEASE)
			SetState(NORMAL);
	}
	else
		SetState(NORMAL);

	if(Label[0]!='\0')
		GUI->Write(&Label[0],left+Area.GetHalbSize().x/2,bottom-Area.GetHalbSize().y/1+5,color);
}



	ButtonControl::ButtonState 
	ButtonControl::GetState(void)
	{
		return (ButtonState)btnState;
	}

	void ButtonControl::SetState(ButtonState state)
	{
		if(state!=btnState)
		{
			btnState = (state<2)? state : (btnState!=DISABLED? state : btnState);

			if(btnState==PRESSED)
			{
				ClickAction(this);
	//			*Switch = !(*Switch);
			}

			printf("GUI: Panel: %s, Button: %s, State: %s...\n",Connection()->GetName(),this->GetText(),States[btnState]);
		}
	}

	void 
	ButtonControl::SetClickerFunc(action clicFunc)
	{
		ClickAction = clicFunc;
	}

   void
	ButtonControl::SetSwitch(bool* pointerToSwitch)
	{
	   Switch = pointerToSwitch;
	}

	void ButtonControl::draw(void)
	{
		if(!vertexBufferID)
			return;
		
		
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);


		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	
	

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[btnState]);
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



	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	}
