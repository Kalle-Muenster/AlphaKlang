#include "MusicScaler.h"


MusicScaler::MusicScaler(void)
{
	Line[0].fallOff = 0.000001f;
}

bool
MusicScaler::Initialize(void)
{
	 //if(this->Connection()->HasConnected<AudioReciever>())
		//return true;
	 //else 
		// this->Connection()->AddConnectable<AudioReciever>();
	 return true;
}

MusicScaler::~MusicScaler(void)
{
}
