---@diagnostic disable: undefined-global

fill(1, 1, 1, 1)
br = synth("bear")

function DebugPrint()
    print("Tick")
    return 1
end

schedule_with(1, DebugPrint)

function draw()
    local dist = math.sin(timeSec()*0.1*math.pi)
    local dist2 = math.sin(timeSec()*0.2*math.pi)
    local level = math.abs(dist);

    synth_set(br, "freq", 200 + level * 400)
    synth_set(br, "amp", level * 0.3)

    rect(width/2 + dist * 100, height/2 + dist2 * 100, 10, 10);
end