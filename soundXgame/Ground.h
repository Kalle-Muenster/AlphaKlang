#ifndef __GROUND_H__
#define __GROUND_H__

#include <vector>
#include "projectGrafics.h"
//#include <GL\glew.h>
//#include <GL\freeglut.h>
//#include <glm.hpp>
//#include <glload\gl_load.hpp>
//#include <glimg/glimg.h>

#include "IGObject.h"
#include "Cam.h"
#include "FirstPerson.h"


class Ground : public IGObject //, public MusicListener
{
private:

	const int count_x; // width & depth muss be a multiple of configMap-1 (example configMap size=4; 4-1=3; count_x=30 is a multiple of 3)
	const int count_z;
	const float x;
	const float y;
	const float z;
	const float width;
	float depth;
	const float heightRange;
	const float dynamicRange;

	int* configMap [5];
	int** heightMap;
	int* dynamicConfigMap [9];
	int** dynamicMap;

	// list for dynamic access
	int** mapList [2];
	float rangeList[2];
	float* valueList[2];

	bool drawPlanes;
	bool drawLines;
	bool coloredTiles;
	float heightVal;
	float dynamicVal;
	float lastDVal;
	float MaximumChange;
	float ChageAdder; 

	int** CalculateMap(int** assignMap, int** config, int size);
	void Update(void);

public:

	static Ground* getInstance();

	Ground(void);
	~Ground(void);
	//void Init(void);
	virtual void draw(void);
	float GetGroundY(float posX, float posZ);

};



#endif