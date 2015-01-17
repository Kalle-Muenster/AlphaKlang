#include "EffectClass.h"

#include "projectMacros.h"

EffectClass::EffectClass(void)
{
	bufferOK = false;
//	screenBufferData = SCENE->GetFXbufferData();
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, screenBufferData);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);


	//vieleicht nich gebraucht...
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREENWIDTH, SCREENHEIGHT);
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil );

	//check if creation was succsessfull
	if(glCheckFramebufferStatus(frameBuffer)==GL_FRAMEBUFFER_COMPLETE)
	  bufferOK = true;
}


EffectClass::~EffectClass(void)
{
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &rboDepthStencil);
}

void
EffectClass::Enable(void)
{
	if(enabled)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	enabled = true;
}

void
EffectClass::Disable(void)
{
	if(enabled)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		enabled = false;
	}
}

void
EffectClass::SetParameter(int Parameter,float Value)
{
	parameter[Parameter] = Value;
}

void 
EffectClass::AplyFXOnFramebuffer(void)
{
	FX_function(&parameter[0]);
}



void
BlurFX::FX_function(float* parameters)
{
	blurSizeH = (1.0 / 300.0)*SCREENWIDTH;
	blurSizeV = (1.0 / 200.0)*SCREENHEIGHT;
	range = parameters[0]+0.5f;
	
	Vecti position;
	for(position.yps = 0; position.yps<SCREENHEIGHT; position.yps++)
	{
		for(position.ix = 0; position.ix<SCREENWIDTH; position.ix++)
		{
			screenBufferData[(position.ix%(short)SCREENWIDTH) + (position.yps/(short)SCREENHEIGHT)] = blurXY(position);
		}
	}
}

data32 
BlurFX::blurXY(Vecti position)
{
	Vecti screen;
	screen.ix = (short)SCREENWIDTH;
	screen.yps =  (short)SCREENHEIGHT;

	data32 color = screenBufferData[(position.ix % screen.ix) +( position.yps / screen.ix)];
	long sum = color.u32;
	for (int x = -range; x <= range; x++)
    {
		for (int y = -range; y <= range; y++)
		{
			sum += screenBufferData[( (short)(position.yps + y*blurSizeV) / screen.ix )+ ((short)(position.ix + x*blurSizeH) % screen.ix)].u32;
		}
	}
	color.u32 = (sum / 81);
	return color;
}
