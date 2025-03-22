#pragma once

#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>





class aPlayerMovement : public Action
{

	void DrawRayS(glm::vec3 pos, glm::vec3 posEnd, GLuint program)
	{
		float distance = glm::distance(pos, posEnd);
		glm::vec3 direction = glm::normalize(posEnd - pos);
		glm::vec3 movingPoint = pos;

		while (glm::distance(pos, movingPoint) < distance)
		{
			// Move the next ball 0.1 times the normalized camera direction
			movingPoint += (direction * 0.10f);
			DrawDebugSphere(movingPoint, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program, object->scene);
		}

	}
public:
	float walkSpeed = 5.f;
	float runSpeed = 10.f;
	float speed = walkSpeed;
	bool isMoving = true;
	glm::vec3 up = glm::vec3(0, 1, 0);      // Common up vector in 3D
	GLuint program;

	enum Direction
	{
		FORWARD,
		BACK,
		LEFT,
		RIGHT,
		NONE
	};

	void Move(Direction direction)
	{
		// Get the camera's rotation (pitch, yaw) in degrees
		glm::vec2 eyeRotation = object->scene->fCamera->getEyeRotation();
		std::cout << eyeRotation.x << " " << eyeRotation.y << std::endl;
		// Convert yaw (the second component) to radians.
		float yaw = glm::radians(eyeRotation.y);

		// Compute the forward direction from the yaw (ignoring pitch for movement)
		// When yaw = 0, forward should be (0,0,1)
		glm::vec3 forward(sin(yaw), 0.0f, cos(yaw));
		forward = glm::normalize(forward);

		// Calculate the left direction (perpendicular to forward in the XZ plane)
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 left = glm::normalize(glm::cross(up, forward));

		// Get the current position (for collision purposes)
		glm::vec3 position = object->mesh->positionXYZ;
		std::vector<sCollision_RayTriangleInMesh> collisions;

		switch (direction)
		{
		case FORWARD:
			if (object->scene->physicsManager->RayCast(position, position + forward * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
			//	break;
			}
			object->mesh->positionXYZ -= left * speed * object->scene->deltaTime;
			break;
		case BACK:
			if (object->scene->physicsManager->RayCast(position, position - forward * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
			//	break;
			}
			object->mesh->positionXYZ += left * speed * object->scene->deltaTime;
			break;
		case LEFT:
			if (object->scene->physicsManager->RayCast(position, position + left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//break;
			}
			object->mesh->positionXYZ += forward * speed * object->scene->deltaTime;
			break;
		case RIGHT:
			if (object->scene->physicsManager->RayCast(position, position - left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//break;
			}
			object->mesh->positionXYZ -= forward * speed * object->scene->deltaTime;
			break;
		default:
			break;
		}
	}

	void Start() override
	{
	//	return;
		object->scene->isFlyCamera = false;
	}

	void Update() override
	{
	//	return;
		object->scene->fCamera->setEyeLocation(object->mesh->positionXYZ);

	//	if (object->scene->isFlyCamera && !glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) return; //IF not fly camera and not pressing shoft, then return


		if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			speed = runSpeed;
		else
			speed = walkSpeed;




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
