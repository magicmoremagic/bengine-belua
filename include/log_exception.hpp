#pragma once
#ifndef BE_BELUA_LOG_ERROR_HPP_
#define BE_BELUA_LOG_ERROR_HPP_

#include "lua_error.hpp"
#include <be/core/default_log.hpp>

namespace be::belua {

void log_exception(const LuaTrace& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = "Lua");
void log_exception(const LuaError& e, v::Verbosity verbosity = v::error, Log& log = default_log(), const char* source = "Lua");

} // be::belua

#endif
