#include "StrangeChaoticView.h"

int StrangeChaoticView::ID = -1;



StrangeChaoticView::StrangeChaoticView(void)
{
	ModeName="StrangeChaoticView";
	isPrimarMode = false;
	convergencer = new StrangeChaoticView::ViewConvergencer();
}


StrangeChaoticView::~StrangeChaoticView(void)
{
	SCENE->Remove(convergencer);
	delete convergencer;
}

bool
StrangeChaoticView::Initialize(void)
{
	ID = this->CamModeID();
	IsActive = false;
	
	SCENE->Remove(convergencer);
	return true;
}

void
StrangeChaoticView::UpdateMode(void)
{
	 glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,SCREENWIDTH,0,SCREENHEIGHT,0,1);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(SCENE->camera->FieldOfView(), SCENE->camera->Aspect(), 0.1f, 1000.0f);

	glEnable(GL_DEPTH_TEST);
	
}

StrangeChaoticView::ViewConvergencer::ViewConvergencer(void)
{
	Initialize();
	SetName("Convergencer");
	//LoadTexture();
}


StrangeChaoticView::ViewConvergencer::~ViewConvergencer(void)
{

}

bool
StrangeChaoticView::Activate(BOOL setter)
{
	if(setter<3)
		this->IsActive = convergencer->IsActive = setter;
	return IsActive;
}

void
StrangeChaoticView::ViewConvergencer::Initialize(void)
{
	verts = List<glm::vec3*,4>();
	uvs = List<glm::vec2*,4>();

	verts.Add(new glm::vec3(0,0,0));
	verts.Add(new glm::vec3(1,0,0));
	verts.Add(new glm::vec3(1,1,0));
	verts.Add(new glm::vec3(0,1,0));

	uvs.Add(new glm::vec2(0,1));
	uvs.Add(new glm::vec2(1,1));
	uvs.Add(new glm::vec2(1,0));
	uvs.Add(new glm::vec2(0,0));

	transform.position = *Vector3::Zero;
	transform.rotation = *Vector3::Zero;
	transform.scale    = Vector3(1,1,1);

	IGObject::InitializeObject();
	UseTexture = false;
	IsVisible = IsActive = true;
}

void
StrangeChaoticView::ViewConvergencer::draw(void)
{
	if(IsActive)
	{
	glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,SCREENWIDTH,0,SCREENHEIGHT,0,1);

		if(UseTexture)
		{	
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture.ID);

		    glColor4f(1,1,1,1);
			for(int i=0;i<4;i++)
			{
				glTexCoord2f(uvs[i]->x,uvs[i]->y);
				glVertex3f(verts[i]->x+transform.position.x-transform.scale.x/2,verts[i]->y+transform.position.y-transform.scale.y/2,verts[i]->z);
			}
		}
		else
		{	
			glDisable(GL_TEXTURE_2D);
			glColor4f((float)color.byte[1]/255,(float)color.byte[2]/255,(float)color.byte[3]/255,(float)color.byte[0]/255);
			for(int i=0;i<4;i++)
				glVertex3f(verts[0]->x+transform.position.x-transform.scale.x/2,verts[0]->y+transform.position.y-transform.scale.y/2,verts[0]->z);
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(SCENE->camera->FieldOfView(), SCENE->camera->Aspect(), 0.1f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	}
	
//	printf("scale: %f,%f,%f\n",transform.scale.x,transform.scale.y,transform.scale.z);//,transform.position.x.transform.position.y,transform.position.z)
}

void 
StrangeChaoticView::ViewConvergencer::LoadTexture(char* filename)
{
	int i = 0;
	while((++i<64)&&(filename[i]!='_'));
	if(i<64)
		sscanf(&filename[i],"_%ix%i.",&texture.w,&texture.h);

	texture.ID = Utility::loadTexture(filename=="ImageFile"? "testbild_1600x900.png" : filename);
	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
	UseTexture = true;
}
	
ProjectMappe::Rectangle
StrangeChaoticView::ViewConvergencer::Position(float X,float Y,float W,float H)
{	
	ProjectMappe::Rectangle area(X,Y,W,H);
	transform.position.x = X+W/2;
	transform.position.y = SCREENHEIGHT - (Y+H/2);
	transform.scale.x = W;
	transform.scale.y = H;
	return area;
}
	
ProjectMappe::Rectangle 
StrangeChaoticView::ViewConvergencer::Position(ProjectMappe::Rectangle r)
{
	return Position(r.GetPosition().x,r.GetPosition().y,r.GetSize().x,r.GetSize().y);
}