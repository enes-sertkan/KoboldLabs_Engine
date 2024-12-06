#pragma once

#include <Lua5.4.7/lua.hpp>
#include <string>
#include <iostream>

class LuaScript {
public:
    LuaScript();
    ~LuaScript();
    bool LoadScript(const std::string& filename);
    void RegisterFunction(const std::string& funcName, lua_CFunction func);

    // Get values from Lua
    float GetGlobalFloat(const std::string& name);
    int GetGlobalInt(const std::string& name);

    lua_State* GetLuaState() { return L; }

private:
    lua_State* L;
};
