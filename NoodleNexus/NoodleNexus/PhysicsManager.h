#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
// 
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"
#include <string>


#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

#include <iostream>
#include <algorithm>



struct sTriangleP
{
	glm::vec3 vertices[3];
	glm::vec3 normal;
	// Maybe other things?
	glm::vec3 intersectionPoint;
};

struct sTrianglePMesh
{
	std::string meshModelName;
	std::string meshInstanceName;	// Friendly name?
	std::vector<sTriangleP> vecTriangles;
};

struct sLine
{
	sLine()
	{
		this->startXYZ = glm::vec3(0.0f);
		this->endXYZ = glm::vec3(0.0f);
	}
	glm::vec3 startXYZ;
	glm::vec3 endXYZ;
	float getLength(void)
	{
		return glm::distance(this->endXYZ, this->startXYZ);
	}
};

struct sCollision_RayTriangleInMesh
{
	double timeOfCollision;

	sLine theRay;
	std::string meshInstanceName;
	std::vector<sTriangleP> vecTriangles;
	// Any other things you might want
};

class PhysicsManager
{
public:
	cVAOManager* VAOMan = NULL;

	std::vector<sTrianglePMesh*> meshColliders;
	std::vector<sCollision_RayTriangleInMesh> vec_RayTriangle_Collisions;


	bool AddTriangleMesh(
		std::string meshModelName,
		glm::vec3 meshWorldPosition,
		glm::vec3 meshWorldOrientation,
		float uniformScale);


	bool RayCast(glm::vec3 start, glm::vec3 end,
		std::vector<sCollision_RayTriangleInMesh>& vec_RayTriangle_Collisions,
		bool bIgnoreBackFacingTriangles = true);
	bool bLineSegment_TriangleCollision(sLine theLine, sTriangleP theTri);
	// returns the barycentric coordinates(u, v, w)
	bool bLineSegment_TriangleCollision(sLine theLine, sTriangleP theTri,
		float& u, float& v, float& w, float& t);
	bool bLineSegment_TriangleCollisionBack(sLine theLine, sTriangleP theTri,
		float& u, float& v, float& w, float& t);
	


};

