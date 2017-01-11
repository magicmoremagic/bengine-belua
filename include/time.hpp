#pragma once
#ifndef BE_LUACORE_TIME_HPP_
#define BE_LUACORE_TIME_HPP_

#include "luacore_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
int open_time(lua_State* L);

extern const luaL_Reg time_module;

} // be::lua
} // be

#endif
