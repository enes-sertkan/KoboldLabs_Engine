#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"
#include "Transform.h"
#include <vector>
#include "Action.h"


class Scene;


//TODO: Change File Name, bc now it's a class.
class Object
{

public:
	std::string name;
	sMesh* mesh;
	Scene* scene;
	Transform* startTranform = new Transform;
	bool isTemporary = false;
	bool isCollisionStatic = false;
	std::vector<std::string> tags;
	bool isActive = true;
	//GLuint program;

	void Destroy();

	std::vector<Action*> actions;
	std::vector<Object*> sceneObjects;

	//void Start()
	//{
	//	for (Action* action : actions)
	//	{
	//		action->Start();
	//	}
	//}

	//void Update()
	//{
	//	for (Action* action : actions)
	//	{
	//		action->Update();
	//	}
	//}

	// New Methods for Position Manipulation
	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& newPosition);
	
};
