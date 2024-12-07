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
		//lua_register(L, "MoveObject", lua_SetObjectPos);
		lua_register(L, "RotateObject", lua_SetObjectRot);
        lua_register(L, "MoveObj", lua_MoveTo);

		//printf("WOW, you are UPDATING\n");
	}

    void Update() override
    {
        float speed = 0.01f;
        float rotationSpeed = 1.0f;
        float deltaTime = 0.016;

        //// Call MoveCar
        //lua_getglobal(L, "MoveCar");
        //if (lua_isfunction(L, -1))
        //{
        //    lua_pushnumber(L, object->mesh->positionXYZ.x);
        //    lua_pushnumber(L, object->mesh->positionXYZ.y);
        //    lua_pushnumber(L, object->mesh->positionXYZ.z);
        //    lua_pushnumber(L, speed);

        //    if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
        //        std::cerr << "Lua error in MoveCar: " << lua_tostring(L, -1) << std::endl;
        //        lua_pop(L, 1); // Pop the error message
        //    }
        //}
        //else
        //{
        //    lua_pop(L, 1); // Remove the invalid global
        //    std::cerr << "MoveCar is not a valid function." << std::endl;
        //}

        ////// Call RotateCar
        //lua_getglobal(L, "RotateCar");
        //if (lua_isfunction(L, -1))
        //{
        //    lua_pushnumber(L, object->mesh->rotationEulerXYZ.x);
        //    lua_pushnumber(L, object->mesh->rotationEulerXYZ.y);
        //    lua_pushnumber(L, object->mesh->rotationEulerXYZ.z);
        //    lua_pushnumber(L, rotationSpeed);

        //    if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
        //        std::cerr << "Lua error in RotateCar: " << lua_tostring(L, -1) << std::endl;
        //        lua_pop(L, 1); // Pop the error message
        //    }
        //}
        //else
        //{
        //    lua_pop(L, 1); // Remove the invalid global
        //    std::cerr << "RotateCar is not a valid function." << std::endl;
        //}

        lua_getglobal(L, "MovObj");  // Get the Lua function MovObj
        if (lua_isfunction(L, -1)) {
            glm::vec3 start = object->mesh->positionXYZ;  // Current position
            glm::vec3 end(10.0f, 5.0f, 15.0f);            // Target position
            float seconds = 5.0f;                         // Duration in seconds
            float deltaTime = 0.016f;                     // Frame time

            // Push arguments onto the Lua stack
            lua_pushstring(L, "racing_desk");  // Object name
            lua_pushnumber(L, start.x);        // Start position X
            lua_pushnumber(L, start.y);        // Start position Y
            lua_pushnumber(L, start.z);        // Start position Z
            lua_pushnumber(L, end.x);          // End position X
            lua_pushnumber(L, end.y);          // End position Y
            lua_pushnumber(L, end.z);          // End position Z
            lua_pushnumber(L, seconds);        // Total time to move
            lua_pushnumber(L, deltaTime);      // Frame time

            // Call Lua function (9 arguments, 0 return values)
            if (lua_pcall(L, 9, 0, 0) != LUA_OK) {
                // Handle Lua error
                std::cerr << "Lua error in MovObj: " << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1);  // Remove error message from the stack
            }
        }
        else {
            lua_pop(L, 1);  // Remove invalid global
            std::cerr << "MovObj is not a valid function." << std::endl;
        }


    }

};