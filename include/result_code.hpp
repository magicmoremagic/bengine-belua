#pragma once
#ifndef BE_BELUA_RESULT_CODE_HPP_
#define BE_BELUA_RESULT_CODE_HPP_

#include "belua_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <be/core/enum_traits.hpp>

/*!! include 'result_code' !! 62 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
enum class ResultCode : U8 {
   ok = 0,
   yield = LUA_YIELD,
   syntax_error = LUA_ERRSYNTAX,
   runtime_error = LUA_ERRRUN,
   not_enough_memory = LUA_ERRMEM,
   handler_error = LUA_ERRERR,
   gc_error = LUA_ERRGCMM,
   file_io_error = LUA_ERRFILE
};

bool is_valid(ResultCode constant) noexcept;
const char* result_code_name(ResultCode constant) noexcept;
std::array<const ResultCode, 8> result_code_values() noexcept;
std::ostream& operator<<(std::ostream& os, ResultCode constant);
int generic_error_condition(ResultCode constant) noexcept;
const char* result_code_msg(ResultCode constant) noexcept;

} // be::belua

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <>
struct EnumTraits<::be::belua::ResultCode> {
   using type = ::be::belua::ResultCode;
   using underlying_type = typename std::underlying_type<type>::type;

   static constexpr std::size_t count = 8;

   static bool is_valid(type value) {
      return ::be::belua::is_valid(value);
   }

   static const char* name(type value) {
      return ::be::belua::result_code_name(value);
   }

   template <typename C = std::array<const type, count>>
   static C values() {
      return {
         ::be::belua::ResultCode::ok,
         ::be::belua::ResultCode::yield,
         ::be::belua::ResultCode::syntax_error,
         ::be::belua::ResultCode::runtime_error,
         ::be::belua::ResultCode::not_enough_memory,
         ::be::belua::ResultCode::handler_error,
         ::be::belua::ResultCode::gc_error,
         ::be::belua::ResultCode::file_io_error,
      };
   }
};

} // be

/* ######################### END OF GENERATED CODE ######################### */

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
class ResultCodeErrorCategory : public std::error_category {
public:
   virtual const char* name() const noexcept { return "Lua Result Code"; }
   virtual S message(int condition) const;
   virtual bool equivalent(int code, const std::error_condition& condition) const noexcept;
};

const std::error_category& result_code_error_category();
std::error_code make_error_code(ResultCode e);
std::error_condition make_error_condition(ResultCode e);
ResultCode result_code(int result);

} // be::belua

namespace std {

///////////////////////////////////////////////////////////////////////////////
template <>
struct is_error_code_enum<be::belua::ResultCode> : std::true_type { };

} // std

#endif
