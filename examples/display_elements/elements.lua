local slider_v, slider_h, knob, xy, grid, wf
local v_rate = 0.5
local h_rate = 0.3
local k_rate = 0.2
local xy_rates = {0.1, 0.15}

local grid_index = 0
local next_time = 0

function number_to_binary(n)
    local bin = {}
    for i = 15, 0, -1 do
        bin[i + 1] = n % 2
        n = math.floor(n / 2)
    end
    return bin
end

function init()
    slider_v = Slider:new("Vslider", 50, 50, 20, 200, true, 0.66)
    slider_h = Slider:new("Hslider", 80, 50, 200, 20, false, 0.66)
    knob = Knob:new("Knob", 300, 150, 40, 0.5)
    xy = XY:new("XY", 400, 50, 150, 0, 0)
    grid = Grid:new("Grid", 600, 50, 60, 5, 4, 4)
    wf = Waveform:new("Waveform", 50, 400, 300, 150, {0.3, 0.5, 0.7, 0.9, 0.7, 0.5, 0.3, 0.1, 0.9, 0.5, 0.2})
    background(0.5, 0.5, 0.5, 1)
    fill(1, 1, 1, 1)
end

function set_grid(ind)
    local bin = number_to_binary(grid_index)
    for i = 1, 16 do
        local row = math.floor((i - 1) / 4) + 1
        local col = ((i - 1) % 4) + 1
        grid:setValue(row, col, bin[i])
    end
end

function draw()
    slider_v:draw()
    slider_h:draw()
    knob:draw()
    xy:draw()
    grid:draw()
    wf:draw()

    local t = timeSec()
    if t >= next_time then
        next_time = t + 0.1
        grid_index = (grid_index + 1) % 10e16
        set_grid(grid_index)
    end

    slider_v:setValue((slider_v.value + v_rate * dt()) % 1)
    slider_h:setValue((slider_h.value + h_rate * dt()) % 1)
    knob:setValue((knob.value + k_rate * dt()) % 1)
    xy:setValues((xy.valueX + xy_rates[1] * dt()) % 1, (xy.valueY + xy_rates[2] * dt()) % 1)
end