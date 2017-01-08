#include "interpolate_string.hpp"
#include "lua_helpers.hpp"
#include <be/core/string_interpolation.hpp>
#include <lua/lualib.h>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg interpolate_string_module { "be.interpolate_string", open_interpolate_string };

namespace {

///////////////////////////////////////////////////////////////////////////////
int interpolate_string(lua_State* L) {
   if (lua_type(L, 1) == LUA_TSTRING) { // anything else is returned unmodified
      S input = raw_string(L, 1);
      if (lua_type(L, 2) == LUA_TFUNCTION) {
         S output = be::interpolate_string(input, [L](const S& str) {
            lua_pushvalue(L, 2); // $ input fn fn
            lua_pushlstring(L, str.c_str(), str.length()); // $ input fn fn key
            lua_call(L, 1, 1); // $ input fn result
            S replacement = to_string(L, -1); // $ input fn resultstring
            lua_settop(L, 2); // $ input fn
            return replacement;
         });
         lua_pushlstring(L, output.c_str(), output.length());
      } else if (lua_type(L, 2) == LUA_TTABLE) {
         S output = be::interpolate_string(input, [L](const S& str) {
            lua_pushlstring(L, str.c_str(), str.length()); // $ input table key
            lua_gettable(L, -2); // $ input table value
            S replacement = to_string(L, -1); // $ input table valuestring
            lua_settop(L, 2); // $ input table
            return replacement;
         });
         lua_pushlstring(L, output.c_str(), output.length());
      }
   }

   return 1;
}

} // be::lua::()

///////////////////////////////////////////////////////////////////////////////
int open_interpolate_string(lua_State* L) {
   lua_pushcfunction(L, interpolate_string);
   return 1;
}

} // be::lua
} // be
