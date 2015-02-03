#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "globalfunctions.h"

using namespace ProjectMappe;

void ProjectMappe::OnLoadContent(void)
{
	// Background Music
	//unsigned int brummsound;
	//AUDIO->LoadeSampleToBank(brummsound,"brumm_s16.wav");
	AUDIO->LoadeBackgroundAudio("DaRioGame v05.wav");
	AUDIO->Play();
	AUDIO->BackgroundMusicVolume(1);

	// Gameplay Elements
	Ground* ground = Ground::getInstance();
	(new SkyBox())->SetName("Skybox");
	//ShaderObj* shaderObj = new ShaderObj();

	// Fountain
	Fountain* fountain = new Fountain();
	fountain->SetPosition(Vector3(-40, 0, 0));
	fountain->size = 50;
	fountain->createRange();
	fountain->SetClambt(0,-1,1);
	fountain->SetThreshold(0,0.33);
	fountain->sensitivity = 5;

	// GUI
	GuiObject* guiding = new GuiObject("panelT_256x512.png");
	guiding->SetName("Editor-Panel");
	guiding->scale(Vector3(256,256,1));
	guiding->AddConnectable<SliderX>();
	guiding->GetConnected<SliderX>()->PositionOnPanel = VectorF(10,10);
	guiding->GetConnected<SliderX>()->SizeScaledPanel = VectorF(0.5,0.5/4);
	guiding->AddConnectable<ButtonControl>();
	guiding->GetConnected<ButtonControl>()->PositionOnPanel = VectorF(10,60);	
	guiding->GetConnected<ButtonControl>()->SizeScaledPanel = VectorF(0.5,0.5/4);
	guiding->GetConnected<ButtonControl>(2)->SetText("Back");
	guiding->GetConnected<ButtonControl>(2)->SetColor(0,0,0,255);
	guiding->IsVisible = false;

	// Qualle
	(new AnimatedSprite<36,3>("Q2_1872x516.png",12,3,25,true))->SetName("Q2animated");
	SCENE->Object("Q2animated")->move(0,5,0);
	SCENE->Object("Q2animated")->IsGrounded(false);
	SCENE->Object("Q2animated")->AddConnectable<ObjectMover<8>>();
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+Utility::GlobalZ*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+Utility::GlobalZ*10+Utility::GlobalX*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+Utility::GlobalX*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+(Utility::GlobalZ-Utility::GlobalX).normalized()*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+(Utility::GlobalZ+Utility::GlobalX).normalized()*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+(Utility::GlobalX-Utility::GlobalZ).normalized()*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->AddWaypoint(SCENE->Object("Q2animated")->getTransform()->position+(-(Utility::GlobalX + Utility::GlobalZ)).normalized()*10);
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->SetSpeed(3);
	SCENE->Object("Q2animated")->AlwaysFaceMovingdirection = true;
	SCENE->Object("Q2animated")->GetConnected<ObjectMover<8>>()->IsActive = true;

	//Voxelplane... 
	//on runtime use press"X"to choose a an Image file from list (watch console!)
	//press "R" to loade the selected image as Image.
	//press "L" to loade the selected image's Bump-map. -> press several times each color channel separate...
	//(new VoxGrid("drei_.ppm"))->SetName("plane_front");
	//SCENE->Object("plane_front")->AddConnectable<VoxControl>();
	//SCENE->Object("plane_front")->move(-80,0,150);
	//((VoxGrid*)SCENE->Object("plane_front"))->MainSizzes.x=0.2;
	//((VoxGrid*)SCENE->Object("plane_front"))->MainSizzes.y=0.045f;

	// Kubus -> fliegender Cube
	new IPrimitivObject();
	GobID temp = SCENE->Object("last created")->GetID();
	((IPrimitivObject*)SCENE->Object(temp))->SetPrimitiv<ICubic>();
	SCENE->Object(temp)->LoadTexture("X-7.png");
	SCENE->Object(temp)->move(5,2,-5);
	SCENE->Object(temp)->GetOrAdd<SmoothObjectMover>()->movingVector.SetMAX(Vector3(10,5,20));
	SCENE->Object(temp)->GetOrAdd<SmoothObjectMover>()->movingVector.SetMIN(Vector3(-15,1,-30));
/*
	new Kubus();
	GobID temp = SCENE->Object("last created")->GetID();
	((Kubus*)SCENE->Object(temp))->SetPrimitiv<ICapsule>();
	SCENE->Object(temp)->LoadTexture("Deckelblech2_256x256.png");
	SCENE->Object(temp)->move(5,20,-5);
	SCENE->Object(temp)->GetOrAdd<SmoothObjectMover>()->movingVector.SetMAX(Vector3(10,25,20));
	SCENE->Object(temp)->GetOrAdd<SmoothObjectMover>()->movingVector.SetMIN(Vector3(-15,21,-30));
*/
	SCENE->Object(temp)->GetOrAdd<SmoothObjectMover>()->movingVector.SetMOVE(Vector3(Circle/100,Circle/200,Circle/150));
	SCENE->Object(temp)->GetConnected<SmoothObjectMover>()->IsActive = true;
	SCENE->Object(temp)->isVisible(true);
	SCENE->Object(temp)->SetName("PrimitivKubus");
	SCENE->Object(temp)->AlwaysFaceMovingdirection = true;
	(new ParticleSystem<100>("Der Blaue Dunst_512x512.png"))->SetName("PrimitivKubus-emission");
	SCENE->Object("PrimitivKubus-emission")->AlwaysFaceMovingdirection=true;
	SCENE->Object("PrimitivKubus-emission")->AddConnectable<ObjectFollower>();
	SCENE->Object("PrimitivKubus-emission")->GetConnected<ObjectFollower>()->SetTarget(SCENE->Object(temp));
	SCENE->Object("PrimitivKubus-emission")->IsGrounded(false);
	((ParticleSystem<100>*)SCENE->Object("PrimitivKubus-emission"))->emittingFrequency = 0.001f;
	((ParticleSystem<100>*)SCENE->Object("PrimitivKubus-emission"))->lifetime = 1.6f;
	((ParticleSystem<100>*)SCENE->Object("PrimitivKubus-emission"))->InitialSize = 0.7f;
	((ParticleSystem<100>*)SCENE->Object("PrimitivKubus-emission"))->ValueChangeReleasepoint = 0.6;
	SCENE->Object(temp)->AddConnectable<primitivSwitcher>();
	SCENE->Object("PrimitivKubus-emission")->SetColor(160,180,255,66);
	SCENE->Object("PrimitivKubus-emission")->IsVisible = true;
	
	/*
	Vector3[] pos = Vector3[18];
		pos[0] = new Vector3(0,0,0);
		pos[1] = new Vector3(5,0,5);

	for(int i = 1; i < 18; i++)
	{
		char dig = (char)(((int)'0')+i);
		(new Cubus("X-7.png"))->SetName("AUDIO01");
		SCENE->Object("AUDIO0" + dig)->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/15-Audio.mp3");
		SCENE->Object("AUDIO01")->move(pos[i]);
		SCENE->Object("AUDIO01")->AddConnectable<MusicScaler>();
		SCENE->Object("AUDIO01")->IsGrounded(true);
		SCENE->Object("AUDIO01")->AddConnectable<MusicScaler>();
		SCENE->Object("AUDIO01")->GetConnected<MusicScaler>()->sensitivity=2;
		SCENE->Object("AUDIO01")->GetConnected<MusicScaler>()->SetThreshold(0,0.02f);
	}
	*/

	//IMeshObject* meshObj;
	//meshObj = (IMeshObject*)SCENE->Object("AUDIO01");	
	//meshObj->GroundedWithPivot = true;



	//GobID vox2 = (new VoxGrid("buntbild_128.ppm"))->GetID();
	//SCENE->Object(vox2)->SetName("VoxelPlane2");
	//SCENE->Object(vox2)->AddConnectable<VoxControl>();
	//SCENE->Object(vox2)->AddConnectable<MusicVox>();
	//((VoxGrid*)SCENE->Object(vox2))->flip();
	Vector3  CubeSpwns[18] = {Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),
							  Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),
							  Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),Vector3(5,2,5),
							  Vector3(5,2,15),Vector3(5,2,5),Vector3(-5,-2,-5),Vector3(-5,2,5),
							  Vector3(5,-2,5),Vector3(5,2,-5)};
	char tempString[32];
	for(int i = 14;i<18;i++)
	{
		//CubeSpwns[i].x+=i*3;
		sprintf(&tempString[0],"AUDIO_%i",i);
		sprintf(&tempString[10],"mp3/%i-Audio.mp3",i);

		new IPrimitivObject();
		GobID id = SCENE->Object("last created")->GetID();
		SCENE->Object(id)->SetName(&tempString[0]);
		((IPrimitivObject*)SCENE->Object(id))->SetPrimitiv<ICubic>(); // cube or which obj
		SCENE->Object(id)->LoadTexture("X-7.png"); // load texture
		SCENE->Object(id)->AddConnectable<AudioEmitter>()->LoadeSample(&tempString[10]); // load music
		SCENE->Object(id)->move(CubeSpwns[i]); // moving to coordinate
		SCENE->Object(id)->IsGrounded(true); // grounded
		SCENE->Object(id)->AddConnectable<MusicScaler>(); // scale to music	->
		SCENE->Object(id)->GetConnected<MusicScaler>()->sensitivity=15;
		SCENE->Object(id)->GetConnected<MusicScaler>()->SetClambt(0,-1.1);
		SCENE->Object(id)->GetConnected<MusicScaler>()->SetThreshold(0,0.0002f);   
		SCENE->Object(id)->GetConnected<AudioEmitter>()->PlayAudio(); // play audio
	}

	// AUDIO 01
	/*(new Cubus("X-7.png"))->SetName("AUDIO01");
	SCENE->Object("AUDIO01")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/15-Audio.mp3");
	SCENE->Object("AUDIO01")->move(-20,0,-4);
	SCENE->Object("AUDIO01")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO01")->IsGrounded(true);
	IMeshObject* meshObj;
	meshObj = (IMeshObject*)SCENE->Object("AUDIO01");	
	meshObj->GroundedWithPivot = true;

	// AUDIO 02
	(new Cubus("X-7.png"))->SetName("AUDIO02");
	SCENE->Object("AUDIO02")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/10-Nanopad.mp3");
	SCENE->Object("AUDIO02")->move(8,0,-2);
	SCENE->Object("AUDIO02")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO02")->IsGrounded(true);

	// AUDIO 03
	(new Cubus("X-7.png"))->SetName("AUDIO03");
	SCENE->Object("AUDIO03")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/11-Audio.mp3");
	SCENE->Object("AUDIO03")->move(12,0,-2);
	SCENE->Object("AUDIO03")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO03")->IsGrounded(true);

	// AUDIO 04
	(new Cubus("X-7.png"))->SetName("AUDIO04");
	SCENE->Object("AUDIO04")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/18-Audio.mp3");
	SCENE->Object("AUDIO04")->move(20,0,-2);
	SCENE->Object("AUDIO04")->IsGrounded(true);

	// AUDIO 05
	(new Cubus("X-7.png"))->SetName("AUDIO05");
	SCENE->Object("AUDIO05")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/16-Audio.mp3");
	SCENE->Object("AUDIO05")->move(2,0,-10);

	// AUDIO 06
	(new Cubus("X-7.png"))->SetName("AUDIO06");
	SCENE->Object("AUDIO06")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/06-CZ_ToogBass.mp3");
	SCENE->Object("AUDIO06")->move(8,0,-10);
	SCENE->Object("AUDIO06")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->sensitivity=15;
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->SetClambt(0,-1.1);
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->SetThreshold(0,0.0002f);

	// AUDIO 07
	(new Cubus("X-7.png"))->SetName("AUDIO07");
	SCENE->Object("AUDIO07")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/19-Audio.mp3");
	SCENE->Object("AUDIO07")->move(12,0,-10);

	// AUDIO 08
	(new Cubus("X-7.png"))->SetName("AUDIO08");
	SCENE->Object("AUDIO08")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/21-Audio.mp3");
	SCENE->Object("AUDIO08")->move(20,0,-10);

	// AUDIO 09
	(new Cubus("X-7.png"))->SetName("AUDIO09");
	SCENE->Object("AUDIO09")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/08-Sforzando.mp3");
	SCENE->Object("AUDIO09")->move(2,0,-18);

	// AUDIO 10
	(new Cubus("X-7.png",true,true))->SetName("AUDIO10");
	SCENE->Object("AUDIO10")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/12-Audio.mp3");
	SCENE->Object("AUDIO10")->move(8,0,-18);
	SCENE->Object("AUDIO10")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO10")->GetConnected<MusicScaler>()->sensitivity=2;
	SCENE->Object("AUDIO10")->GetConnected<MusicScaler>()->SetThreshold(0,0.02f);
	
	// AUDIO 12
	(new Cubus("X-7.png"))->SetName("AUDIO12");
	SCENE->Object("AUDIO12")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/09-Brite Strings.mp3");
	SCENE->Object("AUDIO12")->move(20,0,-18);
	*/

	// Cubus - fliegender Cube
	(new Cubus("X-7.png"))->SetName("AUDIO11");
	SCENE->Object("AUDIO11")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/16-Audio.mp3");
	SCENE->Object("AUDIO11")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->Set3Dparameter(30,200);
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->AudioVolume(1);
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO11")->move(12,0,-18);
	SCENE->Object("AUDIO11")->IsGrounded(false);
	SCENE->Object("AUDIO11")->AddConnectable<SmoothObjectMover>();
	SCENE->Object("AUDIO11")->AlwaysFaceMovingdirection=true;
	(new ParticleSystem<100>("Der Blaue Dunst_512x512.png"))->SetName("AUDIO11-emission");
	SCENE->Object("AUDIO11-emission")->AlwaysFaceMovingdirection=true;
	SCENE->Object("AUDIO11-emission")->AddConnectable<ObjectFollower>();
	SCENE->Object("AUDIO11-emission")->GetConnected<ObjectFollower>()->SetTarget(SCENE->Object("AUDIO11"));
	SCENE->Object("AUDIO11-emission")->IsGrounded(false);
	((ParticleSystem<100>*)SCENE->Object("AUDIO11-emission"))->emittingFrequency = 0.001f;
	((ParticleSystem<100>*)SCENE->Object("AUDIO11-emission"))->lifetime = 1.6f;
	((ParticleSystem<100>*)SCENE->Object("AUDIO11-emission"))->InitialSize = 0.7f;
	((ParticleSystem<100>*)SCENE->Object("AUDIO11-emission"))->ValueChangeReleasepoint = 0.6;
	//((ParticleSystem<100>*)SCENE->Object("AUDIO11-emission"))->SetColor(128,128,255,25);
	SCENE->Object("AUDIO11-emission")->SetColor(160,180,255,66);
	SCENE->Object("AUDIO11-emission")->IsVisible = true;
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->PlayAudio();

	// Play all loaded Audios
	AUDIO->Play();

	// Music Cube
	(new Cubus("kubismus.png"))->SetName("muckubus");
	SCENE->Object("muckubus")->GetOrAdd<AudioEmitter>()->LoadeSample("mp3/20-Audio.mp3",false);
	SCENE->Object("muckubus")->AddConnectable<Randomover>();
	SCENE->Object("muckubus")->AddConnectable<MusicInteractor>();
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->GetLineData(0)->threshold = 1.5;
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->GetLineData(1)->threshold = 0.03f;
	SCENE->Object("muckubus")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("muckubus")->GetConnected<AudioEmitter>()->AudioVolume(1);
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->automaticFallOffAdjust=false;

	// Just some Music Cubes
	unsigned obj;
	float x,y,z;
	x=y=z=0;
	x = 15.0f;
	for(int i = 0;i<15;i++)
	{
		x-=((float)i);
		z+=(2.f*(float)i/15.0f);
		obj = (new Cubus("Deckelblech-2s.png"))->GetID();
		SCENE->Object(obj)->move(x,y,z);
		SCENE->Object(obj)->AddConnectable<Randomover>();
		SCENE->Object(obj)->GetConnected<Randomover>()->SetRotation(true);
		SCENE->Object(obj)->GetConnected<Randomover>()->SetMoving(true);
		SCENE->Object(obj)->GetOrAdd<AudioEmitter>();
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->LoadeSample("mp3/3-Audio.mp3",false);
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->PlayAudio();
		SCENE->Object(obj)->AddConnectable<MusicInteractor>();
		SCENE->Object(obj)->GetConnected<MusicInteractor>()->automaticFallOffAdjust=false;
		SCENE->Object(obj)->SetName("Brummer");
		SCENE->Object(obj)->IsVisible=true;
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->AudioVolume(1);
	}
	  


	// Spectrum Analyzer
	(new SpectrumAnalyzer())->SetName("SpectrumAnalyzer");
	SCENE->Object("SpectrumAnalyzer")->move(0, 0, -35.0f);
	SCENE->Object("SpectrumAnalyzer")->scale(40.0f * 3.5f/128.0f, 0.3f, 2.0f); // 90 ground-tiles * 3.5m width * 128 bands
	((SpectrumAnalyzer*)SCENE->Object("SpectrumAnalyzer"))->Initialize();

	/*string particleImages[3];
	particleImages[0] = "particle1_128x128.png";
	particleImages[1] = "particle2_128x128.png";
	particleImages[2] = "particle3_128x128.png";*/
	
	// Particles
	(new ParticleSystem<500>("particle4_128x128.png"))->SetName("ParticleSystem");
	SCENE->Object("ParticleSystem")->AddConnectable<ObjectMover<3>>();
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(-20,0,40));
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(40,0,-20));
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(-40,0,20));
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->IsActive = true;
	SCENE->Object("ParticleSystem")->IsGrounded(false);
	SCENE->Object("ParticleSystem")->move(0,0,0);
	SCENE->Object("ParticleSystem")->rotate(3,0.2,-1);
	SCENE->Object("ParticleSystem")->scale(7.5,7.5,0);
	SCENE->Object("ParticleSystem")->getTransform()->speed = 20.f;
	((ParticleSystem<500>*)SCENE->Object("ParticleSystem"))->SetColor(128,128,255,25);
	((ParticleSystem<500>*)SCENE->Object("ParticleSystem"))->IsVisible = true;
	SCENE->Object("ParticleSystem")->AddConnectable<MusicInteractor>();
//	SCENE->Object("ParticleSystem")->AddConnectable<Listener>();
//	SCENE->Object("ParticleSystem")->GetConnected<Listener>()->SetListenToChannel(AUDIO->GetBackgroundChannelHandle());
//	SCENE->Object("ParticleSystem")->GetConnected<Listener>()->IsActive=true;
	
	
	
	// Camera
	SCENE->camera->ModeSocket->AddCameraMode<Edit>()->IsActive=false;
	SCENE->camera->ModeSocket->AddCameraMode<StrangeChaoticView>()->IsActive=false;
	

	SCENE->camera->Mode(FIRSTPERSON);
	SCENE->camera->SetTarget(SCENE->Object("ParticleSystem"));
	//SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->GrabTarget();
	//SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->Mode(TargetGrabber::MODE::ROTATE);
	
	//overlay = new ScreenOverlay();
	//overlay->Initialize("framen_1920x1080.png");

	//AUDIO->BackgroundMusicVolume(0.95);
	//AUDIO->Volume(1);

}


/* An Update-event handler */
void 
ProjectMappe::UpdateCycle(void)
{ 
	/*let UPTATE call every object signed in for update it's update-Functions.*/
	UPDATE->DoTheUpdates();
	
	//  <<-- here is a good place for putting update-code when testing new classes or functions which need update.
	//		 but don't implementing IUpdateble-Interface yet,... or for counting a counter..., or ... output to console the last updated values... 
}

/* A Draw Event Handler */
void 
ProjectMappe::RenderCycle(void)
{
	/*let SCENE draws all IDrawable objects contained..  */
	SCENE->DrawAll();

	//  <<-- need to manually draw some object 3D... do it here.



	/*when all 3D-objects finished drawing to the Frame buffer, The GUI-Drawer will switch Matrices and View to 2D-mode,
	 *and goes on drawing 2D-Overlays to the Frame, if there where some GUI-Object loaded...  */ 
	GUI->DrawGUI();

	// <<-- manually drawing 2D can be done here...


}	//->when this function returns, Frame-buffers will swap and the Next frame will begin...  