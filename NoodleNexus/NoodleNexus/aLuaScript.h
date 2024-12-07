#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include <string>
#include <Lua5.4.7/lua.hpp>
#include <iostream>
#include "Scene.hpp"


int lua_MoveObject(lua_State* L);
int lua_RotateTo(lua_State* L);
int lua_FollowACurve(lua_State* L);

class aLuaScript : public Action
{
public:
	std::string luaPath;
	lua_State* L = nullptr;
    float time = 0.f;

	void Start() override
	{
		// Now you can use luaScript to access Lua state and register functions
		L = luaL_newstate();


		if (!L) {
			std::cout << "Lua state is invalid." << std::endl;
		}
		luaL_openlibs(L);

		luaL_dofile(L, "cObjectMovement.lua");
        lua_register(L, "MoveTo", lua_MoveObject);
        lua_register(L, "RotateTo", lua_RotateTo);
        lua_register(L, "FollowACurve", lua_FollowACurve);


		//printf("WOW, you are UPDATING\n");
	}

    void Update() override
    {
        float deltaTime = 0.016f; // Frame time, assuming 60 FPS
        luaL_dofile(L, "cObjectMovement.lua");
        // Define curve parameters
        glm::vec3 start(0.0f, 0.0f, 0.0f);   // Start point
        glm::vec3 control(5.0f, 10.0f, 0.0f); // Control point for the curve
        glm::vec3 end(10.0f, 0.0f, 0.0f);    // End point
        float seconds = 5.0f;

        

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


        // Optionally, you can call RotateObj here if needed
        lua_getglobal(L, "RotateObj");
        if (lua_isfunction(L, -1)) {
            // Add parameters for rotation (example)
            glm::vec3 start = object->mesh->positionXYZ;  // Current position
            glm::vec3 end(10.0f, 5.0f, 15.0f);            // Target position
            float seconds = 5.0f;                         // Duration in seconds

            // Push all required arguments onto the Lua stack
            lua_pushstring(L, "racing_desk");  // Object name
            lua_pushnumber(L, start.x);        // Start position X
            lua_pushnumber(L, start.y);        // Start position Y
            lua_pushnumber(L, start.z);        // Start position Z
            lua_pushnumber(L, end.x);          // End position X
            lua_pushnumber(L, end.y);          // End position Y
            lua_pushnumber(L, end.z);          // End position Z
            lua_pushnumber(L, seconds);        // Total time to move
            lua_pushnumber(L, deltaTime);      // Frame time

            // Call Lua function (8 arguments, 0 return values)
            if (lua_pcall(L, 9, 0, 0) != LUA_OK) {
                std::cerr << "Lua error in RotateObj: " << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1);  // Remove error message from the stack
            }
        }
        else {
            lua_pop(L, 1);  // Remove invalid global
            std::cerr << "RotateObj is not a valid function." << std::endl;
        }
        luaL_dofile(L, "cObjectMovement.lua");
        lua_getglobal(L, "AAA");
        if (lua_isfunction(L, -1)) {
            // Push control points for the curve
            lua_pushstring(L, "racing_desk");   // Object name
            lua_pushnumber(L, start.x);         // Start point (P0) X
            lua_pushnumber(L, start.y);         // Start point (P0) Y
            lua_pushnumber(L, start.z);         // Start point (P0) Z
            lua_pushnumber(L, control.x);       // Control point (P1) X
            lua_pushnumber(L, control.y);       // Control point (P1) Y
            lua_pushnumber(L, control.z);       // Control point (P1) Z
            lua_pushnumber(L, end.x);           // End point (P2) X
            lua_pushnumber(L, end.y);           // End point (P2) Y
            lua_pushnumber(L, end.z);           // End point (P2) Z
            lua_pushnumber(L, seconds);         // Duration
            lua_pushnumber(L, time);       // Frame time

            // Call the Lua function (12 arguments, no returns)
            if (lua_pcall(L, 12, 0, 0) != LUA_OK) {
                std::cerr << "Lua error in MoveAlongCurve: " << lua_tostring(L, -1) << std::endl;
                lua_pop(L, 1); // Remove error message
            }
        }
        else {
            std::cerr << "FUUUUUUCK!" << std::endl;
            lua_pop(L, 1); // Remove invalid global
            
        }


        time += deltaTime;

    
       
    }



};