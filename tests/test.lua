#!/usr/bin/lua

-- simple test suite that checks all provided compatibilty functions
-- and saves their outputs to a file in such a way, that you can
-- easily compare results of different Lua versions side by side!

local print, F, writefile
local ____________________________________________________________
local outfile = "output_" .. _VERSION:gsub("[%s%.]+", "") .. ".txt"
do
   local separator = ("="):rep(70)
   local out = assert(io.open(outfile, "w"))

   local select, tostring = select, tostring
   function print(...)
      local n = select('#', ...)
      for i = 1, n-1 do
         out:write(tostring((select(i, ...))), "\t")
      end
      if n > 0 then
         out:write(tostring((select(n, ...))))
      end
      out:write("\n")
      out:flush()
   end

   function ____________________________________________________________()
      out:write(separator, "\n")
      out:flush()
   end

   local type, unpack = type, table.unpack or unpack
   function F(...)
      local args, n = { ... }, select('#', ...)
      for i = 1, n do
         local t = type(args[i])
         if t ~= "string" and t ~= "number" and t ~= "boolean" then
            args[i] = t
         end
      end
      return unpack(args, 1, n)
   end

   local assert, io = assert, io
   function writefile(name, contents, bin)
      local f = assert(io.open(name, bin and "wb" or "w"))
      f:write(contents)
      f:close()
   end
end


package.path = "../?.lua;" .. package.path
package.cpath = "../?.so;" .. package.cpath
-- load compatibility functions (in Lua 5.1)
require("compat52")

____________________________________________________________''
print("bit32.arshift()", bit32.arshift and bit32.arshift(2^32-4, 2))
print("bit32.band()", bit32.band and bit32.band(7, 5, 3))
print("bit32.bnot()", bit32.bnot and bit32.bnot(0))
print("bit32.bor()", bit32.bor and bit32.bor(1, 2, 4))
print("bit32.btest()", bit32.btest and bit32.btest(7, 5, 3))
print("bit32.bxor()", bit32.bxor and bit32.bxor(7, 5, 4, 3, 1))
print("bit32.extract()", bit32.extract and bit32.extract(7, 0, 2))
print("bit32.replace()", bit32.replace and bit32.replace(2^32-1, 0, 3, 29))
print("bit32.lrotate()", bit32.lrotate and bit32.lrotate(2^31, 1))
print("bit32.lshift()", bit32.lshift and bit32.lshift(2^31+2^30, 1))
print("bit32.rrotate()", bit32.rrotate and bit32.rrotate(2, 2))
print("bit32.rshift()", bit32.rshift and bit32.rshift(16, 3))
____________________________________________________________''
print("debug.getuservalue()", F(debug.getuservalue(false)))
print("debug.setuservalue()", pcall(function()
  debug.setuservalue(false, {})
end))
print("debug.setuservalue()", pcall(function()
  debug.setuservalue(io.stderr, 1)
end))
print("debug.setmetatable()", F(debug.setmetatable({}, {})))
____________________________________________________________''
do
   local t = setmetatable({}, {
      __ipairs = function() return ipairs({ 1, 2, 3 }) end,
      __pairs = function() return pairs({ a = "a" }) end,
   })
   for k,v in pairs(t) do
      print("pairs()", k, v)
   end
   for i,v in ipairs(t) do
      print("ipairs()", i, v)
   end
end
____________________________________________________________''
do
   local code = "print('hello world')\n"
   local badcode = "print('blub\n"
   print("load()", pcall(function() load(true) end))
   print("load()", F(load(badcode)))
   print("load()", F(load(code)))
   print("load()", F(load(code, "[L]")))
   print("load()", F(load(code, "[L]", "b")))
   print("load()", F(load(code, "[L]", "t")))
   print("load()", F(load(code, "[L]", "bt")))
   local f = load(code, "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(code, "[L]", "bt", { print = print })
   print("load()", pcall(f))
   local bytecode = string.dump(f)
   print("load()", F(load(bytecode)))
   print("load()", F(load(bytecode, "[L]")))
   print("load()", F(load(bytecode, "[L]", "b")))
   print("load()", F(load(bytecode, "[L]", "t")))
   print("load()", F(load(bytecode, "[L]", "bt")))
   f = load(bytecode, "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(bytecode, "[L]", "bt", { print = print })
   print("load()", pcall(f))
   local function make_loader(code)
      local mid = math.floor( #code/2 )
      local array = { code:sub(1, mid), code:sub(mid+1) }
      local i = 0
      return function()
         i = i + 1
         return array[i]
      end
   end
   print("load()", F(load(make_loader(badcode))))
   print("load()", F(load(make_loader(code))))
   print("load()", F(load(make_loader(code), "[L]")))
   print("load()", F(load(make_loader(code), "[L]", "b")))
   print("load()", F(load(make_loader(code), "[L]", "t")))
   print("load()", F(load(make_loader(code), "[L]", "bt")))
   f = load(make_loader(code), "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(make_loader(code), "[L]", "bt", { print = print })
   print("load()", pcall(f))
   print("load()", F(load(make_loader(bytecode))))
   print("load()", F(load(make_loader(bytecode), "[L]")))
   print("load()", F(load(make_loader(bytecode), "[L]", "b")))
   print("load()", F(load(make_loader(bytecode), "[L]", "t")))
   print("load()", F(load(make_loader(bytecode), "[L]", "bt")))
   f = load(make_loader(bytecode), "[L]", "bt", {})
   print("load()", pcall(f))
   f = load(make_loader(bytecode), "[L]", "bt", { print = print })
   print("load()", pcall(f))
   writefile("good.lua", code)
   writefile("bad.lua", badcode)
   writefile("good.luac", bytecode, true)
   print("loadfile()", F(loadfile("bad.lua")))
   print("loadfile()", F(loadfile("good.lua")))
   print("loadfile()", F(loadfile("good.lua", "b")))
   print("loadfile()", F(loadfile("good.lua", "t")))
   print("loadfile()", F(loadfile("good.lua", "bt")))
   f = loadfile("good.lua", "bt", {})
   print("loadfile()", pcall(f))
   f = loadfile("good.lua", "bt", { print = print })
   print("loadfile()", pcall(f))
   print("loadfile()", F(loadfile("good.luac")))
   print("loadfile()", F(loadfile("good.luac", "b")))
   print("loadfile()", F(loadfile("good.luac", "t")))
   print("loadfile()", F(loadfile("good.luac", "bt")))
   f = loadfile("good.luac", "bt", {})
   print("loadfile()", pcall(f))
   f = loadfile("good.luac", "bt", { print = print })
   print("loadfile()", pcall(f))
   os.remove("good.lua")
   os.remove("bad.lua")
   os.remove("good.luac")
end
____________________________________________________________''
do
   local function func(throw)
      if throw then
         error("argh")
      else
         return 1, 2, 3
      end
   end
   local function tb(err) return "|"..err.."|" end
   print("xpcall()", xpcall(func, debug.traceback, false))
   print("xpcall()", xpcall(func, debug.traceback, true))
   print("xpcall()", xpcall(func, tb, true))
   local function func2(cb)
     print("xpcall()", xpcall(cb, debug.traceback, "str"))
   end
   local function func3(cb)
     print("pcall()", pcall( cb, "str" ))
   end
   local function cb(arg)
      coroutine.yield( 2 )
      return arg
   end
   local c = coroutine.wrap( func2 )
   print("xpcall()", c(cb))
   print("xpcall()", c())
   local c = coroutine.wrap( func3 )
   print("pcall()", c(cb))
   print("pcall()", c())
end
____________________________________________________________''
do
   local t = setmetatable({ 1 }, { __len = function() return 5 end })
   print("rawlen()", rawlen(t), rawlen("123"))
end
____________________________________________________________''
print("collectgarbage()", collectgarbage("isrunning"))
print("collectgarbage()", collectgarbage("stop"))
print("collectgarbage()", collectgarbage("isrunning"))
print("collectgarbage()", collectgarbage("collect"))
print("collectgarbage()", collectgarbage("isrunning"))
print("collectgarbage()", collectgarbage("step", 10))
print("collectgarbage()", collectgarbage("isrunning"))
print("collectgarbage()", collectgarbage("restart"))
print("collectgarbage()", collectgarbage("isrunning"))
print("collectgarbage()", collectgarbage("count"))
print("collectgarbage()", collectgarbage("generational"))
print("collectgarbage()", collectgarbage("incremental"))
____________________________________________________________''
print("os.execute()", os.execute("exit 1"))
print("os.execute()", os.execute("echo 'hello world'"))
print("os.execute()", os.execute("no_such_file"))
____________________________________________________________''
do
   local t = table.pack("a", nil, "b", nil)
   print("table.(un)pack()", t.n, table.unpack(t, 1, t.n))
end
____________________________________________________________''
do
   print("coroutine.running()", F(coroutine.wrap(function()
      return coroutine.running()
   end)()))
   print("coroutine.running()", F(coroutine.running()))
   local main_co, co1, co2 = coroutine.running()
   -- coroutine.yield
   print("coroutine.yield()", pcall(function()
      coroutine.yield(1, 2, 3)
   end))
   print("coroutine.yield()", coroutine.wrap(function()
      coroutine.yield(1, 2, 3)
   end)())
   print("coroutine.resume()", coroutine.resume(main_co, 1, 2, 3))
   co1 = coroutine.create(function(a, b, c)
      print("coroutine.resume()", a, b, c)
      return a, b, c
   end)
   print("coroutine.resume()", coroutine.resume(co1, 1, 2, 3))
   co1 = coroutine.create(function()
      print("coroutine.status()", "[co1] main is", coroutine.status(main_co))
      print("coroutine.status()", "[co1] co2 is", coroutine.status(co2))
   end)
   co2 = coroutine.create(function()
      print("coroutine.status()", "[co2] main is", coroutine.status(main_co))
      print("coroutine.status()", "[co2] co2 is", coroutine.status(co2))
      coroutine.yield()
      coroutine.resume(co1)
   end)
   print("coroutine.status()", coroutine.status(main_co))
   print("coroutine.status()", coroutine.status(co2))
   coroutine.resume(co2)
   print("coroutine.status()", F(coroutine.status(co2)))
   coroutine.resume(co2)
   print("coroutine.status()", F(coroutine.status(co2)))
end
____________________________________________________________''
print("math.log()", math.log(1000))
print("math.log()", math.log(1000, 10))
____________________________________________________________''
do
   local path, prefix = "./?.lua;?/init.lua;../?.lua", "package.searchpath()"
   print(prefix, package.searchpath("no.such.module", path))
   print(prefix, package.searchpath("no.such.module", ""))
   print(prefix, package.searchpath("compat52", path))
   print(prefix, package.searchpath("no:such:module", path, ":", "|"))
end
____________________________________________________________''
do
   local function mod_func() return {} end
   local function my_searcher(name)
      if name == "my.module" then
         print("package.searchers", "my.module found")
         return mod_func
      end
   end
   local function my_searcher2(name)
      if name == "my.module" then
         print("package.searchers", "my.module found 2")
         return mod_func
      end
   end
   table.insert(package.searchers, my_searcher)
   require("my.module")
   package.loaded["my.module"] = nil
   local new_s = { my_searcher2 }
   for i,f in ipairs(package.searchers) do
      new_s[i+1] = f
   end
   package.searchers = new_s
   require("my.module")
end
____________________________________________________________''
do
   print("string.find()", ("abc\0abc\0abc"):find("[^a\0]+"))
   print("string.find()", ("abc\0abc\0abc"):find("%w+\0", 5))
   for x in ("abc\0def\0ghi"):gmatch("[^\0]+") do
      print("string.gmatch()", x)
   end
   for x in ("abc\0def\0ghi"):gmatch("%w*\0") do
      print("string.gmatch()", #x)
   end
   print("string.gsub()", ("abc\0def\0ghi"):gsub("[\0]", "X"))
   print("string.gsub()", ("abc\0def\0ghi"):gsub("%w*\0", "X"))
   print("string.gsub()", ("abc\0def\0ghi"):gsub("%A", "X"))
   print("string.match()", ("abc\0abc\0abc"):match("([^\0a]+)"))
   print("string.match()", #("abc\0abc\0abc"):match(".*\0"))
   print("string.rep()", string.rep("a", 0))
   print("string.rep()", string.rep("b", 1))
   print("string.rep()", string.rep("c", 4))
   print("string.rep()", string.rep("a", 0, "|"))
   print("string.rep()", string.rep("b", 1, "|"))
   print("string.rep()", string.rep("c", 4, "|"))
   local _tostring = tostring
   function tostring(v)
      if type(v) == "number" then
         return "(".._tostring(v)..")"
      else
         return _tostring(v)
      end
   end
   print("string.format()", string.format("%12.3fx%%sxx%.6s", 3.1, {}))
   print("string.format()", string.format("%-3f %%%s %%s", 3.1, true))
   print("string.format()", string.format("% 3.2g %%d %%%s", 3.1, nil))
   print("string.format()", string.format("%+3d %%d %%%%%10.6s", 3.1, io.stdout))
   print("string.format()", pcall(function()
      print("string.format()", string.format("%d %%s", {}))
   end))
   tostring = _tostring
end
____________________________________________________________''
do
   print("io.write()", io.type(io.write("hello world\n")))
   local f = assert(io.tmpfile())
   print("file:write()", io.type(f:write("hello world\n")))
   f:close()
end
____________________________________________________________''
do
   writefile("data.txt", "123 18.8 hello world\ni'm here\n")
   for a,b in io.lines("test.lua", 2, "*l") do
      print("io.lines()", a, b)
      break
   end
   for l in io.lines("test.lua") do
      print("io.lines()", l)
      break
   end
   for n1,n2,rest in io.lines("data.txt", "*n", "*n", "*a") do
      print("io.lines()", n1, n2, rest)
   end
   for l in io.lines("data.txt") do
      print("io.lines()", l)
   end
   print("io.lines()", pcall(function()
      for l in io.lines("data.txt", "*x") do print(l) end
   end))
   print("io.lines()", pcall(function()
      for l in io.lines("no_such_file.txt") do print(l) end
   end))
   local f = assert(io.open("test.lua", "r"))
   for a,b in f:lines(2, "*l") do
      print("file:lines()", a, b)
      break
   end
   f:close()
   f = assert(io.open("data.txt", "r"))
   for n1,n2,rest in f:lines("*n", "*n", "*a") do
      print("file:lines()", n1, n2, rest)
   end
   f:close()
   f = assert(io.open("data.txt", "r"))
   for l in f:lines() do
      print("file:lines()", l)
   end
   f:close()
   print("file:lines()", pcall(function()
      for l in f:lines() do print(l) end
   end))
   print("file:lines()", pcall(function()
      local f = assert(io.open("data.txt", "r"))
      for l in f:lines("*l", "*x") do print(l) end
      f:close()
   end))
   os.remove("data.txt")
end
____________________________________________________________''
do
   local modname = _VERSION:gsub("^.*(%d+)%.(%d+).-$", "%1%2-testmod")
   local ok, mod = pcall(require, modname)
   if not ok then
      io.write("###  no ", modname, ".so or ", modname, ".dll!  ###\n")
      io.write("'require' claimed: ", mod, "\n")
      io.write("###         Skipping C API tests!         ###\n")
   else
      print("C API", mod.tonumber(12))
      print("C API", mod.tonumber("12"))
      print("C API", mod.tonumber("0"))
      print("C API", mod.tonumber(false))
      print("C API", mod.tonumber("error"))
      print("C API", mod.tointeger(12))
      print("C API", mod.tointeger("12"))
      print("C API", mod.tointeger("0"))
      print("C API", mod.tointeger(math.pi))
      print("C API", mod.tointeger(false))
      print("C API", mod.tointeger("error"))
      print("C API", mod.unsigned(1))
      print("C API", mod.unsigned(2^31+1))
      print("C API", mod.unsigned(2^32+1))
      print("C API", mod.unsigned(-1))
      print("C API", mod.unsigned(2^56))
      print("C API", mod.unsigned("17"))
      print("C API", pcall(mod.unsigned, "0"))
      print("C API", pcall(mod.unsigned, true))
      print("C API", mod.optunsigned())
      print("C API", mod.optunsigned(42))
      print("C API", pcall(mod.optunsigned, true))
      print("C API", mod.len("123"))
      print("C API", mod.len({ 1, 2, 3}))
      print("C API", pcall(mod.len, true))
      local ud, meta = mod.newproxy()
      meta.__len = function() return 5 end
      print("C API", mod.len(ud))
      meta.__len = function() return true end
      print("C API", pcall(mod.len, ud))
      print("C API", mod.copy(true, "string", {}, 1))
      print("C API", mod.rawxetp())
      print("C API", mod.rawxetp("I'm back"))
      print("C API", F(mod.globals()), mod.globals() == _G)
      local t = {}
      print("C API", F(mod.subtable(t)))
      local x, msg = mod.subtable(t)
      print("C API", F(x, msg, x == t.xxx))
      print("C API", F(mod.udata()))
      print("C API", mod.udata("nosuchtype"))
      print("C API", F(mod.uservalue()))
      print("C API", mod.getupvalues())
      print("C API", mod.absindex("hi", true))
      print("C API", mod.tolstring("string"))
      local t = setmetatable({}, {
        __tostring = function(v) return "mytable" end
      })
      print("C API", mod.tolstring( t ) )
      local t = setmetatable({}, {
        __tostring = function(v) return nil end
      })
      print("C API", pcall( mod.tolstring, t ) )
      local a, b = mod.requiref()
      print("C API", type(a), type(b), a.boolean, b.boolean,
            nil==requiref1, b==requiref2, package.loaded.requiref1==a)
      print("debug.getuservalue()", F(debug.getuservalue(ud)))
      print("debug.setuservalue()", F(debug.setuservalue(ud, {})))
      print("debug.getuservalue()", F(debug.getuservalue(ud)))
      print("debug.setuservalue()", F(debug.setuservalue(ud, nil)))
      print("debug.getuservalue()", F(debug.getuservalue(ud)))
   end
end
____________________________________________________________''
io.write("###  Output written to ", outfile, "!  ###\n")

