#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include <string>
#include <Lua5.4.7/lua.hpp>
#include <iostream>


int lua_SetObjectPos(lua_State* L);

class aLuaScript : public Action
{
public:
	std::string luaPath;
	lua_State* L = nullptr;
	

	void Start() override
	{
		// Now you can use luaScript to access Lua state and register functions
		L = luaL_newstate();


		if (!L) {
			std::cout << "Lua state is invalid." << std::endl;
		}
		luaL_openlibs(L);

		luaL_dofile(L, "cObjectMovement.lua");
		lua_register(L, "MoveObject", lua_SetObjectPos);
		//printf("WOW, you are UPDATING\n");
	}

	void Update() override
	{
	
		// Example Lua call
		float speed = 0.01f;
		lua_getglobal(L, "MoveCar");

		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, object->mesh->positionXYZ.x);
			lua_pushnumber(L, object->mesh->positionXYZ.y);
			lua_pushnumber(L, object->mesh->positionXYZ.z);
			lua_pushnumber(L, speed);

			if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
				std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
			}

		}


	}
};