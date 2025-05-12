# skylua

skylua glues LuaJIT via sol2 and CommonLibSSE NG to provide an embedded, fast and lightweight scripting runtime for Skyrim

## features

* Lua 5.1 (JIT)
  * bitwise support
- hot reload

## example

```lua
function SkyLua.OnTick()
  SkyLua.notify("hello world")
end
```

## requirements

- SKSE to load .dll

