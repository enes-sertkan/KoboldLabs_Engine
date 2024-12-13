#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include <Lua5.4.7/lua.hpp>
#include <iostream>
#include "Scene.hpp"
#include "aLuaScript.h"


struct TriggerAndLua
{
	glm::vec3 triggerCenter;
	float radius;
	std::string luaScript;
    float duration;
    float time;
    aLuaScript* scriptAction;

	TriggerAndLua(glm::vec3 center, float sphereRadius , aLuaScript*  scrAction)
	{
		triggerCenter = center;
		radius = sphereRadius;
        scriptAction = scrAction;
	}
};


class aLocTrggersLua : public Action
{
public:
	std::vector<TriggerAndLua> triggers;
    lua_State* L = nullptr;



    void Start() override
    {
        // Now you can use luaScript to access Lua state and register functions
        L = luaL_newstate();


        if (!L) {
            std::cout << "Lua state is invalid." << std::endl;
        }
        luaL_openlibs(L);

        ////luaL_dofile(L, "cObjectMovement.lua");
        //lua_register(L, "MoveTo", lua_MoveObject);
        //lua_register(L, "RotateTo", lua_RotateTo);




        //printf("WOW, you are UPDATING\n");
    }
	void Update() override
	{
	 for (TriggerAndLua trigger:triggers)
         if (glm::distance(object->scene->fCamera->getEyeLocation(), trigger.triggerCenter) < trigger.radius)
         {
             trigger.scriptAction->running = true;
         }
         else
         {
             trigger.scriptAction->running = false;
         }


	}
    void AddTrigger(glm::vec3 center, float sphereRadius, aLuaScript* scrAction)
    {
        TriggerAndLua trigger = TriggerAndLua(center,  sphereRadius,  scrAction);
        triggers.emplace_back(trigger);
    }
};