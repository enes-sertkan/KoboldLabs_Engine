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
	std::string textureName;
	cFBO_RGB_depth myFBO;
	float drawistance = 15.f;
	float FOV = 60.f;

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
		Camera* camera = new Camera( object->mesh->positionXYZ, object->mesh->rotationEulerXYZ, glm::vec2(1920, 1080));
		camera->fov = FOV;
		camera->drawDistance = drawistance;
		camera->scene = object->scene;
		DrawCameraViewToFramebufer(camera, 0, myFBO.ID);
		//printf("WOW, you are UPDATING\n");




	}

};


