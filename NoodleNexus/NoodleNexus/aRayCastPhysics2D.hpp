#pragma once
#include "Action.h";
#include "sObject.h";
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "GLCommon.h"


class aRayCastPhysics2D : public Action
{
public:
	glm::vec3 gravityAcceleration = { 0.f,0.f,0.f };
	glm::vec3 speed = { 0.f,0.f,0.f };
	bool ignoreNextCol = false;

	float baseRayCastLength = 0;
	float speedLengthMultiplier = 1;
	float bounciness = 0;

	bool isPhysicsEnabled = true;

	glm::vec3 airFriction = glm::vec3(0, 0, 0);

	float yChange = 10.f;

	void ApplySpeed()
	{
		//object->mesh->positionXYZ.x += speed.x;
		object->mesh->positionXYZ.y += speed.y;
		object->mesh->positionXYZ.z += speed.z;

	}

	void ApplyAcceleration()
	{
		//speed.x += gravityAcceleration.x * object->scene->deltaTime;
		speed.y += gravityAcceleration.y;//* object->scene->deltaTime;
		speed.z += gravityAcceleration.z;// * object->scene->deltaTime;


	////	speed.x *= 1 - airFriction.x * object->scene->deltaTime;
	//	speed.y *= 1 - airFriction.x * object->scene->deltaTime;
	//	speed.z *= 1 - airFriction.z * object->scene->deltaTime;

	}

	void AddAcceleration(glm::vec3 acceleration)
	{
		//speed.x += acceleration.x * object->scene->deltaTime;
		speed.y += acceleration.y * object->scene->deltaTime;
		speed.z += acceleration.z * object->scene->deltaTime;


	}

	void SetSpeed(glm::vec3 newSpeed)
	{
	//	speed.x = newSpeed.x * object->scene->deltaTime;
		speed.y = newSpeed.y * object->scene->deltaTime;
		speed.z = newSpeed.z * object->scene->deltaTime;
	}

	void GetButtonDown(GLFWwindow* window, std::vector<sCollision_RayTriangleInMesh> collisions)
	{
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			AddAcceleration(glm::vec3(0.1, 0, 0));


		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			AddAcceleration(glm::vec3(-0.1, 0, 0));

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			AddAcceleration(glm::vec3(0, 0, 0.1));

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			AddAcceleration(glm::vec3(0, 0, -0.1));


		if (collisions.size() > 0)
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				AddAcceleration(glm::vec3(0.f, 4.f, 0.f));



	}





	glm::vec3 gravityComponent(const glm::vec3& speed, const glm::vec3& down) {

		glm::vec3 gravityDirection = glm::normalize(down);

		float gravitySpeed = glm::dot(speed, gravityDirection);

		return gravitySpeed * gravityDirection;
	}



	std::vector<sCollision_RayTriangleInMesh> CheckHit(const glm::vec3& down)
	{
		std::vector<sCollision_RayTriangleInMesh> collisions;

		if (ignoreNextCol)
		{
			ignoreNextCol = false;
			return collisions;
		}

		glm::vec3 normSpeed;


		normSpeed = glm::normalize(speed + gravityAcceleration);
		normSpeed = normSpeed * (glm::length(speed * speedLengthMultiplier * object->scene->deltaTime) + baseRayCastLength);
		glm::vec3 gravityDirection = glm::normalize(down);
		glm::vec3 startPos = object->mesh->positionXYZ;
		glm::vec3 endPos = startPos + normSpeed;
		glm::vec3 endPos2 = startPos + gravityDirection * baseRayCastLength;


		


		if (object->scene->physicsManager->RayCast(startPos, endPos2, collisions, false))
		{

			// Update position to avoid getting "stuck" at the collision point
	
			speed.y = 0;

			//HATE THIS
			object->mesh->positionXYZ.y = collisions[0].vecTriangles[0].intersectionPoint.y + baseRayCastLength;
			std::vector<sCollision_RayTriangleInMesh> col;
			collisions = col;//Make this empty
			return collisions;
		}



		if (object->scene->physicsManager->RayCast(startPos, endPos, collisions, false))
		{

			return collisions;
		}


	

		return collisions;
	}

	void HitInteration(std::vector<sCollision_RayTriangleInMesh> collisions)
	{
		/*if (speed.length() < 0.001f)
		{
			speed = glm::vec3(0.f, 0.f, 0.f);
			return;
		}*/


		// Use the surface normal of the first collision for bounce calculation
		glm::vec3 collisionNormal = collisions[0].vecTriangles[0].normal;
		collisionNormal = glm::normalize(collisionNormal);

		// Reflect the speed vector over the collision normal
		glm::vec3 reflectedSpeed = glm::reflect(speed, collisionNormal);

		// Apply bounciness factor to the reflected speed
		speed =  reflectedSpeed* bounciness;


	}

	void Update() override
	{

		std::vector<sCollision_RayTriangleInMesh> collisions = CheckHit(glm::normalize(gravityAcceleration));

		//	GetButtonDown(object->scene->window, collisions);
		

		if (collisions.size() > 0)
		{
			HitInteration(collisions);
		}
		
		if (isPhysicsEnabled == false) return;

			ApplyAcceleration();
			ApplySpeed();
		
	}

	bool CheckGround()
	{

		std::vector<sCollision_RayTriangleInMesh> collisions;
		glm::vec3 gravityDirection = glm::normalize(gravityAcceleration);
		glm::vec3 startPos = object->mesh->positionXYZ;
		glm::vec3 endPos = startPos + gravityDirection * baseRayCastLength;





		if (object->scene->physicsManager->RayCast(startPos, endPos, collisions, false))
		{

			// Update position to avoid getting "stuck" at the collision point

			//HATE THIS
			
			return true;
		}

		return false;

	}

};