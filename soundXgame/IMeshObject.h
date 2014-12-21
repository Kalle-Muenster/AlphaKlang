#ifndef _MESHOBJECT_H_
#define _MESHOBJECT_H_

#include "igobject.h"

typedef char* string;

struct Texture 
{
	enum class Format
	{DUAL=1,GRAY=16,RGB=24,RGBA=GL_RGBA,BGRA=GL_BGRA_EXT,ABGR=GL_ABGR_EXT};
	GLuint ID;
	short w,h;
	GLint format;
	void* pData;
	void Loade(string fileName,short Width,short Height,Format textureFormat = Texture::Format::BGRA);
};

template<const int NUMBER_OF_FRAMES>  
class MultilayerTexture
	: public Texture
{
private:
	GLuint frameIDs[NUMBER_OF_FRAMES];
	unsigned short frameDurations[NUMBER_OF_FRAMES];
	double timer;
	int current;

public:
	MultilayerTexture(int fps = -1)
	{
		current = 0;
		timer = 1000/fps;
		 if(fps>0)
		 {
			 for(int i=0;i<NUMBER_OF_FRAMES;i++)
				frameDurations[i]=timer;
		 }
	}
	virtual ~MultilayerTexture(void){}
	void LoadeFrames(string filenames[])
	{
		for(int i =0; i<NUMBER_OF_FRAMES ; i++)
		{
			frameIDs[i] = Utility::loadTexture(filenames[i]);
		}
	}
	void UpdateFrame(void)
	{
		if((timer+=INPUT->FrameTime) > frameDurations[current])
		{
			timer=0;
			if(++current==NUMBER_OF_FRAMES)
				current=0;

			ID=frameIDs[current];
		}
	}
};

class IMeshObject : 
	public IGObject
{
private:
	void LoadObj(const char* objFileName);
	void DoObjBuffer(void);
protected:
	GLuint					FaceShape;
	GLuint					vertexBufferID;
	GLuint					uvBufferID;
	GLboolean				NoBackfaceCulling;
	GLboolean				UseTexture;
	Texture					texture;
	std::vector<glm::vec3>	verts;
	std::vector<glm::vec2>	uvs;
	std::vector<glm::vec3>	norms;
	Vector3					Pivot;

public:
	data32				color;
	virtual Texture*  	GetTexture(void);
	virtual void		SetTexture(Texture*);
	bool				GroundedWithPivot;

						IMeshObject(void);
	void				InitializeObject(const char* objFile,bool addToSceneGraph=true,Vector3 positionOffset = *Vector3::Zero);
	virtual				 ~IMeshObject(void);

	virtual void		LoadMesh(const char* objFileName);
	virtual void		LoadMesh(const char* objFileName,Vector3 positionOffset);
	virtual IGObject*	LoadTexture(const char* textureFileName);
	virtual void		SetColor(data32 color);
	virtual void		SetColor(unsigned char,unsigned char,unsigned char,unsigned char);
	virtual Vector3		move(Vector3);
	virtual Vector3		move(float,float,float);
	virtual Vector3		rotate(Vector3);
	virtual Vector3		rotate(float,float,float);
	virtual Vector3		scale(Vector3);
	virtual Vector3		scale(float,float,float);
	virtual void		draw(void);
	virtual void		DoLate(void);

};
//class Kollective : 
//	public IConnectable
//{
//private:
//	
//	unsigned portID;
//	bool progressing;
//	IGObject* instance;
//public:
//	unsigned count;
//	bool checkFail(void)
//	{return progressing;}
//	bool through(GobID wire)
//	{return wire != portID;}
//	void apear(IGObject* numberOfCount)
//	{ instance = numberOfCount; progressing = false; }
//	IGObject* get(unsigned number,unsigned ofCount)
//	{
//		portID = number;
//		count = ofCount;
//		progressing = true;
//
//		while(progressing)
//			this->Connection()->through();
//
//		return instance;
//	}
//};
#endif
