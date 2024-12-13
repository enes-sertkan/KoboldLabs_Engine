#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include <string>
#include <Lua5.4.7/lua.hpp>
#include <iostream>
#include "sObject.h"
#include "Scene.hpp"



int lua_MoveObject(lua_State* L);
int lua_RotateTo(lua_State* L);
int lua_SetTexture(lua_State* L);
int lua_SetMeshTransparency(lua_State* L);



struct SoloLuaScript
{
    std::string scriptPath;
    glm::vec3 start;
    glm::vec3 end;
    glm::vec3 control;
    float duration;
    float time = 0;

    std::string text1;
};

class aLuaScript : public Action
{
public:
    std::vector<SoloLuaScript*> luaScripts;
    lua_State* L = nullptr;
    bool running = true;


    //TODO: Function to set up parametrs
    void Start() override
    {
        // Now you can use luaScript to access Lua state and register functions
        L = luaL_newstate();


        if (!L) {
            std::cout << "Lua state is invalid." << std::endl;
        }
        luaL_openlibs(L);


        lua_register(L, "MoveTo", lua_MoveObject);
        lua_register(L, "RotateTo", lua_RotateTo);  
        lua_register(L, "SetTexture", lua_SetTexture);
        lua_register(L, "DAD", lua_SetMeshTransparency);
       







        //printf("WOW, you are UPDATING\n");
    }

    void Update() override
    {
        if (!running) return;


        for (SoloLuaScript* script : luaScripts)
        {

            luaL_dofile(L, script->scriptPath.c_str());


            CallLuaFunction("MoveObj", script->start, script->end, script->duration, script->time, script->control, script->text1);


            script->time += object->scene->deltaTime;


            //EASY REPEWAT
            if (script->time > script->duration)
            {
                script->time = 0;
            }

        }







    }

    void PushData(glm::vec3 start, glm::vec3 end, float duration, float time, glm::vec3 additData, std::string text1)
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
        lua_pushstring(L, text1.c_str());   // Object name
    }



    void CallLuaFunction(std::string functionName, glm::vec3 start, glm::vec3 end, float duration, float time, glm::vec3 additData, std::string text1)
    {
        lua_settop(L, 0);
        lua_getglobal(L, functionName.c_str());  // Get the Lua function MovObj
        if (lua_isfunction(L, -1)) {

            PushData(start, end, duration, time, additData, text1);

            // Call Lua function (9 arguments, 0 return values)
            if (lua_pcall(L, 13, 0, 0) != LUA_OK) {
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

    SoloLuaScript* AddLuaScript(std::string newScript, glm::vec3 start, glm::vec3 end, float duration, glm::vec3 control)
    {
        SoloLuaScript* newLuaScript = new SoloLuaScript();
        newLuaScript->scriptPath = newScript;
        newLuaScript->start = start;
        newLuaScript->end = end;
        newLuaScript->duration = duration;
        newLuaScript->control = control;

        luaScripts.push_back(newLuaScript);

        return newLuaScript;
    }  
    
    SoloLuaScript* AddLuaScript(std::string newScript, glm::vec3 start, glm::vec3 end, float duration, glm::vec3 control, std::string text1)
    {
        SoloLuaScript* newLuaScript = new SoloLuaScript();
        newLuaScript->scriptPath = newScript;
        newLuaScript->start = start;
        newLuaScript->end = end;
        newLuaScript->duration = duration;
        newLuaScript->control = control;
        newLuaScript->text1 = text1;

        luaScripts.push_back(newLuaScript);

        return newLuaScript;

    }
};