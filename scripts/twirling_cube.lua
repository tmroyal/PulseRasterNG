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
local dist1 = 0
local dist2 = 0

metro(0.125, function()
    local freq1 = dist1 * 400 + 200
    local freq2 = dist2 * 400 + 200
    synth("chirp", "note", freq1+freq2, 0.5)
    circle(math.random(0, width), math.random(0, height), 10);
end)

local pattern1= {
    "kick", false, "snare", false, false, "kick", "snare", false
}
local pattern2 ={
    false, "clhh", false, "clhh", false, "clhh", false, "ohh"
}
local idx = 1

metro(0.25, function()
    local d1 = pattern1[idx]
    local d2 = pattern2[idx]

    if d1 then
        synth("drumz", d1)
    end
    if d2 then
        synth("drumz", d2)
    end

    idx = idx + 1
    if idx > #pattern1 then
        idx = 1
    end
end)

function draw()
    dist = math.sin(timeSec()*0.1*math.pi)
    dist2 = math.sin(timeSec()*0.2*math.pi)
    local level = math.abs(dist);

    -- synth_set(br, "freq", 200 + level * 400)
    -- synth_set(br, "amp", level * 0.3)

    rect(width/2 + dist * 100, height/2 + dist2 * 100, 10, 10);
end