#pragma once
//#include <vector>
//#include "aLuaScriptsSerial.h"
#include "aLuaScript.h"
#include "aLuaScriptsSerial.h"
#include "CameraPoint.h"
//



struct CameraAnimation
{
	CameraPoint point;
	float duration;
};

struct AnimationScene
{
	std::vector<aLuaScript*> singleScripts;
	std::vector<aLuaScriptsSerial*> serialScripts;
	std::vector<CameraAnimation> cameraAnimations;

	bool running = false;
	float duration=1;
	
};

class Animator
{

public:
	Scene* scene = nullptr;
	float time = 0;
	float animTime = 0;
	std::vector<Scene> scenes;
	int currentSceneId = 0;
	std::vector<AnimationScene> animScenes;
	int cameraPointId = 0;

	void StopAnimations()
	{
		for(aLuaScript* script:animScenes[currentSceneId].singleScripts)
		{
			script->running = false;
		}

		for (aLuaScriptsSerial* script : animScenes[currentSceneId].serialScripts)
		{
			script->running = false;
		}
	}

	void StartAnimation()
	{
		for (aLuaScript* script : animScenes[currentSceneId].singleScripts)
		{
			script->running = true;
		}

		for (aLuaScriptsSerial* script : animScenes[currentSceneId].serialScripts)
		{
			script->running = true;
		}
	}

	void NextScene()
	{
		if (cameraPointId < animScenes[currentSceneId].cameraAnimations.size() - 2) cameraPointId++;
	}

	void PrevScene()
	{
		if (cameraPointId > 0) cameraPointId--;
	}

	void RestartScene()
	{
		for (aLuaScript *script : animScenes[currentSceneId].singleScripts)
		{
			script->time = 0;
			script->running = true;
		}

		for (aLuaScriptsSerial* script : animScenes[currentSceneId].serialScripts)
		{
			script->time = 0;
			script->running = true;
		}

		scene->currentCameraPoint = animScenes[currentSceneId].cameraAnimations[0].point;
	}

	void Update()
	{
		time += scene->deltaTime;
		animTime += scene->deltaTime;


		AnimationScene currentScene = animScenes[currentSceneId];

		if (animTime > currentScene.cameraAnimations[cameraPointId].duration)
		{
			if(cameraPointId <currentScene.cameraAnimations.size()-2) cameraPointId++;

			scene->currentCameraPoint = currentScene.cameraAnimations[cameraPointId].point;

			animTime = 0;
		}


		if (time > currentScene.duration)
		{
			if (currentSceneId < currentScene.cameraAnimations.size() - 2) currentSceneId++;

			time = 0;

		}

	}

	void AddAnimScene(std::vector<aLuaScript*> singleScripts, std::vector<aLuaScriptsSerial*> serialScripts, std::vector<CameraAnimation> cameraAnimations, float duration)
	{
		AnimationScene animScene;
		animScene.singleScripts = singleScripts;
		animScene.serialScripts = serialScripts;
		animScene.cameraAnimations = cameraAnimations;
		animScene.duration = duration;

		animScenes.push_back(animScene);

	}

};