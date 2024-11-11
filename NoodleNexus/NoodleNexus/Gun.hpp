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
#include "aRayCastPhysics.h"

class Gun : public UsableItem
{
public:
    void Use() override
    {

        std::cout << "Firing projectile!" << std::endl;

        glm::vec3 direction = glm::normalize(scene->fCamera->getTargetRelativeToCamera());


          
            Object* projectile = scene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", position, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec4(0.1, 0.1, 0.1, 1), true, scene->sceneObjects);
            projectile->isTemporary = true;


            // Attach movement, trail, and explosion actions
      /*      aLinearProjectileMovement* movement = new aLinearProjectileMovement();
            movement->direction = direction;
            movement->speed = 35.f;
            movement->acceleration = 20.f;
            scene->AddActionToObj(movement, projectile);*/

            aExhaustTrail* exhaust = new aExhaustTrail();
            scene->AddActionToObj(exhaust, projectile);

            aExplodeOnRayCollision* explodeOnRayCollision = new aExplodeOnRayCollision();
            explodeOnRayCollision->rayDirection = direction;
            explodeOnRayCollision->rayLenght = 2.5f;
            scene->AddActionToObj(explodeOnRayCollision, projectile);


            aExplodeOnRayCollision* explodeOnRayCollision2 = new aExplodeOnRayCollision();
            explodeOnRayCollision2->rayDirection = glm::vec3(0, -1, 0);
            explodeOnRayCollision2->rayLenght = 2.5f;
            scene->AddActionToObj(explodeOnRayCollision2, projectile);


            RayCastPhysics* phys = new RayCastPhysics;
            phys->gravityAcceleration.y = -2;
            phys->baseRayCastLength = 2.3f;
            phys->speed = direction * 6.f;
            phys->bounciness = 0.8f;
            scene->AddActionToObj(phys, projectile);

    
    }

};
