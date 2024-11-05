#pragma once
//#include "sObject.h"

class Object;


class Action
{
private:

public:

	Object* object = nullptr;
	virtual void Start()
	{}
	virtual void Update()
	{
		//printf("WOW, you are UPDATING\n");
	}

};


//
//class MoveForward : public Action
//{
//	void Update() override
//	{
//		object->mesh->positionXYZ.x++;
//	}
//};
//
//class ExplosionLogic : public Action
//{
//	void Update() override
//	{
//		 
//		if (object->mesh->uniformScale >= 5) return; 
//
//		object->mesh->uniformScale+=0.005;
//
//
//
//	}
//};