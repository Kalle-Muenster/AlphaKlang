#include "Ground.h"
#include "projectMacros.h"


Ground* instance;

Ground* Ground::getInstance()
{
	if(!instance)
		instance = new Ground();
	return instance;
}

  
Ground::Ground(void) :
	count_x(40),
	count_z(40),
	x(-20.0f*3.5f),
	y(-5.0f),
	z(20.0f*3.5f),
	width(3.5f),

	// ranges
	heightRange(5.0f),
	dynamicRange(17.0f),

	// height values
	heightVal(100),
	dynamicVal(0),

	// states
	drawPlanes(true), 
	drawLines(true),
	coloredTiles(false),
	dynamicToTop(true)
{
	// individuell vars
	depth = width * -1;

	// settings
	this->SetID(SCENE->Add(this));
	this->LockID();
	IsVisible=true;

	
	// Config Map
	int Tmp[5][5]	=	{  100 ,   30 ,   70 ,   90 ,  100  ,
						    70 ,   20 ,   50 ,   50 ,   50  ,
						    50 ,   10 ,   30 ,   25 ,   70  ,
						     0 ,    5 ,   15 ,   10 ,   40  ,
						    00 ,    0 ,    5 ,   25 ,   70 
						};

	// Base Height Map
	/*int Tmp[10][10]	=	{  100 ,    0 ,  100 ,   40 ,  100 ,    0 ,  100 ,   40 ,  100,  100  ,
						    70 ,   20 ,   50 ,   10 ,   30 ,   20 ,   50 ,   10 ,   30,   30  ,
						    20 ,    0 ,   30 ,   40 ,   80 ,    0 ,   30 ,   40 ,   80,   80  ,
						     0 ,   25 ,   35 ,    0 ,   40 ,   25 ,   35 ,    0 ,   40,   40  ,
						    65 ,   40 ,   85 ,   35 ,  100 ,   40 ,   85 ,   35 ,  100,  100  ,
							70 ,   20 ,   50 ,   10 ,   30 ,   20 ,   50 ,   10 ,   30,   30  ,
							20 ,    0 ,   30 ,   40 ,   80 ,    0 ,   30 ,   40 ,   80,   80  ,
							 0 ,   25 ,   35 ,    0 ,   40 ,   25 ,   35 ,    0 ,   40,   40  ,
							65 ,   40 ,   85 ,   35 ,  100 ,   40 ,   85 ,   35 ,  100,  100  ,
							20 ,    0 ,   30 ,   40 ,   80 ,    0 ,   30 ,   40 ,   80,   80  
						};*/
	for (int i = 0; i < 5; i++)
	{
		configMap[i] = new int [10];
		configMap[i][0] = Tmp[i][0];
		configMap[i][1] = Tmp[i][1];
		configMap[i][2] = Tmp[i][2];
		configMap[i][3] = Tmp[i][3];
		configMap[i][4] = Tmp[i][4];
		/*configMap[i][5] = Tmp[i][5];
		configMap[i][6] = Tmp[i][6];
		configMap[i][7] = Tmp[i][7];
		configMap[i][8] = Tmp[i][8];
		configMap[i][9] = Tmp[i][9];*/
	}
	heightMap = new int* [count_z+1];
	for (int i = 0; i < count_z+1; i++)
	{
		heightMap[i] = new int [count_x+1];
	}

	// Dynamic Height Map
	
	int Tmp2[9][9]	=	{  100 ,  100 ,  100 ,  100 ,  100 ,   70 ,  100 ,  100 ,  100  ,
						    70 ,   20 ,   50 ,   50 ,   60 ,   20 ,   50 ,   50 ,   90  ,
						    50 ,   00 ,   30 ,   25 ,   30 ,   00 ,   30 ,   25 ,   50  ,
						    20 ,   20 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,  100  ,
						    40 ,   10 ,   20 ,    0 ,   35 ,   20 ,    0 ,   10 ,   50  ,
						    70 ,   30 ,    0 ,   10 ,   20 ,   30 ,   50 ,   10 ,   70  ,
						    50 ,   10 ,   20 ,   25 ,   10 ,    0 ,   20 ,    0 ,   90  ,
						    30 ,   30 ,   25 ,   20 ,    0 ,   10 ,   30 ,   20 ,  100  ,
						    20 ,   30 ,   55 ,   35 ,   30 ,   30 ,    5 ,   25 ,  100 
						};
	/*
	int Tmp2[16][16] =	{  100 ,   90 ,   60 ,   80 ,  100 ,   40 ,   80 ,  100 ,   50 ,  100 ,  100 ,  100 ,   70 ,  100 ,  100 ,  100  ,
						    70 ,   20 ,   50 ,   50 ,   60 ,   20 ,   50 ,   50 ,   90 ,   50 ,   50 ,   60 ,   20 ,   50 ,   50 ,   90  ,
						    50 ,   00 ,   30 ,   25 ,   30 ,   00 ,   30 ,   25 ,   50 ,   30 ,   25 ,   30 ,   00 ,   30 ,   25 ,   50  ,
						    20 ,   20 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,   10 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,  100  ,
						    40 ,   10 ,   20 ,    0 ,   35 ,   20 ,    0 ,   10 ,   50 ,   20 ,    0 ,   35 ,   20 ,    0 ,   10 ,   50  ,
						    70 ,   30 ,    0 ,   10 ,   20 ,   30 ,   50 ,   10 ,   70 ,    0 ,   10 ,   20 ,   30 ,   50 ,   10 ,   70  ,
						    50 ,   10 ,   20 ,   25 ,   10 ,    0 ,   20 ,    0 ,   90 ,   20 ,   25 ,   10 ,    0 ,   20 ,    0 ,   90  ,
						    30 ,   30 ,   25 ,   20 ,    0 ,   10 ,   30 ,   20 ,   10 ,   25 ,   20 ,    0 ,   10 ,   30 ,   20 ,  100  ,
						    70 ,   20 ,   50 ,   50 ,   60 ,   20 ,   50 ,   50 ,   90 ,   50 ,   50 ,   60 ,   20 ,   50 ,   50 ,   90  ,
						    50 ,   00 ,   30 ,   25 ,   30 ,   00 ,   30 ,   25 ,   50 ,   30 ,   25 ,   30 ,   00 ,   30 ,   25 ,   50  ,
						    20 ,   20 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,   10 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,  100  ,
						    40 ,   10 ,   20 ,    0 ,   35 ,   20 ,    0 ,   10 ,   50 ,   20 ,    0 ,   35 ,   20 ,    0 ,   10 ,   50  ,
						    70 ,   30 ,    0 ,   10 ,   20 ,   30 ,   50 ,   10 ,   70 ,    0 ,   10 ,   20 ,   30 ,   50 ,   10 ,   70  ,
						    50 ,   10 ,   20 ,   25 ,   10 ,    0 ,   20 ,    0 ,   90 ,   20 ,   25 ,   10 ,    0 ,   20 ,    0 ,   90  ,
						    30 ,   20 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,   10 ,   10 ,   10 ,   10 ,   10 ,   20 ,   10 ,  100  ,
						    20 ,   30 ,   55 ,   35 ,   30 ,   30 ,    5 ,   25 ,  100 ,   55 ,   35 ,   30 ,   30 ,    5 ,   25 ,  100 
						};*/
	for (int i = 0; i < 9; i++)
	{
		dynamicConfigMap[i] = new int [16];
		dynamicConfigMap[i][0] = Tmp2[i][0];
		dynamicConfigMap[i][1] = Tmp2[i][1];
		dynamicConfigMap[i][2] = Tmp2[i][2];
		dynamicConfigMap[i][3] = Tmp2[i][3];
		dynamicConfigMap[i][4] = Tmp2[i][4];
		dynamicConfigMap[i][5] = Tmp2[i][5];
		dynamicConfigMap[i][6] = Tmp2[i][6];
		dynamicConfigMap[i][7] = Tmp2[i][7];
		dynamicConfigMap[i][8] = Tmp2[i][8];
		/*dynamicConfigMap[i][9] = Tmp2[i][9];
		dynamicConfigMap[i][10] = Tmp2[i][10];
		dynamicConfigMap[i][11] = Tmp2[i][11];
		dynamicConfigMap[i][12] = Tmp2[i][12];
		dynamicConfigMap[i][13] = Tmp2[i][13];
		dynamicConfigMap[i][14] = Tmp2[i][14];
		dynamicConfigMap[i][15] = Tmp2[i][15];*/
	}
	dynamicMap = new int* [count_z+1];
	for (int i = 0; i < count_z+1; i++)
	{
		dynamicMap[i] = new int [count_x+1];
	}

	// save both array to list
	mapList[0] = heightMap;
	mapList[1] = dynamicMap;
	rangeList[0] = heightRange;
	rangeList[1] = dynamicRange;
	valueList[0] = &heightVal;
	valueList[1] = &dynamicVal;
	

	// initize map once
	heightMap = CalculateMap(heightMap, configMap, sizeof(configMap) / sizeof(configMap[0]));
	dynamicMap = CalculateMap(dynamicMap, dynamicConfigMap, sizeof(dynamicConfigMap) / sizeof(dynamicConfigMap[0]));

}

Ground::~Ground(void)
{
//	delete[] configMap;
	delete[] heightMap;
//	delete[] dynamicConfigMap;
	delete[] dynamicMap;
//	delete[] mapList;
//	delete[] rangeList;
//	delete[] valueList;
}


int** Ground::CalculateMap(int** assignMap, int** config, int size)
{

	//const int lengthZ = sizeof(config) / sizeof(config[0]);
	const int lengthZ = size;
	const int lengthX =  size;
	const int pointZ = count_z / (lengthZ-1); // wenn map 100 koor bei 3 config-punkte => 0,50,100
	const int pointX = count_x / (lengthX-1); 

	// initialwerte einsetzen
	for (int iz = 0; iz <= count_z; iz++)
	{
		for (int ix = 0; ix <= count_x; ix++)
		{
			if(iz % pointZ == 0 && ix % pointX == 0)
			{
				int tmpZ = (int)iz / pointZ;
				int tmpX = (int)ix / pointX;
				assignMap[iz][ix] = config[tmpZ][tmpX];
			}
		}
	}

	// randwerte horizontal berechnen
	for (int iz = 0; iz <= count_z; iz++)
	{
		bool active = false;
		for (int ix = 0; ix <= count_x; ix++)
		{
			if(iz % pointZ == 0 && ix % pointX == 0)
			{
				active = true;
			}
			else if(active == true)
			{
				int tmpRest = ix % pointX;
				int lastIndexX = (int)ix - tmpRest;
				int nextIndex = ix + (pointX - tmpRest);
						
				int lastInt = assignMap[iz][lastIndexX];
				int nextInt = assignMap[iz][nextIndex];
				int smallerVal, biggerVal;
				bool leftToRight = true;

				if(lastInt <= nextInt)
				{
					smallerVal = lastInt;
					biggerVal = nextInt;
					leftToRight = false;
				}
				else
				{
					smallerVal = nextInt;
					biggerVal = lastInt;
				}
				int diffInt = biggerVal - smallerVal;

				float adding = (float)diffInt / pointX;

				for (int i = 1; i < pointX; i++)
				{
					int newVal;
					if(leftToRight)
						newVal = (int)((float)biggerVal - (float)i * adding);
					else
						newVal = (int)((float)smallerVal + (float)i * adding);

					assignMap[ iz ][ lastIndexX + i ] = newVal;
				}

				ix = nextIndex;
			}
		}
	}

	// randwerte vertikal berechnen
	for (int iz = 0; iz <= count_z; iz++)
	{
		for (int ix = 0; ix <= count_x; ix++)
		{
			if(iz % pointZ == 0 && ix % pointX == 0)
			{
				ix = count_x;
			}
			else
			{
				int tmpRest = iz % pointZ;
				int lastIndexX = (int)iz - tmpRest;
				int nextIndex = iz + (pointZ - tmpRest);
						
				int lastInt = assignMap[lastIndexX][ix];
				int nextInt = assignMap[nextIndex][ix];
				int smallerVal, biggerVal;
				bool leftToRight = true;

				if(lastInt <= nextInt)
				{
					smallerVal = lastInt;
					biggerVal = nextInt;
					leftToRight = false;
				}
				else
				{
					smallerVal = nextInt;
					biggerVal = lastInt;
				}
				int diffInt = biggerVal - smallerVal;

				float adding = (float)diffInt / pointZ;

				for (int i = 1; i < pointZ; i++)
				{
					int newVal;
					if(leftToRight)
						newVal = (int)((float)biggerVal - (float)i * adding);
					else
						newVal = (int)((float)smallerVal + (float)i * adding);

					assignMap[ lastIndexX + i ][ ix ] = newVal;
				}

			}
		}
	}

	//heightMap = assignMap;
	return assignMap;
	
	// zwischenwerte berechnen erfolgt zusammen mit der vertikal-berechnung
	// TODO zur verfeinerung bei zwischenwerten den durchschnitt zwischen vertikal und horizontal mit einschließen

}


void Ground::draw(void)
{

	Update();

	glPushMatrix();
	{
		if(coloredTiles == false)
		{
			glColor3f(0,0,0);
		}

		// Draw Planes
		if(drawPlanes == true)
		{
			glBegin(GL_QUADS);
			{

				// draw
				for (int nz = 0; nz < count_z; nz++) // z
				{
					for (int nx = 0; nx < count_x; nx++) // x
					{
						float heightFrontLeft  =  (float)heightMap[count_z - nz    ][nx    ] / 100 * heightRange;
						float heightFrontRight =  (float)heightMap[count_z - nz    ][nx + 1] / 100 * heightRange;
						float heightRearLeft   =  (float)heightMap[count_z - nz - 1][nx    ] / 100 * heightRange;
						float heightRearRight  =  (float)heightMap[count_z - nz - 1][nx + 1] / 100 * heightRange;
					
						heightFrontLeft  +=  (float)dynamicMap[count_z - nz    ][nx    ] / 100 * dynamicRange / 100 * dynamicVal;
						heightFrontRight +=  (float)dynamicMap[count_z - nz    ][nx + 1] / 100 * dynamicRange / 100 * dynamicVal;
						heightRearLeft   +=  (float)dynamicMap[count_z - nz - 1][nx    ] / 100 * dynamicRange / 100 * dynamicVal;
						heightRearRight  +=  (float)dynamicMap[count_z - nz - 1][nx + 1] / 100 * dynamicRange / 100 * dynamicVal;
					
						if(coloredTiles == true)
						{
							glColor4f( (0 + nx * (255.0f/count_x) ) /255, (100.0f - nz * (100.0f/count_z)) /255, 0.0f, 0.4f);
						}

						glVertex3f(x + nx*width + width ,   y + heightRearRight ,  z + nz*depth + depth  );
						glVertex3f(x + nx*width         ,   y + heightRearLeft ,  z + nz*depth + depth  );
						glVertex3f(x + nx*width         ,   y + heightFrontLeft ,  z + nz*depth          );
						glVertex3f(x + nx*width + width ,   y + heightFrontRight ,  z + nz*depth          );
					}
				}

			}
			glEnd();
		}

		// Draw Lines
		if(drawLines == true)
		{
			glBegin(GL_LINES);
			{

				// draw
				for (int nz = 0; nz < count_z; nz++) // z
				{
					for (int nx = 0; nx < count_x; nx++) // x
					{
						float heightFrontLeft  =  (float)heightMap[count_z - nz    ][nx    ] / 100 * heightRange;
						float heightFrontRight =  (float)heightMap[count_z - nz    ][nx + 1] / 100 * heightRange;
						float heightRearLeft   =  (float)heightMap[count_z - nz - 1][nx    ] / 100 * heightRange;
						float heightRearRight  =  (float)heightMap[count_z - nz - 1][nx + 1] / 100 * heightRange;
					
						heightFrontLeft  +=  (float)dynamicMap[count_z - nz    ][nx    ] / 100 * dynamicRange / 100 * dynamicVal;
						heightFrontRight +=  (float)dynamicMap[count_z - nz    ][nx + 1] / 100 * dynamicRange / 100 * dynamicVal;
						heightRearLeft   +=  (float)dynamicMap[count_z - nz - 1][nx    ] / 100 * dynamicRange / 100 * dynamicVal;
						heightRearRight  +=  (float)dynamicMap[count_z - nz - 1][nx + 1] / 100 * dynamicRange / 100 * dynamicVal;
					
						heightFrontLeft += 0.01f;
						heightFrontRight += 0.01f;
						heightRearLeft += 0.01f;
						heightRearRight += 0.01f;

						//glColor3f( (0 + nx * (255.0f/count_x) ) /255, (100.0f - nz * (100.0f/count_z)) /255, 0.0f);
						glColor3f(1,0,0);

						glVertex3f(x + nx*width + width ,   y + heightRearRight ,  z + nz*depth + depth  ); // left-top
						glVertex3f(x + nx*width         ,   y + heightRearLeft ,  z + nz*depth + depth  ); // right-top
						glVertex3f(x + nx*width + width ,   y + heightRearRight ,  z + nz*depth + depth  ); // left-top
						glVertex3f(x + nx*width + width ,   y + heightFrontRight ,  z + nz*depth          ); // left-bottom
					}
				}
			}
			glEnd();
		}

		
		glColor3f(1,1,1);
	}
	glPopMatrix();
}

void Ground::Update(void)
{
	float speed = 35.0f * (float)INPUT->FrameTime;
	//float speed = 0;
	if(dynamicToTop)
		dynamicVal += speed;
	else
		dynamicVal -= speed;
	if(dynamicVal >= 100)
	{
		dynamicVal = 100;
		dynamicToTop = false;
	}
	else if(dynamicVal <= 0)
	{
		dynamicVal = 0;
		dynamicToTop = true;
	}

	// Update First Person Camera
	Cam* cam = SceneGraph::GetInstance()->camera;
	CAM_MODE modeType = cam->Mode();
	if(modeType == FIRSTPERSON)
	{
		CameraMode* mode = cam->ModeSocket->Get<CameraMode>(2);
		FirstPerson* firstPerson = dynamic_cast<FirstPerson*>(mode);
		float y = Ground::getInstance()->GetGroundY(firstPerson->x, firstPerson->z);
		firstPerson->UpdateHeight(y);
	}

}

// this method returns collision height while object is placing on ground
float Ground::GetGroundY(float posX, float posZ)
{
	// prepare to index
	posX -= x;
	posZ -= z;
	posX /= width;
	posZ /= width;
	posZ *= -1;

	// if element outside ground -> set next to ground
	posX = (posX < 0) ? posX = 0 : (posX > count_x) ? count_x : posX;
	posZ = (posZ < 0) ? posZ = 0 : (posZ > count_z) ? count_z : posZ;

	// floats to ints for index
	int indexX1 = (int)posX;
	int indexZ1 = (int)posZ;

	// calculate next index for values between two tiles
	int indexX2 = (indexX1 < count_x) ? indexX1 + 1 : indexX1;
	int indexZ2 = (indexZ1 < count_z) ? indexZ1 + 1 : indexZ1;

	// calculate rest
	float restX = posX - indexX1;
	float restZ = posZ - indexZ1;

	// calculate index height
	float posY = y;
	
	// loop map list
	for(int i = 0; i < (sizeof(mapList)/sizeof(*mapList)); i++)
	{

		// get points
		float point1 = (float)mapList[i][count_z - indexZ1][indexX1] / 100 * rangeList[i] / 100 * *valueList[i];
		float point2 = (float)mapList[i][count_z - indexZ1][indexX2] / 100 * rangeList[i] / 100 * *valueList[i];
		float point3 = (float)mapList[i][count_z - indexZ2][indexX1] / 100 * rangeList[i] / 100 * *valueList[i];
		float point4 = (float)mapList[i][count_z - indexZ2][indexX2] / 100 * rangeList[i] / 100 * *valueList[i];

		/*
		Points
		3     4
		x-----x
		|     |
		x-----x
		1     2
		*/
	
		// Calculating real Tile-Height with observance of each individual heights point
		float line12 = (point2 - point1) * restX;
		float line34 = (point3 - point1) - (point3 - point4) * restX;
		float normalized = (line34 > line12) ? line12 : line34;
		line34 -= normalized;
		line12 -= normalized;
		float heightX = ((line34 - line12) * restZ + normalized);

	//float posDynY1 += (float)dynamicMap[count_z - indexZ][indexX] / 100 * dynamicRange / 100 * dynamicVal;
		posY += point1;
		posY += heightX;
	}


	return posY;

}