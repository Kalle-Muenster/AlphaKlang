#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "sprite.h"

class Particle
{
public:
	Vector3 position;
	Vector3 movement;
	float time;
	bool active;
	void move(Vector3 to);
	Particle(void);
	~Particle(void);
};

template<const int NUMBER_OF_PARTICLES>
class ParticleSystem :
	public Sprite,
	public IUpdateble
{
private:

	List<Particle*,NUMBER_OF_PARTICLES>		particles;
	Vector3									particlePosition;
	Vector3									particleFaceDirection;
	Vector3									particleRotationAxis;
	Vector3									particleDirection;
	GLfloat									particleRotationAngel;
	GLfloat									particleSize;
	GLfloat									particleAlphaValue;
	GLfloat									InitialSize;
	GLfloat									InitialSpeed;

	int										particlesEmitted;
	void									particleDraw(void)
											{
												 if(!vertexBufferID)
													return;

												glEnable(GL_CULL_FACE);
												glCullFace(GL_BACK);

												if(UseTexture)
												{
													glColor4f((GLfloat)color.byte[1]/255,(GLfloat)color.byte[2]/255,(GLfloat)color.byte[3]/255,particleAlphaValue);
													glEnable(GL_TEXTURE_2D);
													glBindTexture(GL_TEXTURE_2D, texture.ID);
												}
												else
												{
													glColor4b(color.byte[1],color.byte[2],color.byte[3],color.byte[0]);
												}

												glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
												glVertexPointer(3, GL_FLOAT, 0, 0);

												glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
												glTexCoordPointer(2, GL_FLOAT, 0, 0);

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

												glDisable(GL_CULL_FACE);
												glDisable(GL_TEXTURE_2D);
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
													particles[lastActivatedParticle]->active = true;
													timer = particles[lastActivatedParticle]->time = lifetime;
													particlesEmitted++;
												}
											}
	float									timer;
	int										lastActivatedParticle;


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

					//	particles = List<Particle*,NUMBER_OF_PARTICLES>();
						for(int i = 0;i < NUMBER_OF_PARTICLES;i++)
							 particles.Add(new Particle());

						particlePosition = *Vector3::Zero;
						
						getTransform()->rotation = Vector3(1,0,0);
						particleDirection = getTransform()->forward;
						particleFaceDirection = *Vector3::Zero;
						particleRotationAxis = *Vector3::Zero;
						particleRotationAngel = 0.f;
						InitialSize = 0.5f;
						particlesEmitted = 0;
						timer = 0;
						lastActivatedParticle = -1;
						lifetime=5;
						emittingFrequency=0.01;
						accselleration = 1.01f; 
						
						varianz = 1.5f;
						getTransform()->movement = *Vector3::Zero;
						InitialSpeed = 5;
					//	LoadTexture(particleTexture);
						SetID(SCENE->Add(this));
						SetName("ParticleSystem");
						LockID();
						IsVisible = false;
						UPDATE->SignInForUpdate(this);
					}
	virtual		   ~ParticleSystem(void)
					{
						for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
						particles.Distruct(ID);
					}
	virtual void	draw(void)
					{
						Particle P;
						if(this->IsVisible)	
							Emmission(); 

						for(unsigned ID = particles.First(); ID <= particles.Last(); ID = particles.Next(ID))
						{
							P = *particles[ID];
							P.time = P.time>0 ? P.time-INPUT->FrameTime : 0;

							if(P.time > 0)
							{
									P.movement *= accselleration;
									P.position += particles[ID]->movement;

									float timePoint = (P.time/lifetime) ;

									particleSize = InitialSize + (timePoint * 10);
									particleAlphaValue = ((1-timePoint)*color.byte[0])/255;
									particlePosition = P.position;
									particleFaceDirection = particlePosition.direction(SCENE->camera->transform.position);
									particleRotationAxis = Utility::GlobalZ.cros(particleFaceDirection);
									particleRotationAngel = glm::acos(Utility::GlobalZ.dot(particleFaceDirection))/(M_PI/180.0);

									particleDraw();
							}
							else
							{
									P.position = P.movement = *Vector3::Zero;
									P.time = 0;
									particlesEmitted--;
							}
							*particles[ID] = P;
						}
   						//printf("particles: %i\n",particlesEmitted);
					}
			bool	IsActive(BOOL setter = 2)
					{
						if(setter<2)
							IsVisible=setter;

						return IsVisible;
					}
			float	varianz;
			float	accselleration;
			float	lifetime;
			float	emittingFrequency;
	virtual Vector3	rotate(Vector3 r)
					{
						return particleDirection = r.normalized();
						return Sprite::rotate(r);
					}
	virtual void	DoUpdate(void)
					{

					}
};

#endif