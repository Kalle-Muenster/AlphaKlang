#include "DataStructs.h"
#include "VoxelVector.h"
#include "Utility.h"

// Vector3 
//##################################################

const Vector3 _zeroVec3 = Vector3(0,0,0);

const Vector3*const
Vector3::Zero = &_zeroVec3;



Vector3::Vector3(void)
{
	 x = 0;
	 y = 0;
	 z = 0;
}
Vector3::Vector3(float X,float Y,float Z)
{
	 x = X;
	 y = Y;
	 z = Z;
}
Vector3::Vector3(glm::vec3 glVector)
{
	 x = glVector.x;
	 y = glVector.y;
	 z = glVector.z;
}
Vector3::Vector3(BASS_3DVECTOR from)
{
	x=from.x;
	y=from.y;
	z=from.z;
}

Vector3::operator BASS_3DVECTOR(void)
{
	return BASS_3DVECTOR(x,y,z);
}

Vector3::operator glm::vec3(void)
{
	return glm::vec3(x,y,z);
}


bool
Vector3::operator==(Vector3 other)
{
	return (x==other.x && y==other.y && z==other.z);
}

bool 
Vector3::operator==(float scalar)
{
	return (x==scalar && y==scalar && z==scalar);
}

bool
Vector3::operator!=(Vector3 other)
{
	return !(*this==other);
}

bool
Vector3::operator!=(float scalar)
{
	return (x!=scalar || y!=scalar || z!=scalar);
}

Vector3
Vector3::operator+(Vector3 adder)
{
	return Vector3(x + adder.x,y + adder.y,z + adder.z);
}
Vector3
Vector3::operator-(Vector3 subtractor)
{
	return Vector3(x - subtractor.x,y - subtractor.y,z - subtractor.z);
}
Vector3
Vector3::operator-(void)
{
	return Vector3(-x,-y,-z);
}
Vector3
Vector3::operator*(float scalar)
{
	return Vector3(x*scalar,y*scalar,z*scalar);
}
Vector3
Vector3::operator/(float scalar)
{
	return Vector3(x/scalar,y/scalar,z/scalar);
}
void
Vector3::operator+=(Vector3 other)
{
	x+=other.x;
	y+=other.y;
	z+=other.z;
}
void
Vector3::operator-=(Vector3 other)
{
	x-=other.x;
	y-=other.y;
	z-=other.z;
}

void
Vector3::operator*=(float scalar)
{
	x=x*scalar;
	y=y*scalar;
	z=z*scalar;
}

void
Vector3::operator/=(float scalar)
{
	x=x/scalar;
	y=y/scalar;
	z=z/scalar;
}

float
Vector3::dot(Vector3 other)
{
	return x*other.x+y*other.y+z*other.z;
}

Vector3
Vector3::cros(Vector3 other)
{
	other.x = y*other.z-z*other.y;
	other.y = z*other.x-x*other.z;
	other.z = x*other.y-y*other.x;
	return other;
}


bool
Vector3::operator<(Vector3 other)
{
	 return ((other.x - x) + (other.y - y) + (other.z - z)) < 0;
}

bool
Vector3::operator>(Vector3 other)
{
	return ((other.x - x) + (other.y - y) + (other.z - z)) > 0;
}

float 
Vector3::GetLength(void)
{
	return sqrtf(pow(x,2)+pow(y,2)+pow(z,2));
}

float
Vector3::GetPowerOfLength(void)
{
   return (pow(x,2)+pow(y,2)+pow(z,2));
}


float
Vector3::distance(Vector3 to)
{
	return (to - *this).GetLength();
}

Vector3
Vector3::direction(Vector3 to)
{
	return glm::normalize((glm::vec3)(to - *this));
}

Vector3
Vector3::normalized(void)
{
	return glm::normalize(glm::vec3(x,y,z));
}

GLfloat* 
Vector3::operator[](int index)
{
	return ((GLfloat*)&this[index]);
	//switch(index)
	//{
	//case 0:
	//	return x;
	//case 1:
	//	return y;
	//case 2:
	//	return z;
	//}
}



//Color and Sample
//####################################################
void
Data32::BaseInitor(void)
{
	data.u32 = 0;
}


Data32::Data32(void)
{
	BaseInitor();
}

data32*
Data32::GetUnion(void)
{
	return &data;
}

Data32::~Data32(void)
{
	
}




Sample::Sample(void)
{
	data.s16[0]=0;
	data.s16[1]=0;
	initor();
}

void
Sample::initor(void)
{
	L = &data.s16[0];
	R = &data.s16[1];
	Float32 = &data.f32;
}

Sample::Sample(short Left,short Right)
{
	initor();
	data.s16[0] = Left;
	data.s16[1] = Right;
	
}

Sample::Sample(float monoSample)
{
	initor();
	data.f32 = monoSample;
}

Sample::~Sample(void)
{
	delete L,R,Float32;
}

short
Sample::GetMono(void)
{
	return (data.s16[0]+data.s16[1])/2;
}

pcm
Sample::operator[] (int index)
{
	return (&data.s16[index]);
}



Data64::Data64(void)
{
	unionData32[0] = data32();
	unionData32[1] = data32();
}

//Texture Struct:
//############################################
void Texture::Loade(string fileName)
{
	sscanf(fileName,"_%ix%i.",&w,&h);
	Loade(fileName,w,h);
}

void Texture::Loade(string fileName,short Width,short Height,Format textureFormat)
{
	w = Width;
	h = Height;
	format=(int)textureFormat;
	ID = Utility::loadTexture(fileName);
}