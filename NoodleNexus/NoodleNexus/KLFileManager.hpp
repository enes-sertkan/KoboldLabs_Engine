#include "cVAOManager/cVAOManager.h"
#include <string>;
#include "Scene.hpp";

class KLFileManager
{
	sModelDrawInfo ReadModelFile(std::string filePath);
	void WriteModelFile(sModelDrawInfo* model);

	Scene* ReadSceneFile(std::string filePath);
	void WriteScene(Scene* scnee);



};