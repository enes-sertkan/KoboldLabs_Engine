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

  


		//printf("WOW, you are UPDATING\n");
	}

    void Update() override
    {
        float deltaTime = 0.016f; // Frame time, assuming 60 FPS
        luaL_dofile(L, "cObjectMovement.lua");

        glm::vec3 control(-50.f, 200.0f, 20.0f); // Control point for the curve
        glm::vec3 start(-100.0f, 20.0f, 15.0f); //= object->startTranform->position;  // Current position
        glm::vec3 end(100.0f, 5.0f, 15.0f);            // Target position
        float seconds = 10.0f;                         // Duration in seconds
 
        //
       

        //CallLuaFunction("MoveObj", start, end, seconds, time, control);
        //CallLuaFunction("RotateObj", start, end, seconds, time, control);
        CallLuaFunction("MoveAlongCurve", start, end, seconds, time, control);

        //lua_settop(L, 0);
        //lua_getglobal(L, "MoveAlongCurve");
        //if (lua_isfunction(L, -1)) {
        //          // Frame time
        //    PushData(start, end, seconds, time, control);


        //    // Call the Lua function (12 arguments, no returns)
        //    if (lua_pcall(L, 12, 0, 0) != LUA_OK) {
        //        std::cerr << "Lua error in MoveAlongCurve: " << lua_tostring(L, -1) << std::endl;
        //        lua_pop(L, 1); // Remove error message
        //    }
        //}
        //else {
        //    std::cerr << "FUUUUUUCK!" << std::endl;
        //    lua_pop(L, 1); // Remove invalid global
        //    
        //}

        //lua_settop(L, 0);
        time += deltaTime;


        //EASY REPEWAT
        if (time>seconds)
        {
            time = 0;
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
                std::cerr << "Lua error in"<< functionName <<": " << lua_tostring(L, -1) << std::endl;
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