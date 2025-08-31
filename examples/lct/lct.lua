local polysyn, polyfm, polysample, scaler
local nexttick = 0
local ind = 0

function scaled(base)
    return function(desired)
        return 60 + desired - base
    end
end

function fmod(a, b)
    return a - math.trunc(a/b)*b
end

function init()
    nexttick = timeSec() + 1

    local master = load_synth("master")
    synth(master, "amp", 1)

    polysyn = load_synth("lct_polysynth")
    synth(polysyn, "cutoff", 200)
    synth(polysyn, "attack", 50)
    synth(polysyn, "release", 100)

    polyfm = load_synth("lct_polyfm")
    synth(polyfm, "index", 10)
    synth(polyfm, "ratio", 2.01)
    synth(polyfm, "attack", 2.01)
    synth(polyfm, "ratio", 2.01)

    polysample = load_synth("lct_polysampler")
    synth(polysample, "sample", "samples/pizz.wav")
    scaler = scaled(61)
end

function draw()
    local now = timeSec()
    if now > nexttick then
        if ind == 0 then
            synth(polysyn, "play", 200, 1.0)
        elseif ind == 1 then
            synth(polyfm, "play", 400, 0.5)
        elseif ind == 2 then
            synth(polysample, "playm", scaler(60), 0.5)
        end
        nexttick = now + 1
        ind = (ind + 1) % 3
    end
    
    local position = math.fmod(width * timeSec() / 3, width)
    circle(position, height/2, 10)
end
