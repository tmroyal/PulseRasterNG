local slider_v, slider_h, knob, xy, grid, wf

function make_sine(n, f)
    local t = {}
    for i = 0, n - 1 do
        t[i + 1] = 0.3 * (math.sin(i / n * 2 * math.pi * f - math.pi / 2) + 1) / 2
    end
    return t
end

function init()
    slider_v = Slider:new{
        label = "Vslider",
        x = 50, y = 50,
        width = 20, height = 200,
        vertical = true,
        value = 0.66
    }
    slider_v:attachMouseHandlers()

    slider_h = Slider:new{
        label = "Hslider",
        x = 80, y = 50,
        width = 200, height = 20,
        vertical = false,
        value = 0.66
    }
    slider_h:attachMouseHandlers()

    knob = Knob:new{
        label = "Knob",
        x = 300, y = 150,
        radius = 40,
        value = 0.5
    }
    knob:attachMouseHandlers()

    xy = XY:new{
        label = "XY",
        x = 400, y = 50,
        size = 150,
        valueX = 0, valueY = 0
    }
    xy:attachMouseHandlers()

    grid = Grid:new{
        label = "Grid",
        x = 600, y = 50,
        cellSize = 60,
        cellMargin = 5,
        rows = 4, cols = 4
    }
    grid:attachMouseHandlers()

    wf = Waveform:new{
        label = "Waveform",
        x = 50, y = 400,
        width = 300, height = 150,
        values = make_sine(36, 4.5)
    }

    background(0.5, 0.5, 0.5, 1)
    fill(1, 1, 1, 1)
end

function draw()
    slider_v:draw()
    slider_h:draw()
    knob:draw()
    xy:draw()
    grid:draw()
    wf:draw()
end