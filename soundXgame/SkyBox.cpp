#include "projectMacros.h"
#include "SkyBox.h"
#include "Cam.h"
#include "MusicVox.h"



SkyBox::SkyBox(string imagefilename)
{
	AlwaysFaceMovingdirection = false;
	this->conXtor = new SkyBoxConnector();
	this->conXtor->SetConnection(this);
	InitializeObject(imagefilename);
	SetID(SCENE->Add(this));
	LockID();
	
	SetToWall(WALLS::None);
	this->AddConnectable<MusicVox>();
	
}

void
SkyBox::InitializeObject(string imagefilename,bool addToScene)
{
	transform.scale = Vector3(1,1,1);
	transform.position = Vector3::zero;

	walls[front] = new VoxGrid(imagefilename,false);
	walls[front]->AlwaysFaceMovingdirection = false;
	walls[front]->SetName("plane_front");
	walls[front]->LoadeMap("drei_64x64.ppm",0);
	walls[front]->move(-500,-250,-500);

	walls[rear] = new VoxGrid(imagefilename,false);
	walls[rear]->AlwaysFaceMovingdirection = false;
	walls[rear]->SetName("plane_rear");
	walls[rear]->LoadeMap("drei_64x64.ppm",0);
	walls[rear]->move(-500,-250,500);

	walls[left] = new VoxGrid(imagefilename,false);
	walls[left]->AlwaysFaceMovingdirection = false;
	walls[left]->SetName("plane_left");
	walls[left]->LoadeMap("drei_64x64.ppm",0);
	walls[left]->move(-500,-250,-500);
	walls[left]->flip('z');
	walls[left]->LoadeMap("drei_64x64.ppm",0);

	walls[right] = new VoxGrid(imagefilename,false);
	walls[right]->AlwaysFaceMovingdirection = false;
	walls[right]->SetName("plane_right");
	walls[right]->LoadeMap("drei_64x64.ppm",0);
	walls[right]->move(-500,-250,500);
	walls[right]->flip('z');

	walls[top] = new VoxGrid(imagefilename,false);
	walls[top]->AlwaysFaceMovingdirection = false;
	walls[top]->SetName("plane_top");
	walls[top]->LoadeMap("drei_64x64.ppm",0);
	walls[top]->move(-500,500,-500);
	walls[top]->flip('y');
	walls[top]->flipZ();

	walls[bottom] = new VoxGrid(imagefilename,false);
	walls[bottom]->AlwaysFaceMovingdirection = false;
	walls[bottom]->SetName("plane_bottom");
	walls[bottom]->LoadeMap("drei_64x64.ppm",0);
	walls[bottom]->move(-500,-500,-500);
	walls[bottom]->flip('y');
	walls[bottom]->flipZ();
	
	

	for(int i=0;i<6;i++)
	{
		walls[i]->IsGrounded(false);
		UPDATE->SignOutFromUpdate(walls[i]->Get<VoxControl>());
		INPUT->DetachKeyboard(walls[i]->Get<VoxControl>());
		walls[i]->MainSizzes.x = transform.scale.x;
		walls[i]->MainSizzes.y = transform.scale.y*2;
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
	for(int wall = 0; wall < 6; wall++)
	{
		walls[wall]->draw();
	}
}

Transform*
SkyBox::getTransform(void)
{
	return conXtor->current<0 ? &this->transform : walls[conXtor->current]->getTransform();
}

void
SkyBox::SetToWall(WALLS wall)
{
	conXtor->current = (wall>=0 && wall<=5)? wall : WALLS::None;
}

VoxGrid*
SkyBox::Wall(int wall)
{
	return walls[wall];
}

string
SkyBox::CurrentWallName(void)
{
	return SkyBoxConnector::wallNames[conXtor->current];
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
			walls[i]->scale(s);
			walls[i]->move((-500*s.x)+transform.position.x,((i%3>0? i%4==1? 500 : -500 : -250 )*s.y)+transform.position.y,((i%2>0? 500 : i%4>0? 500 : -500)*s.z)+transform.position.z);
	}
	return s;
}


Vector3
SkyBox::move(Vector3 p)
{
	transform.position=p;
	for(int i=0;i<6;i++)
	{
		walls[i]->move((-500*transform.scale.x)+p.x,((i%3>0? i%4==1? 500 : -500 : -250)*transform.scale.y)+p.y ,((i%2>0? 500 : i%4>0? 500 : -500)*transform.scale.z)+p.z);
	}
	return p;
}


char*
SkyBoxConnector::wallNames[] = {"left", "right", "front", "rear", "top", "bottom" ,"None"};

VoxGrid*
SkyBoxConnector::vConnection(void)
{

	return ((SkyBox*)this->Connection())->Wall(current);
}


bool 
SkyBoxConnector::Initialize(void)
{
	current = SkyBox::WALLS::None;
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
		if(key>='0' && key<'9')
		{
			if(lastKey!=key)
			{
				current=(key-48>=0 && key-48<=5)? key-48 : 6;
				printf("SKYBOX: sellected wall: %s\n", wallNames[current]);
			}
			lastKey=key;
		}
		else if(key=='l')
		{
			if(lastKey!='l')
			{
				((SkyBox*)Connection())->LoadeHightMap(files[currentSellection],bumpmapchannel,current>5? -1:current);
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
				((SkyBox*)Connection())->LoadeColorMap(files[currentSellection],current>5? -1:current);
			}
			lastKey='r';
		}
		else for(int i=0;i<6;i++)
		{
			current = i;
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
			int currentWall = current;
			current = i;
			vConnection()->Get<VoxControl>()->DoUpdate();
			current = currentWall;
		}
	}
}

