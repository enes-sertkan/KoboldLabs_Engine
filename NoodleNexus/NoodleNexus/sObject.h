#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "sMesh.h"

struct sObject
{
public:
	std::string name;
	sMesh* mesh;
};