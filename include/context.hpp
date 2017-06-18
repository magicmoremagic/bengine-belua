#pragma once
#ifndef BE_BELUA_CONTEXT_HPP_
#define BE_BELUA_CONTEXT_HPP_

#include "open_module.hpp"
#include <be/core/logging.hpp>
#include <be/core/exceptions.hpp>
#include <be/core/log_attrib_ids.hpp>
#include <gsl/string_span>
#include <lua/lualib.h>

namespace be::belua {
namespace detail {

struct LuaState final : Movable {
public:
   LuaState(bool init = false)
      : L(init ? luaL_newstate() : nullptr) { }

   LuaState(LuaState&& other)
      : L(other.L) {
      other.L = nullptr;
   }

   LuaState& operator=(LuaState&& other) {
      using std::swap;
      swap(L, other.L);
      return *this;
   }

   ~LuaState() {
      if (L) {
         lua_close(L);
         L = nullptr;
      }
   }

   lua_State* L;
};

} // be::belua::detail

class Context final {
public:
   Context();
   Context(std::initializer_list<luaL_Reg> modules, bool open_std_lib = true, bool use_environment = true);
   template <typename I>
   Context(I module_begin, I module_end, bool open_std_lib = true, bool use_environment = true) {
      reset(module_begin, module_end, open_std_lib, use_environment);
   }

   Context(Context&&) = default;
   Context& operator=(Context&&) = default;

   void reset(std::initializer_list<luaL_Reg> modules, bool open_std_lib = true, bool use_environment = true);
   template <typename I>
   void reset(I module_begin, I module_end, bool open_std_lib = true, bool use_environment = true) {
      detail::LuaState new_state(true);
      if (!new_state.L) {
         throw std::bad_alloc();
      }

      luaL_checkversion(new_state.L);

      if (!use_environment) {
         lua_pushboolean(new_state.L, 1);  // signal for libraries to ignore env. vars.
         lua_setfield(new_state.L, LUA_REGISTRYINDEX, "LUA_NOENV");
      }

      lua_gc(new_state.L, LUA_GCSTOP, 0);  // stop collector during initialization

      if (open_std_lib) {
         luaL_openlibs(new_state.L);
      }

      while (module_begin != module_end) {
         open_module(new_state.L, *module_begin);
         ++module_begin;
      }
         
      lua_gc(new_state.L, LUA_GCRESTART, 0); // start collector again

      state_ = std::move(new_state);
   }

   lua_State* L();
   explicit operator bool() const;

   void execute(gsl::cstring_span<> chunk, const S& chunk_name = "Lua");
   bool attempt(gsl::cstring_span<> chunk, const S& chunk_name = "Lua");

   void close();

private:
   detail::LuaState state_;
};

} // be::belua

#endif
