#include "open_module.hpp"
#include "context.hpp"
#include "lua_helpers.hpp"

namespace be {
namespace lua {
namespace {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Parses any '.' characters in a module name to find the correct
///         table to place a module object in.
///
/// \pre    Lua Stack:
///               -1.  Global Environment
///
/// \post   Lua Stack:
///               -1.  Parent table of the unqualified
///
/// \param  L The Lua state.
///
/// \param  module The name of the module, possibly including '.' characters.
///
/// \return The unqualified module name, not including anything before a '.'.
const char* get_parent_table(lua_State* L, const char* module) {
   const char* i = strchr(module, '.');

   if (!i) {
      // if there are no more parent tables, return
      return module;
   }

   // stack(-1) should contain the table where this module name goes.

   lua_pushlstring(L, module, i - module);   // push key name
   lua_rawget(L, -2);                        // pop key and push value

   if (lua_isnil(L, -1)) {             // if key does not exist
      lua_pop(L, 1);                   // pop nil value
      lua_newtable(L);                 // create new table
      lua_pushlstring(L, module, i - module);   // push key name again
      lua_pushvalue(L, -2);            // copy table to top of stack
      lua_rawset(L, -4);               // parent[child] = table
      lua_remove(L, -2);               // remove parent from stack
   } else if (lua_istable(L, -1)) {    // key represents existing table
      lua_remove(L, -2);               // remove parent from stack
   } else {                            // key represents some other value -- error!
      luaL_error(L, "Can't register module; expected nil or table for parent");
      //return nullptr;   // unreachable
   }

   return get_parent_table(L, i + 1);
}

} // be::lua::()

///////////////////////////////////////////////////////////////////////////////
void open_module(lua_State* L, const luaL_Reg& module) {
   open_module(L, module.name, module.func);
}

///////////////////////////////////////////////////////////////////////////////
/// \details Module names including one or more '.' characters will be placed
///         into the corresponding parent table in the global environment.
///
/// \note   An opened module overwrites any object already in its key.
///         For instance, if we have openers for "a.b", "a.c", and "a", and
///         they are registered in that order, the object opened by "a" will
///         overwrite "a.b" and "a.c"'s parent table, making them "invisible"
///         though they could still be accessed using `require`.  If the
///         openers are registered in the order "a", "a.b", "a.c", however,
///         the table opened by "a" will be extended with keys `b` and `c`
///         (overwriting any key by that name created by "a").  This means
///         that if there are openers for both a parent and one of its
///         descendants, the parent should be opened first.
///
/// \param  L The Lua state.
void open_module(lua_State* L, const char* module_name, lua_CFunction module_opener) {
   if (module_name == nullptr) {
      lua_pushcfunction(L, module_opener);
      ecall(L, 0, 0);
   } else {
      lua_pushvalue(L, LUA_REGISTRYINDEX);
      lua_rawgeti(L, -1, LUA_RIDX_GLOBALS);
      lua_remove(L, -2); // remove registry table
      const char* key = get_parent_table(L, module_name);  // replace duplicate env with parent table
      lua_pushstring(L, key); // push key to index parent table

      luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
      lua_getfield(L, -1, module_name);  /* _LOADED[modname] */
      if (!lua_toboolean(L, -1)) {  /* package not already loaded? */
         lua_pop(L, 1);  /* remove field */
         lua_pushcfunction(L, module_opener);
         lua_pushstring(L, module_name);  /* argument to open function */
         ecall(L, 1, 1);  /* call 'openf' to open module */
         lua_pushvalue(L, -1);  /* make copy of module (call result) */
         lua_setfield(L, -3, module_name);  /* _LOADED[modname] = module */
      }
      lua_remove(L, -2);  /* remove _LOADED table */
      lua_rawset(L, -3);      // set parent[key] = module
      lua_pop(L, 1);          // pop parent
   }
}

} // be::lua
} // be
