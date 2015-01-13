#ifndef _ANIMATEDSPRITE_H_
#define _ANIMATEDSPRITE_H_

#include "Sprite.h"


template<const unsigned FRAME_COUNT,const unsigned short NUMBER_OF_LOOPS>
class AnimatedSprite : public Sprite
{
protected:
	GLuint	frameUVBuffers[FRAME_COUNT];
	float	frameDurations[FRAME_COUNT];
	short	LoopStart[NUMBER_OF_LOOPS];
	short	LoopEnd[NUMBER_OF_LOOPS];
	int		currentFrame,currentLoop;
	float	nextFrameTimer;

public:



						AnimatedSprite(string filename, int slicesW, int slicesH, int fps, bool backface=true)
						{
							nextFrameTimer=0;
						
							SetFPS(fps);
						
							verts.push_back(Vector3(-1,-1,0));
							verts.push_back(Vector3(1,-1,0));
							verts.push_back(Vector3(1,1,0));
							verts.push_back(Vector3(-1,1,0));
						
							FaceShape=GL_QUADS;
						
							glm::vec3 temp1 = glm::vec3(0,0,-1);
							glm::vec3 temp2 = glm::vec3(1,0,0);
							glm::vec3 temp3 = glm::vec3(0,1,0);
						
							transform.forward = Vector3(temp1.x,temp1.y,temp1.z);
							transform.right = temp2;
							transform.up = temp3;
						
							glGenBuffers(1, &vertexBufferID);
							glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
							glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
						
							LoadeTexture(filename,slicesW,slicesH);
							SetName(filename);
							SetID(SceneGraph::getInstance()->Add(this));
							LockID();
						
							this->NoBackfaceCulling = backface;
							for(currentLoop=0;currentLoop<NUMBER_OF_LOOPS;currentLoop++)
							{
								LoopStart[currentLoop] = (FRAME_COUNT/NUMBER_OF_LOOPS)*currentLoop;
								LoopEnd[currentLoop] = LoopStart[currentLoop] + ((FRAME_COUNT/NUMBER_OF_LOOPS)-1);
							}
							currentLoop=0;
						}
	void				SetFPS(int fps)
						{
							for(int i=0;i<FRAME_COUNT;i++)
								frameDurations[i] = (1000.0/fps)/1000;
						}
	void				SetFrameDuration(int frameNumber,int timeInMilliSeconds)
						{
							frameDurations[frameNumber] = (float)timeInMilliSeconds/1000;
						}
	void				SetToLoop(int loopNumber,bool reset=false)
						{
							if(reset)
								currentFrame = LoopStart[loopNumber];
							else
								currentFrame = LoopStart[loopNumber] + (currentFrame-LoopStart[currentLoop]);
						
							currentLoop = loopNumber;
						}
	void				LoopConfig(int loopnumber,int firstFrame,int lastframe)
						{
							currentLoop = loopnumber;
							LoopStart[loopnumber] = firstFrame;
							LoopEnd[loopnumber] = lastframe;
						}
	virtual IGObject*	LoadeTexture(string filenames,int slicesW,int slicesH)
						{
							float x,y;
							x=1.0/slicesW;
							y=1.0/slicesH;
							currentFrame=0;
						
							for(int Y=slicesH-1;Y>=0;Y--)
							{
								for(int X=0;X<slicesW;X++)
								{
									uvs.push_back(glm::vec2(x*X,y*Y));
									uvs.push_back(glm::vec2(x*X+x,y*Y));
									uvs.push_back(glm::vec2(x*X+x,y*Y+y));
									uvs.push_back(glm::vec2(x*X,y*Y+y));
									
									glGenBuffers(1, &frameUVBuffers[currentFrame]);
									glBindBuffer(GL_ARRAY_BUFFER, frameUVBuffers[currentFrame]);
									glBufferData(GL_ARRAY_BUFFER, (4 * sizeof(glm::vec2)), &uvs[currentFrame*4], GL_STATIC_DRAW);
						
									currentFrame++;
								}
							}
						
							currentFrame=0;
							uvBufferID = frameUVBuffers[0];
							IMeshObject::LoadTexture(filenames);
							transform.scale = Utility::GetScalevectorByAspect(texture.w/slicesW,texture.h/slicesH);
						
							return this;
						}
	virtual void		draw(void)
						{
							if((nextFrameTimer-=InputManager::getInstance()->FrameTime) < 0)
							{
								if(++currentFrame>LoopEnd[currentLoop])
									currentFrame=LoopStart[currentLoop];
						
								nextFrameTimer = frameDurations[currentFrame];
								uvBufferID = frameUVBuffers[currentFrame];
							}
							IMeshObject::draw();
						}
};

#endif
