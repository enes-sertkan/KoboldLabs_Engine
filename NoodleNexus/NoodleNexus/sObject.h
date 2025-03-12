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

	// Function to get an action of a specific type
	template <typename T>
	T* GetActionOfType() {
		for (auto action : actions) {
			if (dynamic_cast<T*>(action) != nullptr) {
				return dynamic_cast<T*>(action);
			}
		}
		return nullptr; // Action not found
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

	//// Waypoint-related members
	//std::vector<glm::vec3> waypoints;  // List of waypoints for the object
	//size_t currentWaypoint = 0;       // Current waypoint index
	//float movementSpeed = 1.0f;       // Speed of movement

	//// Bezier curve-related members
	//std::vector<glm::vec3> bezierControlPoints; // Bezier control points
	//float t = 0.0f;                              // Parameter t for Bezier
	//float bezierSpeed = 0.1f;                    // Speed of Bezier traversal

	
};
