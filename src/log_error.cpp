#include "log_error.hpp"
#include "result_code.hpp"
#include <be/core/logging.hpp>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
void log_warn(const LuaTrace& e, Log& log, const char* source) {
   be::log_warn(source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << e.lua_trace()
      & attr(ids::log_attr_trace) << e.trace()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_warn(const LuaError& e, Log& log, const char* source) {
   be::log_warn(source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << e.lua_trace()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_error(const LuaTrace& e, Log& log, const char* source) {
   be::log_error(source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << e.lua_trace()
      & attr(ids::log_attr_trace) << e.trace()
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_error(const LuaError& e, Log& log, const char* source) {
   be::log_error(source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << e.lua_trace()
      | log;
}

} // be::belua
