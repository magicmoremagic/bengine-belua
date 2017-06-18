#pragma once
#ifndef BE_BELUA_OPEN_MODULE_HPP_
#define BE_BELUA_OPEN_MODULE_HPP_

#include "belua_autolink.hpp"
#include <be/core/be.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

void open_module(lua_State* L, const luaL_Reg& module);
void open_module(lua_State* L, const char* module_name, lua_CFunction module_opener);

} // be::belua

#endif
