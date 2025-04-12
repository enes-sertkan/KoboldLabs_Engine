#pragma once
#define NOMINMAX  // Disable Windows min/max macros
#include <windows.h>  // If you need Windows headers
#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "MazeGenerator.hpp"
#include <algorithm> 



class aPlayerMovement : public Action
{

	// Stamina variables
	
	const float staminaDrainRate = 10.0f;  // Per second when running
	const float staminaRegenRate = 15.0f;   // Per second when recovering
	const float staminaJumpCost = 0.0f;    // Stamina needed to jump

	// Jumping variables
	float verticalVelocity = 0.0f;
	const float gravity = -15.0f;
	const float jumpForce = 7.5f;
	const float damageJumpForce = 4.5f;
	bool isGrounded = true;
	 float groundLevel = 0.0f;



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
	float currentStamina = 100.0f;
	const float maxStamina = 100.0f;
	float walkSpeed = 4.f;
	float runSpeed = 9.f;
	float speed = walkSpeed;
	bool isMoving = true;
	glm::vec3 up = glm::vec3(0, 1, 0);      // Common up vector in 3D
	GLuint program;
	MazeGenerator* maze = nullptr;



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
		//std::cout << eyeRotation.x << " " << eyeRotation.y << std::endl;
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
			if (CheckMazePos(position - left * speed * 1.5f * object->scene->deltaTime))
			{
				break;
			}
			object->mesh->positionXYZ -= left * speed * object->scene->deltaTime;
			break;
		case BACK:
			if (CheckMazePos(position + left * speed * 1.5f * object->scene->deltaTime))
			{
				break;
			}
			object->mesh->positionXYZ += left * speed * object->scene->deltaTime;
			break;
		case LEFT:
			if (CheckMazePos(position + forward * speed * 1.5f * object->scene->deltaTime))
			{
				break;
			}
			object->mesh->positionXYZ += forward * speed * object->scene->deltaTime;
			break;
		case RIGHT:
			if (CheckMazePos(position - forward * speed * 1.5f * object->scene->deltaTime))
			{
				break;
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
		object->scene->isFlyCamera = true;
		currentStamina = maxStamina;
		groundLevel = object->startTranform->position.y;
	}

	void Update() override {
		if (object->scene->isFlyCamera)

		{
			verticalVelocity = 0;
			return;
		}


		// Handle stamina regeneration/drain
		UpdateStamina();

		// Handle movement inputs
		HandleMovementInputs();

		// Apply gravity and jumping physics
		ApplyGravity();
		GroundCheck();

		// Update camera position first
		object->scene->fCamera->setEyeLocation(object->mesh->positionXYZ);

		//object->mesh->rotationEulerXYZ = object->scene->fCamera->getCameraData()->rotation;
		object->mesh->rotationEulerXYZ.y = -object->scene->fCamera->getCameraData()->rotation.y;
		//object->mesh->rotationEulerXYZ.z = -object->mesh->rotationEulerXYZ.x;  
		//object->mesh->rotationEulerXYZ.x = -object->mesh->rotationEulerXYZ.z;
	}



	bool CheckMazePos(glm::vec3 position)
	{
		glm::vec3 sadFix = position;
		sadFix.x += maze->TILE_SIZE * 0.45;
		sadFix.z += maze->TILE_SIZE * 0.6;

		glm::vec2 mazePos = maze->WorldToGrid(sadFix);


		return maze->IsWall(mazePos.y, mazePos.x);
	
	}

	void DamageJump()
	{
		verticalVelocity += damageJumpForce;;
		isGrounded = false;
	}

	private:


		void UpdateStamina() {
			bool isMoving = glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS ||
				glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS ||
				glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS ||
				glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS;

			if (isMoving && speed == runSpeed) {
				currentStamina -= staminaDrainRate * object->scene->deltaTime;
				currentStamina = (std::max)(currentStamina, 0.0f);

				// Force walk if stamina depleted
				if (currentStamina <= 0.0f) {
					speed = walkSpeed;
				}
			}
			else {
				currentStamina += staminaRegenRate * object->scene->deltaTime;
				currentStamina = (std::min)(currentStamina, maxStamina);
			}
		}

		void HandleMovementInputs() {
			// Speed control
			if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				speed = (currentStamina > 0.0f) ? runSpeed : walkSpeed;
			}
			else {
				speed = walkSpeed;
			}

			// Movement
			if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS) Move(FORWARD);
			if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS) Move(BACK);
			if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS) Move(LEFT);
			if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS) Move(RIGHT);

			// Jumping
			if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS &&
				isGrounded &&
				currentStamina >= staminaJumpCost) {
				verticalVelocity = jumpForce;
				currentStamina -= staminaJumpCost;
				isGrounded = false;
			}
		}

		void ApplyGravity() {
			verticalVelocity += gravity * object->scene->deltaTime;
			object->mesh->positionXYZ.y += verticalVelocity * object->scene->deltaTime;
		}

		void GroundCheck() {
			if (object->mesh->positionXYZ.y <= groundLevel) {
				object->mesh->positionXYZ.y = groundLevel;
				verticalVelocity = 0.0f;
				isGrounded = true;
			}
		}


};
