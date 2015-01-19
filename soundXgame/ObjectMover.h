#ifndef _OBJECTMOVER_H_
#define _OBJECTMOVER_H_

#include "Connectable.h"
#include "UpdateManager.h"

template<unsigned NUMBER_OF_WAYPOINTS>
class ObjectMover :
	public IConnectable,
	public IUpdateble
{
private:
	Vector3 waypoints[NUMBER_OF_WAYPOINTS];
	short currentWaypoint;
	short numberOfWaypoints;
	
public:
	ObjectMover(void)
	{
		currentWaypoint = -1;
		numberOfWaypoints = 0;
	}
	void SetSpeed(float speed)
	{
		Connection()->getTransform()->speed = speed;
	}
	virtual bool Initialize(void)
	{
		UpdateManager::getInstance()->SignInForUpdate(this);
		return true;
	}
	void AddWaypoint(Vector3 waypoint)
	{
		if(numberOfWaypoints < NUMBER_OF_WAYPOINTS)
		{
			waypoints[++currentWaypoint] = waypoint;
			numberOfWaypoints++;
		}
	}
	virtual ~ObjectMover(void)
	{
		UpdateManager::getInstance()->SignOutFromUpdate(this);
	}

	virtual void DoUpdate(void)
	{
		if(IsActive)
		{
			float factor = (this->Connection()->getTransform()->speed * INPUT->FrameTime);
			Vector3 vec = waypoints[currentWaypoint] - this->Connection()->getTransform()->position;
			if(vec.GetLength() < 0.1)
			{
				if(++currentWaypoint == numberOfWaypoints)
					currentWaypoint = 0;
			}
			vec = vec.normalized()*factor;
			vec = this->Connection()->getTransform()->position + vec;
			this->Connection()->move(vec);
		}
	}
};



#endif
