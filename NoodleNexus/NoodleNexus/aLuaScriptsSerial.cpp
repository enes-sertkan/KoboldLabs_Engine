#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "Scene.hpp"
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
	glm::vec3 additData = glm::vec3(0,0,0);
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
	lua_State* L = nullptr;

	std::vector<LuaScript2Points> scripts;
	int currentScriptID = 0;
	bool running = true;
	bool repeat = true;
	float time = 0;

	void Start() override
	{
		//register and stuff
	}

	void Update() override
	{
		if (!running) return;


			

		bool isEnded = RunLuaScript(currentScriptID);


		UpdateTime();

		//Run current script

	}

	bool RunLuaScript(int id)
	{
		if (id >= scripts.size())
		{
			std::cout << "[ERROR] Lua Script ID is too hight.";
			return false;
		}



		CallLuaFunction(scripts[id].script, scripts[id].startVector, scripts[id].endVector, scripts[id].seconds, time, scripts[id].additData);

		return true;

	}

	void UpdateTime()
	{
		time += object->scene->deltaTime;
		CheckAnimEnd();
		
	}

	void CheckAnimEnd()
	{
		if (time > scripts[currentScriptID].seconds)
			{
				currentScriptID++;
			
			}


		if (currentScriptID >= scripts.size())
			if (repeat)
			{
				currentScriptID = 0;
			}
			else
			{
				running = false; return;
			}

	}



	void PushData(glm::vec3 start, glm::vec3 end, float duration, float time, glm::vec3 additData)
	{
		lua_pushstring(L, object->name.c_str());   // Object name
		lua_pushnumber(L, start.x);         // Start point (P0) X
		lua_pushnumber(L, start.y);         // Start point (P0) Y
		lua_pushnumber(L, start.z);         // Start point (P0) Z
		lua_pushnumber(L, end.x);           // End point (P2) X
		lua_pushnumber(L, end.y);           // End point (P2) Y
		lua_pushnumber(L, end.z);           // End point (P2) Z
		lua_pushnumber(L, duration);         // Duration
		lua_pushnumber(L, time);       // Frame time
		lua_pushnumber(L, additData.x);       // Control point (P1) X
		lua_pushnumber(L, additData.y);       // Control point (P1) Y
		lua_pushnumber(L, additData.z);       // Control point (P1) Z

	}

	void CallLuaFunction(std::string functionName, glm::vec3 start, glm::vec3 end, float duration, float time, glm::vec3 additData)
	{


		lua_settop(L, 0);
		lua_getglobal(L, functionName.c_str());  // Get the Lua function MovObj
		if (lua_isfunction(L, -1)) {

			PushData(start, end, duration, time, additData);

			// Call Lua function (9 arguments, 0 return values)
			if (lua_pcall(L, 12, 0, 0) != LUA_OK) {
				// Handle Lua error
				std::cerr << "Lua error in" << functionName << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);  // Remove error message from the stack
			}
		}
		else {
			lua_pop(L, 1);  // Remove invalid global
			std::cerr << functionName << " is not a valid function." << std::endl;
		}
		lua_settop(L, 0);
	}
};
