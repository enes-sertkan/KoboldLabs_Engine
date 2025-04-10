#pragma once
#include "sObject.h"
#include "Action.h"

#include "TurretConfig.h"
#include "LabAttackFactory.h"
#include "aAgent.h"
#include "aParticleEmitter .h"


class Turret {
public:


	cTurretHead* head;
	cTurretNeck* neck;
	cTurretBody* body;
	LabAttackFactory* factory;
	glm::vec3 position;
	Agent* target = nullptr;
	bool constantShooting = false;

    Turret() : head(nullptr), neck(nullptr), body(nullptr), factory(nullptr), position(0) {}

	void RebuildTurret(sTurretCofig* config);
	void RebuildTurretGhost(sTurretCofig* config);

	sTurretCofig* GetConfig() const;
private:
	void UpdatePartPosition(cTurretPart* part, const glm::vec3& newPosition) {
		if (part && part->object) {
			part->object->mesh->positionXYZ = newPosition;
			part->object->startTranform->position = newPosition;
		}
	}


    void SpawnEffect()
    {
        if (!factory) return;
        Object* effectEmiiter = factory->scene->GenerateMeshObjectsFromObject("assets/models/Cube_xyz_n_uv.ply", body->object->GetWorldPosition() , 1.f, glm::vec3(0.f), false, glm::vec4(0.5f, 0.4f, 0.4f, 1.f), false, factory->scene->sceneObjects);

        
        effectEmiiter->mesh->bIsWireframe = true;
        effectEmiiter->mesh->isParticleEmitter = true;
        effectEmiiter->mesh->metal = 0.1;
        effectEmiiter->mesh->bDoNotLight = true;
        aParticleEmitter* particleEmmitter = new aParticleEmitter();
        particleEmmitter->spawnRate = 0.f;
        particleEmmitter->destroyOnNoParticles = true;
        particleEmmitter->particlesToSpawn = 10.f;
        particleEmmitter->minDirection = glm::vec3(-1, 0.1, -1);
        particleEmmitter->maxDirection = glm::vec3(1, 0.7, 1);
        particleEmmitter->colorEnd = glm::vec4(1, 0.647, 0.2, 1.f);;
        particleEmmitter->colorStart = glm::vec4(0.7, 8, 0.3, 1.f);
        particleEmmitter->velocityRange = glm::vec2(4, 8);
        particleEmmitter->sizeStart = 0.1f;
        particleEmmitter->sizeEnd = 0.f;
        particleEmmitter->lifeTimeRange = glm::vec2(0.7f, 1.5f);
        particleEmmitter->damping = glm::vec3(0.93f, 0.9f, 0.93f);
        particleEmmitter->spawnActive = false;
        factory->scene->AddActionToObj(particleEmmitter, effectEmiiter);
        particleEmmitter->Start();

    }


};


