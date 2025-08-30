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

    local master = load_synth("master")
    synth(master, "amp", 0.1)    

    tick  = load_synth("tick")
    metro = load_synth("metronome")
    delay = load_synth("delay")

    synth(tick,  "fx-route",  delay)
    synth(tick,  "fx-amount", 0.8)

    synth(metro, "destination", tick)
    synth(metro, "message", "play", 440, 0.3)
    synth(metro, "interval", 500)

    synth(delay, "fb",    0.8)
    synth(delay, "level", 0.8)
    synth(delay, "lenl",   500)
    synth(delay, "lenr",   600)

    synth(metro, "trigger", 1)

    audioEvent(tostring(metro) .. "-tick", function()
        background(math.random() * 0.5, 0, 0, 1.0)
    end)

    fill(1.0, 0.9, 0.9, 1.0)
end

function draw()
    local t  = timeSec()
    local l  = sine(t, 0.2)
    local l1 = sine(t, 0.5)

    -- Update metro message with dynamic freq and duration
    synth(metro, "message", "play", map(l, 220, 880), map(l1, 0.05, 1.0))

    -- Draw lines evenly spaced
    local h = map(l, 0, height)
    line(x_pos(0, 4), h, x_pos(1, 4), h)

    h = map(l1, 0, height)
    line(x_pos(1, 4), h, x_pos(2, 4), h)

    l = sine(t, 0.3)
    h = map(l, 0, height)
    line(x_pos(2, 4), h, x_pos(3, 4), h)
    synth(metro, "interval", map(l, 200, 800))

    l = sine(t, 0.1)
    h = map(l, 0, height)
    line(x_pos(3, 4), h, x_pos(4, 4), h)
    synth(delay, "lenl", map(l, 350, 400))
    synth(delay, "lenr", map(l, 300, 450))
end
