#pragma once
#include <vector>
#include "aLuaScriptsSerial.h"
#include "aLuaScript.h"

struct Camera
{};

struct Scene
{
	std::vector<aLuaScript> singleScripts;
	std::vector<aLuaScriptsSerial> serialScripts;
	bool running = false;
	float lengthSeconds=1;
	
};

class Animator
{
	float time = 0;
	std::vector<Scene> scenes;
	int currentSceneId = 0;


	void StopAnimations()
	{
	}

	void StartAnimation()
	{

	}

	void NextScene()
	{

	}

	void PrevScene()
	{

	}

	void RestartScene()
	{

	}
	

};