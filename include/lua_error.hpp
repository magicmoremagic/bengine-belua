#pragma once
#ifndef BE_BELUA_LUA_ERROR_HPP_
#define BE_BELUA_LUA_ERROR_HPP_

#include <be/core/exceptions.hpp>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
class LuaError : public RecoverableError {
public:
   LuaError(std::error_code ec, S lua_trace);
   LuaError(std::error_code ec, const char* msg, S lua_trace);
   LuaError(std::error_code ec, const S& msg, S lua_trace);

   LuaError(const LuaError&) = default;
   LuaError(LuaError&& other) noexcept;
   LuaError& operator=(const LuaError&) = default;
   LuaError& operator=(LuaError&& other) noexcept;

   const S& lua_trace() const noexcept;

private:
   S lua_trace_;
};

///////////////////////////////////////////////////////////////////////////////
using LuaTrace = StackTraceException<LuaError>;

} // be::belua

#endif
