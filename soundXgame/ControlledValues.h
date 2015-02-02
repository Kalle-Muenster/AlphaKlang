
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
|  Controlled<Value>:                    |
|  a Variable value which is controlled  |
|  by a simple controlling-mechanism     |
|  it provides:                          |
|                                        |
|        -Invert                         |
|        -Clamp                          |
|        -Compress                       |
|        -Expand                         |
|        -Moderate                       |
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

	unsigned char _mode;
	char userModesCount;
	UserModeControl<ctrlType> * UserMode;

	//the controlling-function
	virtual ctrlType checkValue(unsigned char mode)
	{
		switch(mode)
		{
		// OFF - value will be let how it is..
		case 0:	
			return *_Value;

		/* STILL VALUES */

		// INVERT: - the value will stay the same, but when it's requested it will be returned negated when GET!.
		case Controlled<ctrlType>::ControllMode::Invert:
			return mode==Invert? -(*_Value) : *_Value;

		// CLAMP: - the value will be clamp to MIN/MAX when it's being SET!... 
		case Controlled<ctrlType>::ControllMode::Clamp:
			*_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value;
			break;



		/* DYNAMICS: */

		// COMPRESS: - if the value's delta-movement becomes greater than MIN, it will be damped/Amplified by factor MAX (MAX from 0 to 1 will effect reduction, MAX greater 1 effect's amplification)... 
		case Controlled<ctrlType>::Compress:
			MOVE = *_Value = ( ((*_Value - MOVE) > MIN) || ((*_Value - MOVE) < -MIN) ) ? MOVE + ((*_Value - MOVE)*MAX) : *_Value ;
			break;

		// EXPAND: - if delta-movement will stay below MIN, it will	be effected by factor MAX...
		case Controlled<ctrlType>::Expand:
			MOVE = *_Value = ( ((*_Value - MOVE) < MIN) || ((*_Value - MOVE) > -MIN) ) ? MOVE + ((*_Value - MOVE)*MAX) : *_Value ;
			break;

		// MODERATE: - When delta is greater MIN or lower -MIN, it will be Compressed by 1/MAX. - when delta is between MIN and -MIN, it will be Expanded by MAX...
		case Controlled<ctrlType>::Moderate:
			MOVE = *_Value = ( ((*_Value - MOVE) > MIN) || ((*_Value - MOVE) < -MIN) ) ? MOVE + ((*_Value - MOVE)*(1.0f/MAX)) : MOVE + ((*_Value - MOVE)*MAX) ;
			break;


		/* MOTIVATORS: */

		// CYCLE:  - every time the value is checked, it will move by adding MOVE to it... if it get's greater MAX, it will reset to MIN...
		case Controlled<ctrlType>::ControllMode::Cycle:
			*_Value = *_Value<MIN?MAX-(MIN- *_Value):*_Value>MAX?MIN+(*_Value - MAX): (*_Value+MOVE); 
			break;

		// PINGPONG: - every time it's checked, will move by MOVE. when reaches MAX or MIN, MOVE changes it's sign.	so the movement will change direction.
		case Controlled<ctrlType>::ControllMode::PingPong:
			*_Value = 											(*_Value < MIN) 
																		? 
					  ( MIN + ( MIN - *_Value ) ) + ( MOVE = (-MOVE) )  : 			(*_Value <= MAX) 
																							 ? 
																		   (*_Value + MOVE ) : ( MAX -(*_Value - MAX) ) + ( MOVE = (-MOVE) )   
																																					
					  ; 
			break;

		// USERMODE - implement whatever you able thinking about and attach it to the controller...
		default:
			if(mode == UserMode->ID)
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
	//every time it's ret or written. ...useful for Clamping or Inverting
	//If set to "false", it's Value won't be checked and won't be updated
	//unless you call "Check()" on it manually. ...usefull for MovingValues
	//like pingpong or cycle. it than can be updated once per frame in an
	//update-loop for example, to let its Value stay the same during the
	//whole frame-cycle.
	bool ControllerActive;
	ctrlType MIN,MAX,MOVE;

	//The selectable modes...
	enum ControllMode : unsigned char
	{NONE=0,Invert=1,Clamp=2,Compress=3,Expand=4,Moderate=5,Cycle=6,PingPong=7};
	
	//constructor...
	Controlled(void)
	{
		_Value = new ctrlType();
		userModesCount=7;
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
		*_Value = MIN = val;
	}
	void SetMAX(ctrlType val)
	{
		*_Value = MAX = val;
	}
	void SetMOVE(ctrlType val)
	{
		MOVE = val;
		ControllerActive = true;
		checkValue(_mode);
	}

	

	//returns the UserMode-Class instance if there's one attached...
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

	//set the controller's mode to one of the build-in modes
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

	//resets the pointer to the value an deletes it. and takes the given pointer as new Controlling-Target
	virtual void SetVariable(ctrlType *var)
	{
		delete _Value;
		_Value = var;
	}

	//read the value
	virtual operator ctrlType(void)
	{
		return GetValue();
	}

	//write the value
	virtual ctrlType operator =(ctrlType set)
	{
		return this->SetValue(set);
	}


};


// A Vector of 3 Controlled<float>s
class ControlledVector3
{
public:
	Controlled<float> x;
	Controlled<float> y;
	Controlled<float> z;

	ControlledVector3(void);
	virtual ~ControlledVector3(void);
	void SetMode(Controlled<float>::ControllMode);
	void SetMIN(Vector3 min);
	void SetMAX(Vector3 max);
	void SetMOVE(Vector3 move);
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


#endif