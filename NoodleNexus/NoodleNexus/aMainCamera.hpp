#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "cFBO_RGB_depth.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
void DrawCameraViewToFramebufer(Camera* camera, int programID, int framebufferID);




//#include "sObject.h"

class Object;


class MainCamera : public Action
{
private:

public:
	std::string textureName="main_camera";
	cFBO_RGB_depth myFBO;


	virtual void Start()
	{
		//Create and allocate texture

		std::string error;

		// Initialize with desired resolution
		if (!myFBO.init(1920, 1080, error, textureName, object->scene->textureManager)) {
			//	std::cerr << "FBO Error: " << error << std::endl;
		}

	}
	virtual void Update()
	{
		//	std::cout << object->mesh->positionXYZ.x << " " << std::endl;
		Camera* camera = new Camera(object->mesh->positionXYZ, object->mesh->rotationEulerXYZ, glm::vec2(1024.f, 768.f));
		camera->scene = object->scene;
		DrawCameraViewToFramebufer(object->scene->fCamera->getCameraData(), 0, myFBO.ID);
		//printf("WOW, you are UPDATING\n")

	}

};


