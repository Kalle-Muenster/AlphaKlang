#include "DataStructs.h"

// Vector3 
//##################################################
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
Vector3::operator!=(Vector3 other)
{
	return !(*this==other);
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

BASS_3DVECTOR
Vector3::asBassVector(void)
{
	return BASS_3DVECTOR(x,y,z);
}

float 
Vector3::GetLength(void)
{
	return sqrtf(pow(x,2)+pow(y,2)+pow(z,2));
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








//Color and Sample
//####################################################
void
Data32::BaseInitor(void)
{
	data = data32();
	data.u32 = 0;
}


Data32::Data32(void)
{
	BaseInitor();
}

data32
Data32::GetUnion(void)
{
	return data;
}

Data32::~Data32(void)
{
	
}

Color::Color(void)
{
	BaseInitor();
}

void
Color::initor(void)
{
	BaseInitor();
	R = &data.byte[1];
	G = &data.byte[2];
	B = &data.byte[3];
	A = &data.byte[0];
	uInt = &data.u32;
}

Color::Color(unsigned uint)
{
	initor();
	data.u32 = uint;
}

Color::Color(int r,int g, int b, int a)
{
	initor();
	data.byte[0] = a;
	data.byte[1] = r;
	data.byte[2] = g;
	data.byte[3] = b;
}

chan
Color::operator[] (int index)
{
	return (&this->data.byte[index]);
}

Color::~Color(void)
{
	//delete R;
	//delete G;
	//delete B;
	//delete A;
	//delete uInt;
}


Sample::Sample(void)
{
	initor();
}

void
Sample::initor(void)
{
	BaseInitor();
	data.u32 = 0;
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

