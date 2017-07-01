#pragma once
#ifndef BE_BELUA_LOG_ERROR_HPP_
#define BE_BELUA_LOG_ERROR_HPP_

#include "lua_error.hpp"
#include <be/core/log.hpp>

namespace be::belua {

void log_warn(const LuaTrace& e, Log& log, const char* source = "Lua");
void log_warn(const LuaError& e, Log& log, const char* source = "Lua");

void log_error(const LuaTrace& e, Log& log, const char* source = "Lua");
void log_error(const LuaError& e, Log& log, const char* source = "Lua");

} // be::belua

#endif
