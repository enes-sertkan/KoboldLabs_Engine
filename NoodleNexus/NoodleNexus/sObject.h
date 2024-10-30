#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"
#include "Transform.h"
#include <vector>


//TODO: Change File Name, bc now it's a class.
class Object
{

public:
	std::string name;
	sMesh* mesh;
	Transform* startTranform = new Transform;
	bool isTemporary = false;



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