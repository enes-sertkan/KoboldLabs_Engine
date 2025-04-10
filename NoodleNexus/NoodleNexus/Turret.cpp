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

    SpawnEffect();
}
void Turret::RebuildTurretGhost(sTurretCofig* config)
{
    auto updateTransparency = [](cTurretPart* part) {
        if (part && part->object) {
            part->object->mesh->transperency = 0.5f;
        }
        };

    glm::vec3 oldPos = position;

    // Handle Body
    if (!body || body->ID != config->bodyID) {
        if (body && body->object) {
            oldPos = body->object->mesh->positionXYZ;
        }

        cTurretBody* bodyTemplate = factory->FindTurretBodyTemplate(config->bodyID);
        if (!bodyTemplate) {
            bodyTemplate = factory->FindTurretBodyTemplate(STANDARTBODY);
        }

        if (bodyTemplate) {
            if (!body) {
                body = factory->SpawnTurretBody(oldPos, config->bodyID);
            }
            // Update existing body properties
            body->object->mesh->modelFileName = bodyTemplate->object->mesh->modelFileName;
            body->ID = bodyTemplate->ID;
            body->object->mesh->positionXYZ = oldPos;
            body->connectionTransform = bodyTemplate->connectionTransform;
            updateTransparency(body);
            body->object->actions.clear();
        }
    }
    else if (body->object) {
        updateTransparency(body);
    }

    // Handle Neck
    if (body && body->object) {
        const bool neckNeedsUpdate = !neck || neck->ID != config->neckID;

        if (neckNeedsUpdate) {
            cTurretNeck* neckTemplate = factory->FindTurretNeckTemplate(config->neckID);
            if (!neckTemplate) {
                neckTemplate = factory->FindTurretNeckTemplate(STANDARTNECK);
            }

            if (neckTemplate) {
                if (!neck) {
                    neck = factory->SpawnTurretNeck(body->connectionTransform, config->neckID);
                    body->object->AddChild(neck->object);
                }
                // Update existing neck properties
                neck->object->mesh->modelFileName = neckTemplate->object->mesh->modelFileName;
                neck->ID = neckTemplate->ID;
                neck->object->mesh->positionXYZ = body->connectionTransform;
                neck->object->startTranform->position = body->connectionTransform;
                neck->connectionTransform = neckTemplate->connectionTransform;
                neck->headConnection->mesh->positionXYZ   = neckTemplate->connectionTransform;
                updateTransparency(neck);
                neck->object->actions.clear();
            }
        }
        else if (neck->object) {

            neck->object->mesh->positionXYZ = body->connectionTransform;
            updateTransparency(neck);
        }
    }

    // Handle Head (existing optimized version)
    if (neck && neck->object) {
        const bool headNeedsUpdate = !head || head->ID != config->headID;

        if (headNeedsUpdate) {
            cTurretHead* headTemplate = factory->FindTurretHeadTemplate(config->headID);
            if (!headTemplate) {
                headTemplate = factory->FindTurretHeadTemplate(STANDARTHEAD);
            }

            if (headTemplate) {
                if (!head) {
                    head = factory->SpawnTurretHead(glm::vec3(0), config->headID);
                    neck->headConnection->AddChild(head->object);
                }
                // Update existing head properties
                head->object->mesh->modelFileName = headTemplate->object->mesh->modelFileName;
                head->ID = headTemplate->ID;
                head->object->mesh->positionXYZ = glm::vec3(0);
                head->object->startTranform->position = glm::vec3(0);
                updateTransparency(head);
                head->object->actions.clear();
            }
        }
        else if (head->object) {
           
            head->object->mesh->positionXYZ = glm::vec3(0);
            updateTransparency(head);
        }
    }

    // Clear actions for ghost turret
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