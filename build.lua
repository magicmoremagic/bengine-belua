module 'belua' {
   lib {
      define 'BE_BELUA_IMPL',
      link_project 'luaxx',
      link_project 'core'
   }
}
