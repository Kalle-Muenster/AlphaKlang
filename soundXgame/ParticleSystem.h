#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "sprite.h"



template<const int NUMBER_OF_PARTICLES>
class ParticleSystem :
	public Sprite,
	public IUpdateble
{
private:

	class Particle
	{
	public:
		Vector3 position;
		Vector3 movement;
		float time;
		Particle(void)
		{
			position = *Vector3::Zero;
			movement = *Vector3::Zero;
			time=0;
		}
		~Particle(void){};
	};
	bool movingaway;
	List<Particle*,NUMBER_OF_PARTICLES>		particles;
//	std::vector<Particle*>					particles_stdvec;
	Vector3									particlePosition;
	Vector3									particleDirection;
	Vector3									particleRotationAxis;
	GLfloat									particleRotationAngel;
	GLfloat									particleSize;
	GLfloat									particleAlphaValue;
	
	GLfloat									InitialSpeed;


	int										particlesEmitted;

	void									ParticleCalculation(unsigned ID)
											{						
												Vector3 cameraDirection;
												Particle P = *particles[ID];
											//	Particle P = *particles_stdvec[ID];
												P.time = P.time>0 ? P.time-INPUT->FrameTime : 0;

													if(P.time > 0)
													{
															P.movement *= accselleration;
															P.position += particles[ID]->movement;
															//P.position += particles_stdvec[ID]->movement;

															float timePoint = (P.time/lifetime) ;

															if(ParticlesChangeSize)
																particleSize = InitialSize + ((timePoint < ValueChangeReleasepoint) ? ((1-timePoint) * MaximumSize) : (timePoint * MaximumSize));

															if(ParticlesChangeAlpha)
																particleAlphaValue = ((timePoint)*color.byte[0])/255;

															particlePosition = P.position;
															
															cameraDirection = SCENE->camera->transform.position;

															if(particlePosition.x > cameraDirection.x)
															{
																cameraDirection = particlePosition.direction(cameraDirection);
																particleRotationAngel = glm::acos(Utility::GlobalZ.dot(cameraDirection))/(M_PI/180.0);
															}
															else
															{
															  	cameraDirection = particlePosition.direction(cameraDirection);
																particleRotationAngel = - glm::acos(Utility::GlobalZ.dot(cameraDirection))/(M_PI/180.0);
															}
											
															particleRotationAxis = Utility::GlobalZ.cros(cameraDirection);
															particleDraw();
													}
													else
													{
															P.position = P.movement = *Vector3::Zero;
															P.time = 0;
															particlesEmitted--;
													}
													*particles[ID] = P;
													//*particles_stdvec[ID] = P;
													glBindBuffer(GL_ARRAY_BUFFER,0);
													glBindTexture(GL_TEXTURE_2D, 0);
													glDisable(GL_TEXTURE_2D);
											}

	void									particleDraw(void)
											{
												glColor4f((GLfloat)color.byte[1]/255,(GLfloat)color.byte[2]/255,(GLfloat)color.byte[3]/255,particleAlphaValue);
												glEnable(GL_TEXTURE_2D);
												glBindTexture(GL_TEXTURE_2D, texture.ID);
										
												glPushMatrix();
												{
													// Translation:
													glTranslatef(particlePosition.x, particlePosition.y, particlePosition.z);

													// Rotation:
													glRotatef(particleRotationAngel,particleRotationAxis.x,particleRotationAxis.y,particleRotationAxis.z);

													// Scaling:
													glScalef(transform.scale.x*particleSize,transform.scale.y*particleSize,0);

													// Draw
													glDrawArrays(FaceShape, 0, 4);
												}
												glPopMatrix();

											//	glDisable(GL_CULL_FACE);

											}
	void                                    Emmission(void)
											{
												IsVisible=true;
												timer+=INPUT->FrameTime;
												if(timer >= emittingFrequency)
												{
													lastActivatedParticle=lastActivatedParticle==particles.Last()?particles.First():lastActivatedParticle+1;
													particles[lastActivatedParticle]->movement = (particleDirection * (InitialSpeed * INPUT->FrameTime));
													particles[lastActivatedParticle]->position = this->getTransform()->position + particles[lastActivatedParticle]->movement;
													particles[lastActivatedParticle]->time = lifetime;
													timer = 0;
													//lastActivatedParticle=lastActivatedParticle==NUMBER_OF_PARTICLES-1?0:lastActivatedParticle+1;
													//particles_stdvec[lastActivatedParticle]->movement = (particleDirection * (InitialSpeed * INPUT->FrameTime));
													//particles_stdvec[lastActivatedParticle]->position = this->getTransform()->position + particles_stdvec[lastActivatedParticle]->movement;
													//timer = particles_stdvec[lastActivatedParticle]->time = lifetime;

													particlesEmitted++;
													if(particlesEmitted>0)
														turnMarker -= (turnMarker>0)? 1 : 0;
												}
											}
	float									timer;
	int										lastActivatedParticle;
	unsigned										turnMarker;

public:
					ParticleSystem(string particleTexture)
					{
							verts.push_back(glm::vec3(-1,1,0));
							verts.push_back(glm::vec3(1,1,0));
							verts.push_back(glm::vec3(1,-1,0));
							verts.push_back(glm::vec3(-1,-1,0));

							uvs.push_back(glm::vec2(0,0));
							uvs.push_back(glm::vec2(1,0));
							uvs.push_back(glm::vec2(1,1));
							uvs.push_back(glm::vec2(0,1));

							FaceShape=GL_QUADS;

							transform.forward = Vector3(0,0,-1);
							transform.right = Vector3(1,0,0);
							transform.up = Vector3(0,1,0);

							color.u32 = 0xffffff7f;

							glGenBuffers(1, &vertexBufferID);
							glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
							glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

							glGenBuffers(1, &uvBufferID);
							glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
							glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


							IGObject::InitializeObject(false);
							
							SetUp(particleTexture,true);

				
						for(int i = 0;i < NUMBER_OF_PARTICLES;i++)
							 particles.Add(new Particle());

						ValueChangeReleasepoint = 0.5f;
						ParticlesChangeSize = ParticlesChangeAlpha = true;
						particlePosition = *Vector3::Zero;
						particleSize = 1;
						getTransform()->rotation = Vector3(1,0,0);
						particleDirection = getTransform()->forward;
						particleRotationAxis = *Vector3::Zero;
						particleRotationAngel = 0.f;
						InitialSize = 0.5f;
						particlesEmitted = 0;
						timer = 0;
						turnMarker = NUMBER_OF_PARTICLES-1;
						lastActivatedParticle = -1;
						lifetime=5;
						emittingFrequency=0.01;
						accselleration = 1.01f; 
						MaximumSize = 3;
						varianz = 1.5f;
						getTransform()->movement = *Vector3::Zero;
						InitialSpeed = 7;
						SetID(SCENE->Add(this));
						SetName("ParticleSystem");
						LockID();
						IsVisible = false;
						UPDATE->SignInForUpdate(this);
						transform.speed = 20.f;
						movingaway=false;
					}
	virtual		   ~ParticleSystem(void)
					{
						for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
							particles.Distruct(ID);

						//for(int i = 0;i < NUMBER_OF_PARTICLES;i++)
						//	delete particles_stdvec[i];
					}
	

	virtual void	draw(void)
					{
						if(!vertexBufferID)
							return;


						if(this->IsVisible)	
							Emmission(); 

						if(lastActivatedParticle<0)
							return;

						if(NoBackfaceCulling)
							glDisable(GL_CULL_FACE);
						else
						{
							glEnable(GL_CULL_FACE);
							glCullFace(GL_BACK);
						}

						glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
						glVertexPointer(3, GL_FLOAT, 0, 0);

						glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
						glTexCoordPointer(2, GL_FLOAT, 0, 0);
						bool interleave=false;
						float dist1 = this->getTransform()->position.distance(SCENE->camera->transform.position);  
						float dist2 = (this->getTransform()->position + this->getTransform()->movement).distance(SCENE->camera->transform.position);
						if(dist1>dist2)
						{
							if(movingaway)
							{
								movingaway = false;
								turnMarker = lastActivatedParticle;
							}

						//	for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
						//		ParticleCalculation(ID);
							 
							
							for(unsigned ID = particles.StartCycle(turnMarker); ID == particles.Cycle(); ID = particles.Next(ID))
								ParticleCalculation(ID);

						}
						else
						{
							if(!movingaway)
							{
								movingaway = true;
								turnMarker = lastActivatedParticle;
							}
							
						//	for(unsigned ID = particles.Last(); ID > particles.First(); ID = particles.Prev(ID))
						//		 ParticleCalculation(ID);
						//	ParticleCalculation(particles.First());

							for(unsigned ID=particles.StartCycle(turnMarker);ID==particles.Cycle();ID=particles.Prev(ID))
							   ParticleCalculation(ID);
							//for(unsigned ID = turnMarker; ID > particles.First(); ID = particles.Prev(ID))
							//	 ParticleCalculation(ID);
							//ParticleCalculation(particles.First());

							//for(unsigned ID = turnMarker+1; ID <= particles.Last(); ID = particles.Next(ID))
							//	 ParticleCalculation(ID);
							
							//for(unsigned i=turnMarker-1;i>=0;i--)
							//	ParticleCalculation(i);
							//for(unsigned i = turnMarker;i<NUMBER_OF_PARTICLES;i++)
							//	ParticleCalculation(i);
						}
					}
	virtual Vector3	rotate(Vector3 r)
					{
						return particleDirection = r.normalized();
						return Sprite::rotate(r);
					}
			float	varianz;
			float	accselleration;
			float	lifetime;
			float	emittingFrequency;
			bool	ParticlesChangeSize;
			bool	ParticlesChangeAlpha;
			float	ValueChangeReleasepoint;
			float	MaximumSize;
			GLfloat	InitialSize;
	virtual void	DoUpdate(void)
					{

					}
};

#endif