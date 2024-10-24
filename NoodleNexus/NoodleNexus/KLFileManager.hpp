#include "cVAOManager/cVAOManager.h"
#include <string>;
#include "Scene.hpp";

class KLFileManager
{
	sModelDrawInfo ReadObjectFile(std::string filePath);
	void WriteObjectFile(sObject* object);

	Scene* ReadSceneFile(std::string filePath);
	void WriteScene(Scene* scnee);



};