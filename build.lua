module 'luacore' {
   lib {
      define 'BE_LUACORE_IMPL',
      link_project 'luaxx',
      link_project 'core'
   }
}
