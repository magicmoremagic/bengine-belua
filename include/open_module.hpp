#pragma once
#ifndef BE_LUACORE_OPEN_LIB_HPP_
#define BE_LUACORE_OPEN_LIB_HPP_

#include "luacore_autolink.hpp"
#include <be/core/be.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace lua {

void open_module(lua_State* L, const luaL_Reg& module);
void open_module(lua_State* L, const char* module_name, lua_CFunction module_opener);

} // be::lua
} // be

#endif
