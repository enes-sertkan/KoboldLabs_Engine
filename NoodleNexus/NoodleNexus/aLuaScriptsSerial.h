#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"

#include <string>
#include <Lua5.4.7/lua.hpp>
#include <iostream>

int lua_MoveObject(lua_State* L);
int lua_RotateTo(lua_State* L);
int lua_SetTexture(lua_State* L);
int lua_SetMeshTransparency(lua_State* L);


struct LuaScript2Points
{
	std::string script;
	glm::vec3 startVector = glm::vec3(0,0,0);
	glm::vec3 endVector = glm::vec3(0,0,0);
	glm::vec3 additData = glm::vec3(0,0,0);
	float seconds = 0;


	LuaScript2Points(std::string _script, glm::vec3 _startVector, glm::vec3 _endVector, float _seconds, glm::vec3 _additData)
	{
		script = _script;
		startVector = _startVector;
		endVector = _endVector;
		seconds = _seconds;
		additData = _additData;

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
	std::string baseLuaunctionName = "MoveObj";


	void Start() override
	{
		// Now you can use luaScript to access Lua state and register functions
		L = luaL_newstate();


		if (!L) {
			std::cout << "Lua state is invalid." << std::endl;
		}
		luaL_openlibs(L);
		time = 0;

		//register and stuff
	}

	void Update() override
	{
		if (!running) return;



		UpdateTime();
		std::cout << time << std::endl;

		bool isEnded = RunLuaScript(currentScriptID);



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
		time += 0.016f;//object->scene->deltaTime;
		CheckAnimEnd();
		
	}

	void CheckAnimEnd()
	{
		if (time > scripts[currentScriptID].seconds)
			{
				currentScriptID++;
				time = 0;
			
			}


		if (currentScriptID >= scripts.size())
			if (repeat)
			{
				currentScriptID = 0;
				time = 0;
			}
			else
			{
				running = false; return;
			}

	}

	void AddMoveScript(std::string _script, glm::vec3 _startVector, glm::vec3 _endVector, float _duration, glm::vec3 additData)
	{
		LuaScript2Points moveData = LuaScript2Points(_script,_startVector,_endVector, _duration, additData);
		scripts.push_back(moveData);
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

	void CallLuaFunction(std::string fileName, glm::vec3 start, glm::vec3 end, float duration, float time, glm::vec3 additData)
	{

		luaL_dofile(L, fileName.c_str());

		lua_register(L, "MoveTo", lua_MoveObject);
		lua_register(L, "RotateTo", lua_RotateTo);
		lua_register(L, "SetTexture", lua_SetTexture);
		lua_register(L, "DAD", lua_SetMeshTransparency);


		lua_settop(L, 0);
		lua_getglobal(L, baseLuaunctionName.c_str());  // Get the Lua function MovObj
		if (lua_isfunction(L, -1)) {

			PushData(start, end, duration, time, additData);

			// Call Lua function (9 arguments, 0 return values)
			if (lua_pcall(L, 12, 0, 0) != LUA_OK) {
				// Handle Lua error
				std::cerr << "Lua error in" << baseLuaunctionName << ": " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);  // Remove error message from the stack
			}
		}
		else {
			lua_pop(L, 1);  // Remove invalid global
			std::cerr << baseLuaunctionName << " is not a valid function." << std::endl;
		}
		lua_settop(L, 0);
	}
};
