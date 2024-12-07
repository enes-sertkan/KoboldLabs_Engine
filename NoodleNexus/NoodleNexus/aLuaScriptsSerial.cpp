#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include <string>
#include <Lua5.4.7/lua.hpp>
#include <iostream>


int lua_SetObjectPos(lua_State* L);
int lua_SetObjectRot(lua_State* L);
int lua_MoveTo(lua_State* L);

struct LuaScript2Points
{
	std::string script;
	glm::vec3 startVector = glm::vec3(0,0,0);
	glm::vec3 endVector = glm::vec3(0,0,0);
	float seconds = 0;


	LuaScript2Points(std::string _script, glm::vec3 _startVector, glm::vec3 _endVector, float _seconds)
	{
		script = _script;
		startVector = _startVector;
		endVector = _endVector;
		seconds = _seconds;

	}
};


class aLuaScriptsSerial : public Action
{
public:

	std::vector<LuaScript2Points> scripts;
	int currentScriptID = 0;
	bool running = true;
	bool repeat = true;

	void Start() override
	{

	}

	void Update() override
	{
		if (!running) return;

		if (currentScriptID >= scripts.size())
			if (repeat)
			{
				currentScriptID = 0;
			}
			else
			{
				running = false; return;
			}

			

		bool isEnded = RunLuaScript(currentScriptID);

		if (isEnded)
		{
			currentScriptID++;
		}


		//Run current script

	}

	bool RunLuaScript(int id)
	{
		if (id >= scripts.size())
		{
			std::cout << "[ERROR] Lua Script ID is too hight.";
			return false;
		}


	}
};
