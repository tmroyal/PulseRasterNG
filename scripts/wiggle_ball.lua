---@diagnostic disable: undefined-global

function draw()
    local dist = math.sin(timeSec()*2*math.pi)
    fill(255, 0, 0, 255)
    circle(width/2 + dist * 100, height/2, 20);
    print(dist*100)
end