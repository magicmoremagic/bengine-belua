#pragma once
#ifndef BE_BELUALUA_HELPERS_HPP_
#define BE_BELUALUA_HELPERS_HPP_

#include "belua_autolink.hpp"
#include <be/core/be.hpp>
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace belua {

void ecall(lua_State* L, int n_args, int n_results);

S get_string(lua_State* L, int idx, const S& default_value);
S raw_string(lua_State* L, int idx);
S to_string(lua_State* L, int idx);
S check_string(lua_State* L, int idx);

} // be::belua
} // be

#endif

