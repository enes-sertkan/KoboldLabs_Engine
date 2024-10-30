#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"
#include "Transform.h"
#include <vector>

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