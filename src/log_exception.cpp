#include "log_exception.hpp"
#include "result_code.hpp"
#include <be/core/logging.hpp>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
void log_exception(const LuaTrace& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << e.lua_trace()
      & attr(ids::log_attr_trace) << StackTrace(e.trace())
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const LuaError& e, Log& log, v::Verbosity verbosity, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_category) << e.code().category().name()
      & attr(ids::log_attr_error) << result_code(e.code().value())
      & attr(ids::log_attr_error_code) << e.code().value()
      & attr(ids::log_attr_description) << e.code().message()
      & attr(ids::log_attr_trace) << S(e.lua_trace())
      | log;
}

} // be::belua
