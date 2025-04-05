#pragma once
#include "GLCommon.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

struct sSTCollider
{
	bool isOn=false;
	glm::vec3 position;
	float radius = 0.5;
	float blendingRadius = 1.f;
};


struct sSTData
{
	int shellCount = 150;
	float verticalTightening = 0.5f;
	float verticalExponent = 0.1;
	float shellLength = 0.11f;

};