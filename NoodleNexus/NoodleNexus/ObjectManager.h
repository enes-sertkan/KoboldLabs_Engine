#pragma once

#include "sObject.h"
#include "Lua5.4.7/lua.hpp"

class ObjectManager {
public:
    static int BindToLua(lua_State* L);
    static void MoveObject(Object* obj, float x, float y, float z);
};
