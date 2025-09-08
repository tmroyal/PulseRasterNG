local tick, metro, delay

local function sine(t, f)
    return math.sin(2 * math.pi * f * t)
end

local function map(val, min, max)
    return min + (val + 1) * 0.5 * (max - min)
end

-- Helper to get evenly spaced x positions
local function x_pos(i, divisions)
    return (i / divisions) * width
end

function init()
    local main = load_patch("main")
    msg(main, "amp", 0.1)    

    tick  = load_patch("tick")
    metro = load_patch("metronome")
    delay = load_patch("delay")

    msg(tick,  "fx-route",  delay)
    msg(tick,  "fx-amount", 0.8)

    msg(metro, "destination", tick)
    msg(metro, "message", "play", 440, 0.3)
    msg(metro, "interval", 500)

    msg(delay, "fb",    0.8)
    msg(delay, "level", 0.8)
    msg(delay, "lenl",   500)
    msg(delay, "lenr",   600)

    msg(metro, "trigger", 1)

    patchEvent(tostring(metro) .. "-tick", function()
        background(math.random() * 0.5, 0, 0, 1.0)
    end)

    fill(1.0, 0.9, 0.9, 1.0)
end

function draw()
    local t  = timeSec()
    local l  = sine(t, 0.2)
    local l1 = sine(t, 0.5)

    -- Update metro message with dynamic freq and duration
    msg(metro, "message", "play", map(l, 220, 880), map(l1, 0.05, 1.0))

    -- Draw lines evenly spaced
    local h = map(l, 0, height)
    line(x_pos(0, 4), h, x_pos(1, 4), h)

    h = map(l1, 0, height)
    line(x_pos(1, 4), h, x_pos(2, 4), h)

    l = sine(t, 0.3)
    h = map(l, 0, height)
    line(x_pos(2, 4), h, x_pos(3, 4), h)
    msg(metro, "interval", map(l, 200, 800))

    l = sine(t, 0.1)
    h = map(l, 0, height)
    line(x_pos(3, 4), h, x_pos(4, 4), h)
    msg(delay, "lenl", map(l, 350, 400))
    msg(delay, "lenr", map(l, 300, 450))
end
