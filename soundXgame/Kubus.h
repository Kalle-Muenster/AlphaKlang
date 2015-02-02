#ifndef _Kubus_h_
#define _Kubus_h_

#include "SceneGraph.h"
#include "connectable.h"
#include "IMeshObject.h"



class Kubus :
	public IObjection<IConXtor>,
	public IPrimitiv<ICubic>
{
protected:
	bool	NoBackfaceCulling;

public:
	Kubus(void);
	virtual ~Kubus(void);
	virtual void InitializeObject(void);
	virtual void draw(void);
	virtual Transform* getTransform(void);
	template<typename PT> void SetPrimitiv(void)
		{
			this->primitiv->SetPrimitiv<PT>();
			//this->primitiv = PT();
			//this->primitiv->component = this;
		}
	IlPrimitivo* Prim;
};


class PrimitivObject : 
	public IObjection<IConXtor>,
	public IPrimitiv<IBall>
{
protected:
	bool	NoBackfaceCulling;

public:
	PrimitivObject(void);
	virtual ~PrimitivObject(void);
	virtual void InitializeObject(void);
	virtual void draw(void);
	virtual Transform* getTransform(void);

};


#endif
