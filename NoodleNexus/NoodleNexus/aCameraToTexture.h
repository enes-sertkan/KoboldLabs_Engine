#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "cFBO_RGB_depth.hpp"
#include "Camera.hpp"
void DrawCameraViewToFramebufer(Camera* camera, int programID, int framebufferID);




//#include "sObject.h"

class Object;


class CameraToTexture : public Action
{
private:

public:
	cFBO_RGB_depth myFBO;


	virtual void Start()
	{
	//Create and allocate texture

		std::string error;

		// Initialize with desired resolution
		if (!myFBO.init(1024, 768, error, "camera1", object->scene->textureManager)) {
		//	std::cerr << "FBO Error: " << error << std::endl;
		}
	
	}
	virtual void Update()
	{
		std::cout << object->mesh->positionXYZ.x << " " << std::endl;
		Camera* camera = new Camera( object->mesh->positionXYZ, object->mesh->rotationEulerXYZ, glm::vec2(1024.f, 768.f));
		camera->scene = object->scene;
		DrawCameraViewToFramebufer(camera, 0, myFBO.ID);
		//printf("WOW, you are UPDATING\n");




		// Left pitches negative (rotate around Y a negative value)
		if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			object->mesh->rotationEulerXYZ.x += 0.1f;
		}
		if (glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			object->mesh->rotationEulerXYZ.x -= 0.1f;
		}

		// Left pitches negative (rotate around Y a negative value)
		if (glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			object->mesh->rotationEulerXYZ.y += 0.1f;
		}
		if (glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			object->mesh->rotationEulerXYZ.y -= 0.1f;
		}

	}

};


