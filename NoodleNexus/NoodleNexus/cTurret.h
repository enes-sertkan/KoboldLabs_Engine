#pragma once
#include "sObject.h"
#include "Action.h"

#include "TurretConfig.h"
#include "LabAttackFactory.h"




class Turret {
public:


	cTurretPart* head;
	cTurretPart* neck;
	cTurretPart* body;
	LabAttackFactory* factory;
	glm::vec3 position;

    Turret() : head(nullptr), neck(nullptr), body(nullptr), factory(nullptr), position(0) {}

    void RebuildTurret(sTurretCofig* config)
    {
        if (body && body->object) {
            position = body->object->mesh->positionXYZ;
            body->object->Destroy();
        }
        if (neck && neck->object) neck->object->Destroy();
        if (head && head->object) head->object->Destroy();

        // Fixed spawning calls
        body = factory->SpawnTurretBody(position, config->bodyID);
        if (body && body->object) {
            neck = factory->SpawnTurretNeck(body->connectionTransform, config->neckID);
            body->object->AddChild(neck->object);
            neck->object->mesh->positionXYZ = body->connectionTransform;
            neck->object->startTranform->position= body->connectionTransform;

            if (neck && neck->object) {
                head = factory->SpawnTurretHead(neck->connectionTransform, config->headID);
                neck->object->AddChild(head->object);
                head->object->mesh->positionXYZ = neck->connectionTransform;
                head->object->startTranform->position= neck->connectionTransform;
            }
        }
    }
};


