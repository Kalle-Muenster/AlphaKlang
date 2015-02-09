#include "SliderX.h"
#include "Utility.h"
#include <math.h>

#define INPUT InputManager::getInstance()


bool const // set the "SliderX" class NOT to be Per-Object-Singleton... (we want to use as many sliders on a panel we need.)
SliderX::canHaveMultipleInstances = true;

SliderX::SliderX(void)
{
	TypeHashCode = (unsigned)typeid(SliderX).hash_code();


	//setting the slider's values to be in clambt ranges from 0-1 as default-values 
	float  step=1.f/128.f;
	ValueX.SetUp(0,1,0.5,step,ValueX.Clamp);
	ValueX.ControllerActive=true;
	ValueY.SetUp(0,1,0.5,step,ValueY.Clamp);
	ValueY.ControllerActive=true;

	lastMouse = VectorF(0,0);
	DimensionsSwitched=false;
	YIsUnderControll=XIsUnderControll = false;


	//loading the slider's default-skin-image
    texture.w = 64;
    texture.h = 64;
	texture.format =  GL_RGBA;
	
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture("GUI/sliderX_64x64.png");
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	
	// Setting up the vertex-buffer. (maybe a primitive here better.)
	verts[0] = glm::vec3(0,0,0);
	verts[1] = glm::vec3(1,0,0);
	verts[2] = glm::vec3(1,-1,0);
	verts[3] = glm::vec3(0,-1,0);
    // and read it out to GL 
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
	

		//setting the UV's for the color-bars (X and Y)
	 	uvs[0]=glm::vec2(0,ValueY.MOVE);
		uvs[1]=glm::vec2(0.5,ValueY.MOVE);
		uvs[2]=glm::vec2(0.5,0);
		uvs[3]=glm::vec2(0,0);
		//...
		uvs[4]=glm::vec2(0.5,ValueY.MOVE);
		uvs[5]=glm::vec2(1,ValueY.MOVE);
		uvs[6]=glm::vec2(1,0);
		uvs[7]=glm::vec2(0.5,0);


	//setting up the UV's for the background image and foreground-frame
	for(int Ypos=0;Ypos<2;Ypos++)
	{
		float p = 1.f/4.f;
		uvs[8+(Ypos)*4]=glm::vec2(0,p*(Ypos+2));
		uvs[9+(Ypos)*4]=glm::vec2(1,p*(Ypos+2));
		uvs[10+(Ypos)*4]=glm::vec2(1,p*(Ypos+2)+p);
		uvs[11+(Ypos)*4]=glm::vec2(0,p*(Ypos+2)+p);
	
		glGenBuffers(1, &frameUVBuffers[Ypos]);
		glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[Ypos]);
		glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[(Ypos+2)*4], GL_STATIC_DRAW);
	}

	INPUT->attachMouseClick(this);
}


SliderX::~SliderX(void)
{
	INPUT->DetachMouseClick(this);
}

//Calculate the element's actual Screen-Position
//by it's parent-panels position,
//and it's own position on that panel
ProjectMappe::Rectangle
SliderX::GetArea(void)
{
	ControllElement::GetArea();
	ValueY.MOVE=1/((right-left)+2);
	return Area;
}


 
//regular per-frame update...
void SliderX::DoUpdate(void)
{
	GetArea();
	if(XIsUnderControll)
	{
		if(INPUT->Mouse.LEFT.DOUBLE )  // if Double-Clicked, change the Value-Axis...
		{ DimensionsSwitched = !DimensionsSwitched; 
			return;	}

		if(INPUT->Mouse.LEFT.RELEASE)  // if button released drop controlled-state
			this->XIsUnderControll = false;

		this->Connection()->getTransform()->scale.y = ValueX*100;
	}

	if(YIsUnderControll)
		if(INPUT->Mouse.MIDDLE.RELEASE)	  // same for Y-Axis
			this->YIsUnderControll = false;

	if(Label[0]!='\0')	  // draw the element's label-string if it has one defined...
		GuiManager::getInstance()->Write(&Label[0],Area.GetPosition());


}




//Mouse-Motion observing function:
void 
SliderX::mouseMotion(int x, int y)
{	
	//prevent cycling-feedback caused by mouse-warp-function
	if( lastMouse.x==x && lastMouse.y==y )
		return;	 
	
	//if user has the element under control	 
	//when (mouse-over and button-hold-down)
	if( XIsUnderControll || YIsUnderControll )
	{
		lastMouse.x = (x-lastMouse.x);  //get the mouse-movement
		lastMouse.y = (y-lastMouse.y);
		
		// check if X/Y values are switched
		if(DimensionsSwitched)	  
		{	
			if(YIsUnderControll)	 // and calculate the slider's new values by the width of mouse-movement 
				ValueX = (ValueX + ((lastMouse.y*0.5) / (Area.GetSize().x)));
			if(XIsUnderControll)
				ValueY = (ValueY + ((lastMouse.x*0.5) / (Area.GetSize().x)));
		} 
		else
		{
			if(XIsUnderControll)
				ValueX = ValueX + ((lastMouse.x *0.5)/ (Area.GetSize().x));
			if(YIsUnderControll)
				ValueY = ValueY + ((lastMouse.y *0.5)/ (Area.GetSize().x));
		}
		lastMouse.x = x;  // remember mouse-positions for next-frame movement-calculation... 
		lastMouse.y = y;

		sprintf(&Label[0],"X: %f, Y: %f",(float)ValueX,(float)ValueY);
	}
	else // If element not under user's control, sign it out from the Mouse-Update-Event
		INPUT->DetachMouseMove(this);
}



//Mouse-clicks observing function:
void 
SliderX::mouseClicks(int button,bool IsPressed,VectorF position)
{
	GetArea();	 // check if mouse is clicked and if its over this element
	 if((button!=1) &&  Area.Containes(position))
	 {
		 if(IsPressed)	  // if clicked, sign in for Mouse-Move-Event invocation   / as mouse-movement-observer
			INPUT->attachMouseMove(this);

		 XIsUnderControll = button==0?IsPressed:XIsUnderControll;	   // set flags signaling "under control" if button pressed
		 YIsUnderControll = button==2?IsPressed:YIsUnderControll;

		 //set lastMouse to new warp-position, to prevent counting the warp as a regular mouse-movement,
		 //when warping the cursor to the actual slider-position when clicked...
		 lastMouse = VectorF(left + ((DimensionsSwitched?ValueY:ValueX) *(right-left)),Area.GetCenter().y);
		 glutWarpPointer(lastMouse.x,lastMouse.y);
	 }
}





// Draw-function: called by SCENE
void SliderX::draw(void)
{
	if(!vertexBufferID)
		return;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// at first, draw the slider's background image.
	_DrawBackground();

	if(DimensionsSwitched)	// then draw the colored slider-bar sized to it's actual value... 
		_DrawBar(4,ValueY,ValueY.MOVE);
	else
		_DrawBar(0,ValueX,ValueX.MOVE);

	// than draw the slider's main-image-frame...
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

	glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// Translation:
		VectorF values = GetArea().GetPosition(); 
		glTranslatef(values.x+(right-left)*0.01f, values.y+((bottom-top)*0.95f),0);  

		// Rotation:  (parent-panel's rotation + element's own rotation on the panel...)
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		values = Area.GetSize() * 0.95f;
		glScalef(values.x,values.y,0);

		// Draw
		glDrawArrays(GL_QUADS, 0, 4);
	}
	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER,0);

}

void 
SliderX::_DrawBar(short i,float position,float move)
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	uvs[i].y = uvs[i+1].y = (position+move)/2.f;
	uvs[i+2].y = uvs[i+3].y =  (uvs[i].y + move)/2.f;

	glPushMatrix();
	{
		// Translation:
		VectorF tempVec = GetArea().GetPosition(); 
		glTranslatef(tempVec.x, tempVec.y+((bottom-top)*0.9),0);

		// Rotation:
		glRotatef(this->Connection()->getTransform()->rotation.z + this->angle, 0, 0, -1);

		// Scaling:
		tempVec = Area.GetSize();
		glScalef(tempVec.x*position,tempVec.y*0.9,0);

		// draw..
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
}

void 
SliderX::_DrawForeground(void)
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
		VectorF values = Area.GetPosition(); 
		glTranslatef(values.x, values.y+(bottom-top),0); 

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