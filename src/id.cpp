#include "id.hpp"
#include <be/core/id.hpp>

namespace be {
namespace lua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg id_module { "be.Id", open_id };

namespace {

///////////////////////////////////////////////////////////////////////////////
const char* metatable() {
   return "class be.Id";
}

///////////////////////////////////////////////////////////////////////////////
Id parse_id(lua_State* L) {
   Id id;

   // if the first parameter is "be.Id", remove it.  This
   // allows either be.Id:new() or be.Id.new() to be used.
   if (lua_gettop(L) >= 1 && lua_istable(L, 1)) {
      lua_getmetatable(L, 1);
      luaL_getmetatable(L, metatable());

      if (lua_rawequal(L, -1, -2)) {
         lua_remove(L, 1);
      }

      lua_pop(L, 2); // remove metatables
   }

   int type = lua_type(L, 1);
   switch (type) {
      case LUA_TNONE:
      case LUA_TNIL:
         break;

      case LUA_TNUMBER:
         id = Id((U64)lua_tointeger(L, 1));
         break;

      case LUA_TSTRING:
         id = Id(lua_tostring(L, 1));
         break;

      default:
         luaL_argerror(L, 1, "expected integer or string!");
   }

   return id;
}

///////////////////////////////////////////////////////////////////////////////
int id_new(lua_State* L) {
   I64 val = (I64)(U64)parse_id(L);
   lua_settop(L, 0);
   lua_pushinteger(L, val);
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int id_name(lua_State* L) {
   Id id = parse_id(L);
   S name = get_id_name(id);
   lua_settop(L, 0);
   lua_pushlstring(L, name.c_str(), name.length());
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int id_to_string(lua_State* L) {
   Id id = parse_id(L);
   S str = (S)id;
   lua_settop(L, 0);
   lua_pushlstring(L, str.c_str(), str.length());
   return 1;
}

///////////////////////////////////////////////////////////////////////////////
int id_canonical(lua_State* L) {
   Id id = parse_id(L);
   S str = get_canonical_id_string(id);
   lua_settop(L, 0);
   lua_pushlstring(L, str.c_str(), str.length());
   return 1;
}

} // be::lua::()

///////////////////////////////////////////////////////////////////////////////
int open_id(lua_State* L) {
   luaL_Reg fn[] {
      { "new", id_new },
      { "name", id_name },
      { "tostring", id_to_string },
      { "canonical", id_canonical },
      { nullptr, nullptr }
   };

   luaL_Reg meta[] {
      { "__call", id_new },
      { nullptr, nullptr }
   };

   // create class and and set class metatable
   luaL_newlib(L, fn);
   luaL_newmetatable(L, metatable());
   luaL_setfuncs(L, meta, 0);
   lua_setmetatable(L, -2);

   return 1;
}

} // be::lua
} // be
