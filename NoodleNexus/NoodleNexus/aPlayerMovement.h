#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>

class aPlayerMovement : public Action
{
public:
	float speed=1;
	bool isMoving = true;
	glm::vec3 up = glm::vec3(0, 1, 0);      // Common up vector in 3D


	enum Direction
	{
		FORWARD,
		BACK,
		LEFT,
		RIGHT
	};

	void Move(Direction direction)
	{
		glm::vec3 forward = object->scene->fCamera->getTargetRelativeToCamera();
		glm::vec3 left = glm::cross(up, forward);
		//I guess, we shouldn't really calculate left if we do not need it. But it's like nothing, right?
		//LATER TO DO ?

		switch (direction)
		{

		case aPlayerMovement::FORWARD:
			object->mesh->positionXYZ.x += forward.x * speed;
			object->mesh->positionXYZ.z += forward.z * speed;
			break;
		case aPlayerMovement::BACK:
			object->mesh->positionXYZ.x -= forward.x * speed;
			object->mesh->positionXYZ.z -= forward.z * speed;
			break;
		case aPlayerMovement::LEFT:
			object->mesh->positionXYZ.x += left.x * speed;
			object->mesh->positionXYZ.z += left.z * speed;
			break;
		case aPlayerMovement::RIGHT:
			object->mesh->positionXYZ.x -= left.x * speed;
			object->mesh->positionXYZ.z -= left.z * speed;
			break;

		default:
			break;
		}

	}

	

	void Update() override
	{
		
		if (object->scene->isFlyCamera) return;

		if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Move(FORWARD);
		}
		if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Move(BACK);
		}

		if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Move(LEFT);
		}
		if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Move(RIGHT);
		}
	
	}
};