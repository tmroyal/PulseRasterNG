-- Test MIDI input
print("Testing MIDI input...")

-- Open a MIDI port (using Arturia MiniLab mkII from the port listing)
local result = openPort(0, "Arturia MiniLab mkII")
print("Opening MIDI port result:", result)

-- Keep the script running
function setup()
    print("Setup complete - MIDI should be working now")
    print("Play some keys on your MIDI device")
end

function update()
    -- Nothing needed here, MIDI callbacks should handle input
end