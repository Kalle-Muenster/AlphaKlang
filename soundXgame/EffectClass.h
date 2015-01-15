#ifndef _EFFECT_CLASS_
#define _EFFECT_CLASS_

#include "projectGrafics.h"
#include "DataStructs.h"

class EffectClass
{
protected:

	bool enabled;
	bool bufferOK;
	float parameter[3];
	data32* screenBufferData;

public:

	GLuint			frameBuffer;
	GLuint			texColorBuffer;
	GLuint			rboDepthStencil;

					EffectClass(void);
	virtual			~EffectClass(void);
	void			Enable(void);
	void			Disable(void);
	void			SetParameter(int,float);
	void			AplyFXOnFramebuffer(void);
	virtual void	FX_function(float*)=0;
};

class BlurFX : public EffectClass
{
private:
	float blurSizeH;
	float blurSizeV;
	int	  range;

public: 
	virtual void FX_function(float*);
	data32 blurXY(Vecti);
};
#endif
