#pragma once


class aTurretHead;
class aTurretBody;
class aTurretNeck;
class aTurretHead;

enum eTurretBodyID
{
	STANDARTBODY
};

enum eTurretNeckID
{
	STANDARTNECK,
	AIMNECK
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
	glm::vec3 connectionTransform = glm::vec3(0);

};

class cTurretHead : public cTurretPart {
public:
	aTurretHead* action;
	eTurretHeadID ID;

};

class cTurretNeck : public cTurretPart {
public:
	aTurretNeck* action;
	Object* headConnection;
	eTurretNeckID ID;

};

class cTurretBody : public cTurretPart {
public:
	aTurretBody* action;
	eTurretBodyID ID;

};