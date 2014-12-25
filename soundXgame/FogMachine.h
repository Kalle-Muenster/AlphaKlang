#ifndef _FOGMACHINE_H_
#define _FOGMACHINE_H_

#define MAXIMUM_NUMBER_OF_PARTICLES	(1000)

#include "List.h"
#include "Sprite.h"
#include "UpdateManager.h"

struct Particles
{
	Vector3 position;
	Vector3 movement;
	float time;
	void move(Vector3 to);
	Particles(void);
	~Particles(void);
};

	 
class FogMachine :
	public IGObject,
	public IUpdateble
	{
	protected:
		List<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES> fogs;
		List<Particles*,MAXIMUM_NUMBER_OF_PARTICLES> particles;
		std::vector<glm::vec3> drawingVertices;
		std::vector<glm::vec2> drawingUVs;
		MultilayerTexture<3> texture;
		bool particlesAlwaysFaceCamera;
		GLuint vertexBufferID;
		GLuint uvBufferID;
		Vector3 particleDirection;
	    Vector3 particleRotationAxis;
		GLfloat particleRotationAngel;
		Vector3 particlePosition;
		GLfloat particleSize;
		int Emmission;

	public:
		FogMachine(string[]);
	//	virtual void InitializeObject(void);
		virtual ~FogMachine(void);

		void SellectParticleImage(void);
		float varianz;
		float accselleration;
		float emittingAngle;
		float lifetime;
		float emittingFrequency;
		float timer;
		int lastActivatedParticle;

		void LoadeParticle(string[]);
		void MachDampf(void);
		virtual void draw(void);
		virtual void DoUpdate(void);
		void drawParticle(void);

	};

#endif