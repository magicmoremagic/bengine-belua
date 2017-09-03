#include "lua_error.hpp"

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
LuaError::LuaError(std::error_code ec, S lua_trace)
   : RecoverableError(ec),
     lua_trace_(std::move(lua_trace)) { }

///////////////////////////////////////////////////////////////////////////////
LuaError::LuaError(std::error_code ec, const char* msg, S lua_trace)
   : RecoverableError(ec, msg),
     lua_trace_(std::move(lua_trace)) { }

///////////////////////////////////////////////////////////////////////////////
LuaError::LuaError(std::error_code ec, const S& msg, S lua_trace)
   : RecoverableError(ec, msg),
     lua_trace_(std::move(lua_trace)) { }

///////////////////////////////////////////////////////////////////////////////
LuaError::LuaError(LuaError&& other) noexcept
   : RecoverableError(other.code(), other.what()),
     lua_trace_(std::move(other.lua_trace_)) { }

///////////////////////////////////////////////////////////////////////////////
LuaError& LuaError::operator=(LuaError&& other) noexcept {
   *((RecoverableError*)this) = *(RecoverableError*)&other;
   lua_trace_ = std::move(other.lua_trace_);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
const S& LuaError::lua_trace() const noexcept {
   return lua_trace_;
}

} // be::belua
