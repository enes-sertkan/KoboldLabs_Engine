#pragma once


class aTurretHead;
class aTurretBody;
class aTurretNeck;
class aTurretHead;

enum eTurretBodyID
{
	STANDARTBODY,
	TRIPLEBODY,
	ROCKETBODY
};

enum eTurretNeckID
{
	STANDARTNECK,
	AIMNECK,
	ROCKETNECK
};

enum eTurretHeadID
{
	STANDARTHEAD,
	TRIPLEHEAD,
	ROCKETHEAD
};

template<typename T>
struct EnumInfo {
	// Default implementation - override for each enum
	static int count() { return 3; }
};

// Specialize for each turret part enum
template<>
struct EnumInfo<eTurretBodyID> {
	static int count() { return 3; } // Number of body types
};

template<>
struct EnumInfo<eTurretNeckID> {
	static int count() { return 3; } // STANDARTNECK and AIMNECK
};

template<>
struct EnumInfo<eTurretHeadID> {
	static int count() { return 3; } // Number of head types
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
	std::vector<glm::vec3> barrelsPos;


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