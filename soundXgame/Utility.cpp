#include "projectMacros.h"
#include "projectGrafics.h"
#include "Utility.h"


Vector3	
Utility::GlobalX = Vector3(1,0,0);

Vector3	
Utility::GlobalY = Vector3(0,1,0);

Vector3	
Utility::GlobalZ = Vector3(0,0,-1);



//void _rotate90(float partOf90,float & a,float &b)
//{
//	a=a*partOf90;
//	b=b/(1.0-partOf90);
//}



u32_2s16_4b8::u32_2s16_4b8(void)
{
	this->u32 = 0;
}

u32_2s16_4b8::u32_2s16_4b8(int set)
{
	this->u32 = set;
}

Loader::Loader(void)
{
	_w=_h=_length=0;
	_Data = std::vector<u32_2s16_4b8>();
}


Loader::Loader(char* filename)
{
	_w=_h=_length=0;
	LoadeFile(filename);
}

Loader::~Loader(void)
{
//	delete data;
}

int
Loader::width(int setter)
{
	if(setter)
	{
		_w=setter;
		_length=_w*_h;
	}
	return _w;
}

int
Loader::height(int setter)
{
	if(setter)
	{
		_h=setter;
		_length=_w*_h;
	}
	return _h;
}

int
Loader::count(void)
{
	return _length;
}

LoaderColor
Loader::Pixel(int index)
{
	if(index<_length)
		return (LoaderColor)&_Data[index];
	else
		throw "Nö...";
}

LoaderColor
Loader::Pixel(int x,int y)
{
	return &_Data[y*_w+x];
}


void*
Loader::LoadeFile(char* tmp)
{
	char filename[64];
	sprintf(&filename[0],"Data/%s",tmp);

	u32_2s16_4b8 pixel;
	pixel.u32 = 0;
	bool read = true;
	int value=0;
	int MaximumChannelValue=0;
	int W=0;
	int H=0;
	char readBuffer[32];
	FILE* file = fopen(filename,"rb");
	fscanf(file,"%s\n",readBuffer);
	if(strcmp(readBuffer,"P3")==0)
	{// --File is RGB PPM file...
		bool W_H_ok=false;
		bool channelValueOk = false;
		int chanelCount=0;
		int pixelCount=-1;
		while(pixelCount<W*H)
		{
		fscanf(file,"%s\n",readBuffer);
			if(readBuffer[0]=='#')
			{
				char readChar='x';

				do{for(int i=0;i<sizeof(readBuffer);i++)
					{
						if(readBuffer[i]=='\0')
							{readChar='\n';break;}
						}
					if(readChar!='\n')
						fscanf(file,"%s\n",readBuffer);
				}while(readChar!='\n');
			}
			else
			{
				if((!W_H_ok)||(W==0)||(H==0))
					{
						sscanf(readBuffer,"%i",&value);
						if(W==0)
							W=value;
						else if(H==0)
						{H=value;pixelCount=0;
							W_H_ok=true;}
					}
				else if(!channelValueOk)
					{sscanf(readBuffer,"%i\n",&MaximumChannelValue);channelValueOk=true;}
				else 
				{
					sscanf(readBuffer,"%i\n",&value);
					pixel.byte[chanelCount]=value;
					if(++chanelCount==3)
					{
						pixel.byte[chanelCount] = 255;
						_Data.push_back(pixel);
						chanelCount=0;
						pixelCount++;
					}
				}
			}
		}
		if(W_H_ok)
			{ _w = W; _h = H; _length = W*H;}
		fclose(file);
		return data;
	}
else// other file-type.. later..
	return NULL;
}



/* loadObj-function: 
 * loades vertices,uv's and normals from OBJ-file to given vectors and the Shape within it's faces are made up...
 * returns a string containing the Object's Name or Type if's specified in file.. */
char* 
Utility::loadObj(const char* tmp,  std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals,GLuint &faceShape)
{     
	char filename[64];
	sprintf(&filename[0],"Data/%s",tmp);

	faceShape = GL_TRIANGLES;
	bool HasNormals = false;
	char ObjectName[64];
		 ObjectName[0] = '\0';

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> vertexTextureIndices;
	std::vector<unsigned int> normalIndices;

	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempVertexTextures;
	std::vector<glm::vec3> tempNormals;

	FILE* file = fopen(filename, "r");

	//if(file == NULL) 
	if(!file)
	{
		std::cerr << "File not found" << filename << std::endl;
		throw "FileNotFound";
		//exit(1);
	}

	while(true) 
	{
		char line[64];

		int Result = fscanf(file, "%s", line);

		if(Result == -1) 
		{
			break;
		}
		if(strcmp(line, "o") == 0)
		{
			fscanf(file,"%s", ObjectName); 
		}
		else if(strcmp(line, "v") == 0) 
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		else if(strcmp(line, "vt") == 0) 
		{
			glm::vec2 vertexTexture;
			fscanf(file, "%f %f\n", &vertexTexture.x, &vertexTexture.y);
			tempVertexTextures.push_back(vertexTexture);
		}
		else if(strcmp(line, "vn") == 0) 
		{
			HasNormals = true;
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n",  &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if(strcmp(line, "fs") == 0)
		{ 
			fscanf(file, "%s", line);
			if(strcmp(line,"QUADS")==0)
				faceShape = GL_QUADS;
		}
		else if(strcmp(line, "f") == 0) 
		{
			unsigned int *tempVertexIndex;
			unsigned int *tempVertexTextureIndex;
			unsigned int *tempNormalIndex;

			if(faceShape==GL_QUADS)
			{
				unsigned int DataVertexIndex[4];
				unsigned int DataVertexTextureIndex[4]; 
				unsigned int DataNormalIndex[4];

				tempVertexIndex = &DataVertexIndex[0];
				tempVertexTextureIndex = &DataVertexTextureIndex[0];
				tempNormalIndex = &DataNormalIndex[0];

				//Read Quads wth predefined Normals
				if(HasNormals)
				{
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
						&tempVertexIndex[0], &tempVertexTextureIndex[0], &tempNormalIndex[0],
						&tempVertexIndex[1], &tempVertexTextureIndex[1], &tempNormalIndex[1],
						&tempVertexIndex[2], &tempVertexTextureIndex[2], &tempNormalIndex[2],
						&tempVertexIndex[3], &tempVertexTextureIndex[3], &tempNormalIndex[3]);

					vertexIndices.push_back(tempVertexIndex[0]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[0]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[1]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[1]);
					normalIndices.push_back(tempNormalIndex[1]);

					vertexIndices.push_back(tempVertexIndex[2]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[2]);
					normalIndices.push_back(tempNormalIndex[2]);

					vertexIndices.push_back(tempVertexIndex[3]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[3]);
					normalIndices.push_back(tempNormalIndex[3]);
				}
				else // Quads without normals..
				{
					fscanf(file, "%d/%d %d/%d %d/%d %d/%d\n",
						&tempVertexIndex[0], &tempVertexTextureIndex[0],
						&tempVertexIndex[1], &tempVertexTextureIndex[1],
						&tempVertexIndex[2], &tempVertexTextureIndex[2],
						&tempVertexIndex[3], &tempVertexTextureIndex[3]);

					vertexIndices.push_back(tempVertexIndex[0]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[0]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[1]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[1]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[2]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[2]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[3]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[3]);
					normalIndices.push_back(tempNormalIndex[0]);
				}
			}
			else if(faceShape==GL_TRIANGLES)
			{
				unsigned int DataVertexIndex[3];
				unsigned int DataVertexTextureIndex[3]; 
				unsigned int DataNormalIndex[3];

				tempVertexIndex = &DataVertexIndex[0];
				tempVertexTextureIndex = &DataVertexTextureIndex[0];
				tempNormalIndex = &DataNormalIndex[0];

				if(HasNormals) //Triangles with predefined normals...
				{
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
						&tempVertexIndex[0], &tempVertexTextureIndex[0], &tempNormalIndex[0],
						&tempVertexIndex[1], &tempVertexTextureIndex[1], &tempNormalIndex[1],
						&tempVertexIndex[2], &tempVertexTextureIndex[2], &tempNormalIndex[2]);

					vertexIndices.push_back(tempVertexIndex[0]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[0]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[1]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[1]);
					normalIndices.push_back(tempNormalIndex[1]);

					vertexIndices.push_back(tempVertexIndex[2]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[2]);
					normalIndices.push_back(tempNormalIndex[2]);
				}
				else//Triangles without normals
				{
					fscanf(file, "%d/%d %d/%d %d/%d\n", 
						&tempVertexIndex[0], &tempVertexTextureIndex[0],
						&tempVertexIndex[1], &tempVertexTextureIndex[1],
						&tempVertexIndex[2], &tempVertexTextureIndex[2]);

					vertexIndices.push_back(tempVertexIndex[0]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[0]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[1]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[1]);
					normalIndices.push_back(tempNormalIndex[0]);

					vertexIndices.push_back(tempVertexIndex[2]);
					vertexTextureIndices.push_back(tempVertexTextureIndex[2]);
					normalIndices.push_back(tempNormalIndex[0]);
				}
			}
		}
	} // <- End of While-Cycle..


	//Copying the temp-vectors to the given vectors, wich should be filled...
	for(int i = 0; i < vertexIndices.size(); i++) 
	{
		glm::vec3 vertex = tempVertices[vertexIndices[i] -1];
		vertices.push_back(vertex);
	}

	for(int i = 0; i < vertexTextureIndices.size(); i++) 
	{
		glm::vec2 uv = tempVertexTextures[vertexTextureIndices[i] - 1];
		uvs.push_back(uv);
	}

	if(HasNormals)for(int i = 0; i < normalIndices.size();i++)
	{
		glm::vec3 normale = tempNormals[normalIndices[i] - 1];
		normals.push_back(normale);
	}

	//returning the Object's name if found in the file,otherwhise a "blank"-string...
	return &ObjectName[0];
}


std::vector<Texture*> _loadedtTextures;
std::vector<char*> _textureNames;


Texture* _loadeWithLodePNG(const char* fileName)
{
	Texture* tex = new Texture();
	std::vector<unsigned char> image;
	std::vector<unsigned char> reversi;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, &fileName[0],LCT_RGBA);

	// If there's an error, display it.
	if(error != 0)
	{	std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return NULL; }

	tex->w = width;
	tex->h = height;
	tex->format=tex->RGBA;

	//reverse Y-order !!!
	int stride = tex->w*4;
	for(int Y=tex->h-1;Y>=0;Y--)
		for(int X=0;X<stride;X+=4)
		{
			reversi.push_back(image[(Y*stride)+X]);
			reversi.push_back(image[(Y*stride)+X+1]);
			reversi.push_back(image[(Y*stride)+X+2]);
			reversi.push_back(image[(Y*stride)+X+3]);
		}

	tex->pData = &reversi[0];

	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1,&tex->ID);
	glBindTexture(GL_TEXTURE_2D,tex->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pData);

	return tex;
}

GLuint
Utility::loadTexture(const char* tmp)
{

	char filename[64];
	sprintf(&filename[0],"Data/%s",tmp);

	//glload::LoadTest loadTest = glload::LoadFunctions();
	
	char temp[64];
	temp[63]='\0';
	short i = -1;

	if(!_loadedtTextures.size())
	{//if there are no texture's loaded yet, and lists need initialization...
	 
			//init vector-lists-variables..
			_textureNames = std::vector<char*>();
			_loadedtTextures = std::vector<Texture*>();
			
			//loade the texture and save it'ID to list...
			_loadedtTextures.push_back(_loadeWithLodePNG(filename));

			//save the texture filename...
			while(((temp[++i]=filename[i])!='\0')&&(i<64)); 
			_textureNames.push_back(temp);

			//return the textureID
			return _loadedtTextures[0]->ID;

	}
	else 
	{//if list vectors already initialized and contain at least one texture...
		for(auto it=_textureNames.begin();it!=_textureNames.end();it++)
		{i++; //iterate through all loaded textures and check if requested one
			  //already contained in list.

			if(Utility::StringCompareRecursive(filename,(*it))<0)
				return _loadedtTextures[i]->ID; //return the already contained textureID;
		}i = -1;

		
			
		//save the textureID to list...
		_loadedtTextures.push_back(_loadeWithLodePNG(filename));

		//save the texture's filename...	
		while(((temp[++i]=filename[i])!='\0')&&(i<64));
		_textureNames.push_back(temp);

		i=_loadedtTextures.size()-1;
		//and return the textureID.
		return _loadedtTextures[i]->ID;
		
	}
	return 0;
}


/*
 * Returns a values between 0 and 1
 */
float
Utility::GetRandom(void)
{
	float rand = std::rand() % 100;
	rand /= 100;
	return rand;
}


bool _randomizatorSeeded=false;
//random float from 0 to max;


float
Utility::Random(void)
{
	if(!_randomizatorSeeded)
	{
		std::srand(GetTickCount());
		_randomizatorSeeded=true;
	}
	int randomfloat = std::rand();
	return ((float)randomfloat/32767);
}

float
Utility::Random(float MAX_float)
{
	return Utility::Random()*MAX_float;
}

Vector3
Utility::RandomVector3(void)
{
	Vector3 vec3 = Vector3(Utility::Random(2.0f)-1.0f,Utility::Random(2.0f)-1.0f,Utility::Random(2.0f)-1.0f);
	return vec3;
}

Vector3
Utility::RandomDirection(void)
{
	return Utility::RandomVector3().normalized();
}

Vector3
Utility::GetScalevectorByAspect(int width,int height,float zDepth)
{
	Vector3 result = *Vector3::Zero;
			result.x=zDepth;
			result.y=zDepth;
			result.z=zDepth;

	if(width > height)
		result.y=((float)height/width);
	else if(width < height)
		result.x=((float)width/height);

	return result;
}

// recursive string cooperation... returns -1 if strings are equal, 
// else returns the number of character at which they where different.
int
_cmprStrngsRec(const char* A,const char* B,int position)
{
	if((A[position]=='\0')&&(B[position]=='\0'))
		return -1;
	else if(A[position]==B[position])
		return _cmprStrngsRec(A,B,++position);
	else
		return position;
}

//Compare 2 strings case sensitive by iteration..
bool
_compareStrings(const char* A,const char* B)
{
	bool result = false;
	int i = -1;
	while(A[++i]!='\0' && B[i]!='\0') 
	{
		if(A[i]==B[i])
			result = true;
		else
			return false;
	}
	return result;
}

//Compare 2 strings case sensitive by recursion...
unsigned
_compareStringsRecursive(const char* A,const char* B)
{
	return _cmprStrngsRec(A,B,0);
}

bool
Utility::StringCompareIterative(const char* A,const char* B)
{
	return _compareStrings(A,B);
}

unsigned
Utility::StringCompareRecursive(const char* A,const char* B)
{
	 return _compareStringsRecursive(A,B);
}