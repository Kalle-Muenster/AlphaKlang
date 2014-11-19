#include "Musikubus.h"

void
_CubusOnMusic(int line,ListenerData* data,IGObject* sender)
{
	((Musikubus*)sender)->MoveOnMusic(line,data);
}

Musikubus::Musikubus(void)
{

	InitializeCubus();

	MoveIt = &_CubusOnMusic;

	this->musikInput = AddConnectable<MusicListener>();
	musikInput->AddListenerCallback(0,MoveIt,this);

}


Musikubus::~Musikubus(void)
{
	delete motivators;
}

//void
//Musikubus::InitializeObject(bool adToScene=true)
//{
//	SCENE->Add(new Musikubus())
//}

void
Musikubus::DoEarly(void)
{
	float* fftWin = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Medium);
	motivators = musikInput->listen(fftWin);
}

void
Musikubus::MoveOnMusic(int line ,ListenerData* data)
{
	//-> put code hear...
}