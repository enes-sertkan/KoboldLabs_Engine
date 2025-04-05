#pragma once
#include "sObject.h"
#include "Action.h"

#include "TurretConfig.h"
#include "LabAttackFactory.h"




class Turret {
public:


	cTurretHead* head;
	cTurretNeck* neck;
	cTurretBody* body;
	LabAttackFactory* factory;
	glm::vec3 position;

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




};


