#include "Musikubus.h"
#include "projectMacros.h"


Musicubus::Musicubus(void)
{

	InitializeCubus("CubusX_2.png");
	this->musikInput = AddConnectable<MusicListener>();
	//musikInput->GetLineData(0)->MINClampf = 0.5f;
	//musikInput->GetLineData(0)->MAXClampf = 1.5f;


}

Musicubus::Musicubus(string textureFile,bool backfaces,bool addToScene)
{
	InitializeCubus(textureFile,"03.wav",backfaces,addToScene);
	this->SetName("MusikCubus");
	this->IsVisible=true;
	this->musikInput = AddConnectable<MusicListener>();
}

Musicubus::~Musicubus(void)
{

}

//void
//Musikubus::InitializeObject(bool adToScene=true)
//{
//	SCENE->Add(new Musikubus())
//}


