#include "FogMachine.h"
#include "projectMacros.h"
#include "Cam.h"


FogMachine::FogMachine(string filename)
{
	fogs = List<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES>();
	this->getTransform()->position = *Vector3::Zero;
	this->getTransform()->forward = Vector3(0,0,-1);
	this->getTransform()->speed = 0.0001;
//	LoadeParticle(filename);
	for(int i=0;i<MAXIMUM_NUMBER_OF_PARTICLES;i++)
	{
		fogs.Add(new Sprite(filename,false));
		fogs[i]->getTransform()->position = this->transform.position;
	}
	timer=0;
	lifetime=5;
	emittingFrequency=0.05;
	varianz = 1.5f;

	
	IsVisible=true;
	lastActivatedParticle = -1;
	accselleration = 1.00000001;
	particlesAlwaysFaceCamera=true;

	SetID(SCENE->Add(this));
	LockID();
}

float rotator = 0.f;
void
FogMachine::DoUpdate(void)
{
	Vector3 vecTemp1 = *Vector3::Zero;
	Vector3 vecTemp2 = *Vector3::Zero;

	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID=fogs.Next(ID))
	{
		if(fogs[ID]->IsVisible)
		{
			fogs[ID]->move(fogs[ID]->getTransform()->position + (fogs[ID]->getTransform()->movement/100 * this->accselleration));

			if(particlesAlwaysFaceCamera)
			{
				vecTemp2 = fogs[ID]->getTransform()->position.direction(SCENE->camera->transform.position);
				fogs[ID]->rotate(glm::acos(Utility::GlobalZ.dot(vecTemp2))/(M_PI/180.0),Utility::GlobalZ.cros(vecTemp2));
			}
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
	data32 color;
	glEnable(GL_TEXTURE_2D);
		int i = -1;
	while((++i<64)&&(nebelFile[i]!='_'));
	if(i<64)
		sscanf(&nebelFile[i],"_%ix%i.",&texture.w,&texture.h);

	this->texture.ID = Utility::loadTexture(nebelFile);


	//texture.ID = Utility::loadTexture(nebelFile);
	//Loader loader(nebelFile);
	//std::vector<unsigned int>* Bitmap1,*Bitmap2,*Bitmap3;

	//Bitmap1 = new std::vector<unsigned int>();
	//Bitmap2 = new std::vector<unsigned int>();
	//Bitmap3 = new std::vector<unsigned int>();

	//for(int i = 0; i<loader.count(); i++)
	//{
	////	color.byte[3] = loader.Pixel(i)->byte[3];

	//	color.byte[0] = loader.Pixel(i)->byte[0];
	//	color.byte[1] = loader.Pixel(i)->byte[0]; 
	//	color.byte[2] = loader.Pixel(i)->byte[0]; 
	//	color.byte[3] = loader.Pixel(i)->byte[0]; 
	//	Bitmap1->push_back(color.u32);

	//	color.byte[0] = color.byte[1] = color.byte[2] = color.byte[3] = loader.Pixel(i)->byte[1];
	//	Bitmap2->push_back(color.u32);

	//	color.byte[0] = color.byte[1] = color.byte[2] = color.byte[3] = loader.Pixel(i)->byte[2];
	//	Bitmap3->push_back(color.u32);
	//}
	//
	//glGenTextures(3,&textureIDs[0]);
	//glBindTexture(GL_TEXTURE_2D,textureIDs[0]);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA_EXT,loader.width(),loader.height(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,Bitmap1);
	//glBindTexture(GL_TEXTURE_2D,textureIDs[1]);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA_EXT,loader.width(),loader.height(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,Bitmap2);
	//glBindTexture(GL_TEXTURE_2D,textureIDs[2]);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA_EXT,loader.width(),loader.height(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,Bitmap3);

	//texture.ID = textureIDs[0];
	//texture.w=loader.width();
	//texture.h=loader.height();
	//texture.format = GL_RGBA;

	//for(unsigned ID=fogs.First();ID<=fogs.Last();ID=fogs.Next(ID))
	//{
	//	fogs[ID]->SetTexture(&texture);
	//	fogs[ID]->SomeValue=0;
	//}
}


//particleVarianz must be a value between 0 and 3
//it weightens the possibility of apearing specific particles.
//A Random float from 0 to 3 will be genrated, multiplied by 
//"varianz", devidet by 3 and casted to int.
void 
_setParticleVariant(float varianz)
{
	glPixelTransferf(GL_MAP_COLOR,true);
	switch ((int)(Utility::Random(3)*varianz/3.f))
	{
	case 0:
		 glPixelTransferf(GL_RED_SCALE,1);
		 glPixelTransferf(GL_GREEN_SCALE,0);
		 glPixelTransferf(GL_BLUE_SCALE,0);
		break;
	case 1:
		 glPixelTransferf(GL_RED_SCALE,0);
		 glPixelTransferf(GL_GREEN_SCALE,1);
		 glPixelTransferf(GL_BLUE_SCALE,0);
		break;
	case 2:
		 glPixelTransferf(GL_RED_SCALE,0);
		 glPixelTransferf(GL_GREEN_SCALE,0);
		 glPixelTransferf(GL_BLUE_SCALE,1);
		break;
	}
}


GLuint
FogMachine::SellectParticleImage(void)
{
	return textureIDs[(int)(Utility::Random(3)*varianz/3.f)];
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
			{
		//		_setParticleVariant(varianz);
		//		fogs[ID]->GetTexture()->ID = SellectParticleImage();
				fogs[ID]->draw();
			}
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