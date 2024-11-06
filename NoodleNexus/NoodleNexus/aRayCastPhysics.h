#pragma once
#include "Action.h";
#include "sObject.h";
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class RayCastPhysics : public Action
{
public:
	glm::vec3 gravityAcceleration = { 0.f,0.f,0.f };
	glm::vec3 speed = { 0.f,0.f,0.f };

	float baseRayCastLength = 0;
	float speedLengthMultiplier = 1;


	void ApplySpeed()
	{
		object->mesh->positionXYZ.x += speed.x;
		object->mesh->positionXYZ.y += speed.y;
		object->mesh->positionXYZ.z += speed.z;


	}

	void ApplyAcceleration()
	{
		speed.x += gravityAcceleration.x;
		speed.y += gravityAcceleration.y;
		speed.z += gravityAcceleration.z;

	}

	std::vector<sCollision_RayTriangleInMesh> CheckHit()
	{
		//normalize speed direction
		glm::vec3 normSpeed;
		//if (glm::length(speed) == 0)
		//normSpeed = glm::vec3(0, 0, 0);
		//else 
		//{
		//	
		//}

		normSpeed = glm::normalize(speed + gravityAcceleration);
		normSpeed = normSpeed * (glm::length(speed*speedLengthMultiplier) + baseRayCastLength);
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

		speed = -speed * 0.7f;//glm::vec3(0.f,-1.f,0.f);// -speed*0.8f;
		//object->mesh->positionXYZ = collisions[0].theRay.endXYZ;
 }

	void Update() override
	{

		std::vector<sCollision_RayTriangleInMesh> collisions = CheckHit();

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