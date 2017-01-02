#include "context.hpp"
#include "lua_exception.hpp"
#include <be/core/exceptions.hpp>
#include <be/core/logging.hpp>
#include <cassert>

namespace be {
namespace lua {
namespace detail {
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

} // be::lua::detail::()

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

} // be::lua::detail

///////////////////////////////////////////////////////////////////////////////
Context::Context() { }

///////////////////////////////////////////////////////////////////////////////
Context::Context(std::initializer_list<luaL_Reg> modules, bool open_std_lib, bool use_environment) {
   reset(modules.begin(), modules.end(), open_std_lib, use_environment);
}

///////////////////////////////////////////////////////////////////////////////
void Context::reset(std::initializer_list<luaL_Reg> modules, bool open_std_lib, bool use_environment) {
   reset(modules.begin(), modules.end(), open_std_lib, use_environment);
}

///////////////////////////////////////////////////////////////////////////////
lua_State* Context::L() {
   assert(state_.L);
   return state_.L;
}

///////////////////////////////////////////////////////////////////////////////
Context::operator bool() const {
   return !!state_.L;
}

///////////////////////////////////////////////////////////////////////////////
void Context::execute(gsl::cstring_span<> chunk, const S& chunk_name) {
   lua_settop(state_.L, 0);
   int status = luaL_loadbufferx(state_.L, chunk.data(), chunk.length(), chunk_name.c_str(), nullptr);
   if (status == LUA_OK) {
      detail::ecall(state_.L, 0, 0);
   } else if (status == LUA_ERRMEM) {
      throw std::bad_alloc();
   } else {
      const char* temp = lua_tostring(state_.L, -1);
      S msg;
      if (temp != nullptr) {
         msg = temp;
      }
      throw LuaError(status, msg, S());
   }
}

///////////////////////////////////////////////////////////////////////////////
bool Context::attempt(gsl::cstring_span<> chunk, const S& chunk_name) {
   try {
      execute(chunk, chunk_name);
      return true;
   } catch (const LuaError& e) {
      lua_error(e, default_log());
   } catch (const LuaException& e) {
      lua_error(e, default_log());
   } catch (const Recoverable<>& e) {
      be::log_error("Lua") << "Exception while executing Lua!"
         & attr(ids::log_attr_message) << S(e.what())
         & attr(ids::log_attr_name) << chunk_name
         & attr(ids::log_attr_trace) << e.trace()
         | default_log();
   } catch (const RecoverableException<>& e) {
      be::log_error("Lua") << "Exception while executing Lua!"
         & attr(ids::log_attr_message) << S(e.what())
         & attr(ids::log_attr_name) << chunk_name
         | default_log();
   } catch (const std::exception& e) {
      be::log_error("Lua") << "Unknown exception while executing Lua!"
         & attr(ids::log_attr_message) << S(e.what())
         & attr(ids::log_attr_name) << chunk_name
         | default_log();
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
void Context::close() {
   state_ = detail::LuaState();
}

} // be::lua
} // be
