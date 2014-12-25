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
	float speed;
public:
	ObjectMover(void)
	{
		currentWaypoint = -1;
		numberOfWaypoints = 0;
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
	};
	virtual void DoUpdate(void)
	{
		if(IsActive)
		{
			if((waypoints[currentWaypoint] - this->Connection()->getTransform()->position).GetLength() < 0.1)
			{
				if(++currentWaypoint == numberOfWaypoints)
					currentWaypoint = 0;
			}
			this->Connection()->getTransform()->position += (this->Connection()->getTransform()->position.direction(waypoints[currentWaypoint]) *(this->Connection()->getTransform()->speed * INPUT->FrameTime));
		}
	}
};

#endif
