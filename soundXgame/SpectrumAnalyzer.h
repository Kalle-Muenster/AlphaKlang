#ifndef _SPECTRUM_ANALYZER_H_
#define _SPECTRUM_ANALYZER_H_

#include "igobject.h"
#include "Cubus.h"
#include "UpdateManager.h"

#define SPECTRUM_SIZE (256)

class SpectrumAnalyzer :
	public Cubus,
	public IUpdateble
{
private: 
	short bandWidth;
	Cubus* bands[128];
	float* fftData;
	bool _gotThisFrameSpectrumData;
	virtual void ChangeColor(float bandLevel);
	virtual void ChangeSize(float bandLevel);

public:
	SpectrumAnalyzer(void);
	virtual ~SpectrumAnalyzer(void);
	virtual void InitializeSpectrumAnalyzer(void);
	virtual void DoEarly(void);
	virtual void DoUpdate(void);
	virtual void DoLate(void);
	virtual void draw(void);

};

#endif
