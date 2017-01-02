#pragma once
#ifndef BE_LUACORE_ID_HPP_
#define BE_LUACORE_ID_HPP_

#include "luacore_autolink.hpp"
#include <be/core/id.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
int open_id(lua_State* L);

extern const luaL_Reg id_module;

} // be::lua
} // be

#endif
