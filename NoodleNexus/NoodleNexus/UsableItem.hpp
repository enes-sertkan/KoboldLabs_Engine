
#pragma once


#include "Scene.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>


class UsableItem
{
	
public:

	Scene* scene = nullptr;
	glm::vec3 position;
	GLuint program;

	virtual void Use()
	{
		std::cout << "NO LAZER TIME" << std::endl;
	}
};