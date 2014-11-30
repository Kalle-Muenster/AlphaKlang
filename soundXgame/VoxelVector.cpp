#include "VoxelVector.h"



#undef Rectangle; 

//Static zero instanses for initializing..
VectorI _VectorIZero = VectorI();
const VectorI* const VectorI::Zero = &_VectorIZero;
VectorF _VectorFZero = VectorF();
const VectorF* const VectorF::Zero = &_VectorFZero;
Vecti _VectiZero = Vecti();
const Vecti* const Vecti::Zero = &_VectiZero;



ProjectMappe::Rectangle _RectangleZero = ProjectMappe::Rectangle();
const ProjectMappe::Rectangle *const ProjectMappe::Rectangle::Zero = &_RectangleZero;


void
VoxtructsInitiator::initiateVoxtructs(void)
{
	_VectorIZero.Yps = _VectorIZero.iX = 0;
	_VectorFZero.x = _VectorFZero.y = 0;
	_VectiZero.yps = _VectiZero.ix = 0;
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

VectorPF
VectorPF::operator *(float s)
	{
		VectorPF newVector;
		*newVector.x = *x *s;
		*newVector.y = *y *s;
		return newVector;
	}

VectorPF
VectorPF::operator /(float s)
	{
		VectorPF newVector;
		*newVector.x=*x/s;
		*newVector.y=*y/s;
		return newVector;
	}


ProjectMappe::Rectangle::Rectangle(void)
{
	Position = *VectorF::Zero;
	HalbSize = *VectorF::Zero;
}

VectorF
ProjectMappe::Rectangle::position(void)
{
	return Position;
}

VectorF
ProjectMappe::Rectangle::position(float x,float y)
{
	Position.x=x;
	Position.y=y;
	return Position;
}

VectorF
ProjectMappe::Rectangle::position(VectorF xy)
{
	return Position=xy;
}

VectorF
ProjectMappe::Rectangle::size(void)
{
	return HalbSize*2;;
}

VectorF
ProjectMappe::Rectangle::size(float x,float y)
{
	HalbSize.x = x/2.0f;
	HalbSize.y = y/2.0f;
	return size();
}

VectorF
ProjectMappe::Rectangle::size(VectorF wh)
{
	HalbSize = wh/2.0f;
	return size();
}

VectorF
ProjectMappe::Rectangle::center(void)
{
	return Position + HalbSize;
}

