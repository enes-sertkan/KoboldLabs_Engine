#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "UsableItem.hpp"
#include "aLinearProjectileMovement.hpp"
#include "aExhaustTrail.hpp"
#include "aExplodeOnRayCollision.hpp"
#include "aChangeColorOverTime.hpp"
#include "aSizeUpThenDown.hpp"

class Gun : public UsableItem
{
public:
    void Fire(glm::vec3 target)
    {

        std::cout << "Firing projectile!" << std::endl;

        std::vector<sCollision_RayTriangleInMesh> collisions;
        if (scene->physicsManager->RayCast(position, target, collisions, false))
        {
            glm::vec3 hitPoint = collisions[0].vecTriangles[0].intersectionPoint;
            glm::vec4 startColor = glm::vec4(1.f, 0.f, 0.f, 1.f);
            glm::vec4 endColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);

            // Create projectile object
            scene->DrawRay(position, hitPoint, program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);
            Object* projectile = scene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", position, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec4(0.1, 0.1, 0.1, 1), true, scene->sceneObjects);
            projectile->isTemporary = true;


            // Attach movement, trail, and explosion actions
            aLinearProjectileMovement* movement = new aLinearProjectileMovement();
            movement->target = target;
            scene->AddActionToObj(movement, projectile);

            aExhaustTrail* exhaust = new aExhaustTrail();
            scene->AddActionToObj(exhaust, projectile);

            aExplodeOnRayCollision* explosion = new aExplodeOnRayCollision();
            scene->AddActionToObj(explosion, projectile);

        }
        else
        {
            scene->DrawRay(position, target, program, glm::vec4(100.f, 1.f, 1.f, 1.f), false, 0.1f);
        }
    }

};
