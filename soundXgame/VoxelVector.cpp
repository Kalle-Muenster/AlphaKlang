#include "VoxelVector.h"



#undef Rectangle; 

//Static zero instanses for initializing..
VectorI _VectorIZero = VectorI();
const VectorI* const VectorI::Zero = &_VectorIZero;
VectorF _VectorFZero = VectorF();
const VectorF* const VectorF::Zero = &_VectorFZero;
Vecti _VectiZero = Vecti();
const Vecti* const Vecti::Zero = &_VectiZero;
VectorPF _VectorPFzero = VectorPF();
const VectorPF* const VectorPF::Zero = &_VectorPFzero;


ProjectMappe::Rectangle _RectangleZero = ProjectMappe::Rectangle();
const ProjectMappe::Rectangle *const ProjectMappe::Rectangle::Zero = &_RectangleZero;


void
VoxtructsInitiator::initiateVoxtructs(void)
{
	_VectorIZero.Yps = _VectorIZero.iX = 0;
	_VectorFZero.x = _VectorFZero.y = 0;
	_VectiZero.yps = _VectiZero.ix = 0;
	_VectorPFzero.x = 0;
	_VectorPFzero.y = 0;
	//_RectangleZero.SetPosition(*VectorF::Zero);
	//_RectangleZero.SetSize(*VectorF::Zero);
}



VectorI
VectorI::operator  *(float s)
	{
		VectorI neu = *VectorI::Zero;
		neu.iX = iX*=s;
		neu.Yps = Yps*=s;
		return neu;
	}

Vecti
Vecti::operator  *(float s)
	{
		Vecti neu = *Vecti::Zero;
		neu.ix = ix*=s;
		neu.yps = yps*=s;
		return neu;
	}



VectorF::operator VectorI()
	{
		VectorI newVector = *VectorI::Zero;
		newVector.iX=x;
		newVector.Yps=y;
		return newVector;
	}

VectorF
VectorF::operator +(VectorF other)
	{
		other.x+=x;
		other.y+=y;
		return other;
	}

VectorF
VectorF::operator-(VectorF subtractor)
	{
		subtractor.x=x-subtractor.x;
		subtractor.y=y-subtractor.y;
		return subtractor;
	}

VectorF
VectorF::operator *(float s)
	{
		VectorF newVector=*VectorF::Zero;
		newVector.x=x*s;
		newVector.y=y*s;
		return newVector;
	}

VectorF
VectorF::operator /(float s)
	{
		VectorF newVector=*VectorF::Zero;
		newVector.x=x/s;
		newVector.y=y/s;
		return newVector;
	}

bool
	VectorF::operator==(VectorF other)
	{
		return (x==other.x && y==other.y);
	}

bool
	VectorF::operator!=(VectorF other)
	{
		return (x!=other.x || y!=other.y);
	}

bool
	VectorF::operator<(VectorF other)
	{
		return x<other.x && y<other.y;
	}

bool
	VectorF::operator>(VectorF other)
	{
		return x>other.x && y>other.y;
	}

VectorF
	VectorF::cros(VectorF other)
{
	other.x = y*other.x-x*other.y;
	other.y = x*other.y-y*other.x;
	return other;
}

VectorPF::operator VectorI()
	{
		VectorI newVector;
		newVector.iX = *x;
		newVector.Yps = *y;
		return newVector;
	}

VectorPF::operator VectorF()
	{
		VectorF newVector;
		newVector.x = *x;
		newVector.y = *y;
		return newVector;
	}

void
VectorPF::operator =(VectorF setter)
{
	*x = setter.x;
	*y = setter.y;
}

VectorF
VectorPF::operator *(float s)
	{
		VectorF newVector;
		newVector.x= *x *s;
		newVector.y= *y *s;
		return newVector;
	}

VectorF
VectorPF::operator /(float s)
	{
		VectorF newVector;
		newVector.x=*x/s;
		newVector.y=*y/s;
		return newVector;
	}

VectorF
VectorPF::operator +(VectorPF vec)
{
	VectorF neuVector;
	neuVector.x = *x + *vec.x;
	neuVector.y = *y + *vec.y;
	return neuVector;
}

VectorF
VectorPF::operator -(VectorPF vec)
{
	VectorF neuVector;
	neuVector.x = *x - *vec.x;
	neuVector.y = *y - *vec.y;
	return neuVector;
}

ProjectMappe::Rectangle::Rectangle(void)
{
	float halbX,halbY,centerX,centerY;
	Center.x = &centerX;
	Center.y = &centerY;
	HalbSize.x = &halbX;
	HalbSize.y = &halbY;
	halbX=halbY=centerX=centerY=0;
}

ProjectMappe::Rectangle::Rectangle(float pX,float pY,float width,float height)
{
	float hW,hH,cX,cY;
	hW = width/2;
	hH = height/2;
	cX = pX+hW;
	cY = pY+hH;
	HalbSize.x=&hW;
	HalbSize.y=&hH;
	Center.x=&cX;
	Center.y=&cY;
}

ProjectMappe::Rectangle::Rectangle(float *centerX,float *centerY,float *halbWidth,float *halbHeight)
{
	HalbSize.x = halbWidth;
	HalbSize.y = halbHeight;
	Center.x = centerX;
	Center.y = centerY;
}

VectorF
ProjectMappe::Rectangle::GetPosition(void)
{
	return Center-HalbSize;
}

void
ProjectMappe::Rectangle::SetPosition(float x,float y)
{
	*Center.x = *HalbSize.x + x;
	*Center.y = *HalbSize.y + y;
}

void
ProjectMappe::Rectangle::SetPosition(VectorF xy)
{
	SetPosition(xy.x,xy.y);
}

VectorF
ProjectMappe::Rectangle::GetSize(void)
{
	return (VectorF)HalbSize * 2.f;
}

VectorF
ProjectMappe::Rectangle::GetHalbSize(void)
{
	return HalbSize;
}

void
ProjectMappe::Rectangle::SetSize(float W,float H)
{
	*HalbSize.x = (W/2.f);
	*HalbSize.y = (H/2.f);
}

void
ProjectMappe::Rectangle::SetSize(VectorF wh)
{
	SetSize(wh.x,wh.y);
}

VectorF
ProjectMappe::Rectangle::GetCenter(void)
{
	return Center;
}

bool 
ProjectMappe::Rectangle::Containes(VectorF point)
{
	bool contains =	(point < Center+HalbSize) && (point > Center-HalbSize);
	return contains;
}