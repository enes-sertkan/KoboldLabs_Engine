#pragma once
//#include "glm/vec3.hpp"

struct sSTCollider
{
	//glm::vec3 position;
	float radius = 0.5;;
	float blendingRadius = 0.5f;
};


struct sSTData
{
	int shellCount = 256;
	float verticalTightening = 0.5;
	float verticalExponent = 3.f;
	float shellLength = 0.02;

};