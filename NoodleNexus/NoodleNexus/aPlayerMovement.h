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
			DrawDebugSphere(movingPoint, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program, object->scene->vaoManager);
		}

	}
public:
	float walkSpeed=30.f;
	float runSpeed = 80.f;
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
		glm::vec3 forward = object->scene->fCamera->getTargetRelativeToCamera();
		forward.y = 0;
		
		forward = glm::normalize(forward);
		glm::vec3 left = glm::cross(up, forward);
		glm::vec3 position;
		position = object->mesh->positionXYZ; 
		std::vector<sCollision_RayTriangleInMesh> collisions;
	/*	DrawRayS(position, position + forward * speed * 2.f, program);
		DrawRayS(position, position - forward * speed * 2.f, program);
		DrawRayS(position, position + left * speed * 2.f, program);
		DrawRayS(position, position - left * speed * 2.f, program);*/

		switch (direction)
		{
			
		case aPlayerMovement::FORWARD:
			if (object->scene->physicsManager->RayCast(position, position + forward*speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
			break;
			}
			object->mesh->positionXYZ.x += forward.x * speed * object->scene->deltaTime;
			object->mesh->positionXYZ.z += forward.z * speed * object->scene->deltaTime;
			break;

		case aPlayerMovement::BACK:
			if (object->scene->physicsManager->RayCast(position, position - forward * speed* 1.5f * object->scene->deltaTime, collisions, false))
			{
			//	std::cout << "COLLISION";
				// 
				break;
			}

			object->mesh->positionXYZ.x -= forward.x * speed * object->scene->deltaTime;
			object->mesh->positionXYZ.z -= forward.z * speed * object->scene->deltaTime;
			break;
		case aPlayerMovement::LEFT:
			if (object->scene->physicsManager->RayCast(position, position + left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
				// 
				break;
			}


			object->mesh->positionXYZ.x += left.x * speed * object->scene->deltaTime;
			object->mesh->positionXYZ.z += left.z * speed * object->scene->deltaTime;
			break;
		case aPlayerMovement::RIGHT:
			if (object->scene->physicsManager->RayCast(position, position - left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
				// 
				break;
			}


			object->mesh->positionXYZ.x -= left.x * speed * object->scene->deltaTime;
			object->mesh->positionXYZ.z -= left.z * speed * object->scene->deltaTime;
			break;

		default:
			break;
		}

	}

	

	void Update() override
	{
		
		if (object->scene->isFlyCamera && !glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) return; //IF not fly camera and not pressing shoft, then return


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