

#include "DataStructs.h"


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