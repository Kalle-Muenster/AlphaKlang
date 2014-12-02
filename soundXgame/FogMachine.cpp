#include "FogMachine.h"
#include "projectMacros.h"
#include "Cam.h"


FogMachine::FogMachine(void)
{
	fogs = List<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES>();
	for(int i=0;i<MAXIMUM_NUMBER_OF_PARTICLES;i++)
	{
		fogs.Add(new Sprite());
		fogs[i]->GetTexture()=this->texture;
	}

}

void
FogMachine::DoUpdate(void)
{
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID=fogs.Next(ID))
	{
		fogs[0]->getTransform()->position += (fogs[ID]->getTransform()->movement * this->accselleration);

		if(particlesAlwaysFaceCamera)
			fogs[ID]->rotate( (SCENE->camera->transform.position - fogs[ID]->getTransform()->position).normal() );

	}
}

FogMachine::~FogMachine(void)
{
	UPDATE->SignOutFromUpdate(this);
}

void
FogMachine::Loade(char* nebelFile)
{
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture(nebelFile);
	texture.format = GL_RGBA;
}


//particleVarianz must be a value between 0 and 3
//it weightens the possibility of apearing specific particles.
//A Random float from 0 to 3 will be genrated, multiplied by 
//"varianz", devidet by 3 and casted to int.
void 
_setParticleVariant(float varianz)
{
	switch ((int)(Utility::Random(3)*varianz/3.f))
	{
	case 0:
		 glPixelTransferf(GL_MAP_COLOR,GL_PIXEL_MAP_R_TO_R);
		break;
	case 1:
		 glPixelTransferf(GL_MAP_COLOR,GL_PIXEL_MAP_G_TO_G);
		break;
	case 2:
		 glPixelTransferf(GL_MAP_COLOR,GL_PIXEL_MAP_B_TO_B);
		break;
	}
}

void
FogMachine::draw(void)
{
	
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID = fogs.Next(ID))
	{
		_setParticleVariant(this->varianz);
		fogs[ID]->draw();
	}
}

void
FogMachine::MachDampf(void)
{
 //TODO
}