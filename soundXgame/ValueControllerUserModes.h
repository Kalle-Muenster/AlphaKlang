/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
|  Controlled<Value>::Usermodes:                          |
|  attacheble properties for the Controlled<Value> class. |
|  by: K. Muenster                                        |
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifndef _VALUECONTROLLER_USERMODES_
#define _VALUECONTROLLER_USERMODES_

#include "DataStructs.h" 

typedef const unsigned char cBYTE;
#define Circle (360 * 3.14159265358979323846 / 180)

//base-class... not instacible..
template<typename cType> class UserModeControl
{
protected:
	void* controller;
  public:
	cType *MIN,*MAX,*MOVE;

	virtual void Init(void)=0;
	virtual void InitFirst(cType*min,cType*max,cType*move,void *cntrlr)
	{
		controller = cntrlr;
		MIN = min;
		MAX = max;
		MOVE = move;
	};

	UserModeControl(void){ID = -1;}
	virtual ~UserModeControl(void)
	{
		delete MIN;
		delete MAX;
		delete MOVE;
		delete controller;
	}
	virtual cType checkVALUE(cType* pVALUE)=0;
	unsigned short ID;
};

 
//Inverts The Value.
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

//Clamp the Value to MIN/MAX if CLAMP=true;
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

//In- or (if INVERT) Decrement the Value by MOVE everytime it's getted. 
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

//Like a normal pingpong but following a sine-curve
//...usefull for LFO's in Audio or for Movement-Automation things elsewhere 
template<typename cType> class SineControlled
	: public MovingValue<cType>
{
private:
	cType HALBRANGE;
public:
	cType PINGPONG_VALUE;
	
	virtual void Init(void)
	{
		InvertController<cType>::Init();
		*MOVE = Circle/360;
		PINGPONG_VALUE = 0;
		CLAMP=false;
		INVERT=true;
	}

	virtual cType checkVALUE(cType* pVALUE)
	{
	   PINGPONG_VALUE += *MOVE;
	   PINGPONG_VALUE =  PINGPONG_VALUE>Circle? PINGPONG_VALUE-Circle : PINGPONG_VALUE;
	   HALBRANGE = (*MAX - *MIN)/2;
	   *pVALUE = (glm::sin(PINGPONG_VALUE)*HALBRANGE)+(-HALBRANGE - *MIN);
	   return  InvertController<cType>::checkVALUE(pVALUE);
	}
};

//take's a pointer to another value...
//when that changes, the Value "tries" to 
//follow it by In-or Decrementing by MOVE...
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
	virtual ~Follower(void)
	{
	 delete MIN,MAX,MOVE,pTARGET;
	}
};

//If it's Value is Set higher than MAX it will be MOVE
//if it's Set lower than MIN it will be -MOVE
//if it's set to somthing between MIN/MAX it will not change.
//..usefull for implementing functions using Threshold-Values... 
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
	virtual ~Trigger(void)
	{
	 delete MIN,MAX,MOVE,pTARGET;
	}
};

//It takes NUMBER_OF_PINS pointers to other values...
//and starts Moving the first one until it reaches the second's value.
//then it switches the pointers and continues,- now Moving the second one,
//until it reaches the third, then switches the pointer and goes on moving the third
//when reached the last PIN it switch's back to the first....
//...maybe usefull for somthing...
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

	virtual ~Jongleur(void)
	{
		delete MIN,MAX,MOVE,pTARGET;
		delete [] pPIN;
	}
};

//takes NUMBER_OF_PINS + one TARGET pointers.. calculates the pin's average value
//and scale's it by TARGET...
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

#endif