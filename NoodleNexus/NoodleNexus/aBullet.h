#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"
#include "aProjectileMovement.hpp"
#include "aParticleEmitter .h"

class aBullet : public Action {
private:


    float lifetime = 0.0f;
    const float maxLifetime = 3.0f; // Self-destruct after 3 seconds
   

public:
    float damage = 1;
    LabAttackFactory* factory = nullptr;
    aParticleEmitter* particles = nullptr;
    aProjectileMovement* projectile = nullptr;

    virtual void Start() {
        lifetime = 0.0f; // Reset timer on spawn
    }

    virtual void Update() {
       
        // 2. Lifetime tracking
        lifetime += object->scene->deltaTime;
        if (lifetime >= maxLifetime) {

            DestroyBullet();
            return;
           // Self-destruct after timeout
        }

        if (!projectile || !factory) return;

        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec3 pos = object->GetWorldPosition() + glm::normalize(projectile->speed) * 1.35f;

        if (pos.y < factory->maze->floorHeight)
        {
            DestroyBullet();
            return;
        }
            
        glm::vec3 sadFix = pos ;
        sadFix.x += factory->maze->TILE_SIZE * 0.45;
        sadFix.z += factory->maze->TILE_SIZE * 0.65;
    //    std::cout << "SAD FIX: " << sadFix.x << " " << sadFix.y << sadFix.z << std::endl;
        glm::vec2 mazePos = factory->maze->WorldToGrid(sadFix);


        if (factory->maze->IsWall(mazePos.y, mazePos.x))
        {

            DestroyBullet();
            return;
        }


    }


    void DestroyBullet()
    {
        SpawnHitEffect();
        if (particles)
        {
            particles->object->RemoveParent();
            particles->spawnActive = false;
            particles->destroyOnNoParticles = true;
        }

        object->Destroy();

    }



    void SpawnHitEffect()
    {
        if (!factory) return;
        Object* effectEmiiter = factory->scene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", object->GetWorldPosition() + glm::normalize(projectile->speed) * 0.52f , 1.f, glm::vec3(0.f), false, glm::vec4(0.5f, 0.4f, 0.4f, 1.f), false, factory->scene->sceneObjects);




        effectEmiiter->mesh->isParticleEmitter = true;
        effectEmiiter->mesh->metal = 0.1;
        effectEmiiter->mesh->bDoNotLight = true;
        aParticleEmitter* particleEmmitter = new aParticleEmitter();
        particleEmmitter->spawnRate = 0.f;
        particleEmmitter->destroyOnNoParticles = true;
        particleEmmitter->spawnActive = false;
        particleEmmitter->particlesToSpawn = 5.f;
        particleEmmitter->minDirection = glm::vec3(-1, -1, -1);
        particleEmmitter->maxDirection = glm::vec3(1, 1, 1);
        particleEmmitter->colorEnd = glm::vec4(0.8, 0.8, 0.2,1.f);
        particleEmmitter->colorStart = glm::vec4(1, 0.647, 0.2,1.f);
        particleEmmitter->velocityRange = glm::vec2(5, 15);
        particleEmmitter->sizeStart = 0.1f;
        particleEmmitter->sizeEnd = 0.f;
        particleEmmitter->lifeTimeRange = glm::vec2(0.5f, 1.1f);

        factory->scene->AddActionToObj(particleEmmitter, effectEmiiter);
        particleEmmitter->Start();

    }
};