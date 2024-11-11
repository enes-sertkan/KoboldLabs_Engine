#pragma once
#include "Action.h";
#include "sObject.h";
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#include "GLCommon.h"


class RayCastPhysics : public Action
{
public:
	glm::vec3 gravityAcceleration = { 0.f,0.f,0.f };
	glm::vec3 speed = { 0.f,0.f,0.f };

	float baseRayCastLength = 0;
	float speedLengthMultiplier = 1;
	float bounciness = 0.95;
	glm::vec3 airFriction = glm::vec3(1, 1, 1);

	float yChange = 10.f;

	void ApplySpeed()
	{
		object->mesh->positionXYZ.x += speed.x;
		object->mesh->positionXYZ.y += speed.y;
		object->mesh->positionXYZ.z += speed.z;

	}

	void ApplyAcceleration()
	{
		speed.x += gravityAcceleration.x * object->scene->deltaTime;
		speed.y += gravityAcceleration.y * object->scene->deltaTime;
		speed.z += gravityAcceleration.z * object->scene->deltaTime;


		speed.x *= 1 - airFriction.x * object->scene->deltaTime;
		speed.y *= 1 - airFriction.x * object->scene->deltaTime;
		speed.z *= 1 - airFriction.x * object->scene->deltaTime;

	}

	void AddAcceleration(glm::vec3 acceleration)
	{
		speed.x += acceleration.x * object->scene->deltaTime;
		speed.y += acceleration.y * object->scene->deltaTime;
		speed.z += acceleration.z * object->scene->deltaTime;


	}

	void SetSpeed(glm::vec3 newSpeed)
	{
		speed.x = newSpeed.x * object->scene->deltaTime;
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


		if (collisions.size()>0)
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			AddAcceleration(glm::vec3(0.f, 4.f, 0.f));



	}


	


	glm::vec3 gravityComponent(const glm::vec3& speed, const glm::vec3& down) {
		
		glm::vec3 gravityDirection = glm::normalize(down);

		float gravitySpeed = glm::dot(speed, gravityDirection);

		return gravitySpeed * gravityDirection;
	}



	std::vector<sCollision_RayTriangleInMesh> CheckHit()
	{
		glm::vec3 normSpeed;


		normSpeed = glm::normalize(speed + gravityAcceleration);
		normSpeed = normSpeed * (glm::length(speed*speedLengthMultiplier * object->scene->deltaTime) + baseRayCastLength);
		glm::vec3 startPos = object->mesh->positionXYZ;
		glm::vec3 endPos = startPos + normSpeed;

		std::vector<sCollision_RayTriangleInMesh> collisions;

		if (object->scene->physicsManager->RayCast(startPos, endPos, collisions, false))
		{

			return collisions;
		}

		return collisions;
	}

	void HitInteration(std::vector<sCollision_RayTriangleInMesh> collisions)
	{
		if (speed.length() < 0.001f)
		{
			speed = glm::vec3(0.f, 0.f, 0.f);
			return;
		}

		
		speed -= 2.f*gravityComponent(speed, gravityAcceleration)*bounciness;

			
		object->mesh->positionXYZ.y = collisions[0].vecTriangles[0].intersectionPoint.y + baseRayCastLength;


		//glm::vec3(0.f,-1.f,0.f);// -speed*0.8f;
		//object->mesh->positionXYZ = collisions[0].theRay.endXYZ;
 }

	void Update() override
	{

		std::vector<sCollision_RayTriangleInMesh> collisions = CheckHit();

	//	GetButtonDown(object->scene->window, collisions);

		if (collisions.size() > 0)
		{
			HitInteration(collisions);
		}
		else
		{
			ApplyAcceleration();
			ApplySpeed();
		}

	}


};