#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//#include "Connectable.h"
#include "DataStructs.h"

 


typedef const unsigned char cBYTE;

 template<typename cType> class UserModeControl
{
protected:
	void* controller;
  public:
	cType *MIN,*MAX,*MOVE;

	virtual void Init(void){};
	virtual void InitFirst(cType*min,cType*max,cType*move,void *cntrlr)
	{
		controller = cntrlr;
		MIN = min;
		MAX = max;
		MOVE = move;
	};

	UserModeControl(void){ID = -1;}

	virtual cType checkVALUE(cType* pVALUE)=0;
	unsigned short ID;
};

 

template<typename cType> class InvertController 
	: public UserModeControl<cType>
{
public:
	virtual void Init(void)
	{
		INVERT = false;
	}

	bool INVERT;
	virtual cType checkVALUE(cType* pVALUE)
	{
		return INVERT? -(*pVALUE) : *pVALUE;
	}
};

template<typename cType> class ClambController 
	: public InvertController<cType>
{
public:
		virtual void Init(void)
		{
			InvertController<cType>::Init();
			*MIN = -1;
			*MAX = 1;
			CLAMP = true;
		}
	bool CLAMP;
	virtual cType checkVALUE(cType* pVALUE)
	{
		  *pVALUE = *pVALUE<*MIN?*MIN:*pVALUE>*MAX?*MAX:*pVALUE;
		  return InvertController<cType>::checkVALUE(pVALUE);
	}
};



template<typename cType> class MovingValue 
	: public ClambController<cType>
{
public:
		virtual void Init(void)
		{
			ClambController<cType>::Init();
			CLAMP = false;
			*MOVE = 0.001f;
		}


	virtual cType checkVALUE(cType* pVALUE)
	{
		*pVALUE = *pVALUE+ INVERT? -*MOVE : *MOVE;
		return ClambController<cType>::checkVALUE(pVALUE);
	}
};

const float Circle = 360 * 3.14159265358979323846 / 180;

template<typename cType> class SineControlled
	: public MovingValue<cType>
{
protected: 
	
public:
	float PINGPONG_VALUE,HALBRANGE,SHIFT;
	
	virtual void Init(void)
	{
		InvertController<cType>::Init();
		*MOVE = Circle/360;
		PINGPONG_VALUE = 0;
		HALBRANGE = (*MAX - *MIN)/2;
		SHIFT = (-HALBRANGE - *MIN);
		CLAMP=false;
		INVERT=false;
	}

	virtual cType checkVALUE(cType* pVALUE)
	{
	   PINGPONG_VALUE += *MOVE;
	   PINGPONG_VALUE =  PINGPONG_VALUE>Circle? PINGPONG_VALUE-Circle : PINGPONG_VALUE;
	   *pVALUE = (glm::sin(PINGPONG_VALUE)*HALBRANGE)+SHIFT;
	   return  InvertController<cType>::checkVALUE(pVALUE);
	}
};

template<typename cType> class Follower 
	: public MovingValue<cType>
{
public:
		virtual void Init(void)
		{
			MovingValue<cType>::Init();
		}

    cType* pTARGET;
	virtual cType checkVALUE(cType* pVALUE)
	{
		INVERT = *pTARGET < *pVALUE ? true : false ;	
		return MovingValue<cType>::checkVALUE(pVALUE);
	}
};



template<typename cType> class Trigger 
	: public MovingValue<cType>
{
public:
		virtual void Init(void)
		{
			MovingValue<cType>::Init();
			*MIN = -10;
			*MAX = 10;
			*MOVE = 1.f;
		}

    cType* pTARGET;
	virtual cType checkVALUE(cType* pVALUE)
	{
		*pVALUE = *pTARGET>=*MAX? *MOVE : *pTARGET<=*MIN? -*MOVE : *pVALUE;
		return InvertController<cType>::checkVALUE(pVALUE);
	}
};

template<typename cType,cBYTE NUMBER_OF_PINS> class Jongleur 
	: public Follower<cType>
{
protected:
		virtual void Init(void)
		{
			Follower<cType>::Init();
			*MIN = -1;
			*MAX = 1;
			*MOVE = 0.001f;
			currentPIN = 0;
		}
public:
	cType* pPIN[NUMBER_OF_PINS];
	char currentPIN;
	virtual cType checkVALUE(cType* pVALUE)
	{
		int lastpin=currentPIN;
		pTARGET = pPIN[currentPIN];
		*pVALUE = *pVALUE < *pTARGET ? *pVALUE + *MOVE : *(pVALUE = pPIN[currentPIN++])+*MOVE ;
		if(currentPIN==NUMBER_OF_PINS)
			currentPIN=0;
		if(lastpin!=currentPIN)
			pPIN[lastpin]=pTARGET;
		 *pVALUE = *pTARGET;
		 return ClambController<cType>::checkVALUE(pVALUE);
	}
};

template<typename cType,cBYTE NUMBER_OF_PINS> class Calculator 
	: public Jongleur<cType,NUMBER_OF_PINS>
{
protected:
	virtual void Init(void)
	{
		Jongleur<cType,NUMBER_OF_PINS>::Init();
		CLAMP=INVERT=false;
	}
public:
  	virtual cType checkVALUE(cType* pVALUE)
	{
		  *pVALUE = (((*pPIN[0]+ *pPIN[1]+ *pPIN[2])/3) * (*pTARGET));
		  return ClambController<cType>::checkVALUE(pVALUE);
	}
};



template<typename ctrlType> class Controlled
{	  

protected:
	ctrlType MIN,MAX,MOVE;
	ctrlType* _Value;
	
	bool CheckAtGet;

	short _mode;
	short userModesCount;
	UserModeControl<ctrlType> * UserMode;

	virtual ctrlType checkValue(unsigned short mode)
	{
		switch(mode)
		{
		case 0:
			return *_Value;
		case (unsigned short)Controlled<ctrlType>::ControllMode::Invert:
			return mode==Invert? -(*_Value) : *_Value;
		case( unsigned short)Controlled<ctrlType>::ControllMode::Clamp:
			*_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value;
			break;
		case ( unsigned short)Controlled<ctrlType>::ControllMode::Cycle:
			*_Value = *_Value<MIN?MAX-(MIN- *_Value):*_Value>MAX?MIN+(*_Value - MAX): (*_Value+MOVE); 
			break;
		case ( unsigned short)Controlled<ctrlType>::ControllMode::PingPong:
			*_Value = (*_Value<MIN)? ( ( MIN + ( MIN - *_Value ) ) + ( MOVE = (-MOVE) ) ) : (*_Value>MAX)? ( ( MAX -(*_Value - MAX) ) + ( MOVE = (-MOVE) ) ) : (*_Value+MOVE) ; 
			break;
		default:
			if((userModesCount>4)&&(UserMode->ID==mode)&&(UserMode!=NULL))
				return UserMode->checkVALUE(_Value);
		}	
		return *_Value;
	}
public:
	bool ControllerActive;
	enum ControllMode {NONE=0,Invert=1,Clamp=2,Cycle=3,PingPong=4};	
	void SetMIN(ctrlType val)
	{
		MIN = val;
	}
	void SetMAX(ctrlType val)
	{
		MAX = val;
	}
	void SetMOVE(ctrlType val)
	{
		MOVE = val;
	}
	template<typename Umode> Umode* GetUserMode(void)
	{
		return (Umode*)UserMode;
	}
	template<typename Umode> void SetUserMode(ctrlType initial)
	{

		if(UserMode)
		{	delete UserMode; 
			userModesCount--; }

		UserMode = new Umode();
		UserMode->InitFirst(&MIN,&MAX,&MOVE,this);
		UserMode->Init();

		*_Value = initial;

		UserMode->ID = ++userModesCount;
		Mode(UserMode);

	}
	template<typename Umode> void SetUserMode(ctrlType min,ctrlType max,ctrlType initial,ctrlType move)
	{
		MIN=min;
		MAX=max;
		MOVE=move;

		if(UserMode)
		{	delete UserMode; 
			userModesCount--; }

		UserMode = new Umode();
		UserMode->InitFirst(&MIN,&MAX,&MOVE,this);
		UserMode->Init();

		*_Value = initial;

		UserMode->ID = ++userModesCount;
		Mode(UserMode);

	}
	virtual ctrlType SetValue(ctrlType setter)
	{
		*_Value = setter;
		return checkValue(Mode());
	}
	virtual ctrlType GetValue(void)
	{
		return checkValue(Mode());
	}
	Controlled(void)
	{
		this->_Value = new ctrlType();
		userModesCount=4;
		ControllerActive = false;
		CheckAtGet = true;
		UserMode = NULL;
	}

	virtual ~Controlled(void)
	{
		delete _Value;
	}



	//virtual bool IsControlled(BOOL active=3)
	//{
	//	if(active<2)
	//		ControllerActive=active;
	//	
	//	return ControllerActive;
	//}

	virtual ControllMode Mode(ControllMode mode=ControllMode::NONE)
	{
		if(mode!=ControllMode::NONE)
		{	_mode=mode;
			if(mode==ControllMode::Clamp) 
				CheckAtGet = false; 
			else if(mode==ControllMode::Invert) 
				CheckAtGet = false;
			else 
				CheckAtGet = true;
		}
		return (ControllMode)_mode;
	}

	virtual ControllMode Mode(UserModeControl<ctrlType>* mode)
	{

			if(_mode!=mode->ID)
			{
				UserMode = mode;
				_mode = UserMode->ID;
				CheckAtGet=true;
			}			 		


		return (ControllMode)_mode;
	}

	virtual Controlled* SetUp(ctrlType min,ctrlType max,ctrlType initial,ctrlType move,ControllMode mode)
	{
		*_Value = initial;
		MAX=max;
		MIN=min;
		MOVE=move;
		Mode(mode);
		return this;
	}

	virtual Controlled* SetUp(ctrlType min,ctrlType max,ctrlType initial,ctrlType move,UserModeControl<ctrlType> *mode)
	{
		SetUp(min,max,initial,move,ControllMode::Clamp);
		*_Value = initial;
		if(mode->ID < 0)
			mode->ID = ++userModesCount;
		Mode(mode);

		return this;
	}

	virtual void SetVariable(ctrlType & var)
	{
		delete _Value;
		_Value = &var;
	}

	//getter...
	virtual operator ctrlType(void)
	{
		return CheckAtGet? GetValue() : *_Value;
	}

	//setter
	ctrlType operator =(ctrlType set)
	{
		return this->SetValue(set);
	}


};



class ControlledFloat : public Controlled<float>
{
protected:
	virtual float checkValue(unsigned short mode);
	float MIN,MAX,MOVE;
public:
	ControlledFloat(void);
	virtual ~ControlledFloat(void);
	virtual Controlled<float>* SetUp(Controlled<float>::ControllMode mode,float initial);
	virtual Controlled<float>* SetUp(Controlled<float>::ControllMode mode,float min,float max,float move,float initial);
	float operator=(float setter);
};


struct IDimension
{

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	bool DimensionDirty;				// true if this Dimensions have Changed and the Directions need to be Updated..
			// Updates the Dimensions...
};

struct IDirection
{

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	bool DirectionsDirty;				// true if the Directions have unUpdated changes...
	
};

struct IPhysic
{
	Vector3 movement;			// movement is a relative position and shows the difference to the last frame -> it's the Object's moving direction * speed
	ControlledFloat speed;		// moving-speed
	float depth;				// amount of matter per qm... 1 means 1Kg per 1m^3
	float thickness;			// the thicker the harder... 1 means indistructable, 0 means cookie

	ControlledFloat mass;			// the Weight of an object: depth*scale;

	bool PhysicsDirty;			// physicaly dirty... 

};

class ITransform 
{
public:
IDimension dimension;
IDirection direction;
IPhysic	   physics;
								ITransform(void);
virtual							~ITransform(void);
bool							IsDirty(void);
void							SetClean(void); 
bool							CleanDimensions(void);
bool							CleanDirections(void);		// Get's the dirt off...
bool							SetPhysicClean(void);	// cleanUp...
const static ITransform* const	Zero;
};

#endif