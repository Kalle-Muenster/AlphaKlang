#include "FogMachine.h"
#include "projectMacros.h"
#include "Cam.h"

void
Particles::move(Vector3 to)
{
	movement = to - position;
	position = to;
}

Particles::Particles(void)
{
	movement = *Vector3::Zero;
	position = *Vector3::Zero;
	time = 0;
}

Particles::~Particles(void)
{

}

FogMachine::FogMachine(string filenames[])
{	

	drawingVertices.push_back(glm::vec3(-1,1,0));
	drawingVertices.push_back(glm::vec3(1,1,0));
	drawingVertices.push_back(glm::vec3(1,-1,0));
	drawingVertices.push_back(glm::vec3(-1,-1,0));

	drawingUVs.push_back(glm::vec2(0,0));
	drawingUVs.push_back(glm::vec2(1,0));
	drawingUVs.push_back(glm::vec2(1,1));
	drawingUVs.push_back(glm::vec2(0,1));


	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, drawingVertices.size() * sizeof(glm::vec3), &drawingVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, drawingUVs.size() * sizeof(glm::vec2), &drawingUVs[0], GL_STATIC_DRAW);

	fogs = List<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES>();
	particles = List<Particles*,MAXIMUM_NUMBER_OF_PARTICLES>();
	this->getTransform()->position = *Vector3::Zero;
	this->getTransform()->forward = Vector3(0,0,-1);
	this->getTransform()->speed = 0.00001;
	LoadeParticle(filenames);
	for(int i=0;i<MAXIMUM_NUMBER_OF_PARTICLES;i++)
	{
		particles.Add(new Particles());
		//fogs.Add(new Sprite(filenames[0],false));
		//fogs[i]->getTransform()->position = this->transform.position;
		//fogs[i]->IsVisible = false;
	}

	timer=0;
	lifetime=20;
	emittingFrequency=0.1;
	varianz = 1.5f;
	Emmission = 0;
	
	IsVisible=false;
	lastActivatedParticle = -1;
	accselleration = 1.000001;
	particlesAlwaysFaceCamera=true;
	particleSize = 1;
	SomeValue = 0;

	SetID(SCENE->Add(this));
	LockID();
}


void
FogMachine::DoUpdate(void)
{
	Particles P;

	
	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
	{
		P = *particles[ID];
		if(P.movement != 0)
		{ 
			particles[ID]->move(P.position + P.movement/100 * accselleration);
		}
	}
	 
  	/*
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID=fogs.Next(ID))
	{
		if(fogs[ID]->IsVisible)
		{
			fogs[ID]->move(fogs[ID]->getTransform()->position + fogs[ID]->getTransform()->movement* this->accselleration);

			if(particlesAlwaysFaceCamera)
			{
				particleDirection = fogs[ID]->getTransform()->position.direction(SCENE->camera->transform.position);
				fogs[ID]->rotate(glm::acos(Utility::GlobalZ.dot(particleDirection))/(M_PI/180.0),Utility::GlobalZ.cros(particleDirection));
			}
		}
	}
	*/
	
}

FogMachine::~FogMachine(void)
{
	UPDATE->SignOutFromUpdate(this);
}

void
FogMachine::LoadeParticle(string nebelFiles[])
{
	data32 color;
	glEnable(GL_TEXTURE_2D);
		int i = -1;
	while((++i<64)&&(nebelFiles[0][i]!='_'));
	if(i<64)
		sscanf(&nebelFiles[0][i],"_%ix%i.",&texture.w,&texture.h);



	texture.LoadeFrames(nebelFiles);
	texture.SetFrame(0);


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


void
FogMachine::SellectParticleImage(void)
{
	texture.SetFrame((int)(Utility::Random(3)*varianz/3.f));
}

void
FogMachine::draw(void)
{
	if(this->IsVisible)	
		MachDampf(); 
	
	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
	{
		if(particles[ID]->movement!=0)
		{
			particles[ID]->time += INPUT->FrameTime;
			if(particles[ID]->time >= lifetime)
			{
				particles[ID]->position = particles[ID]->movement = *Vector3::Zero;
				particles[ID]->time = 0;
				Emmission--;
			}
			else
			{
				particlePosition = particles[ID]->position;
				particleDirection = particlePosition.direction(SCENE->camera->transform.position);
				particleRotationAxis = Utility::GlobalZ.cros(particleDirection);
				particleRotationAngel = glm::acos(Utility::GlobalZ.dot(particleDirection))/(M_PI/180.0);
				//SellectParticleImage();
				drawParticle();
			}
		}
	}
   	printf("particles: %i\n",Emmission);
	/*
	for(GobID ID = fogs.First(); ID <= fogs.Last(); ID = fogs.Next(ID))
	{
		if(fogs[ID]->IsVisible)
		{
			float temp = fogs[ID]->SomeValue;
			fogs[ID]->SetSomeValue(temp + (float)INPUT->FrameTime);
			if(fogs[ID]->SomeValue >= lifetime)
			{
				fogs[ID]->getTransform()->position = this->getTransform()->position;
				fogs[ID]->getTransform()->movement = *Vector3::Zero;
				fogs[ID]->IsVisible = false;
				fogs[ID]->SetSomeValue(0);
				Emmission--;
			}
			else
			{
				fogs[ID]->draw();
			}
		}
	}
	*/
	
}



void
FogMachine::drawParticle(void)
{
	if(!vertexBufferID)
		return;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.ID);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		// Translate
		glTranslatef(particlePosition.x,particlePosition.y,particlePosition.z);

		// Rotate
		glRotatef(particleRotationAngel,particleRotationAxis.x,particleRotationAxis.y,particleRotationAxis.z);
		
		// Scale
		glScalef(getTransform()->scale.x,getTransform()->scale.y,getTransform()->scale.z);
		
		// Draw
		glDrawArrays(GL_QUADS, 0, drawingVertices.size());
	}
	glPopMatrix();

	//glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

void
FogMachine::MachDampf(void)
{
	this->IsVisible=true;
	//Stupsing an the next particle if emitting-timer elapsed...
	timer+=INPUT->FrameTime;
	if(timer >= emittingFrequency)
	{
//		lastActivatedParticle++;
		lastActivatedParticle=lastActivatedParticle==particles.Last()?particles.First():lastActivatedParticle+1;
		particles[lastActivatedParticle]->move((this->getTransform()->forward-this->getTransform()->position)*this->getTransform()->speed*INPUT->FrameTime);
		particles[lastActivatedParticle]->time = 0;

		//lastActivatedParticle = (lastActivatedParticle==fogs.Last())? fogs.First() : lastActivatedParticle;
		//fogs[lastActivatedParticle]->IsVisible=true;
		//fogs[lastActivatedParticle]->move((this->getTransform()->forward)*(this->getTransform()->speed*INPUT->FrameTime));
		//fogs[lastActivatedParticle]->SetSomeValue(0);

		timer=0;
		Emmission++;
		
	}
}