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

	void RebuildTurret(sTurretCofig* config)
	{
		if (body && body->object) {

			position = body->object->mesh->positionXYZ;
			body->object->Destroy();
		}

		if (neck && neck->object) 
			neck->object->Destroy();

		if (head && head->object)
			head->object->Destroy();

		body =  factory->SpawnTurretBody(position, config->bodyID);

		neck =  factory->SpawnTurretBody(body->connectionTransform, config->bodyID);
		body->object->AddChild(neck->object);
		neck->object->mesh->positionXYZ = body->connectionTransform;

		head = factory->SpawnTurretBody(neck->connectionTransform, config->bodyID);
		neck->object->AddChild(head->object);
		head->object->mesh->positionXYZ = neck->connectionTransform;

	}
};


