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

void Turret::RebuildTurretGhost(sTurretCofig* config)
{
    // Helper function to update part transparency
    auto updateTransparency = [](cTurretPart* part) {
        if (part && part->object) {
            part->object->mesh->transperency = 0.5f;
        }
        };

    glm::vec3 oldPos = glm::vec3(0);


    // Handle Body
    if (!body || body->ID != config->bodyID) {
      
        if (body && body->object) {
            oldPos = body->object->mesh->positionXYZ;
            body->object->Destroy();
        }
        body = factory->SpawnTurretBody(oldPos, config->bodyID);
        if (body && body->object) {
            body->object->mesh->transperency = 0.5f;
            position = oldPos; // Update position from new body
            body->object->actions.clear();
            updateTransparency(body);
        }
    }
    else if (body->object) {
        // Update existing body position
   
   
        updateTransparency(body);
    }

    // Handle Neck
    if (body && body->object) {
        const bool neckNeedsUpdate = !neck || neck->ID != config->neckID;

        if (neckNeedsUpdate) {
            if (neck && neck->object) neck->object->Destroy();
            neck = factory->SpawnTurretNeck(body->connectionTransform, config->neckID);

            if (neck && neck->object) {
                body->object->AddChild(neck->object);
                neck->object->mesh->positionXYZ = body->connectionTransform;
                neck->object->startTranform->position = body->connectionTransform;
                updateTransparency(neck);
            }
        }
        else if (neck->object) {
            // Update existing neck position
            body->object->AddChild(neck->object);
            neck->object->actions.clear();
            neck->object->mesh->positionXYZ = body->connectionTransform;
            updateTransparency(neck);
        }
    }

    // Handle Head
    if (neck && neck->object) {
        const bool headNeedsUpdate = !head || head->ID != config->headID;

        if (headNeedsUpdate) {
            if (head && head->object) head->object->Destroy();
            head = factory->SpawnTurretHead(neck->connectionTransform, config->headID);


            if (head && head->object) {
                head->object->actions.clear();
                neck->headConnection->AddChild(head->object);
                head->object->mesh->positionXYZ = glm::vec3(0);
                head->object->startTranform->position = glm::vec3(0);
                updateTransparency(head);
            }
        }
        else if (head->object) {
            neck->headConnection->AddChild(head->object);
            // Update existing head position
            head->object->mesh->positionXYZ = neck->connectionTransform;
            updateTransparency(head);
        }
    }

    // Clear any actions for ghost turret
    if (body) body->action = nullptr;
    if (neck) neck->action = nullptr;
    if (head) head->action = nullptr;
}




sTurretCofig* Turret::GetConfig() const
{
    sTurretCofig* config = new  sTurretCofig();

    config->bodyID = body ? body->ID : eTurretBodyID::STANDARTBODY;
    config->neckID = neck ? neck->ID : eTurretNeckID::STANDARTNECK;
    config->headID = head ? head->ID : eTurretHeadID::STANDARTHEAD;

    return config;
}