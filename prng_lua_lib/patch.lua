-- patch.lua
local Patch = {}
Patch.__index = Patch

-- Patch(name, args?) -> loads PD patch and applies optional args table
function Patch:new(name, args)
  local h = assert(load_patch(name), "load_patch failed: "..tostring(name))
  local o = setmetatable({ name = name, handle = h }, self)
  if args then o:apply(args) end
  return o
end

-- Send one PD message (varargs)
function Patch:msg(cmd, ...)
  pd_msg(self.handle, cmd, ...)
  return self
end

-- Apply a table of { cmd = value | {args...}, ... }
function Patch:apply(t)
  for cmd, v in pairs(t) do
    if type(v) == "table" then
      pd_msg(self.handle, cmd, table.unpack(v))
    else
      pd_msg(self.handle, cmd, v)
    end
  end
  return self
end

-- Bind a PD event symbol to a Lua callback
-- Example: p:on("42-rec", function(a,b) ... end)  -- if your receiver is handle.."-rec"
function Patch:on_i(symbol, fn, opts)
  assert(type(symbol)=="string" and type(fn)=="function", "on(symbol, fn[, opts])")
  local inject = not (opts and opts.self == false)   -- default: inject self
  if inject then
    patchEvent(symbol, function(...) return fn(self, ...) end)
  else
    patchEvent(symbol, fn)
  end
  return self
end

-- optional sugar: never inject self
function Patch:on(symbol, fn)
  return self:on_i(symbol, fn, { self = false })
end

-- Free underlying PD patch
function Patch:free()
  if self.handle then
    pcall(free_patch, self.handle)
    self.handle = nil
  end
end

return setmetatable(Patch, {
  __call = function(_, name, args) return Patch:new(name, args) end
})
