#include "cVAOManager/cVAOManager.h"
#include <string>;
#include "Scene.hpp";

class KLFileManager
{
public:
	sModelDrawInfo ReadModelFile(const std::string& filePath);
	void WriteModelFile(const sModelDrawInfo* model, std::string fileName);

	Scene* ReadSceneFile(std::string filePath);
	void WriteSceneFile(const Scene* scene, std::string fileName);
};