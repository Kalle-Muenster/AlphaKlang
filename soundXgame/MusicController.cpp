#include "MusicController.h"
#include "GuiControlls.h"
#include "SceneGraph.h"
#include "Camera.h"

void _MusicListenerPanel_BackButtonClick(IConnectable* sender)
{
	((MusicControllerPanel*)((ButtonControl*)sender)->Connection())->DetachListener(((MusicControllerPanel*)((ButtonControl*)sender)->Connection())->GetListener());
}

MusicControllerPanel::MusicControllerPanel(void)
{
	InitializeGUIObject();
	SetUp("GUI/panelT_256x512.png",false,false);
	SetName("Listener-Panel");
	SetText("No Object selected");
	scale(Vector3(256,256,1));
	move(Vector3(10,10,0));
	AddConnectable<Knob>();
	GetConnected<Knob>(1)->PositionOnPanel = VectorF(10,80);
	GetConnected<Knob>(1)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(1)->Value.SetUp(0,5,0,0.1,Controlled<float>::Clamp);
	GetConnected<Knob>(1)->SetText("Line Number: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(2)->PositionOnPanel = VectorF(165,80);
	GetConnected<Knob>(2)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(2)->Value.MOVE = 2;
	GetConnected<Knob>(2)->Value = -1;
	GetConnected<Knob>(2)->SetText("Motivator MIN: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(3)->PositionOnPanel = VectorF(10,180);
	GetConnected<Knob>(3)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(3)->Value.MOVE = 2;
	GetConnected<Knob>(3)->Value = 1;
	GetConnected<Knob>(3)->SetText("Motivator MAX: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(4)->PositionOnPanel = VectorF(165,180);
	GetConnected<Knob>(4)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(4)->Value.SetUp(0,126,1,2,Controlled<float>::Clamp);
	GetConnected<Knob>(4)->SetText("LowerBound: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(5)->PositionOnPanel = VectorF(10,280);
	GetConnected<Knob>(5)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(5)->Value.SetUp(1,127,64,2,Controlled<float>::Clamp);
	GetConnected<Knob>(5)->SetText("UpperBound: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(6)->PositionOnPanel = VectorF(165,280);
	GetConnected<Knob>(6)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(6)->Value.SetUp(0,5,0,0.0001,Controlled<float>::Clamp);
	GetConnected<Knob>(6)->SetText("Threshold: ");
	AddConnectable<Knob>();
	GetConnected<Knob>(7)->PositionOnPanel = VectorF(10,360);
	GetConnected<Knob>(7)->SizeScaledPanel = VectorF(0.25,0.25);
	GetConnected<Knob>(7)->Value.SetUp(0,5,0,0.0001,Controlled<float>::Clamp);
	GetConnected<Knob>(7)->SetText("Falloff: ");
	AddConnectable<ButtonControl>();
	GetConnected<ButtonControl>(8)->PositionOnPanel = VectorF(200,450);	
	GetConnected<ButtonControl>(8)->SizeScaledPanel = VectorF(0.2,0.2);
	GetConnected<ButtonControl>(8)->SetText("   Back");
	GetConnected<ButtonControl>(8)->SetClickerFunc(_MusicListenerPanel_BackButtonClick);
	ShowTitle = true;
}

MusicControllerPanel::~MusicControllerPanel(void)
{

}

void
MusicControllerPanel::BindToMusicListener(MusicController* listener)
{
	controller = listener;
	SetText(listener->Connection()->GetName());
	GetConnected<Knob>(1)->Value = 0;
	GetConnected<Knob>(2)->Value = controller->GetLineData(0)->lowerBound;
	GetConnected<Knob>(3)->Value = controller->GetLineData(0)->upperBound;
	GetConnected<Knob>(4)->Value = controller->GetLineData(0)->MINClampf;
	GetConnected<Knob>(5)->Value = controller->GetLineData(0)->MAXClampf;
	GetConnected<Knob>(6)->Value = controller->GetLineData(0)->threshold;
	GetConnected<Knob>(7)->Value = controller->GetLineData(0)->fallOff;
	this->isVisible(true);
}

void
MusicControllerPanel::DetachListener(MusicController* listener)
{
	if(controller == listener)
	{
		this->isVisible(false);
		controller = NULL;
	}
}

MusicController* 
MusicControllerPanel::GetListener(void)
{
	return controller;
}



void
MusicControllerPanel::UpdateListenerValues(void)
{
	if(controller!=NULL)
	{
		short line = GetConnected<Knob>(1)->Value;
		controller->GetLineData(line)->enabled = true;
		controller->SetLineBounds(line,GetConnected<Knob>(4)->Value,GetConnected<Knob>(5)->Value,controller->GetLineData(line)->bandWidth);
		controller->SetClambt(line,GetConnected<Knob>(2)->Value,GetConnected<Knob>(3)->Value);
		controller->SetThreshold(line,GetConnected<Knob>(6)->Value);
		controller->GetLineData(line)->fallOff = GetConnected<Knob>(7)->Value; 
	}
}

bool
MusicController::EditingEnabled = false;

MusicController::MusicController(void)
{


	//Line[0].enabled = true;
	//Line[0].clampf = true;
	//Line[0].MINClampf = 0.0f;
	//Line[0].MAXClampf = 10.0f;
	//Line[0].threshold = 0.05f;
	//Line[0].fallOff = 0.00001f;
	//Line[0].lowerBound = 0;
	//Line[0].upperBound = 9;
	//Line[0].bandWidth = 5;

	IsAttachedToPanel = false;
}

float* 
MusicController::GetFFTData(void)
{
	return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}

void
MusicController::DoUpdate(void)
{
	if(EditingEnabled)
	{
		if(SCENE->camera->GetTarget()->GetID() == this->Connection()->GetID())
		{
			if(!IsAttachedToPanel)
			{
				((MusicControllerPanel*)GUI->Panel("Listener-Panel"))->BindToMusicListener(this);
				IsAttachedToPanel =	true;
			}

			if(GUI->Panel("Listener-Panel")->IsVisible) 
				((MusicControllerPanel*)GUI->Panel("Listener-Panel"))->UpdateListenerValues();
		}
		else
		{
			if(IsAttachedToPanel)
			{
				((MusicControllerPanel*)GUI->Panel("Listener-Panel"))->DetachListener(this);
				IsAttachedToPanel = false;
			}
		}
	}
	else
		IsAttachedToPanel=false;

	MusicListener::DoUpdate();
}
