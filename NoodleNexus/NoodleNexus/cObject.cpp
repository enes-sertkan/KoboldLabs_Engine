#include "sObject.h"
#include "Scene.hpp"



void Object::Destroy()
{
	scene->RemoveObject(this);
}

glm::vec3 Object::GetPosition() const {
    return this->startTranform->position;
}

void Object::SetPosition(const glm::vec3& newPosition) {
    this->startTranform->position = newPosition;
}
