#pragma once
#ifndef BE_LUACORE_INTERPOLATE_STRING_HPP_
#define BE_LUACORE_INTERPOLATE_STRING_HPP_

#include "luacore_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
int open_interpolate_string(lua_State* L);

extern const luaL_Reg interpolate_string_module;

} // be::lua
} // be

#endif
