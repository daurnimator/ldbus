package = "bit32"

version = "5.2.2-1"

source = {
   url = "https://raw.github.com/hishamhm/lua-compat-5.2/bitlib-5.2.2/lbitlib.c",
}

description = {
   summary = "Lua 5.2 bit manipulation library",
   detailed = [[
      bit32 is the native Lua 5.2 bit manipulation library,
      backported to Lua 5.1
   ]],
   license = "MIT/X11",
   homepage = "http://lua.org/work/",
}

dependencies = {
   "lua >= 5.1, < 5.2"
}

build = {
   type = "builtin",
   modules = {
      bit32 = "lbitlib.c",
   }
}
