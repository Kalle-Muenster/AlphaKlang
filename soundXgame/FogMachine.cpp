#include "FogMachine.h"
#include "projectMacros.h"
#include "Cam.h"


FogMachine::FogMachine(void)
{
	fogs = List<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES>();
	this->getTransform()->position = *Vector3::Zero;
	this->getTransform()->forward = Vector3(0,0,-1);
	this->getTransform()->speed = 1;
	for(int i=0;i<MAXIMUM_NUMBER_OF_PARTICLES;i++)
	{
		fogs.Add(new Sprite());
		fogs[i]->getTransform()->position = this->transform.position;
	}
	timer=0;
	lifetime=5;
	emittingFrequency=0.05;
	varianz = 1.5f;
	LoadeParticle("Modus Grau_64x64.png");
	IsVisible=true;
	lastActivatedParticle = -1;
	accselleration = 1.0001;

	SetID(SCENE->Add(this));
	LockID();
}

void
FogMachine::DoUpdate(void)
{
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID=fogs.Next(ID))
	{
		if(fogs[ID]->IsVisible)
		{
			fogs[ID]->move(fogs[ID]->getTransform()->position + (fogs[ID]->getTransform()->movement * this->accselleration));

			if(particlesAlwaysFaceCamera)
				fogs[ID]->rotate( (SCENE->camera->transform.position - fogs[ID]->getTransform()->position).normal() );
		}

	}
}

FogMachine::~FogMachine(void)
{
	UPDATE->SignOutFromUpdate(this);
}

void
FogMachine::LoadeParticle(char* nebelFile)
{
	glEnable(GL_TEXTURE_2D);
	texture.ID = Utility::loadTexture(nebelFile);
	int i=0;
	while((++i<64)&&(nebelFile[i]!='_'));
	if(i<64)
		sscanf(&nebelFile[i],"_%ix%i.",&texture.w,&texture.h);
	texture.format = GL_RGBA;
	for(unsigned ID=fogs.First();ID<=fogs.Last();ID=fogs.Next(ID))
	{
		fogs[ID]->SetTexture(&texture);
		fogs[ID]->SomeValue=0;
	}

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
	MachDampf();
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID = fogs.Next(ID))
	{
		if(fogs[ID]->IsVisible)
		{
			if((fogs[ID]->SomeValue += INPUT->FrameTime)>=lifetime)
			{
				fogs[ID]->getTransform()->position = this->getTransform()->position;
				fogs[ID]->getTransform()->movement = *Vector3::Zero;
				fogs[ID]->IsVisible = false;
				fogs[ID]->SomeValue = 0;
			}
			else
				fogs[ID]->draw();
		}
	}
}

void
FogMachine::MachDampf(void)
{
	this->IsVisible=true;
	//Stupsing an the next particle if emitting-timer elapsed...
	if((timer+=INPUT->FrameTime) >= emittingFrequency)
	{
		fogs[++lastActivatedParticle]->move((this->getTransform()->forward-this->getTransform()->position)*this->getTransform()->speed*INPUT->FrameTime);
		lastActivatedParticle=lastActivatedParticle==fogs.Last()?fogs.First():lastActivatedParticle;
		timer=0;
	}
}