function init()
    openPort(0, "Arturia MiniLab mkII")

    local mm = Patch:new("midi_mon")

    local rev = Patch:new("vfreeverb", {
        size=0.9,
    })

    local main = Patch:new("main")


    local synth = Patch:new("lct_polysynth", {
        cutoff = 2000,
        attack = 50,
        release = 200,
        fx_route=rev.handle,
    })

    local nr = Patch:new("note_route", {
        channel=1,
        note_off=0,
        freqmode=1,
        ampmode=1,
        destination=synth.handle,
        message="play"
    })
    local ctl_route = Patch:new("ctl_route", {
        channel=1,
        ctl=74,
        destination=synth.handle,
        message="fx_amount",
        x1=0,
        x2=1,
    })
end

function draw()
end