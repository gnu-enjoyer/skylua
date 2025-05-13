# skylua

skylua glues LuaJIT via sol2 and CommonLibSSE NG to provide an embedded, fast and lightweight scripting runtime for Skyrim

## features

* Lua 5.1 (JIT)
  * bitwise support
- hot reload
- auto complete

https://github.com/user-attachments/assets/40409424-91d4-40e8-af5f-a9a8acf84edf

## quick start

1. dl latest release and place `skylua.dll` in `<path to skyrim>/SKSE/Plugins/`
2. skylua runs .lua files in `<path to skyrim>/SkyLua`
3. (optional) lua definitions can be placed in `./SkyLua/types/skylua.lua`

## requirements

- SKSE
- Address Library for SKSE
