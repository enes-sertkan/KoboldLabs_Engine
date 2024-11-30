#pragma once


#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>


class aDrawAim : public Action
{
public:
	float speed = 30.f;
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

	void DrawRay2(glm::vec3 pos, glm::vec3 posEnd, GLuint program)
	{
		float distance = glm::distance(pos, posEnd);
		glm::vec3 direction = glm::normalize(posEnd - pos);
		glm::vec3 movingPoint = pos;
		while (glm::distance(pos, movingPoint) < distance)
		{
			// Move the next ball 0.1 times thw normalized camera direction
			movingPoint += (direction * 0.10f);
			DrawDebugSphere(movingPoint, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.01f, program, object->scene);
		}

	}


	void UpdateAim()
	{

		glm::vec3 startPos = object->scene->fCamera->getEyeLocation();

		glm::vec3 endPos = startPos + glm::normalize(object->scene->fCamera->getTargetRelativeToCamera())*1000.f;

		startPos.y -= 5;

		std::vector<sCollision_RayTriangleInMesh> collisions;


		;
		if (object->scene->physicsManager->RayCast(startPos, endPos, collisions, false))
		{
			DrawDebugSphere(collisions[0].vecTriangles[0].intersectionPoint , glm::vec4(1.f, 0.f, 0.f, 1.f),10.f, program, object->scene);

		}
	}


	
	void Update() override
	{
		UpdateAim();
	}
};