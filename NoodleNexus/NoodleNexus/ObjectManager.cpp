#pragma once

#include "ObjectManager.h"
#include <Lua5.4.7/lua.hpp>
#include <iostream>
//
//int ObjectManager::BindToLua(lua_State* L) {
//    if (!L) {
//        std::cerr << "Lua state is null." << std::endl;
//        return 0;
//    }
//
//    if (lua_gettop(L) < 1 || !lua_isuserdata(L, 1)) {
//        lua_pushstring(L, "Expected light userdata as the first argument.");
//        lua_error(L); // Will throw a Lua error
//        return 0;     // Unreachable, but here for safety
//    }
//
//    Object* obj = static_cast<Object*>(lua_touserdata(L, 1));
//    if (!obj) {
//        lua_pushstring(L, "Invalid object passed to BindToLua.");
//        lua_error(L);
//        return 0;
//    }
//
//    obj->SetPosition(glm::vec3(25.0f, 0.0f, 0.0f));
//    return 0;
//}
//
//
//void ObjectManager::MoveObject(Object* obj, float x, float y, float z) {
//    if (obj) {
//        obj->SetPosition(glm::vec3(x , y, z));    
//        std::cout << "Object pos x" << obj->mesh->positionXYZ.x << std::endl;
//        
//    }
//    std::cout << "Object pos y" << obj->mesh->positionXYZ.y << std::endl;
//
//}
