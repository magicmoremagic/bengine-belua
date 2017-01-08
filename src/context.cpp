#include "context.hpp"
#include "lua_exception.hpp"
#include "lua_helpers.hpp"
#include <be/core/exceptions.hpp>
#include <be/core/logging.hpp>
#include <cassert>

namespace be {
namespace lua {

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
      ecall(state_.L, 0, 0);
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
