#include "lua_helpers.hpp"
#include "lua_exception.hpp"
#include <cassert>

namespace be::belua {
namespace {

///////////////////////////////////////////////////////////////////////////////
int trace(lua_State* L) {
   S msg;
   if (lua_isstring(L, 1)) {
      std::size_t len;
      const char* ptr = lua_tolstring(L, 1, &len);
      msg.assign(ptr, len);
   } else if (!lua_isnoneornil(L, 1)) {
      std::size_t len;
      const char* ptr = luaL_tolstring(L, 1, &len);
      msg.assign(ptr, len);
      lua_pop(L, 1);
   }
   msg.append(1, 29); // group separator
   luaL_traceback(L, L, msg.c_str(), 1);
   return 1;
}

} // be::belua::()

///////////////////////////////////////////////////////////////////////////////
void ecall(lua_State* L, int n_args, int n_results) {
   int base = lua_gettop(L) - n_args;  /* function index */
   lua_pushcfunction(L, trace);  /* push traceback function */
   lua_insert(L, base);  /* put it under chunk and args */
   int status = lua_pcall(L, n_args, n_results, base);
   lua_remove(L, base);  /* remove traceback function */

   if (status == LUA_OK) {
      return;
   } else if (status == LUA_ERRMEM) {
      throw std::bad_alloc();
   } else {
      S msg;
      S lua_trace;
      if (lua_type(L, -1) == LUA_TSTRING) {
         msg = lua_tostring(L, -1);
         auto i = msg.find(29, 0);

         if (i != std::string::npos && msg.substr(i + 1, 17) == "\nstack traceback:" ) {
            lua_trace = msg.substr(i + 18);
            msg = msg.substr(0, i);
         }
      } else {
         msg = "Error calling Lua function!";
      }

      throw LuaError(status, msg, lua_trace);
   }
}

///////////////////////////////////////////////////////////////////////////////
S get_string(lua_State* L, int idx, const S& default_value) {
   if (lua_type(L, idx) == LUA_TSTRING) {
      return raw_string(L, idx);
   } else {
      return default_value;
   }
}

///////////////////////////////////////////////////////////////////////////////
S raw_string(lua_State* L, int idx) {
   std::size_t len;
   const char* ptr = lua_tolstring(L, idx, &len);
   return S(ptr, len);
}

///////////////////////////////////////////////////////////////////////////////
S to_string(lua_State* L, int idx) {
   std::size_t len;
   const char* ptr = luaL_tolstring(L, idx, &len);
   return S(ptr, len);
}

///////////////////////////////////////////////////////////////////////////////
S check_string(lua_State* L, int idx) {
   std::size_t len;
   const char* ptr = luaL_checklstring(L, idx, &len);
   return S(ptr, len);
}

} // be::belua
