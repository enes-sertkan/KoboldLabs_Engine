#pragma once


#include "cVAOManager/cVAOManager.h"
#include <string>;
#include <sstream> 
#include <fstream> 
#include "Scene.hpp";

class KLFileManager
{
public:
	sModelDrawInfo ReadModelFile(const std::string& filePath);
	void WriteModelFile(const sModelDrawInfo* model, std::string fileName, std::string type );

	Scene* ReadSceneFile(std::string filePath);
	void WriteSceneFile(const Scene* scene, std::string fileName);

	std::string SerializeSceneToString(const Scene* scene);
	bool CompareSceneToSavedData(const Scene* scene, const std::string& savedData);
};