#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"
#include "Transform.h"
#include <vector>

class Scene;

class Object
{

public:
	std::string name;
	sMesh* mesh;
	Scene* scene;
	Transform* startTranform = new Transform;
	bool isTemporary = false;

	std::string type;

	Object() : isTemporary(false), mesh(nullptr) {}

	void Destroy();

	//std::vector<Action*> actions;

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
	
};