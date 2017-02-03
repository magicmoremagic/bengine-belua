#pragma once
#ifndef BE_BELUALUA_EXCEPTION_HPP_
#define BE_BELUALUA_EXCEPTION_HPP_

#include <be/core/exceptions.hpp>
#include <be/core/log.hpp>

namespace be {
namespace belua {

class LuaException : public RecoverableException<int> {
public:
   explicit LuaException(int error, S msg, S lua_trace)
      : RecoverableException(std::move(error), std::move(msg)),
        lua_trace_(std::move(lua_trace)) { }

   const S& lua_trace() const {
      return lua_trace_;
   }

private:
   S lua_trace_;
};

using LuaError = StackTraceException<LuaException>;

void lua_warn(const LuaError& error, Log& log, const char* source = "Lua");
void lua_warn(const LuaException& e, Log& log, const char* source = "Lua");

void lua_error(const LuaError& error, Log& log, const char* source = "Lua");
void lua_error(const LuaException& e, Log& log, const char* source = "Lua");

} // be::belua
} // be

#endif
