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

};


