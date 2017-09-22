#pragma once
#ifndef BE_BELUA_LUA_HELPERS_HPP_
#define BE_BELUA_LUA_HELPERS_HPP_

#include <be/core/be.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

void ecall(lua_State* L, int n_args, int n_results);

S get_string(lua_State* L, int idx, const S& default_value);
S raw_string(lua_State* L, int idx);
S to_string(lua_State* L, int idx);
S check_string(lua_State* L, int idx);

} // be::belua

#endif

