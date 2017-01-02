module { name = 'luacore',
   projects = {
      lib {
         src = {
            'src/*.cpp'
         },
         preprocessor = {
            'BE_LUACORE_IMPL'
         },
         libs = {
            'luaxx'
         }
      }
   }
}
