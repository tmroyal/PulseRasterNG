local current_msg = "Click button to see mouse events"

function init()
    mouseDown(0, function(x, y)
        current_msg = "Mouse button 0 down at (" .. x .. ", " .. y .. ")"
    end)
    mouseUp(0, function(x, y)
        current_msg = "Mouse button 0 up at (" .. x .. ", " .. y .. ")"
    end)
    print("initialized")
end

function draw()
    text(current_msg, 10, 10, 32)
end