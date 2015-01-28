/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
|  Controlled<Value>:                    |
|  a Variable value which is controlled  |
|  by a simple controlling-mechanism     |
|  it provides:                          |
|                                        |
|        -Invert                         |
|        -Clamp                          |
|        -Cycle                          |
|        -PingPong                       |
|                                        |
|  it also support attaching UserDefined |
|  Mode-Properties which can be derived  |
|  from the UserMode<ValueType> class... |
|                                        |
|  by: Kalle Muenster                    |
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef _VALUE_CONTROLLER_
#define _VALUE_CONTROLLER_


#include "ValueControllerUserModes.h"



template<typename ctrlType> class Controlled
{	  
protected:
	ctrlType* _Value;  //pointer to the value which is controlled; 
	
	
	bool CheckAtGet;

	short _mode;
	short userModesCount;
	UserModeControl<ctrlType> * UserMode;

	//the controlling-function
	virtual ctrlType checkValue(unsigned short mode)
	{
		switch(mode)
		{
		case 0:
			return *_Value;
		case (unsigned short)Controlled<ctrlType>::ControllMode::Invert:
			return mode==Invert? -(*_Value) : *_Value;
		case (unsigned short)Controlled<ctrlType>::ControllMode::Clamp:
			*_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value;
			break;
		case (unsigned short)Controlled<ctrlType>::ControllMode::Cycle:
			*_Value = *_Value<MIN?MAX-(MIN- *_Value):*_Value>MAX?MIN+(*_Value - MAX): (*_Value+MOVE); 
			break;
		case (unsigned short)Controlled<ctrlType>::ControllMode::PingPong:
			*_Value = (*_Value < MIN)? ( MIN + ( MIN - *_Value ) ) + ( MOVE = (-MOVE) ) 
								     : (*_Value > MAX)? ( MAX -(*_Value - MAX) ) + ( MOVE = (-MOVE) ) 
												      : (*_Value + MOVE); 
			break;
		default:
			if((userModesCount>4)&&(UserMode!=NULL)&&(UserMode->ID==mode))
				return UserMode->checkVALUE(_Value);
		}	
		return *_Value;
	}

	virtual ctrlType SetValue(ctrlType setter)
	{
		*_Value = setter;
		return (ControllerActive && !CheckAtGet)? checkValue(Mode()) : setter;
	}

	virtual ctrlType GetValue(void)
	{
		return (ControllerActive && CheckAtGet)? checkValue(Mode()) 
											   : *_Value;
	}

public:
	//If set "true", the Value will be checked and updated
	//every time it's ret or written. ...usefull for Clamping or Inverting
	//If set to "false", it's Value won't be checked and won't be updated
	//unless you call "Check()" on it manually. ...usefull for MovingValues
	//like pingpong or cycle. it than can be updated once per frame in an
	//update-loop for example, to let its Value stay the same during the
	//whole frame-cycle.
	bool ControllerActive;
	ctrlType MIN,MAX,MOVE;

	//The selectable modes...
	enum ControllMode {NONE=0,Invert=1,Clamp=2,Cycle=3,PingPong=4};	
	
	//constructor...
	Controlled(void)
	{
		_Value = new ctrlType();
		userModesCount=4;
		ControllerActive = false;
		CheckAtGet = true;
		UserMode = NULL;
	}

	virtual ~Controlled(void)
	{
		if(UserMode!=NULL)
			delete UserMode;
		delete _Value;
	}

	
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

	

	//returns the UserMode instance if there's attached one..
	template<typename Umode> Umode* GetUserMode(void)
	{
		return (Umode*)UserMode;
	}

	//template-function for setting the mode to an Usermode-class-type. 
	//it will instantiate and attach the UserMode to the controller...
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

	//template-function for setting the mode to an Usermode-class-type. 
	//it will instantiate, initialize and attach the UserMode to the controller...
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

	//Checks the Value and return it.  (even if Controller is deactivated)
	//... use it to perform updating manually and not to be called automatically,
	//every time the value is "getted" when in a "MovingValue-Mode"...
	ctrlType Check(void)
	{
		ControllerActive=false;
		return checkValue(Mode());
	}

	//set the controller-Mode
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

	//set a UserControlMode-instance as controller-mode
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

	//Initializes a Controlled<ValueType> with MIN,MAX,Initial,MOVE and Mode... 
	virtual Controlled* SetUp(ctrlType min,ctrlType max,ctrlType initial,ctrlType move,ControllMode mode)
	{
		*_Value = initial;
		MAX=max;
		MIN=min;
		MOVE=move;
		Mode(mode);
		return this;
	}

    //Initializes a Controlled<ValueType> with MIN,MAX,Initial,MOVE and set's as Mode a given UserMode-Instance 
	virtual Controlled* SetUp(ctrlType min,ctrlType max,ctrlType initial,ctrlType move,UserModeControl<ctrlType> *mode)
	{
		SetUp(min,max,initial,move,ControllMode::Clamp);
		*_Value = initial;
		if(mode->ID < 0)
			mode->ID = ++userModesCount;
		Mode(mode);

		return this;
	}

	//resets the pointer to another value by the given pointer... deletes the old one
	virtual void SetVariable(ctrlType *var)
	{
		delete _Value;
		_Value = var;
	}

	//getter...
	virtual operator ctrlType(void)
	{
		return GetValue();
	}

	//setter
	virtual ctrlType operator =(ctrlType set)
	{
		return this->SetValue(set);
	}


};


//A Vector3 of Controlled<float>s
class ControlledVector3
{
public:
	Controlled<float> x;
	Controlled<float> y;
	Controlled<float> z;

	ControlledVector3(void);
	virtual ~ControlledVector3(void);
	void SetMode(Controlled<float>::ControllMode);
	void ControllersActive(bool);
	Vector3 Check(void);
	template<typename usermode> void SetUserMode(void)
									 {
									 	x.SetUserMode<usermode>(x);
									 	y.SetUserMode<usermode>(y);
									 	z.SetUserMode<usermode>(z);
									 }
	virtual operator Vector3(void);
	virtual Vector3 operator =(Vector3 setter);
};

//überflüssig, kommt weg...
class ControlledFloat : public Controlled<float>
{
protected:
	virtual float checkValue(unsigned short mode);
public:
	ControlledFloat(void);
	virtual ~ControlledFloat(void);
	virtual ControlledFloat* SetUp(Controlled<float>::ControllMode mode,float initial);
	virtual ControlledFloat* SetUp(float min,float max,float move,float initial,Controlled<float>::ControllMode mode);
};

#endif