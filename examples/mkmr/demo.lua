local harp, chimes
local next_time = 0
local ind = 0

function init()
    local main = load_patch("main")
    local reverb = load_patch("vfreeverb")
    pd_msg(reverb, "size", 0.1)
    local chorus = load_patch("mkmr_chorus")
    pd_msg(chorus, "destination", reverb)

    harp = load_patch("mkmr_harp")
    pd_msg(harp, "destination", chorus)

    local phaser = load_patch("mkmr_phaser")
    pd_msg(phaser, "destination", reverb)
    pd_msg(phaser, "dl", 1000)
    pd_msg(phaser, "dr", 9000)

    chimes = load_patch("mkmr_chimes")
    pd_msg(chimes, "destination", phaser)
    next_time = timeSec() + 1
end

function draw()
    local now = timeSec()
    if now > next_time then
        ind = (ind + 1) % 2
        next_time = now + 1
        if ind == 1 then
            pd_msg(harp, "play", 550, 0.1)
        else
            pd_msg(chimes, "play", 560, 1)
        end
    end
    circle(width/2, height/2, 10)
end