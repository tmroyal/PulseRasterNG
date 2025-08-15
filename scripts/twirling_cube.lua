---@diagnostic disable: undefined-global

function draw()
    local dist = math.sin(timeSec()*2*math.pi)

    fill(0, 255, 255, 255);
    rect(width/2 + dist * 100, height/2, width/4, height/4);
end