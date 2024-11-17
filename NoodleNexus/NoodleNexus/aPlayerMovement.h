#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "aRayCastPhysics2D.hpp"




class aPlayerMovement : public Action
{

	void DrawRayS(glm::vec3 pos, glm::vec3 posEnd, GLuint program) {
		glm::vec3 direction = glm::normalize(posEnd - pos);
		float distance = glm::distance(pos, posEnd);
		for (float t = 0.0f; t <= distance; t += 0.1f) {
			glm::vec3 point = pos + direction * t;
			DrawDebugSphere(point, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f, program);
		}
	}
public:
	float walkSpeed= 15.f;
	float runSpeed = 50.f;
	float speed = walkSpeed;
	float jumpSpeed = 10.f;
	bool isMoving = true;
	glm::vec3 up = glm::vec3(0, 1, 0); 
	aRayCastPhysics2D* phys;
	
	// Common up vector in 3D
	GLuint program;

	enum Direction
	{
		FORWARD,
		BACK,
		LEFT,
		RIGHT,
		NONE
	};

	void Move(Direction direction) {
		glm::vec3 forward(0.0f, 0.0f, 1.0f); // Forward vector
		forward = glm::normalize(forward);

		glm::vec3 left = glm::cross(up, forward); // Left vector
		glm::vec3 position = object->mesh->positionXYZ; // Tank's current position

		glm::vec3 offset[] = {
			glm::vec3(-1.0f, 0.0f, -1.0f), // Bottom-left corner
			glm::vec3(1.0f, 0.0f, -1.0f),  // Bottom-right corner
			glm::vec3(-1.0f, 0.0f, 1.0f),  // Top-left corner
			glm::vec3(1.0f, 0.0f, 1.0f)    // Top-right corner
		};

		float tankRadius = 1.0f; // Adjust based on the tank's size
		glm::vec3 directionVector;
		switch (direction) {
		case FORWARD:
			directionVector = forward;
			break;
		case BACK:
			directionVector = -forward;
			break;
		case LEFT:
			directionVector = left;
			break;
		case RIGHT:
			directionVector = -left;
			break;
		default:
			return;
		}

		bool canMove = true;

		// Check collisions for all corners
		for (const auto& corner : offset) {
			glm::vec3 checkPosition = position + corner * tankRadius + directionVector * speed * 0.999f * object->scene->deltaTime;
			std::vector<sCollision_RayTriangleInMesh> collisions;

			if (object->scene->physicsManager->RayCast(position + corner * tankRadius, checkPosition, collisions, false)) {
				canMove = false;
				break;
			}
		}

		if (canMove) {
			object->mesh->positionXYZ += directionVector * speed * object->scene->deltaTime;
			DrawRayS(position, position + directionVector * speed * 2.f, program);
		}
		else {
			std::cout << "Blocked: Can't move in the desired direction.\n";
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
				object->mesh->rotationEulerXYZ.y = 360;
				Move(FORWARD);
			}
			else if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
			{
				object->mesh->rotationEulerXYZ.y = 180;
				Move(BACK);
			}
			else if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
			{
				object->mesh->rotationEulerXYZ.y = 90;
				Move(LEFT);
			}
			else if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
			{
				object->mesh->rotationEulerXYZ.y = -90;
				Move(RIGHT);

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

		
	}

};