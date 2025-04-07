#include "cVAOManager.h"

#include "../GLCommon.h"	// All the GLFW, etc. (OpenGL stuff)

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

#include <sstream>

#include <fstream>
#include <iostream>

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
//	glm::vec3 maxValues;
//	glm::vec3 minValues;

//	scale = 5.0/maxExtent;		-> 5x5x5
//	float maxExtent;

	return;
}

// Takes:
// * 
// Returns: true if the file is loaded
bool readPlyFile_XYZ(sModelDrawInfo& modelDrawInfo)
{

	// ************************************************************
		// Read the top of the file to get some info.

		// Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
	std::ifstream plyFile(modelDrawInfo.meshPath);     // May also see .c_str() "c style string, char*"

	if ( ! plyFile.is_open()) 
	{
		return false;
	}

	std::string token = "";

	if (!plyFile.is_open())
	{
		return false;
	}

	//element vertex 1889
	while (token != "vertex")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfVertices = 0;
	plyFile >> modelDrawInfo.numberOfVertices;


	//element face 3851
	while (token != "face")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfTriangles = 0;
	plyFile >> modelDrawInfo.numberOfTriangles;

	//end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5 
	while (token != "end_header")
	{
		plyFile >> token;
	};


	// 
//    std::cout << numberOfVertices << std::endl;
//    std::cout << numberOfTriangles << std::endl;

	//property float x
	//property float y
	//property float z

	//struct sPlyVertex
	//{
	//    float x, y, z, confidence, intensity;
	//};
	////
	//// and 
	//// 
	//// 3 572 584 1040 
	//struct sTriangle
	//{
	//    unsigned int vertIndex_0;
	//    unsigned int vertIndex_1;
	//    unsigned int vertIndex_2;
	//};

	// Load the data from the file
//    sPlyVertex* pPlyVertices = new sPlyVertex[numberOfVertices];
//	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb[modelDrawInfo.numberOfVertices];
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices->r = 1.0f;
		modelDrawInfo.pVertices->g = 1.0f;
		modelDrawInfo.pVertices->b = 1.0f;


		// Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
//		modelDrawInfo.pVertices[index].confidence = 0;
//		modelDrawInfo.pVertices[index].intensity = 0;
	}

	// Load the triangle info from the file
	struct sPlyFileTriangle
	{
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};
//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++)
	{
		// 3 737 103 17 
		int discard = 0;
		plyFile >> discard;
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		pPlyFileTriangles[index].vertIndex_0 -= 1; // Convert to 0-based

		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		pPlyFileTriangles[index].vertIndex_1 -= 1;

		plyFile >> pPlyFileTriangles[index].vertIndex_2;
		pPlyFileTriangles[index].vertIndex_2 -= 1;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++)
	{
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}


	return true;
}

// Takes:
// * 
// Returns: true if the file is loaded
bool readPlyFile_XYZ_Normal(sModelDrawInfo& modelDrawInfo)
{

	// ************************************************************
		// Read the top of the file to get some info.

		// Read all the text until I get to the word "vertex"
//    std::ifstream plyFile("assets/models/bun_zipper_res3.ply");
	std::ifstream plyFile(modelDrawInfo.meshPath);     // May also see .c_str() "c style string, char*"

	if (!plyFile.is_open())
	{
		return false;
	}

	std::string token = "";

	if (!plyFile.is_open())
	{
		return false;
	}

	//element vertex 1889
	while (token != "vertex")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfVertices = 0;
	plyFile >> modelDrawInfo.numberOfVertices;


	//element face 3851
	while (token != "face")
	{
		plyFile >> token;
	};
	// Next info is the number of vertices
	//int numberOfTriangles = 0;
	plyFile >> modelDrawInfo.numberOfTriangles;

	//end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5 
	while (token != "end_header")
	{
		plyFile >> token;
	};



	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// end_header
	// -0.0369122 0.127512 0.00276757 0.850855 0.5
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++)
	{
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		plyFile >> modelDrawInfo.pVertices[index].nx;
		plyFile >> modelDrawInfo.pVertices[index].ny;
		plyFile >> modelDrawInfo.pVertices[index].nz;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices->r = 1.0f;
		modelDrawInfo.pVertices->g = 1.0f;
		modelDrawInfo.pVertices->b = 1.0f;


		// Only has xyz, so stop here
//        plyFile >> allFileInfo.pPlyVertices[index].confidence;
//        plyFile >> allFileInfo.pPlyVertices[index].intensity;
//		modelDrawInfo.pVertices[index].confidence = 0;
//		modelDrawInfo.pVertices[index].intensity = 0;
	}

	// Load the triangle info from the file
	struct sPlyFileTriangle
	{
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};
	//    sTriangle* pPlyTriangles = new sTriangle[numberOfTriangles];
	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++)
	{
		// 3 737 103 17 
		int discard = 0;
		plyFile >> discard;
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++)
	{
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}


	return true;
}


// Takes:
// * 
// Returns: true if the file is loaded
bool readPlyFile_XYZ_Normal_NoUV(sModelDrawInfo& modelDrawInfo) {
	std::ifstream plyFile(modelDrawInfo.meshPath);
	std::string token = "";

	if (!plyFile.is_open()) {
		return false;
	}

	// Read until we find "vertex"
	while (token != "vertex") {
		plyFile >> token;
	}

	// Read the number of vertices
	plyFile >> modelDrawInfo.numberOfVertices;

	// Read until we find "face"
	while (token != "face") {
		plyFile >> token;
	}

	// Read the number of triangles
	plyFile >> modelDrawInfo.numberOfTriangles;

	// Read until we find "end_header"
	while (token != "end_header") {
		plyFile >> token;
	}

	// Allocate memory for vertex data
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// Read vertex data
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++) {
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		// Read normals
		plyFile >> modelDrawInfo.pVertices[index].nx;
		plyFile >> modelDrawInfo.pVertices[index].ny;
		plyFile >> modelDrawInfo.pVertices[index].nz;

		// Set all the vertices to white (1,1,1)
		modelDrawInfo.pVertices[index].r = 1.0f;
		modelDrawInfo.pVertices[index].g = 1.0f;
		modelDrawInfo.pVertices[index].b = 1.0f;

		// Skip UV coordinates (if they are present)
		float u, v; // Temporary variables for UVs
		plyFile >> u; // Read and discard the s coordinate
		plyFile >> v; // Read and discard the t coordinate
	}

	// Load triangle info from the file
	struct sPlyFileTriangle {
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};

	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++) {
		int discard = 0;
		plyFile >> discard; // Read the number of vertices in the face (usually 3 for triangles)
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;
	}

	// Copy the triangle data to a 1D array...
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;

	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++) {
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}

	// Clean up
	delete[] pPlyFileTriangles; // Don't forget to free allocated memory

	return true;
}

bool readPlyFile_XYZ_Normal_RGBA(sModelDrawInfo& modelDrawInfo) {
	std::ifstream plyFile(modelDrawInfo.meshPath);
	std::string token = "";

	if (!plyFile.is_open()) {
		return false;
	}

	// Read until we find "vertex"
	while (token != "vertex") {
		plyFile >> token;
	}

	// Read the number of vertices
	plyFile >> modelDrawInfo.numberOfVertices;

	// Read until we find "face"
	while (token != "face") {
		plyFile >> token;
	}

	// Read the number of triangles
	plyFile >> modelDrawInfo.numberOfTriangles;

	// Read until we find "end_header"
	while (token != "end_header") {
		plyFile >> token;
	}

	// Allocate memory for vertex data
	modelDrawInfo.pVertices = new sVertex_SHADER_FORMAT_xyz_rgb_N_UV[modelDrawInfo.numberOfVertices];

	// Read vertex data
	for (unsigned index = 0; index != modelDrawInfo.numberOfVertices; index++) {
		plyFile >> modelDrawInfo.pVertices[index].x;
		plyFile >> modelDrawInfo.pVertices[index].y;
		plyFile >> modelDrawInfo.pVertices[index].z;

		// Read normals
		plyFile >> modelDrawInfo.pVertices[index].nx;
		plyFile >> modelDrawInfo.pVertices[index].ny;
		plyFile >> modelDrawInfo.pVertices[index].nz;

		// Read RGBA color values and normalize from uchar to float (0-1)
		unsigned int red, green, blue, alpha;
		plyFile >> red >> green >> blue >> alpha;
		modelDrawInfo.pVertices[index].r = red / 255.0f;
		modelDrawInfo.pVertices[index].g = green / 255.0f;
		modelDrawInfo.pVertices[index].b = blue / 255.0f;
		modelDrawInfo.pVertices[index].a = alpha / 255.0f;

		// Skip UV coordinates (texture_u, texture_v)
		float u, v;
		plyFile >> u >> v;
		modelDrawInfo.pVertices[index].u = u;
		modelDrawInfo.pVertices[index].v = v;

		// Check EOF and read errors for faces
		if (plyFile.eof()) {
			std::cout << "ERROR: Failed to read vertice " << index
				<< " of " << modelDrawInfo.numberOfVertices << "  EOF  " << std::endl;
			break;

		}

		if (plyFile.fail()) {
			std::cout << "ERROR: Failed to read vertice " << index
				<< " of " << modelDrawInfo.numberOfVertices << "  FAIL  " << std::endl;
			break;

		}
	}

	// Load triangle info from the file
	struct sPlyFileTriangle {
		unsigned int vertIndex_0;
		unsigned int vertIndex_1;
		unsigned int vertIndex_2;
	};

	sPlyFileTriangle* pPlyFileTriangles = new sPlyFileTriangle[modelDrawInfo.numberOfTriangles];
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++) {
		int discard = 0;
		plyFile >> discard; // Read the number of vertices in the face (usually 3 for triangles)
		plyFile >> pPlyFileTriangles[index].vertIndex_0;
		plyFile >> pPlyFileTriangles[index].vertIndex_1;
		plyFile >> pPlyFileTriangles[index].vertIndex_2;

		// Check EOF and read errors for faces
		if (plyFile.eof()) {
			std::cout << "ERROR: Failed to read face " << index
				<< " of " << modelDrawInfo.numberOfTriangles << "  EOF  "<<std::endl;
			break;
	
		}

		if (plyFile.fail()) {
			std::cout << "ERROR: Failed to read face " << index
				<< " of " << modelDrawInfo.numberOfTriangles << "  FAIL  "<<std::endl;
			break;
	
		}

		//if (pPlyFileTriangles[index].vertIndex_0 >= modelDrawInfo.numberOfVertices) {
		//	std::cout << "ERROR: Invalid indices in " << modelDrawInfo.meshPath
		//		<< " (max index: " << pPlyFileTriangles[index].vertIndex_0
		//		<< ", vertices: " << modelDrawInfo.numberOfVertices << ")"
		//		<< std::endl;

		//}
	}

	// Copy the triangle data to a 1D array
	modelDrawInfo.numberOfIndices = modelDrawInfo.numberOfTriangles * 3;
	modelDrawInfo.pIndices = new unsigned int[modelDrawInfo.numberOfIndices];

	unsigned int index = 0;
	for (unsigned int triIndex = 0; triIndex != modelDrawInfo.numberOfTriangles; triIndex++) {
		modelDrawInfo.pIndices[index + 0] = pPlyFileTriangles[triIndex].vertIndex_0;
		modelDrawInfo.pIndices[index + 1] = pPlyFileTriangles[triIndex].vertIndex_1;
		modelDrawInfo.pIndices[index + 2] = pPlyFileTriangles[triIndex].vertIndex_2;
		index += 3;
	}

	unsigned int maxIndex = 0;
	for (unsigned int index = 0; index != modelDrawInfo.numberOfTriangles; index++) {
		maxIndex = std::max(maxIndex, pPlyFileTriangles[index].vertIndex_0);
		maxIndex = std::max(maxIndex, pPlyFileTriangles[index].vertIndex_1);
		maxIndex = std::max(maxIndex, pPlyFileTriangles[index].vertIndex_2);

	
	}

	

	// Clean up
	delete[] pPlyFileTriangles;

	return true;
}



bool cVAOManager::LoadModelIntoVAO(
	std::string fileName,
	sModelDrawInfo& drawInfo,
	unsigned int shaderProgramID)

{
	// Load the model from file
	// (We do this here, since if we can't load it, there's 
	//	no point in doing anything else, right?)

	drawInfo.meshPath = fileName;

	// TODO: Load the model from file
//	if (!readPlyFile_XYZ(drawInfo))
//	{
//		return false;
//	}

	if (drawInfo.fileType == "XYZN")
	{
		if (!readPlyFile_XYZ_Normal(drawInfo))
		{
			return false;
		}

	}
	else if (drawInfo.fileType == "XYZNUV")
	{
		if (!readPlyFile_XYZ_Normal_NoUV(drawInfo))
		{
			return false;
		}
	}
	else if (drawInfo.fileType == "XYZNUVRGBA")
	{
		if (!readPlyFile_XYZ_Normal_RGBA(drawInfo))
		{
			return false;
		}
	}


	// Calculate extents
	drawInfo.calculateExtents();

	// Calculate tangents
	CalculateTangents(drawInfo);

	// Create a VAO (Vertex Array Object)
	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);

	// Vertex buffer
	glGenBuffers(1, &(drawInfo.VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV) * drawInfo.numberOfVertices,
		(GLvoid*)drawInfo.pVertices,
		GL_STATIC_DRAW);

	// Index buffer
	glGenBuffers(1, &(drawInfo.IndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * drawInfo.numberOfIndices,
		(GLvoid*)drawInfo.pIndices,
		GL_STATIC_DRAW);

	// Get attribute locations from shader
	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
	GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNormal");
	GLint vUV_location = glGetAttribLocation(shaderProgramID, "vUV");
	GLint vtangent_location = glGetAttribLocation(shaderProgramID, "vTangent"); // Add this line for the tangent

	// Set the vertex attributes for the shader
	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location,
		3,           // vPos
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, x));

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location,
		3,           // vCol
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, r));

	glEnableVertexAttribArray(vnorm_location);
	glVertexAttribPointer(vnorm_location,
		3,           // vNormal
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, nx));

	glEnableVertexAttribArray(vUV_location);
	glVertexAttribPointer(vUV_location,
		2,           // vUV
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, u));

	// Set the tangent attribute
	glEnableVertexAttribArray(vtangent_location);
	glVertexAttribPointer(vtangent_location,
		3,           // vTangent
		GL_FLOAT, GL_FALSE,
		sizeof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV),
		(void*)offsetof(sVertex_SHADER_FORMAT_xyz_rgb_N_UV, tx)); // Tangent offset

	// Unbind VAO and buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Disable vertex attributes
	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vnorm_location);
	glDisableVertexAttribArray(vUV_location);
	glDisableVertexAttribArray(vtangent_location); // Disable tangent attribute

	// Store the draw information into the map
	this->m_map_ModelName_to_VAOID[drawInfo.meshPath] = drawInfo;

	return true;

}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo) 
{
	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}


//glm::vec3 maxXYZ, minXYZ, extenXYZ, centreXYZ;
void sModelDrawInfo::calculateExtents(void)
{
	// Are there any vertices?
	if (this->numberOfIndices == 0)
	{
		return;
	}

	// Assume the 1st index is the max and min
	this->minXYZ.x = this->pVertices[0].x;
	this->minXYZ.y = this->pVertices[0].y;
	this->minXYZ.z = this->pVertices[0].z;

	this->maxXYZ.x = this->pVertices[0].x;
	this->maxXYZ.y = this->pVertices[0].y;
	this->maxXYZ.z = this->pVertices[0].z;

	for (unsigned int index = 0; index != this->numberOfVertices; index++)
	{
		// Compare each axis
		if (this->pVertices[index].x < this->minXYZ.x)
		{
			this->minXYZ.x = this->pVertices[index].x;
		}
		if (this->pVertices[index].y < this->minXYZ.y)
		{
			this->minXYZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].z < this->minXYZ.z)
		{
			this->minXYZ.z = this->pVertices[index].z;
		}
		// 
		if (this->pVertices[index].x > this->maxXYZ.x)
		{
			this->maxXYZ.x = this->pVertices[index].x;
		}
		if (this->pVertices[index].y > this->maxXYZ.y)
		{
			this->maxXYZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].z > this->maxXYZ.z)
		{
			this->maxXYZ.z = this->pVertices[index].z;
		}
	}//for (unsigned int index

	this->extenXYZ.x = this->maxXYZ.x - this->minXYZ.x;
	this->extenXYZ.y = this->maxXYZ.y - this->minXYZ.y;
	this->extenXYZ.z = this->maxXYZ.z - this->minXYZ.z;
	
	// TODO: Anything else you need calculated

	return;
}


bool cVAOManager::getTriangleMeshInfo(
	std::string meshName,
	std::vector<sTriangle>& vecTriangles)
{
	sModelDrawInfo meshInfo;
	if (!this->FindDrawInfoByModelName(meshName, meshInfo))
	{
		// Can't find that mesh... 
		return false;
	}

	vecTriangles.clear();

	// Copy that meshInfo to the vecTriangle vector
	// 
	// Recal that the index information is a 1D array, 
	//	and that every 3 indices is one triangle
	// Then we look up the vertex infom from the index
	//  info. AND we'll have to calculate the normal, too
	// (which is the cross product of the vertex locations)
	// 
	// 	sVertex_SHADER_FORMAT_xyz_rgb_N* pVertices;	
	//	unsigned int* pIndices;
	//
	// Remember that the element (triangle) indexes are in sets of 3s
	// pIndices[0] & pIndices[1] & pIndices[2] are triangle 0
	// pIndices[3] & pIndices[4] & pIndices[5] are triangle 1
	// ... and so on
	for (unsigned int index = 0; index != meshInfo.numberOfIndices; index += 3)
	{
		unsigned int vertIndex0 = meshInfo.pIndices[index + 0];
		unsigned int vertIndex1 = meshInfo.pIndices[index + 1];
		unsigned int vertIndex2 = meshInfo.pIndices[index + 2];

		sTriangle curTriangle;
		curTriangle.vertices[0].x = meshInfo.pVertices[vertIndex0].x;
		curTriangle.vertices[0].y = meshInfo.pVertices[vertIndex0].y;
		curTriangle.vertices[0].z = meshInfo.pVertices[vertIndex0].z;

		curTriangle.vertices[1].x = meshInfo.pVertices[vertIndex1].x;
		curTriangle.vertices[1].y = meshInfo.pVertices[vertIndex1].y;
		curTriangle.vertices[1].z = meshInfo.pVertices[vertIndex1].z;

		curTriangle.vertices[2].x = meshInfo.pVertices[vertIndex2].x;
		curTriangle.vertices[2].y = meshInfo.pVertices[vertIndex2].y;
		curTriangle.vertices[2].z = meshInfo.pVertices[vertIndex2].z;

		// Calculate the normal
		// https://en.wikipedia.org/wiki/Cross_product

		curTriangle.normal =
			glm::cross(curTriangle.vertices[1] - curTriangle.vertices[0],
				       curTriangle.vertices[2] - curTriangle.vertices[0]);
		// Normalize it
		curTriangle.normal = glm::normalize(curTriangle.normal);

		vecTriangles.push_back(curTriangle);

	}//for (unsigned int index




	return true;
}


void cVAOManager::CalculateTangents(sModelDrawInfo& drawInfo)
{
	sVertex_SHADER_FORMAT_xyz_rgb_N_UV* vertices = drawInfo.pVertices;
	unsigned int numVertices = drawInfo.numberOfVertices;
	unsigned int* indices = drawInfo.pIndices;
	unsigned int numIndices = drawInfo.numberOfIndices;

	for (unsigned int i = 0; i < numIndices; i += 3)
	{
		
		unsigned int i0 = indices[i];
		unsigned int i1 = indices[i + 1];
		unsigned int i2 = indices[i + 2];
		if (i0 >= drawInfo.numberOfVertices || i1 >= drawInfo.numberOfVertices || i2 >= drawInfo.numberOfVertices) {


			continue;
		}
		vertices[i0].tx = 0;
		vertices[i0].ty = 0;
		vertices[i0].tz = 0;

		vertices[i1].tx = 0;
		vertices[i1].ty = 0;
		vertices[i1].tz = 0;

		vertices[i2].tx = 0;
		vertices[i2].ty = 0;
		vertices[i2].tz = 0;

	}
	for (unsigned int i = 0; i < numIndices; i += 3)
	{
		unsigned int i0 = indices[i];
		unsigned int i1 = indices[i + 1];
		unsigned int i2 = indices[i + 2];

		if (i0 >= drawInfo.numberOfVertices || i1 >= drawInfo.numberOfVertices || i2 >= drawInfo.numberOfVertices) {

			std::cerr << "Invalid index detected! Skipping this triangle." << std::endl;
			continue;
		}

		glm::vec3 v0(vertices[i0].x, vertices[i0].y, vertices[i0].z);
		glm::vec3 v1(vertices[i1].x, vertices[i1].y, vertices[i1].z);
		glm::vec3 v2(vertices[i2].x, vertices[i2].y, vertices[i2].z);

		glm::vec2 uv0(vertices[i0].u, vertices[i0].v);
		glm::vec2 uv1(vertices[i1].u, vertices[i1].v);
		glm::vec2 uv2(vertices[i2].u, vertices[i2].v);

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float det = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
		if (fabs(det) < 1e-6) continue;
		float f = 1.0f / det;

		glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
		glm::vec3 bitangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

		// Accumulate tangent for each vertex of the triangle
		vertices[i0].tx += tangent.x;
		vertices[i0].ty += tangent.y;
		vertices[i0].tz += tangent.z;

		vertices[i1].tx += tangent.x;
		vertices[i1].ty += tangent.y;
		vertices[i1].tz += tangent.z;

		vertices[i2].tx += tangent.x;
		vertices[i2].ty += tangent.y;
		vertices[i2].tz += tangent.z;

		// Optionally accumulate bitangent if needed (uncomment if required)
		// vertices[i0].bx += bitangent.x;
		// vertices[i0].by += bitangent.y;
		// vertices[i0].bz += bitangent.z;
		// vertices[i1].bx += bitangent.x;
		// vertices[i1].by += bitangent.y;
		// vertices[i1].bz += bitangent.z;
		// vertices[i2].bx += bitangent.x;
		// vertices[i2].by += bitangent.y;
		// vertices[i2].bz += bitangent.z;
	}

	for (unsigned int i = 0; i < numVertices; ++i) {
		glm::vec3 tangent = glm::normalize(glm::vec3(vertices[i].tx, vertices[i].ty, vertices[i].tz));
		vertices[i].tx = tangent.x;
		vertices[i].ty = tangent.y;
		vertices[i].tz = tangent.z;
	}
}