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

	int health = 100;        
	bool isDestructible = false;

	void TakeDamage(int damageAmount) {
		if (!isDestructible) return;  

		health -= damageAmount;

		if (health <= 0) {
			health = 0;
			Destroy(); 
		}
	}

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
