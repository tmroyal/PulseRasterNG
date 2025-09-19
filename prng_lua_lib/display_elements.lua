-- TODO: add mouse/touch support

local Slider = {}
-- patch.lua
Slider.__index = Slider

function Slider:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.w = args.width or args.w or 100
    o.h = args.height or args.h or 20
    o.vertical = args.vertical or false
    o.value = args.value or 0.5

    local tw = textWidth(o.label, 12)
    o._text_x = o.x + (o.w - tw) / 2
    return o
end

function Slider:setValue(val)
    self.value = math.max(0, math.min(1, val))
end

function Slider:draw()
    -- color is set externally
    thickness(1)
    text(self.label, self._text_x, self.y - 15, 12)
    box(self.x, self.y, self.w, self.h)
    if self.vertical then
        local fillHeight = self.h * self.value
        local fillY = self.y + (self.h - fillHeight)
        rect(self.x, fillY, self.w, fillHeight)
    else
        local fillWidth = self.w * self.value
        rect(self.x, self.y, fillWidth, self.h)
    end
end

local Knob = {}
Knob.__index = Knob

function Knob:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.radius = args.radius or 20
    o.value = args.value or 0.5

    local tw = textWidth(o.label, 12)
    o._text_x = o.x - tw / 2
    return o
end

function Knob:setValue(val)
    self.value = math.max(0, math.min(1, val))
end

function Knob:draw()
    -- color is set externally
    thickness(1)
    text(self.label, self._text_x, self.y - self.radius - 15, 12)
    ring(self.x, self.y, self.radius)
    local angle = self.value * math.pi * 2 - math.pi / 2
    local knobX = self.x + math.cos(angle) * self.radius
    local knobY = self.y + math.sin(angle) * self.radius
    line(self.x, self.y, knobX, knobY)
end

local XY = {}
XY.__index = XY

function XY:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.size = args.size or 100
    o.valueX = args.valueX or args.x_value or 0.5
    o.valueY = args.valueY or args.y_value or 0.5

    local tw = textWidth(o.label, 12)
    o._text_x = o.x + (o.size - tw) / 2
    return o
end

function XY:setValues(valX, valY)
    self.valueX = math.max(0, math.min(1, valX))
    self.valueY = math.max(0, math.min(1, valY))
end

function XY:draw()
    thickness(1)
    text(self.label, self._text_x, self.y - 15, 12)
    box(self.x, self.y, self.size, self.size)
    local fillX = self.x + self.size * self.valueX
    local fillY = self.y + self.size * (1 - self.valueY)
    line(self.x, fillY, self.x + self.size, fillY)
    line(fillX, self.y, fillX, self.y + self.size)
    rect(fillX - 3, fillY - 3, 6, 6)
end

local Button = {}
Button.__index = Button

function Button:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.w = args.width or args.w or 50
    o.h = args.height or args.h or 20
    o.value = args.value or false

    local tw = textWidth(o.label, 12)
    o._text_x = o.x + (o.w - tw) / 2
    return o
end

function Button:setValue(val)
    if type(val) == "boolean" then
        self.value = val
    elseif type(val) == "number" then
        self.value = (val ~= 0)
    end
end

function Button:draw()
    -- color is set externally
    thickness(1)
    if #self.label > 0 then
        text(self.label, self._text_x, self.y - 15, 12)
    end
    if self.value then
        rect(self.x, self.y, self.w, self.h)
    else
        box(self.x, self.y, self.w, self.h)
    end
end

local Grid = {}
Grid.__index = Grid

function Grid:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.rows = args.rows or 4
    o.cols = args.cols or 4
    local cellSize = args.cellSize or args.cell_size or 20
    local cellMargin = args.cellMargin or args.cell_margin or 2
    o.buttons = {}

    for r = 1, o.rows do
        o.buttons[r] = {}
        for c = 1, o.cols do
            local bx = o.x + (c - 1) * (cellSize + cellMargin)
            local by = o.y + (r - 1) * (cellSize + cellMargin)
            o.buttons[r][c] = Button:new{
                label = "",
                x = bx,
                y = by,
                width = cellSize,
                height = cellSize,
                value = false
            }
        end
    end

    local tw = textWidth(o.label, 12)
    o._text_x = o.x + (o.cols * cellSize - tw) / 2
    return o
end

function Grid:setValue(row, col, val)
    if row >= 1 and row <= self.rows and col >= 1 and col <= self.cols then
        self.buttons[row][col]:setValue(val)
    end
end

function Grid:draw()
    thickness(1)
    text(self.label, self._text_x, self.y - 15, 12)
    for r = 1, self.rows do
        for c = 1, self.cols do
            self.buttons[r][c]:draw()
        end
    end
end

local WaveformLine = {}
WaveformLine.__index = WaveformLine

function WaveformLine:new(x1, y1, x2, y2)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.x1 = x1
    o.y1 = y1
    o.x2 = x2
    o.y2 = y2
    return o
end

function WaveformLine:draw()
    line(self.x1, self.y1, self.x2, self.y2)
end

local Waveform = {}
Waveform.__index = Waveform

function Waveform:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.w = args.width or args.w or 200
    o.h = args.height or args.h or 100
    o.values = args.values or {}

    if #o.values > o.w / 8 then
        error("Too many values for waveform width. Only width/8 values allowed.")
    end

    o.line_thickness = 1
    local tw = textWidth(o.label, 12)
    o._text_x = o.x + (o.w - tw) / 2
    return o
end

function Waveform:constructLines()
    self.line_thickness = math.max(1, math.floor(self.w / #self.values - 1)) * 1.1
    local lines = {}
    local center = self.y + self.h / 2
    local bin_size = self.w / (#self.values - 1)
    for i = 1, #self.values - 1 do
        local disp = self.values[i] * (self.h / 2)
        local x = self.x + (i - 1) * bin_size + bin_size / 2
        local y1 = center - disp
        local y2 = center + disp
        table.insert(lines, WaveformLine:new(x, y1, x, y2))
    end
    return lines
end

function Waveform:setValues(vals)
    self.values = vals or {}
    if #self.values > self.w / 8 then
        error("Too many values for waveform width. Only width/8 values allowed.")
    end
end

function Waveform:draw()
    thickness(1)
    text(self.label, self._text_x, self.y - 15, 12)
    box(self.x, self.y, self.w, self.h)
    if #self.values < 2 then
        return
    end
    local lines = self:constructLines()
    thickness(self.line_thickness)
    for _, lineObj in ipairs(lines) do
        lineObj:draw()
    end
end

return {
    Slider = Slider,
    Knob = Knob,
    XY = XY,
    Button = Button,
    Grid = Grid,
    Waveform = Waveform,
}
