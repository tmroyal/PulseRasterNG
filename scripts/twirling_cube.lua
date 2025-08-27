---@diagnostic disable: undefined-global

fill(1, 1, 1, 1)

local last = timeSec()
local dist1 = 0
local dist2 = 0

local coords = {
    x=50,
    y=100
}

metro(0.125, function()
    local freq1 = dist1 * 400 + 200
    local freq2 = dist2 * 400 + 200
    synth("chirp", "note", freq1+freq2, 0.5)
    coords.x = math.random(0, width)
    coords.y = math.random(0, height)
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

    if isButtonDown(0, 7) then
        synth("drumz", "bongo_shak")
    end
end)

-- audioEvent("hey", function(a)
-- end)

function draw()
    dist = getAxis(0,0)
    dist2 = getAxis(0,1)
    local level = math.abs(dist);

    circle(coords.x, coords.y, 10);
    rect(width/2 + dist * 100, height/2 + dist2 * 100, 10, 10);
end