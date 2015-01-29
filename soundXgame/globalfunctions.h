#ifndef _GLOBAL_FUNCTIONS_INCLUDED_
#define _GLOBAL_FUNCTIONS_INCLUDED_


namespace ProjectMappe
{



//Functions:
int prepareForExit(void);
void InitGlut(void);
void LoadContent(void);
void OnLoadContent(void);
void GlInit(void);
void InitEngine(int&,char**);
void UpdateCycle(void);
void RenderCycle(void);
void OnDisplay(void);
void OnIdle(void);
void OnReshape(int,int);
void MouseMoveFunc(int,int);
void MouseClicks(int,int,int,int);
void MouseWheelFunc(int,int,int,int);
void processSpecialKeys(int,int,int);
void keyboardInput(unsigned char,int,int);
void keyboardUpInput(unsigned char,int,int);
unsigned int GetGLWindowID(void);
//void GamePadFunc(unsigned,int,int,int);

};

#endif // !1
