#pragma once
enum eTurretBodyID
{
	STANDARTBODY
};

enum eTurretNeckID
{
	STANDARTNECK
};

enum eTurretHeadID
{
	STANDARTHEAD
};

struct sTurretCofig
{
	eTurretBodyID bodyID;
	eTurretNeckID neckID;
	eTurretHeadID headID;
};



class cTurretPart {
public:
	Object* object;
	Action* action;
	glm::vec3 connectionTransform = glm::vec3(0);

};

class cTurretHead : public cTurretPart {
public:
	eTurretHeadID ID;

};

class cTurretNeck : public cTurretPart {
public:
	eTurretNeckID ID;

};

class cTurretBody : public cTurretPart {
public:
	eTurretBodyID ID;

};