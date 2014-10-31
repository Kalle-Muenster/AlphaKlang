#include "VoxGrid.h"

VoxGrid::VoxGrid(void)
{

}

VoxGrid::VoxGrid(const char* ppmfile)
{
	Initiate(ppmfile);
}


VoxGrid::~VoxGrid(void)
{

}

//void
//VoxGrid::draw(void)
//{
	//glPushMatrix();
//	{
//		glBegin(GL_PROJECTION);
//		glLoadIdentity();

//		this->Draw(this->position);

//		glEnd();
//	}
//	glPopMatrix();
//}