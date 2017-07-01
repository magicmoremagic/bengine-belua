#include "result_code.hpp"

/*!! include 'result_code' !! 83 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
bool is_valid(ResultCode constant) noexcept {
   switch (constant) {
      case ResultCode::ok:
      case ResultCode::yield:
      case ResultCode::syntax_error:
      case ResultCode::runtime_error:
      case ResultCode::not_enough_memory:
      case ResultCode::handler_error:
      case ResultCode::gc_error:
      case ResultCode::file_io_error:
         return true;
      default:
         return false;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* result_code_name(ResultCode constant) noexcept {
   switch (constant) {
      case ResultCode::ok:                return "ok";
      case ResultCode::yield:             return "yield";
      case ResultCode::syntax_error:      return "syntax_error";
      case ResultCode::runtime_error:     return "runtime_error";
      case ResultCode::not_enough_memory: return "not_enough_memory";
      case ResultCode::handler_error:     return "handler_error";
      case ResultCode::gc_error:          return "gc_error";
      case ResultCode::file_io_error:     return "file_io_error";
      default:
         return "?";
   }
}

///////////////////////////////////////////////////////////////////////////////
std::array<const ResultCode, 8> result_code_values() noexcept {
   return ::be::EnumTraits<ResultCode>::values<>();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, ResultCode constant) {
   if (is_valid(constant)) {
      os << result_code_name(constant);
   } else {
      os << static_cast<I64>(static_cast<U8>(constant));
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////
int generic_error_condition(ResultCode constant) noexcept {
   switch (constant) {
      case ResultCode::not_enough_memory: return static_cast<int>(std::errc::not_enough_memory);
      case ResultCode::file_io_error:     return static_cast<int>(std::errc::io_error);
      default:
         return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* result_code_msg(ResultCode constant) noexcept {
   switch (constant) {
      case ResultCode::ok:                return "Execution completed without error";
      case ResultCode::yield:             return "Coroutine yielded";
      case ResultCode::syntax_error:      return "Failed to compile chunk";
      case ResultCode::runtime_error:     return "Encountered unexpected error";
      case ResultCode::not_enough_memory: return "Memory allocation failed";
      case ResultCode::handler_error:     return "Encountered an error while running error handler";
      case ResultCode::gc_error:          return "Encountered an error while running __gc metamethod";
      case ResultCode::file_io_error:     return "A problem occurred while accessing a file";
      default:
         return "An unknown error occurred";
   }
}

} // be::belua


/* ######################### END OF GENERATED CODE ######################### */

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
S ResultCodeErrorCategory::message(int condition) const {
   return result_code_msg(result_code(condition));
}

///////////////////////////////////////////////////////////////////////////////
bool ResultCodeErrorCategory::equivalent(int code, const std::error_condition& condition) const noexcept {
   if (condition.category() == std::generic_category() &&
       condition.value() == generic_error_condition(result_code(code))) {
      return true;
   }

   if (condition.category() == result_code_error_category() &&
       condition.value() == code) {
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
const std::error_category& result_code_error_category() {
   static ResultCodeErrorCategory instance;
   return instance;
}

///////////////////////////////////////////////////////////////////////////////
std::error_code make_error_code(ResultCode e) {
   return std::error_code(static_cast<int>(e), result_code_error_category());
}

///////////////////////////////////////////////////////////////////////////////
std::error_condition make_error_condition(ResultCode e) {
   return std::error_condition(static_cast<int>(e), result_code_error_category());
}

///////////////////////////////////////////////////////////////////////////////
ResultCode result_code(int result) {
   return static_cast<ResultCode>(result);
}

} // be::belua
