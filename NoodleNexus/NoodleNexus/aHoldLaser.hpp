#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "PhysicsManager.h"
#include <vector>
#include "aExplosion.h"
#include "Scene.hpp"

class aHoldLaser : public Action
{public:
	glm::vec3 startPos;
	glm::vec3 endPos;
	GLuint program;
	double timer = 0;
	double timeToHold = 0.5;

	void DrawRay(glm::vec3 pos, glm::vec3 posEnd)
	{
		float distance = glm::distance(pos, posEnd);
		glm::vec3 direction = glm::normalize(posEnd - pos);
		glm::vec3 movingPoint = pos;
		while (glm::distance(pos, movingPoint) < distance)
		{
			// Move the next ball 0.1 times the normalized camera direction
			movingPoint += (direction * 1.f);
			DrawDebugSphere(movingPoint, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.05f, program);
		}

	}

	void Update() override
	{
		DrawRay(startPos, endPos);
		timer += object->scene->deltaTime;

		if (timer > timeToHold)
		{
			object->Destroy();
			return;
		}

	}
};
