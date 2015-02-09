#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"

void
_OnScaleY(IConnectable* sender)
{
	GUI->Write("Y-changed",200,200);
}

void
_OnBackButton(IConnectable* sender)
{
	sender->Connection()->isVisible(false);
}

void _setFALLOFFAMOUNT(IConnectable*)
{
	((SpectrumAnalyzer*)GUI->Panel("SpectrumAnalizer-GUI-Panel"))->SetFalloff(GUI->Panel("SpectrumAnalizer-GUI-Panel")->GetConnected<Knob>(3)->Value);
}


SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::iherited.push_back((unsigned)typeid(IEditable).hash_code());
	IGObject::InitializeObject();
	this->ChartHeight = 100.0f;

	GUIPanel = new GuiObject("GUI/panelT_256x512.png");
	GUIPanel->scale(Vector3(256,128,1));
	GUIPanel->move(Vector3(20,20,0));
	GUIPanel->SetName("SpectrumAnalizer-GUI-Panel");
	GUIPanel->AddConnectable<Knob>();
	GUIPanel->GetConnected<Knob>(1)->SetText(" knopX");
	GUIPanel->GetConnected<Knob>(1)->PositionOnPanel = VectorF(25,100);
	GUIPanel->GetConnected<Knob>(1)->SizeScaledPanel = VectorF(0.2,0.2);
	GUIPanel->GetConnected<Knob>(1)->SetColor(0,0,0,255);
	GUIPanel->GetConnected<Knob>(1)->Value.SetUp(0,1,0.5,0.001,Controlled<float>::Clamp);
	GUIPanel->GetConnected<Knob>(1)->Value.SetVariable(&getTransform()->rotation.y);
	GUIPanel->AddConnectable<Knob>();
	GUIPanel->GetConnected<Knob>(2)->SetText(" knopY");
	GUIPanel->GetConnected<Knob>(2)->PositionOnPanel = VectorF(85,100);
	GUIPanel->GetConnected<Knob>(2)->SizeScaledPanel = VectorF(0.2,0.2);
	GUIPanel->GetConnected<Knob>(2)->SetColor(0,0,0,255);
	GUIPanel->GetConnected<Knob>(2)->Value.SetUp(0,1,0.5,0.001,Controlled<float>::Clamp);
	GUIPanel->GetConnected<Knob>(2)->Value.SetVariable(&getTransform()->scale.y);
	GUIPanel->AddConnectable<Knob>();
	((SpectrumAnalyzer*)GUIPanel->GetConnected<Knob>(3)->Connection())->SetFalloff(0.5);
	GUIPanel->GetConnected<Knob>(3)->SetText(" knopQX");
	GUIPanel->GetConnected<Knob>(3)->PositionOnPanel = VectorF(145,100);
	GUIPanel->GetConnected<Knob>(3)->SizeScaledPanel = VectorF(0.2,0.2);
	GUIPanel->GetConnected<Knob>(3)->SetColor(0,0,0,255);
}

void
SpectrumAnalyzer::Initialize(void)
{
	//Setting up a color...
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=60;

	// Setting up an offset-vector 
	// (will be added to each Sub-object's vertices's)  
	Vector3 offset = getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=(SPECTRUM_SIZE*0.5);
	
	// Instantiate as many Cubes as there are frequency-bands 
	// in the used fft-window... for each, increase its position-offset
	// by its size along the offset-direction.
	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X + i;
		bands[i]=(new Cubus(color,true,false,offset));
		bands[i]->IsGrounded(true);
		bands[i]->GroundedWithPivot = true;
		bands[i]->scale(getTransform()->scale);
	}
	
	// reset movement every step you select it for executing with bands
	transform.movement = *Vector3::Zero;
	
	/* Setting the Meters "FallOff"...
	 * as lower the value, as slower the visuals will
	 * Fall back to Zero if there's no signal. */
	fallOffAmount = 0.05f / 3.0f * ChartHeight;
	
	// this set's the hight of the analyzers chart.
	// band measures will multiplied by it.
	getTransform()->scale.y = ChartHeight;

	//get being invoked at EarlyUpdate, to get the FFT-Data...
	UPDATE->SignInForEarlyUpdate(this);

}

void 
SpectrumAnalyzer::SetFalloff(float falloff)
{
	fallOffAmount = falloff;
}

void
SpectrumAnalyzer::DoEarly(void)
{ // Get The FFT-Data-Buffer from desired audio-channel.
  // ...in this case the Background audio channel....
	fftData = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}

void
SpectrumAnalyzer::DoUpdate(void)
{//Reading out the Early-getted fft-buffer and apply
 //it's values on the Sub-Cubes....
	float changeFactor;
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		//scaling data values to the desired size...
		//also let's flatten the scale and maybe do some compression 
		//to make the chart looks more strait, and the bands 
		//look'n more equivalent then real spectrum-data will do...  

		changeFactor = (fftData[i]*((float)1+i*0.128));         
		ChangeSize(i,changeFactor * getTransform()->scale.y);
		ChangeColor(i,changeFactor);
	}
}

void
SpectrumAnalyzer::DoLate(void)
{
	//maybe the sub-updates here better...
	
}

void 
SpectrumAnalyzer::ChangeColor(int index,float factor)
{
	//Do flashing the colors.....
	color.byte[1] = ((factor*10) * 255.0);
	color.byte[2] = (1.0 - (factor*10))*255.0;
	bands[index]->SetColor(color);
}
void
SpectrumAnalyzer::ChangeSize(int band,float newScaleY)
{   
	//apply the measured and compressed signal values to the
	//Sub-cube's Transform scale's if the values have changed...

	//if the new value is lower than the old value,
	//shrink the cube's Y-Axis by the "Fall-back"-value...
	if(newScaleY < bands[band]->getTransform()->scale.y)
		bands[band]->getTransform()->scale.y -= fallOffAmount;
	else  // if th new value is higher than the old one, just set it's Y-scale to the new value...
		bands[band]->getTransform()->scale.y = newScaleY;
}

void
SpectrumAnalyzer::draw(void)
{
	if(!IsVisible)
		return;

	Vector3 vec;
	for(int i = 0;i<360;i+=90)
	{
		vec = getTransform()->rotation + Vector3(0,i,0);
		rotate(vec.x,vec.y,vec.z);
		drawOnce();
	}
}

void
SpectrumAnalyzer::drawOnce(void)
{
	if(_CHANGEDposition || _CHANGEDrotation || _CHANGEDscale)
	{
		// Re-Positioning,seizing and rotating the SubCubes
		// if the whole analyzer object has been moved,resized 
		// or rotated during last update..  -->> maybe this could be don at LateUpdate better... don't know....
		for(int i=0;i<SPECTRUM_SIZE;i++)
		{
			if(_CHANGEDposition)
				bands[i]->getTransform()->position += transform.movement;
			if(_CHANGEDrotation)
				bands[i]->getTransform()->rotation = getTransform()->rotation;
			if(_CHANGEDscale)
				bands[i]->scale(transform.scale.x,bands[i]->getTransform()->scale.y,transform.scale.z);
		}
		_CHANGEDposition = _CHANGEDrotation = _CHANGEDscale = false;
		transform.movement = *Vector3::Zero;
	}

	//draw each sub-cube....
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		bands[i]->draw();
	}	
}

Vector3
SpectrumAnalyzer::move(float X,float Y,float Z)
{
	IMeshObject::move(Vector3(X,Y,Z));
	_CHANGEDposition=true;
	return getTransform()->position;
}

Vector3
SpectrumAnalyzer::rotate(float X,float Y,float Z)
{
	_CHANGEDrotation=true;
	return IMeshObject::rotate(Vector3(X,Y,Z));
}

// scale this object to the size each sub object will have.. 
Vector3
SpectrumAnalyzer::scale(float X,float Y,float Z)
{
	_CHANGEDscale=true;
	return IMeshObject::scale(Vector3(X,Y,Z));
}

SpectrumAnalyzer::~SpectrumAnalyzer(void)
{   // don't forget signing out from the
	// Events we're registered to if destructing...
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
	UPDATE->SignOutFromLateUpdate(this);
	for(int i = SPECTRUM_SIZE-1;i>=0;i--)
		bands[i]->~Cubus();
	//delete fftData;
}