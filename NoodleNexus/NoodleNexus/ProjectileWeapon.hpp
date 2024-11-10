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
#include "aExplosion.hpp"

class ProjectileWeapon : public UsableItem
{
public:
    glm::vec3 target;

    void Use() override
    {
        std::cout << "Firing projectile!" << std::endl;

        // Create projectile object
        Object* projectile = scene->GenerateMeshObjectsFromObject("ProjectileModelPath", position, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), false, glm::vec4(0.2, 0.6, 0.4, 1), true, scene);

        // Attach actions to the projectile
        aLinearProjectileMovement* movement = new aLinearProjectileMovement();
        movement->target = target;
        scene->AddActionToObj(movement, projectile);

        aExhaustTrail* exhaust = new aExhaustTrail();
        scene->AddActionToObj(exhaust, projectile);

        aExplosion* explosion = new aExplosion();
        explosion->explosionColor = glm::vec4(1.0f, 0.6f, 0.0f, 1.0f); // Customize for each weapon
        scene->AddActionToObj(explosion, projectile);
    }
};
