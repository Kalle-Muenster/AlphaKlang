#include "projectMacros.h"
#include "projectGrafics.h"
#include "Utility.h"

Vector3::Vector3(void)
{
	x=y=z=0.0f;
}

Vector3::Vector3(float X,float Y,float Z)
{
	bassVector.x = x = X;
	bassVector.y = y = Y;
	bassVector.z = z = Z;
}
Vector3::Vector3(glm::vec3 glVector)
{
	bassVector.x = x = glVector.x;
	bassVector.y = y = glVector.y;
	bassVector.z = z = glVector.z;
}
Vector3::operator BASS_3DVECTOR()
{
	return BASS_3DVECTOR(this->x,this->y,this->z);
}

Vector3
Vector3::operator+(Vector3 adder)
{
	return Vector3(this->x + adder.x,this->y + adder.y,this->z + adder.z);
}

Vector3
Vector3::operator-(Vector3 subtractor)
{
	return Vector3(this->x - subtractor.x,this->y - subtractor.y,this->z - subtractor.z);
}

BASS_3DVECTOR*
Vector3::asBassVector(void)
{
	bassVector.x = this->x;
	bassVector.y = this->y;
	bassVector.z = this->z;
	return &bassVector;
}



u32_2s16_4b8::u32_2s16_4b8(void)
{
	this->u32 = 0;
}

u32_2s16_4b8::u32_2s16_4b8(int set)
{
	this->u32 = set;
}
//
//BitMap::BitMap(const POWERS_OF_TWO w,const POWERS_OF_TWO h)
//{
//	_bitmap.width=w;
//	sizeInByte = w*h*4;
//	Height(h);
//}
//
//BitMap::~BitMap(void)
//{
//	
//}
//
//unsigned
//BitMap::GetSizeInByte(void)
//{
//	return sizeInByte;
//}
//
//unsigned short
//BitMap::Width(POWERS_OF_TWO w)
//{
//	if(w)
//	{
//		for(int i=0;i<_bitmap.height;i++)
//			switch(w)
//			{
//			case _64:
//				_bitmap.data[i]=ScanLine_64();
//				break;
//			case _128:
//				_bitmap.data[i]=ScanLine_128();
//				break;
//			case _256:
//				_bitmap.data[i]=ScanLine_256();
//				break;
//			}
//
//		_bitmap.width=w;
//		sizeInByte = _bitmap.width*_bitmap.height*4;
//
//		for(int y = 0;y<_bitmap.height;y++)
//			for(int x=0;x<w;x++)
//				_bitmap.data[y].data[x]=u32_2s16_4b8(0);
//	}
//	return _bitmap.width;
//}
//
//void
//BitMap::Init(POWERS_OF_TWO w,POWERS_OF_TWO h)
//{
//	if(h)
//	{
//
//		switch(h)
//		{
//		case _64:
//			_bitmap = BitMap_64();
//			for(int i = 0;i<h;i++)
//				switch(w)
//			{
//			case _64:
//				_bitmap.data[i]=ScanLine_64();
//				break;
//			case _128:
//				_bitmap.data[i]=ScanLine_128();
//				break;
//			case _256:
//				_bitmap.data[i]=ScanLine_256();
//				break;
//			}
//			break;
//		case _128:
//			_bitmap = BitMap_128();
//			for(int i = 0;i<h;i++)
//					switch(w)
//			{
//			case _64:
//				_bitmap.data[i]=ScanLine_64();
//				break;
//			case _128:
//				_bitmap.data[i]=ScanLine_128();
//				break;
//			case _256:
//				_bitmap.data[i]=ScanLine_256();
//				break;
//			}
//			break;
//		case _256:
//			_bitmap = BitMap_256();
//			for(int i = 0;i<h;i++)
//					switch(w)
//			{
//			case _64:
//				_bitmap.data[i]=ScanLine_64();
//				break;
//			case _128:
//				_bitmap.data[i]=ScanLine_128();
//				break;
//			case _256:
//				_bitmap.data[i]=ScanLine_256();
//				break;
//			}
//			break;
//		}
//		_bitmap.height=h;
//		_bitmap.width=(POWERS_OF_TWO)w;
//		for(int y = 0;y<_bitmap.height;y++)
//			for(int x=0;x<w;x++)
//				_bitmap.data[y].data[x]=u32_2s16_4b8(0);
//	}
//	
//	return _bitmap.height;
//}
//
//Color
//BitMap::Pixel(unsigned short x,unsigned short y,int color)
//{
//	if(color != NULL)
//		_bitmap.data[y].data[x].s32 = color;
//	return _bitmap.data[y].data[x];
//}
//
//Color
//BitMap::Pixel(int index)
//{
//	return _bitmap.data[index/_bitmap.width].data[index%_bitmap.width];
//}
//
//void*
//BitMap::GetData(int offset,int sizeBytes)
//{
//	if(sizeBytes)
//		if(offset+sizeBytes>sizeInByte)
//			throw "index out of range error: in BitMap";
//	return &_bitmap.data[offset/_bitmap.width].data[offset%_bitmap.width];
//}
	

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

Color
Loader::Pixel(int index)
{
	if(index<_length)
		return (Color)&_Data[index];
	else
		throw "Nö...";
}

Color
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
	FILE* file = fopen(filename,"r");
//	fseek(file,0,SEEK_SET);
	fscanf(file,"%s\n",readBuffer);
	fscanf(file,"%s\n",readBuffer);
	fscanf(file,"%i %i\n",&W,&H);
	_w=W;
	_h=H;
	_length = _w*_h;
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
	fclose(file);
	return data;
}

void
Utility::loadObj(const char* filename,  std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals)
{
	

	std::vector<unsigned int> vertexIndices, vertexTextureIndices, normalIndices;

	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempVertexTextures;
	std::vector<glm::vec3> tempNormals;

	FILE* file = fopen(filename, "r");

	if(file == NULL) 
	{
		std::cerr << "File not found" << filename << std::endl;
		exit(1);
	}

	while(1) 
	{
		char line[32];

		int res = fscanf(file, "%s", line);

		if(res == -1) 
		{
			break;
		}

		if(strcmp(line, "v") == 0) 
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
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if(strcmp(line, "f") == 0) 
		{
			unsigned int tempVertexIndex[3], tempVertexTextureIndex[3],
				tempNormalIndex[3];

			fscanf(file, "%d/%d %d/%d %d/%d\n", 
				&tempVertexIndex[0], &tempVertexTextureIndex[0],
				&tempVertexIndex[1], &tempVertexTextureIndex[1],
				&tempVertexIndex[2], &tempVertexTextureIndex[2]
			);

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



