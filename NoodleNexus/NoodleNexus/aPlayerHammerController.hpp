#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aIconNearObject.h"
#include "aHammerAction.hpp"
#include "aScoreManager.h"
#include "aToggleHammerVisibility.hpp"
#include <GLFW/glfw3.h>

class aPlayerHammerController : public Action
{
public:
	std::string equipTarget;
	bool isEquipped = false;
	aHammerAction* hammerAction;
	aScoreManager* scoreManager = new aScoreManager();



	void Update() override
	{
		for (Object* obj : object->scene->sceneObjects)
		{
			if (obj->name == "Hammer")
			{
				if (object->scene->IsNearby(equipTarget, object->mesh->positionXYZ))
				{
					CreateHammerIcon(glm::vec3(0.0f, 1.0f, 0.5f), 0.07f);
					//object->mesh->bIsVisible = false;  // Hide the item

					isEquipped = true;
					//object->mesh->objectColourRGBA = glm::vec4(0.3, 0.4, 0.1, 1.0f);


					obj->Destroy();
				}
			}
		}

		if (!isEquipped) return;

		if (glfwGetKey(object->scene->window, GLFW_KEY_E) == GLFW_PRESS)
		{

			hammerAction->isOn = true;
		}
		else
		{
			hammerAction->isOn = false;

		}
	}

	Object* CreateHammerIcon(glm::vec3 position, float scale = 0.07f)
	{
		glm::vec3 rotation = glm::vec3(45, -90, 0);
		glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.1f, 1.0f);
		//create new object with hummer model
		Object* hammer = object->scene->GenerateMeshObjectsFromObject(
			"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply",
			position,
			scale,
			rotation,
			true,
			color,
			false,
			object->scene->sceneObjects

		);
		hammer->name = "Hammer";

		//use the iconaction, give model name, and everything.

		aIconNearObject* iconAction = new aIconNearObject();
		Object* player = object->scene->sceneObjects[2];
		hammerAction = new aHammerAction();

		GLFWwindow* glfwWindow = object->scene->window;

		aToggleHammerVisibility* toggleAction = new aToggleHammerVisibility(glfwWindow);
		toggleAction->hammerObject = hammer;


		if (scoreManager)
		{
			hammerAction->scoreManager = scoreManager;
		}

		iconAction->isOn = true;
		iconAction->modelName = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply";
		iconAction->offset = glm::vec3(0.0f, 9.0f, 2.0f);
		iconAction->objectToFollow = player;


		object->scene->AddActionToObj(iconAction, hammer);
		object->scene->AddActionToObj(hammerAction, hammer);
		object->scene->AddActionToObj(toggleAction, hammer);


		hammer->isTemporary = true;

		return hammer;

	}
};