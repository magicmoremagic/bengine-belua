#include "logging.hpp"
#include <be/core/logging.hpp>
#include <unordered_map>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg logging_module { "be.log", open_logging };

namespace {

///////////////////////////////////////////////////////////////////////////////
std::unordered_map<S, Nil>& intern_map() {
   static std::unordered_map<S, Nil> m;
   return m;
}

///////////////////////////////////////////////////////////////////////////////
const char* get_source(lua_State* L) {
   const char* source = "Lua";
   if (lua_gettop(L) >= 3 && lua_type(L, 3) == LUA_TSTRING) {
      S src = lua_tostring(L, 3);
      auto& interns = intern_map();
      auto it = interns.find(src);
      if (it == interns.end()) {
         auto result = interns.insert({ std::move(src), Nil() });
         source = result.first->first.c_str();
      } else {
         source = it->first.c_str();
      }
   }
   return source;
}

///////////////////////////////////////////////////////////////////////////////
template <v::Verbosity V, bool ShortForm = false>
int logging_log(lua_State* L) {
   Log& log = default_log();

   if (check_verbosity(V, log.verbosity_mask())) {
      const char* source = get_source(L);
      std::size_t length;
      const char* cmsg = luaL_tolstring(L, 1, &length);
      S msg(cmsg, length);
      lua_pop(L, 1);

      if (lua_gettop(L) >= 2 && lua_type(L, 2) == LUA_TTABLE) {
         LogRecord rec;
         be_log(V, source) & short_form(ShortForm) << msg || rec;

         lua_settop(L, 2);
         lua_pushnil(L);
         while (lua_next(L, 2) != 0)
         {
            // ... (-2) ? key (-1) ? value
            const char* key = luaL_tolstring(L, -2, nullptr);  // ... (-3) ? key (-2) ? value (-1) str key
            const char* val = luaL_tolstring(L, -2, nullptr);  // ... (-4) ? key (-3) ? value (-2) str key (-1) str value

            log_nil() & attr(key) << S(val) || rec;

            lua_pop(L, 3);                                     // ... (-1) ? key
         }

         rec | log;
      } else {
         be_log(V, source) & short_form(ShortForm) << msg | log;
      }
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
int logging_verbosity_mask(lua_State* L) {
   Log& log = default_log();

   auto old_mask = log.verbosity_mask();

   if (lua_gettop(L) >= 1 && lua_type(L, 1) == LUA_TNUMBER) {
      log.verbosity_mask((U16)lua_tointeger(L, 1));
   }

   lua_settop(L, 0);
   lua_pushinteger(L, old_mask);
   return 1;
}

} // be::lua::()

///////////////////////////////////////////////////////////////////////////////
int open_logging(lua_State* L) {
   luaL_Reg free_fns[] {
      { "fatal",           logging_log<v::fatal> },
      { "error",           logging_log<v::error> },
      { "warning",         logging_log<v::warning> },
      { "notice",          logging_log<v::notice> },
      { "info",            logging_log<v::info> },
      { "verbose",         logging_log<v::verbose> },
      { "debug",           logging_log<v::debug> },

      { "short_fatal",     logging_log<v::fatal, true> },
      { "short_error",     logging_log<v::error, true> },
      { "short_warning",   logging_log<v::warning, true> },
      { "short_notice",    logging_log<v::notice, true> },
      { "short_info",      logging_log<v::info, true> },
      { "short_verbose",   logging_log<v::verbose, true> },
      { "short_debug",     logging_log<v::debug, true> },

      { "verbosity_mask", logging_verbosity_mask },
      { nullptr, nullptr }
   };

   luaL_newlib(L, free_fns);

   lua_createtable(L, 0, 8);
   lua_pushinteger(L, (lua_Integer)v::nothing); lua_setfield(L, -2, "nothing");
   lua_pushinteger(L, (lua_Integer)v::errors_or_worse); lua_setfield(L, -2, "errors_or_worse");
   lua_pushinteger(L, (lua_Integer)v::warnings_or_worse); lua_setfield(L, -2, "warnings_or_worse");
   lua_pushinteger(L, (lua_Integer)v::notices_or_worse); lua_setfield(L, -2, "notices_or_worse");
   lua_pushinteger(L, (lua_Integer)v::info_or_worse); lua_setfield(L, -2, "info_or_worse");
   lua_pushinteger(L, (lua_Integer)v::verbose_or_worse); lua_setfield(L, -2, "verbose_or_worse");
   lua_pushinteger(L, (lua_Integer)v::debug_or_worse); lua_setfield(L, -2, "debug_or_worse");
   lua_pushinteger(L, (lua_Integer)v::everything); lua_setfield(L, -2, "everything");
   
   lua_setfield(L, -2, "v");

   return 1;
}

} // be::lua
} // be
