#ifndef _PrimitivObject_h_
#define _PrimitivObject_h_


#include "Connectable.h"
#include "iprimitive.h"




class IPrimitivObject :
	public IObjection<IConXtor>
{
protected:
	bool	NoBackfaceCulling;
	PrimitivMesh pri;
public:
	IPrimitivObject(void);
	virtual ~IPrimitivObject(void);
	virtual void InitializeObject(void);
	virtual void draw(void);
	virtual Transform* getTransform(void);
	template<typename PT> void SetPrimitiv(void)
		{
			pri.shape = PT::shape;
			pri.VertsCount = PT::VertsCount;
			pri.vertsBufferID = PT::vertsBufferID;
			pri.paintBufferID = PT::paintBufferID;
			pri.normsBufferID = PT::normsBufferID;
		}
};


#endif
