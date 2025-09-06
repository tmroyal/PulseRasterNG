local harp, chimes
local next_time = 0
local ind = 0

function init()
    local main = load_synth("main")
    local reverb = load_synth("vfreeverb")
    synth(reverb, "size", 0.1)
    local chorus = load_synth("mkmr_chorus")
    synth(chorus, "destination", reverb)

    harp = load_synth("mkmr_harp")
    synth(harp, "destination", chorus)

    local phaser = load_synth("mkmr_phaser")
    synth(phaser, "destination", reverb)
    synth(phaser, "dl", 1000)
    synth(phaser, "dr", 9000)

    chimes = load_synth("mkmr_chimes")
    synth(chimes, "destination", phaser)
    next_time = timeSec() + 1
end

function draw()
    local now = timeSec()
    if now > next_time then
        ind = (ind + 1) % 2
        next_time = now + 1
        if ind == 1 then
            synth(harp, "play", 550, 0.1)
        else
            synth(chimes, "play", 560, 1)
        end
    end
    circle(width/2, height/2, 10)
end