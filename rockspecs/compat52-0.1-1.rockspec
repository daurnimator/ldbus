package = "compat52"
version = "0.1-1"
source = {
   url = "https://github.com/hishamhm/lua-compat-5.2/archive/v0.1.zip",
   dir = "lua-compat-5.2-0.1",
}
description = {
   summary = "Compatibility module providing Lua-5.2-style APIs for Lua 5.1",
   detailed = [[
      This is a small module that aims to make it easier to write Lua code
      in a Lua-5.2-style that runs on both Lua 5.1 and Lua 5.2. This does *not*
      make Lua 5.1 entirely compatible with Lua 5.2, but it brings the API
      closer to that of Lua 5.2.
   ]],
   homepage = "https://github.com/hishamhm/lua-compat-5.2",
   license = "MIT/X11",
}
dependencies = {
   "lua >= 5.1, < 5.3",
   "bit32"
}
build = {
   type = "builtin",
   modules = {
      ["compat52"] = "compat52.lua",
   }
}
