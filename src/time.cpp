#include "time.hpp"
#include <be/core/time.hpp>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg time_module { "be.time", open_time };

namespace {

///////////////////////////////////////////////////////////////////////////////
int time_seconds_now(lua_State* L) {
   lua_settop(L, 0);
   lua_pushnumber(L, tu_to_seconds(ts_now()));
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_ts_now(lua_State* L) {
   lua_settop(L, 0);
   lua_pushinteger(L, ts_now().count());
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_perf_now(lua_State* L) {
   lua_settop(L, 0);
   lua_pushinteger(L, perf_now());
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_perf_to_seconds(lua_State* L) {
   U64 perf = static_cast<U64>(luaL_checkinteger(L, 1));
   lua_settop(L, 0);
   lua_pushnumber(L, perf_delta_to_seconds(perf));
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_perf_to_tu(lua_State* L) {
   U64 perf = static_cast<U64>(luaL_checkinteger(L, 1));
   lua_settop(L, 0);
   lua_pushinteger(L, perf_delta_to_tu(perf).count());
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_tu_to_seconds(lua_State* L) {
   TU tu = static_cast<TU>(luaL_checkinteger(L, 1));
   lua_settop(L, 0);
   lua_pushnumber(L, tu_to_seconds(tu));
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int time_seconds_to_tu(lua_State* L) {
   F64 seconds = luaL_checknumber(L, 1);
   lua_settop(L, 0);
   lua_pushinteger(L, seconds_to_tu(seconds).count());
   return 1;
}

} // be::lua::()

  ///////////////////////////////////////////////////////////////////////////////
int open_time(lua_State* L) {
   luaL_Reg fn[] {
      { "seconds_now", time_seconds_now },
      { "ts_now", time_ts_now },
      { "perf_now", time_perf_now },
      { "perf_to_seconds", time_perf_to_seconds },
      { "perf_to_tu", time_perf_to_tu },
      { "tu_to_seconds", time_tu_to_seconds },
      { "seconds_to_tu", time_seconds_to_tu },
      { nullptr, nullptr }
   };

   luaL_newlib(L, fn);
   return 1;
}

} // be::lua
} // be
