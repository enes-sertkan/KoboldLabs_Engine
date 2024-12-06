#include "LuaScript.h"

LuaScript::LuaScript() {
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaScript::~LuaScript() {
    lua_close(L);
}

bool LuaScript::LoadScript(const std::string& filename) {
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        return false;
    }
    return true;
}

void LuaScript::RegisterFunction(const std::string& funcName, lua_CFunction func) {
    lua_register(L, funcName.c_str(), func);
}

float LuaScript::GetGlobalFloat(const std::string& name) {
    lua_getglobal(L, name.c_str());
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        return 0.0f;
    }
    float value = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return value;
}

int LuaScript::GetGlobalInt(const std::string& name) {
    lua_getglobal(L, name.c_str());
    if (!lua_isnumber(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    int value = (int)lua_tointeger(L, -1);
    lua_pop(L, 1);
    return value;
}
