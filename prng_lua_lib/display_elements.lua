-- Base class for interactive display elements
local DisplayElement = {}
DisplayElement.__index = DisplayElement

function DisplayElement:new(args)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.label = args.label or ""
    o.x = args.x or 0
    o.y = args.y or 0
    o.mouse_down = false
    o.mouse_handlers_attached = false

    return o
end

function DisplayElement:withinBounds(x, y)
    return x >= self.x and x <= (self.x + (self.w or self.size or self.radius * 2)) and
           y >= self.y and y <= (self.y + (self.h or self.size or self.radius * 2))
end

function DisplayElement:attachMouseHandlers()
    if not self.mouse_handlers_attached then
        mouseDown(0, function(x, y) self:onMouseDown(x, y) end)
        mouseUp(0, function(x, y) self:onMouseUp(x, y) end)
        self.mouse_handlers_attached = true
    end
end

function DisplayElement:onMouseDown(x, y)
    if self:withinBounds(x, y) then
        self.mouse_down = true
        if self.onInteractionStart then
            self:onInteractionStart(x, y)
        end
    end
end

function DisplayElement:onMouseUp(x, y)
    if self.mouse_down then
        self.mouse_down = false
        if self.onInteractionEnd then
            self:onInteractionEnd(x, y)
        end
    end
end

function DisplayElement:setOnChangeHandler(handler)
    self.onChange = handler
end

function DisplayElement:triggerChange(value)
    if self.onChange then
        self:onChange(value)
    end
end

local Slider = {}
Slider.__index = Slider
setmetatable(Slider, {__index = DisplayElement})

function Slider:new(args)
    local o = DisplayElement.new(self, args)

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

    if self.mouse_down then
        self:updateMousePosition()
    end
end

function Slider:updateMousePosition()
    local mx, my = mousePosition()
    local lastValue = self.value
    if self.vertical then
        local relY = math.max(0, math.min(self.h, my - self.y))
        self:setValue(1 - (relY / self.h))
    else
        local relX = math.max(0, math.min(self.w, mx - self.x))
        self:setValue(relX / self.w)
    end

    if lastValue ~= self.value then
        self:triggerChange(self.value)
    end
end

function Slider:onInteractionStart(x, y)
    self:updateMousePosition()
end

local Knob = {}
Knob.__index = Knob
setmetatable(Knob, {__index = DisplayElement})

function Knob:new(args)
    local o = DisplayElement.new(self, args)

    o.radius = args.radius or 20
    o.value = args.value or 0.5
    o.last_mouse_angle = 0

    local tw = textWidth(o.label, 12)
    o._text_x = o.x - tw / 2
    return o
end

function Knob:setValue(val)
    self.value = math.max(0, math.min(1, val))
end

function Knob:draw()
    thickness(1)
    text(self.label, self._text_x, self.y - self.radius - 15, 12)
    ring(self.x, self.y, self.radius)
    local angle = self.value * math.pi * 2 + math.pi / 2
    local knobX = self.x + math.cos(angle) * self.radius
    local knobY = self.y + math.sin(angle) * self.radius
    line(self.x, self.y, knobX, knobY)

    local value_text = string.format("%.2f", self.value)
    local value_tw = textWidth(value_text, 10)
    text(value_text, self.x - value_tw / 2, self.y + self.radius + 20, 10)

    if self.mouse_down then
        self:updateMousePosition()
    end
end

function Knob:updateMousePosition()
    local mx, my = mousePosition()

    if not self.start_mouse_x then
        return
    end

    -- Movement-based calculation
    local dx = mx - self.start_mouse_x
    local dy = my - self.start_mouse_y
    local total_movement = dx + dy
    local normalized_movement = total_movement / (self.radius * 4)

    local new_value = math.max(0, math.min(1, normalized_movement))

    local lastValue = self.value
    self:setValue(new_value)

    if lastValue ~= self.value then
        self:triggerChange(self.value)
    end
end

function Knob:withinBounds(x, y)
    local dx = x - self.x
    local dy = y - self.y
    local distance = math.sqrt(dx * dx + dy * dy)
    return distance <= self.radius
end

function Knob:onInteractionStart(x, y)
    local mx, my = mousePosition()
    self.start_mouse_x = mx
    self.start_mouse_y = my
end

local XY = {}
XY.__index = XY
setmetatable(XY, {__index = DisplayElement})

function XY:new(args)
    local o = DisplayElement.new(self, args)

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

    if self.mouse_down then
        self:updateMousePosition()
    end
end

function XY:updateMousePosition()
    local mx, my = mousePosition()
    local relX = math.max(0, math.min(self.size, mx - self.x))
    local relY = math.max(0, math.min(self.size, my - self.y))

    local lastValueX = self.valueX
    local lastValueY = self.valueY

    self:setValues(relX / self.size, 1 - (relY / self.size))

    if lastValueX ~= self.valueX or lastValueY ~= self.valueY then
        self:triggerChange({x = self.valueX, y = self.valueY})
    end
end

function XY:onInteractionStart(x, y)
    self:updateMousePosition()
end

local Button = {}
Button.__index = Button
setmetatable(Button, {__index = DisplayElement})

function Button:new(args)
    local o = DisplayElement.new(self, args)

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

function Button:onInteractionStart(x, y)
    local lastValue = self.value
    self:setValue(not self.value)

    if lastValue ~= self.value then
        self:triggerChange(self.value)
    end
end

local Grid = {}
Grid.__index = Grid
setmetatable(Grid, {__index = DisplayElement})

function Grid:new(args)
    local o = DisplayElement.new(self, args)

    o.rows = args.rows or 4
    o.cols = args.cols or 4
    local cellSize = args.cellSize or args.cell_size or 20
    local cellMargin = args.cellMargin or args.cell_margin or 2
    o.cellSize = cellSize
    o.cellMargin = cellMargin
    o.buttons = {}

    o.w = o.cols * (cellSize + cellMargin) - cellMargin
    o.h = o.rows * (cellSize + cellMargin) - cellMargin

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

function Grid:onInteractionStart(x, y)
    for r = 1, self.rows do
        for c = 1, self.cols do
            local button = self.buttons[r][c]
            if button:withinBounds(x, y) then
                local lastValue = button.value
                button:setValue(not button.value)
                if lastValue ~= button.value then
                    self:triggerChange({row = r, col = c, value = button.value})
                end
                return
            end
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
        if disp < 1 then
            disp = 1
        end
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
