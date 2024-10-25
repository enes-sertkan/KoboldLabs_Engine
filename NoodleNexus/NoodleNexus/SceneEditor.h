#pragma once
#include "Scene.hpp"
#include "Transform.h"
#include "GLCommon.h"
#include "sObject.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

class SceneEditor
{
public:
	void Start();

	Scene* scene; //Current scene

	Object* selectedObject;

	int objIndex=0;

	std::string pathToSelectBox;



	void Update();

	
	void HandleInputAsync(GLFWwindow* window);

	void SaveScene(std::string name);

	Object* PickNextObject();
	
};

