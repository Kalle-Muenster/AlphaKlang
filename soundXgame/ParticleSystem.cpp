#include "ParticleSystem.h"


Particle::Particle(void)
{
	position = *Vector3::Zero;
	movement = *Vector3::Zero;
	time=0;
}

Particle::~Particle(void)
{

}

void 
Particle::move(Vector3 to)
{
	movement = to - position;
	position = to;
}


//template<const int NUMBER_OF_PARTICLES>
//ParticleSystem<NUMBER_OF_PARTICLES>::ParticleSystem(void)
//{
//	 particles = List<Particle*,NUMBER_OF_PARTICLES>();
//	 for(int i = 0;i < NUMBER_OF_PARTICLES;i++)
//		 particles.Add(new Particle());
//
//	 particlePosition = *Vector3::Zero;
//	 particleDirection = *Vector3::Zero;
//	 particleRotationAxis = *Vector3::Zero;
//	 particleRotationAngel = 0.f
//	 particleSize = 1.f;
//	 particlesEmitted = 0;
//	 timer = 0;
//	 lastActivatedParticle = -1;
//	 lifetime=20;
//	 emittingFrequency=0.1;
//	 varianz = 1.5f;
//
//	 SetID(SCENE->Add(this));
//	 SetName("ParticleSystem");
//	 LockID();
//}



//template<const int NUMBER_OF_PARTICLES>
//ParticleSystem<NUMBER_OF_PARTICLES>::~ParticleSystem(void)
//{
//	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
//		particles.Distruct(ID);
//}

//template<const int NUMBER_OF_PARTICLES>	void
//ParticleSystem<NUMBER_OF_PARTICLES>::Emmission(void)
//{
//	IsVisible=true;
//	timer+=INPUT->FrameTime;
//	if(timer >= emittingFrequency)
//	{
//		lastActivatedParticle=lastActivatedParticle==particles.Last()?particles.First():lastActivatedParticle+1;
//		particles[lastActivatedParticle]->move((this->getTransform()->forward-this->getTransform()->position)*this->getTransform()->speed*INPUT->FrameTime);
//		timer = particles[lastActivatedParticle]->time = 0;
//		Emmission++;
//	}
//}

//
//template<const int NUMBER_OF_PARTICLES>	void
//ParticleSystem<NUMBER_OF_PARTICLES>::DoUpdate(void)
//{
//	Particle P;
//	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
//	{
//		P = *particles[ID];
//		if(P.movement != 0)
//		{ 
//			particles[ID]->move(P.position + P.movement/100 * accselleration);
//		}
//	}
//}
//

//template<const int NUMBER_OF_PARTICLES> void
//ParticleSystem<NUMBER_OF_PARTICLES>::draw(void)
//{
//	if(this->IsVisible)	
//		Emmission(); 
//	
//	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
//	{
//		if(particles[ID]->movement!=0)
//		{
//			particles[ID]->time += INPUT->FrameTime;
//			if(particles[ID]->time >= lifetime)
//			{
//				particles[ID]->position = particles[ID]->movement = *Vector3::Zero;
//				particles[ID]->time = 0;
//				Emmission--;
//			}
//			else
//			{
//				particlePosition = particles[ID]->position;
//				particleDirection = particlePosition.direction(SCENE->camera->transform.position);
//				particleRotationAxis = Utility::GlobalZ.cros(particleDirection);
//				particleRotationAngel = glm::acos(Utility::GlobalZ.dot(particleDirection))/(M_PI/180.0);
//
//				particleDraw();
//			}
//		}
//	}
//   	printf("particles: %i\n",Emmission);
//}

//template<const int NUMBER_OF_PARTICLES>	void
//ParticleSystem<NUMBER_OF_PARTICLES>::particleDraw(void)
//{
//	 if(!vertexBufferID)
//		return;
//
//	if(NoBackfaceCulling == false)
//	{
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);
//	}
//
//	if(UseTexture)
//	{
//		glColor3f(1,1,1);
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, texture.ID);
//	}
//	else
//	{
//		glColor4b(color.byte[1],color.byte[2],color.byte[3],color.byte[0]);
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
//	glVertexPointer(3, GL_FLOAT, 0, 0);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
//	glTexCoordPointer(2, GL_FLOAT, 0, 0);
//
//	glPushMatrix();
//	{
//		// Translation:
//		glTranslatef(particlePosition.x, particlePosition.y, particlePosition.z);
//
//		// Rotation:
//		glRotatef(particleRotationAngel,particleRotationAxis.x,particleRotationAxis.y,particleRotationAxis.z);
//		
//		// Scaling:
//		glScalef(particleSize,particleSize,getTransform()->scale.z);
//		
//		// Draw
//		glDrawArrays(FaceShape, 0, verts.size());
//	}
//	glPopMatrix();
//
//	glDisable(GL_CULL_FACE);
//	glDisable(GL_TEXTURE_2D);
//}

//template<const int NUMBER_OF_PARTICLES>	bool
//ParticleSystem<NUMBER_OF_PARTICLES>::IsActive(BOOL setter)
//{
//	if(setter<2)
//		IsVisible=setter;
//
//	return IsVisible;
//}