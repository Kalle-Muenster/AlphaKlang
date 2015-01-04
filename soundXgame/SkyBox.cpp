#include "projectMacros.h"
#include "SkyBox.h"
#include "Cam.h"
#include "MusicVox.h"


SkyBox::SkyBox(string imagefilename)
{
	this->conXtor = new SkyBoxConnector();
	this->conXtor->SetConnection(this);
	InitializeObject(imagefilename);
	SetID(SCENE->Add(this));
	LockID();
	
	SetWallvConnection(WALLS::left);
	//AddConnectable<MusicVox>();
//	
	
}

void
SkyBox::InitializeObject(string imagefilename,bool addToScene)
{
	transform.scale=Vector3(1,1,1);
	transform.position = Vector3::zero;

	walls[front] = new VoxGrid(imagefilename,false);
//	walls[front]->conXtor->ConIDs[0] = (ConID)walls[front]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[front]->SetName("plane_front");
	walls[front]->LoadeMap("drei_64x64.ppm",0);
	walls[front]->move(-500,-250,-500);
	walls[front]->MainSizzes.x = transform.scale.x;
	walls[front]->MainSizzes.y = transform.scale.y*2;
//	walls[front]->AddConnectable<MusicVox>();

	walls[rear] = new VoxGrid(imagefilename,false);
//	walls[rear]->conXtor->ConIDs[0] = (ConID)walls[rear]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[rear]->SetName("plane_rear");
	walls[rear]->LoadeMap("drei_64x64.ppm",0);
	walls[rear]->move(-500,-250,500);
	walls[rear]->MainSizzes.x = transform.scale.x;
	walls[rear]->MainSizzes.y = transform.scale.y*2;
//	walls[rear]->AddConnectable<MusicVox>();

	walls[left] = new VoxGrid(imagefilename,false);
//	walls[left]->conXtor->ConIDs[0] = (ConID)walls[left]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[left]->SetName("plane_left");
	walls[left]->LoadeMap("drei_64x64.ppm",0);
	walls[left]->move(-500,-250,-500);
	walls[left]->MainSizzes.x = transform.scale.x;
	walls[left]->MainSizzes.y = transform.scale.y*2;
	walls[left]->flip('z');
	walls[left]->LoadeMap("drei_64x64.ppm",0);
//	walls[left]->AddConnectable<MusicVox>();

	walls[right] = new VoxGrid(imagefilename,false);
//	walls[right]->conXtor->ConIDs[0] = (ConID)walls[right]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[right]->SetName("plane_right");
	walls[right]->LoadeMap("drei_64x64.ppm",0);
	walls[right]->move(-500,-250,500);
	walls[right]->MainSizzes.x = transform.scale.x;
	walls[right]->MainSizzes.y = transform.scale.y*2;
	walls[right]->flip('z');
//	walls[right]->AddConnectable<MusicVox>();

	walls[top] = new VoxGrid(imagefilename,false);
//	walls[top]->conXtor->ConIDs[0] = (ConID)walls[top]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[top]->SetName("plane_top");
	walls[top]->LoadeMap("drei_64x64.ppm",0);
	walls[top]->move(-500,500,-500);
	walls[top]->MainSizzes.x = transform.scale.x;
	walls[top]->MainSizzes.y = transform.scale.y*2;
	walls[top]->flip('y');
	walls[top]->flipZ();
//	walls[top]->AddConnectable<MusicVox>();

	walls[bottom] = new VoxGrid(imagefilename,false);
//	walls[bottom]->conXtor->ConIDs[0] = (ConID)walls[bottom]->conXtor->ConnectConnectableInstance(this->conXtor);
	walls[bottom]->SetName("plane_bottom");
	walls[bottom]->LoadeMap("drei_64x64.ppm",0);
	walls[bottom]->move(-500,-500,-500);
	walls[bottom]->MainSizzes.x = transform.scale.x;
	walls[bottom]->MainSizzes.y = transform.scale.y*2;
	walls[bottom]->flip('y');
	walls[bottom]->flipZ();
	
	walls[bottom]->AddConnectable<MusicVox>();

	for(int i=0;i<6;i++)
	{
		walls[i]->IsGrounded(false);
		UPDATE->SignOutFromUpdate(walls[i]->GetConnected<VoxControl>());
		INPUT->DetachKeyboard(walls[i]->GetConnected<VoxControl>());
	}


}

SkyBox::~SkyBox(void)
{
	for(int i = 0;i<6;i++)
		delete walls[i];
}



void
SkyBox::draw(void)
{
	for(int wall = left; wall <= bottom; wall++)
	{
		walls[wall]->draw();
	}
}


void
SkyBox::SetWallvConnection(WALLS wall)
{
	((SkyBoxConnector*)conXtor)->current = wall;
}

VoxGrid*
SkyBox::Wall(int wall)
{
	return walls[wall];
}

void
SkyBox::LoadeHightMap(string ppmImageFileName,int chanel,int wall)
{
	 if(wall>=0)
		 walls[wall]->LoadeMap(ppmImageFileName,chanel);
	 else
		 for(int i = 0; i<6; i++)
			walls[i]->LoadeMap(ppmImageFileName,chanel);
}

void
SkyBox::LoadeColorMap(string ppmFile,int wall)
{
	if(wall>=0)
		walls[wall]->ReLoade(ppmFile);
	else
		for(int i=0;i<6;i++)
			walls[i]->ReLoade(ppmFile);
}

Vector3 
SkyBox::scale(Vector3 s)
{
	transform.scale=s;
	for(int i=0;i<6;i++)
	{
		//	walls[i]->scale(s);
		//	walls[i]->move(-500, i%3>0? i%4==1? 500 : -500 : -250 ,i%2>0? 500 : i%4>0? 500 : -500);
	}
	return s;
}


Vector3
SkyBox::move(Vector3 p)
{
	transform.position=p;
	for(int i=0;i<6;i++)
	{
		//walls[i]->getTransform()
		//walls[i]->move(-500,i%3>0? i%4==1? 500 : -500 : -250 ,i%2>0? 500 : i%4>0? 500 : -500);
	}
	return p;
}


VoxGrid*
SkyBoxConnector::vConnection(void)
{

	return ((SkyBox*)this->Connection())->Wall(current);
}

IGObject*
SkyBoxConnector::Connection(void)
{
	return ((SkyBox*)IConnectable::Connection())->Wall(current);
}

bool 
SkyBoxConnector::Initialize(void)
{
	current = SkyBox::WALLS::left;
	TypeHashCode = (unsigned)typeid(SkyBoxConnector).hash_code();
	InputManager::getInstance()->attachKey(this);
	UpdateManager::getInstance()->SignInForUpdate(this);
	return true;
}

void 
SkyBoxConnector::keyPress(char key)
{
	if(SCENE->camera->GetTarget() == this->Connection())
	{
		if(key=='l')
		{
			if(lastKey!='l')
			{
				((SkyBox*)Connection())->LoadeHightMap(files[currentSellection],bumpmapchannel);
				if(++bumpmapchannel>3)
				bumpmapchannel=0; 
			}
			lastKey='l';
		}
		else if(key=='x')
		{
			if(lastKey!='x')
			{
				currentSellection=currentSellection==5?0:currentSellection+1;
				printf("Sellected Image: %s\n",files[currentSellection]);
			}
			lastKey='x';
		}
		else if(key=='r')
		{
			if(lastKey!='r')
			{
				((SkyBox*)Connection())->LoadeColorMap(files[currentSellection]);
			}
			lastKey='r';
		}
		else for(int i=0;i<6;i++)
		{
			current = (SkyBox::WALLS)i;
			vConnection()->Get<VoxControl>()->keyPress(key);
		}
	}
}

void 
SkyBoxConnector::DoUpdate(void)
{	
	if(SCENE->camera->GetTarget() == this->Connection())
	{
		if(lastKey!='\0')
		{
			timer+=INPUT->FrameTime;
			if(timer>0.5)
			{
				timer=0;
				lastKey='\0';
			}
		}
		for(int i=0;i<6;i++)
		{
			current = (SkyBox::WALLS)i;
			vConnection()->Get<VoxControl>()->DoUpdate();
		}
	}
}