#include "lua_exception.hpp"
#include <be/core/logging.hpp>
#include <gsl/string_span>

namespace be {
namespace belua {

void lua_warn(const LuaError& e, Log& log, const char* source) {
   be::log_warn(source) << S(e.what())
      & attr(ids::log_attr_code) << e.error()
      & attr(ids::log_attr_trace) << e.lua_trace()
      & attr(ids::log_attr_trace) << e.trace()
      | log;
}

void lua_warn(const LuaException& e, Log& log, const char* source) {
   be::log_warn(source) << S(e.what())
      & attr(ids::log_attr_code) << e.error()
      & attr(ids::log_attr_trace) << e.lua_trace()
      | log;
}

void lua_error(const LuaError& e, Log& log, const char* source) {
   be::log_error(source) << S(e.what())
      & attr(ids::log_attr_code) << e.error()
      & attr(ids::log_attr_trace) << e.lua_trace()
      & attr(ids::log_attr_trace) << e.trace()
      | log;
}

void lua_error(const LuaException& e, Log& log, const char* source) {
   be::log_error(source) << S(e.what())
      & attr(ids::log_attr_code) << e.error()
      & attr(ids::log_attr_trace) << e.lua_trace()
      | log;
}

} // be::belua
} // be

