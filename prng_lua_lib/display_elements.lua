-- TODO: add mouse/touch support

local Slider = {}
-- patch.lua
Slider.__index = Slider

function Slider:new(label, x, y, w, h, vertical, init)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.label = label
    o.x = x
    o.y = y
    o.w = w
    o.h = h
    o.vertical = vertical
    o.value = init or 0.5
    local tw = textWidth(label, 12)
    o._text_x = x + (w - tw) / 2
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

function Knob:new(label, x, y, radius, init)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.label = label
    o.x = x
    o.y = y
    o.radius = radius
    o.value = init or 0.5
    local tw = textWidth(label, 12)
    o._text_x = x - tw / 2
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

function XY:new(label, x, y, size, initX, initY)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.label = label
    o.x = x
    o.y = y
    o.size = size
    o.valueX = initX or 0.5
    o.valueY = initY or 0.5
    local tw = textWidth(label, 12)
    o._text_x = x + (size - tw) / 2
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

function Button:new(label, x, y, w, h, init)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.label = label
    o.x = x
    o.y = y
    o.w = w
    o.h = h
    o.value = init or false
    local tw = textWidth(label, 12)
    o._text_x = x + (w - tw) / 2
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

function Grid:new(label, x, y, cellSize, cellMargin, rows, cols)
    local o = {}
    setmetatable(o, self)
    self.__index = self
    o.label = label
    o.x = x
    o.y = y
    o.rows = rows
    o.cols = cols
    o.buttons = {}

    for r = 1, rows do
        o.buttons[r] = {}
        for c = 1, cols do
            local bx = x + (c - 1) * (cellSize + cellMargin)
            local by = y + (r - 1) * (cellSize + cellMargin)
            o.buttons[r][c] = Button:new("", bx, by, cellSize, cellSize, false)
        end
    end

    local tw = textWidth(label, 12)
    o._text_x = x + (cols * cellSize - tw) / 2
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

return {
    Slider = Slider,
    Knob = Knob,
    XY = XY,
    Button = Button,
    Grid = Grid,
}
