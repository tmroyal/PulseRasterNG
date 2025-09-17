local x = 0
local degree = 1

function init()
    background(0.6, 0.4, 0.5, 1.0)
    fill(0.1, 0, 0.3, 1.0)

    local scale = {}
    for i = 1, 15 do
        scale[i] = 10 * (i + 10)
    end

    openPort(0, "Arturia MiniLab mkII")

    local main = Patch:new("main")

    local rev = Patch:new("vfreeverb", {size=0.7, damp=0.3})
    local harp = Patch:new("mkmr_harp", {
        fx_route=rev.handle,
        fx_amount=0.5,
    })

    local ctl_route = Patch:new("ctl_route", {
        channel=1,
        ctl=74,
        raw_dest="local",
        x1=0,
        x2=1,
    })

    patchEvent("local", function(i)
        x = i
        local next_degree = math.floor(x * #scale) + 1
        if next_degree ~= degree then
            degree = next_degree
            harp:msg("play", scale[degree], 0.1)
        end
    end)
end

function draw()
    local screen_x = x * width
    circle(screen_x, height/2, 40)
end