-- TODO: add mouse/touch support

local Slider = {}
-- patch.lua
Slider.__index = Slider

function Slider:new(label, x, y, w, h, vertical, init)
    local o = {};
    setmetatable(o, self);
    self.__index = self;
    o.label = label;
    o.x = x;
    o.y = y;
    o.w = w;
    o.h = h;
    o.vertical = vertical;
    o.value = init or 0.5;
    local tw = textWidth(label, 12);
    o._text_x = x + (w - tw) / 2;
    return o;
end

function Slider:setValue(val)
    self.value = math.max(0, math.min(1, val));
end

function Slider:draw()
    -- color is set externally
    thickness(1);
    text(self.label, self._text_x, self.y - 15, 12);
    box(self.x, self.y, self.w, self.h);
    if self.vertical then
        local fillHeight = self.h * self.value;
        local fillY = self.y + (self.h - fillHeight);
        rect(self.x, fillY, self.w, fillHeight);
    else
        local fillWidth = self.w * self.value;
        rect(self.x, self.y, fillWidth, self.h);
    end
end

local Knob = {}
Knob.__index = Knob

function Knob:new(label, x, y, radius, init)
    local o = {};
    setmetatable(o, self);
    self.__index = self;
    o.label = label;
    o.x = x;
    o.y = y;
    o.radius = radius;
    o.value = init or 0.5;
    local tw = textWidth(label, 12);
    o._text_x = x - tw / 2;
    return o;
end

function Knob:setValue(val)
    self.value = math.max(0, math.min(1, val));
end

function Knob:draw()
    -- color is set externally
    thickness(1);
    text(self.label, self._text_x, self.y - self.radius - 15, 12);
    ring(self.x, self.y, self.radius);
    local angle = self.value * math.pi * 2 - math.pi / 2;
    local knobX = self.x + math.cos(angle) * self.radius;
    local knobY = self.y + math.sin(angle) * self.radius;
    line(self.x, self.y, knobX, knobY);
end

return {
    Slider = Slider,
    Knob = Knob,
}
