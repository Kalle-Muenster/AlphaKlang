#include "projectMacros.h"
#include "projectGrafics.h"
#include "Utility.h"


void _rotate90(float partOf90,float & a,float &b)
{
	a=a*partOf90;
	b=b/(1.0-partOf90);
}



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

Loader::Loader(const char* filename)
{
	_w=_h=_length=0;
	LoadeFile(filename);
}

Loader::~Loader(void)
{
	delete data;
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
Loader::LoadeFile(const char* filename)
{
	int value=0;
	int W=0;
	int H=0;
	char readBuffer[32];
	FILE* file = fopen(filename,"rb");
//	fseek(file,0,SEEK_SET);
	fscanf(file,"%s\n",readBuffer);
	fscanf(file,"%s\n",readBuffer);
	//fscanf(file,"%s\n",readBuffer);
	fscanf(file,"%i %i\n",&W,&H);
	_w=W;
	_h=H;
	_length = _w*_h;
	fscanf(file,"%i\n",&value);
		if(value==255)
		{
	for(int i = 0;i<_length;i++)
	{
		_Data.push_back(u32_2s16_4b8());
		int x=i%_w;
		int y=i/_w;
		fscanf(file,"%i\n",&value);
		_Data[i].byte[0]=value;
		fscanf(file,"%i\n",&value);
		_Data[i].byte[1]=value;
		fscanf(file,"%i\n",&value);
		_Data[i].byte[2]=value;
		_Data[i].byte[3]=255;
	}
		}
	fclose(file);
	return data;
}



/* loadObj-function: 
 * loades vertices,uv's and normals from OBJ-file to given vectors and the Shape within it's faces are made up...
 * returns a string containing the Object's Name or Type if's specified in file.. */
char* 
Utility::loadObj(const char* filename,  std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals,GLuint &faceShape)
{     
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
					&tempVertexIndex[0], &tempVertexTextureIndex[0],// &tempNormalIndex[0],
					&tempVertexIndex[1], &tempVertexTextureIndex[1],// &tempNormalIndex[1],
					&tempVertexIndex[2], &tempVertexTextureIndex[2],// &tempNormalIndex[2],
					&tempVertexIndex[3], &tempVertexTextureIndex[3]);// &tempNormalIndex[3]);

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
					&tempVertexIndex[0], &tempVertexTextureIndex[0],// &tempNormalIndex[0],
					&tempVertexIndex[1], &tempVertexTextureIndex[1],// &tempNormalIndex[1],
					&tempVertexIndex[2], &tempVertexTextureIndex[2]);// &tempNormalIndex[2]);

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


GLuint
Utility::loadTexture(const char* filename)
{

	glload::LoadTest loadTest = glload::LoadFunctions();

	try
	{
		std::auto_ptr<glimg::ImageSet> pImageSet(glimg::loaders::stb::LoadFromFile(filename));
		return glimg::CreateTexture(pImageSet.get(), 0);
	}
	catch(glimg::loaders::stb::StbLoaderException &e)
	{
		std::cerr << "Failed loading file";
	}
	catch(glimg::TextureGenerationException &e)
	{
		std::cerr << "Texture creation failed";
	}

	return 0;
}

void 
Utility::Rotate90(float partOf90,float & A,float & B)
{
	_rotate90(partOf90,A,B);
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