---@diagnostic disable: undefined-global

function draw()
    local dist = math.sin(timeMs()*2*math.pi)

    fill(255, 0, 255, 255)
    circle(width/2 + dist * 100, height/2, width/4)
end