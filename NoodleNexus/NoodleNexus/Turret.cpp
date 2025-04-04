#include "cTurret.h"
#include "aTurretHead.hpp"
#include "aTurretNeck.h"
#include "aTurretBody.h"
void Turret::RebuildTurret(sTurretCofig* config)
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
        body->action->turret = this;
        neck->object->mesh->positionXYZ = body->connectionTransform;
        neck->object->startTranform->position = body->connectionTransform;
        neck->action->turret = this;
        if (neck && neck->object) {
            head = factory->SpawnTurretHead(neck->connectionTransform, config->headID);
            neck->headConnection->AddChild(head->object);
            head->object->mesh->positionXYZ = glm::vec3(0);
            head->object->startTranform->position = glm::vec3(0);

            head->action->turret = this;
        }
    }
}