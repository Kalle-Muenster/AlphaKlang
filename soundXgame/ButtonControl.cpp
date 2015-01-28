#include "ButtonControl.h"
#include "projectMacros.h"


 const char*
ButtonControl::States[] = {"DISABLED","NORMAL","HOVER","PRESSED"};

ButtonControl::ButtonControl(void)
{
	btnState = ButtonState::NORMAL;


    texture.w = 64;
    texture.h = 64;
	texture.format =  GL_RGBA;
	
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("button_64x64.png");
	
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
ButtonControl::DoEarly(void)
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

//	GuiManager::getInstance()->Write(States[btnState],10,160);
}

	bool ButtonControl::Initialize(void)
	{
		Area = *ProjectMappe::Rectangle::Zero;
		UPDATE->SignOutFromUpdate(this);
		UPDATE->SignInForEarlyUpdate(this);
		GuiManager::getInstance()->Add(this);
		Panel = ProjectMappe::Rectangle(&this->Connection()->getTransform()->position.x,
										&this->Connection()->getTransform()->position.y,
										&this->Connection()->getTransform()->scale.x,
										&this->Connection()->getTransform()->scale.y );
		PositionOnPanel = Panel.GetHalbSize();
		SizeScaledPanel.x = 0.1;
		SizeScaledPanel.y = SizeScaledPanel.x/4;
		this->angle = 0;
		GetArea();



		return true;
	}

	ProjectMappe::Rectangle
	ButtonControl::GetArea(void)
	{

		VectorF	vec = Panel.GetSize();
		vec.x *= SizeScaledPanel.x;
		vec.y = vec.x/4;
		Area.SetSize(vec);
		vec = (Panel.GetPosition() + PositionOnPanel )-Area.GetHalbSize();
		Area.SetPosition(vec);
		left=Area.GetCenter().x-Area.GetHalbSize().x;
		right=Area.GetCenter().x+Area.GetHalbSize().x;
		top=Area.GetCenter().y-Area.GetHalbSize().y;
		bottom=Area.GetCenter().y+Area.GetHalbSize().y;
		return Area;
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
				ClickAction(this);

			printf("ButtonState: %s...\n",States[btnState]);
		}
	}

	void 
	ButtonControl::SetClickerFunc(action clicFunc)
	{
		ClickAction = clicFunc;
	}

	char tempstring[32];

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
		VectorF values = Panel.GetPosition() + PositionOnPanel;

		glTranslatef(values.x, values.y, 0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		values = GetArea().GetSize();
		glScalef(values.x,values.y,0);
		
		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	}
