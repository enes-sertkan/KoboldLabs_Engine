#include "UsableItem.hpp"
#include "vector"
#include "PhysicsManager.h"
#include "aChangeColorOverTime.hpp"
#include "aSizeUpThenDown.hpp"


class Laser : public UsableItem
{
public:
	glm::vec3 target;
	glm::vec3 cameraPos;
	float lazerLength;

	void Use() override
	{
		std::cout << "LAZER TIME" << std::endl;

		std::vector<sCollision_RayTriangleInMesh> collisions;
		if (scene->physicsManager->RayCast(cameraPos, target, collisions, false))
		{
		
			glm::vec3 hitPoint = collisions[0].vecTriangles[0].intersectionPoint;
			glm::vec4 startColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
			glm::vec4 endColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);


			scene->DrawRay(position, hitPoint,  program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);
			Object* heatSphere = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_xyz_n_uv.ply", hitPoint, 0.f, glm::vec3(0.f, 0.f, 0.f), true, startColor, false, scene->sceneObjects);
			heatSphere->isTemporary = true;


			aChangeColorOverTime* changeColorOverTimeAction = new aChangeColorOverTime();
			changeColorOverTimeAction->startColour = startColor;
			changeColorOverTimeAction->endColour = endColor;
			changeColorOverTimeAction->secondsToChange = 1.f;
			scene->AddActionToObj(changeColorOverTimeAction, heatSphere);


			aSizeUpThenDown* sizeUpDownAction = new aSizeUpThenDown();
			sizeUpDownAction->scales.push_back(0.f);
			sizeUpDownAction->scales.push_back(1.5f);
			sizeUpDownAction->scales.push_back(0.7f);
			sizeUpDownAction->scales.push_back(0.f);
			
			sizeUpDownAction->secondsToChange.push_back(0.1f);
			sizeUpDownAction->secondsToChange.push_back(1.f);

			scene->AddActionToObj(sizeUpDownAction, heatSphere);
			
		}
		else
		{
			scene->DrawRay(position, target, program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);
		}
		
	}
};