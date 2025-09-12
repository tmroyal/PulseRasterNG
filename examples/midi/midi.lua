function init()
    openPort(0, "Arturia MiniLab mkII")

    local mm = Patch:new("midi_mon")
end

function draw() 
end