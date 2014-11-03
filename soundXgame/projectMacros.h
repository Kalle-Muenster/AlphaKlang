#ifndef _POJECT_MACROS__
#define _POJECT_MACROS__

#include <vector>
#include <iostream>
#include "InputManager.h"
#include "SceneGraph.h"
#include "UpdateManager.h"
#include "BassAudio.h"



#define SCREENWIDTH (800)
#define SCREENHEIGHT (600)
#define DINGERBUFFERLENGTH (16384)
#define CONTROLERPOLLINTERVAL (50)
//#define CAM_ORTHOGRAFIC "Orthografic"
//#define CAM_PERSPECTIVE "Perspective"

#include <windows.h>


#define INPUT InputManager::getInstance()
#define SCENE SceneGraph::GetInstance()
#define UPDATE UpdateManager::getInstance()
#define AUDIO BassAudio::GetInstance()




#endif