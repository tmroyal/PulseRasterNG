---@diagnostic disable: undefined-global

fill(1, 1, 1, 1)
-- br = synth("bear")
-- synth("bugger")

-- function DebugPrint()
--     print("Tick")
--     return 1
-- end

-- schedule_with(1, DebugPrint)

local last = timeSec()

metro(0.25, function()
    print("Metro Tick")
    print("Time: " .. timeSec())
    synth("clink")
    print("error: " .. (timeSec() - last - 0.25))
    last = timeSec()
    circle(math.random(0, width), math.random(0, height), 10);
    return 1
end)

timer(5.0, function()
    print("Timer Tick")
end)

function draw()
    local dist = math.sin(timeSec()*0.1*math.pi)
    local dist2 = math.sin(timeSec()*0.2*math.pi)
    local level = math.abs(dist);

    -- synth_set(br, "freq", 200 + level * 400)
    -- synth_set(br, "amp", level * 0.3)

    rect(width/2 + dist * 100, height/2 + dist2 * 100, 10, 10);
end