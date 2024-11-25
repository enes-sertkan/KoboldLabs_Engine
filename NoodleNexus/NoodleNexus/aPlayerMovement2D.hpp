#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "aModelsFramesAnimator.hpp"
#include "aRayCastPhysics2D.hpp"





class aPlayerMovement2D : public Action
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
			DrawDebugSphere(movingPoint, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program);
		}

	}
public:
	aModelsFramesAnimator* animator = nullptr;
	float walkSpeed = 30.f;
	float runSpeed = 80.f;
	float speed = walkSpeed;
	bool isMoving = true;
	glm::vec3 up = glm::vec3(0, 1, 0);      // Common up vector in 3D
	GLuint program;
	aRayCastPhysics2D* phys = nullptr;

	void SetAnimator(aModelsFramesAnimator* newAnimator)
	{
		animator = newAnimator;
	}


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
		position = object->mesh->positionXYZ; //object->scene->fCamera->getEyeLocation(); //object->scene->fCamera->getEyeLocation();
		std::vector<sCollision_RayTriangleInMesh> collisions;
		/*	DrawRayS(position, position + forward * speed * 2.f, program);
			DrawRayS(position, position - forward * speed * 2.f, program);
			DrawRayS(position, position + left * speed * 2.f, program);
			DrawRayS(position, position - left * speed * 2.f, program);*/

		switch (direction)
		{

		case aPlayerMovement2D::FORWARD:
			if (object->scene->physicsManager->RayCast(position, position + forward * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
				break;
			}

			break;

		case aPlayerMovement2D::BACK:
			if (object->scene->physicsManager->RayCast(position, position - forward * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//	std::cout << "COLLISION";
					// 
				break;
			}


			break;
		case aPlayerMovement2D::LEFT:
			if (object->scene->physicsManager->RayCast(position, position + left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
				// 
				break;
			}


			//object->mesh->positionXYZ.x += left.x * speed * object->scene->deltaTime;
			object->mesh->positionXYZ.z -=  speed * object->scene->deltaTime;
			break;
		case aPlayerMovement2D::RIGHT:
			if (object->scene->physicsManager->RayCast(position, position - left * speed * 1.5f * object->scene->deltaTime, collisions, false))
			{
				//std::cout << "COLLISION";
				// 
				break;
			}


			object->mesh->positionXYZ.z +=speed * object->scene->deltaTime;
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

		bool anyButtonPressed = false;
		bool walkingAnim = false;

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
			if (animator)
				animator->lookingRight = true;
			if (!walkingAnim)
			{
				animator->ChangeAnimation(1);
				walkingAnim = true;
			}
			Move(LEFT);
			anyButtonPressed = true;
		}
		if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (animator)
				animator->lookingRight = false;

			if (!walkingAnim)
			{
				animator->ChangeAnimation(1);
				walkingAnim = true;
			}

			Move(RIGHT);
			anyButtonPressed = true;
		}


		if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!phys) return;
			if (!phys->CheckGround()) return;


			glm::vec3 physSpeed = phys->speed;
			physSpeed.y = 3;
			phys->speed = physSpeed;
			phys->ignoreNextCol = true;

		}


		//Not good to base animations based on buttons and not speed. But ok...
		if (!anyButtonPressed)
		{
			animator->ChangeAnimation(0);
			walkingAnim = false;
		}

	}

};