#include "sObject.h"
#include"Scene.hpp"


void Object::Destroy()
{
	scene->RemoveObject(this);
}