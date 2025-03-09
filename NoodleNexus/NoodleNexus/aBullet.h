#pragma once
#include "sObject.h"
#include "Action.h"


class Object;


class aBullet : public Action
{
private:

public:

	virtual void Start()
	{
	}
	virtual void Update()
	{
		//printf("WOW, you are UPDATING\n");
	}

};


