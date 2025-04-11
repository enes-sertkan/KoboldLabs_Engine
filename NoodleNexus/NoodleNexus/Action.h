#pragma once
//#include "sObject.h"

class Object;


class Action
{
private:

public:

	bool isActive = true;
	Object* object = nullptr;
	virtual void Start()
	{}
	virtual void Update()
	{
		//printf("WOW, you are UPDATING\n");
	}

	virtual void OnDestroy()
	{
		object = nullptr;
	}



	virtual Action* Clone() const
	{
		Action* clone = new Action(*this); // Uses copy constructor
		clone->object = nullptr; // Reset the object reference
		return clone;
	}
	Action()
	{ }

	// Copy constructor
	Action(const Action& other) : object(nullptr)
	{
		// Copy any base class members here
		// (Currently just the object pointer which we set to null)
	}
};


