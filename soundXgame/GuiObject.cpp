#include "GuiObject.h"
#include "projectMacros.h"

GuiObject::GuiObject(void)
{

	Initialize();
}


GuiObject::~GuiObject(void)
{
}

void
GuiObject::Initialize(void)
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
	IsVisible = UseTexture = false;
	IsActive = true;
}

void
GuiObject::draw(void)
{
	if(IsActive)
	{
		if(UseTexture)
		{	glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,texture.ID);

		    glColor4f(1,1,1,1);
			for(int i=0;i<4;i++)
			{
				glTexCoord2f(uvs[i]->x,uvs[i]->y);
				glVertex3f(verts[i]->x+transform.position.x-transform.scale.x/2,verts[i]->y+transform.position.y-transform.scale.y/2,verts[i]->z);
			}
		}
		else
		{	glDisable(GL_TEXTURE_2D);
			glColor4f((float)color.byte[1]/255,(float)color.byte[2]/255,(float)color.byte[3]/255,(float)color.byte[0]/255);
			for(int i=0;i<4;i++)
				glVertex3f(verts[0]->x+transform.position.x-transform.scale.x/2,verts[0]->y+transform.position.y-transform.scale.y/2,verts[0]->z);
		}
	}
	
//	printf("scale: %f,%f,%f\n",transform.scale.x,transform.scale.y,transform.scale.z);//,transform.position.x.transform.position.y,transform.position.z)
}

void 
GuiObject::LoadTexture(char* filename)
{
	int i = 0;
	while((++i<64)&&(filename[i]!='_'));
	if(i<64)
		sscanf(&filename[i],"_%ix%i.",&texture.w,&texture.h);

	texture.ID = Utility::loadTexture(filename);
	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
}
	
ProjectMappe::Rectangle
GuiObject::Position(float X,float Y,float W,float H)
{	ProjectMappe::Rectangle area(X,Y,W,H);
	transform.position.x = X+W/2;
	transform.position.y = SCREENHEIGHT - (Y+H/2);
	transform.scale.x = W;
	transform.scale.y = H;
	return area;
}
	
ProjectMappe::Rectangle 
GuiObject::Position(ProjectMappe::Rectangle r)
{
	return Position(r.position().x,r.position().y,r.size().x,r.size().y);
}