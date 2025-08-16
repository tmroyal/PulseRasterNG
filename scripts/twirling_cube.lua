---@diagnostic disable: undefined-global

fill(1, 1, 1, 1)

function draw()
    local dist = math.sin(timeSec()*0.1*math.pi)
    local dist2 = math.sin(timeSec()*0.2*math.pi)
    local level = math.abs(dist);

    rect(width/2 + dist * 100, height/2 + dist2 * 100, 10, 10);
end