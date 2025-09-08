local polysyn, polyfm, polysample, scaler
local nexttick = 0
local ind = 0

function scaled(base)
    return function(desired)
        return 60 + desired - base
    end
end

function init()
    nexttick = timeSec() + 1

    local main = load_patch("main")
    pd_msg(main, "amp", 1)

    polysyn = load_patch("lct_polysynth")
    pd_msg(polysyn, "cutoff", 200)
    pd_msg(polysyn, "attack", 50)
    pd_msg(polysyn, "release", 100)

    polyfm = load_patch("lct_polyfm")
    pd_msg(polyfm, "index", 10)
    pd_msg(polyfm, "ratio", 2.01)
    pd_msg(polyfm, "attack", 2.01)
    pd_msg(polyfm, "ratio", 2.01)

    polysample = load_patch("lct_polysampler")
    pd_msg(polysample, "sample", "samples/pizz.wav")
    scaler = scaled(61)
end

function draw()
    local now = timeSec()
    if now > nexttick then
        if ind == 0 then
            pd_msg(polysyn, "play", 200, 1.0)
        elseif ind == 1 then
            pd_msg(polyfm, "play", 400, 0.5)
        elseif ind == 2 then
            pd_msg(polysample, "playm", scaler(60), 0.5)
        end
        nexttick = now + 1
        ind = (ind + 1) % 3
    end
    
    local position = math.fmod(width * timeSec() / 3, width)
    circle(position, height/2, 10)
end
