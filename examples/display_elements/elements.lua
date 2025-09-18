local slider_v, slider_h, knob;
local v_rate = 0.5;
local h_rate = 0.3;
local k_rate = 0.2;

function init()
    slider_v = Slider:new("Vslider", 50, 50, 20, 200, true, 0.66);
    slider_h = Slider:new("Hslider", 80, 50, 200, 20, false, 0.66);
    knob = Knob:new("Knob", 300, 150, 40, 0.5);
    background(0.5, 0.5, 0.5, 1);
    fill(1, 1, 1, 1);
end

function draw()

    slider_v:draw();
    slider_h:draw();
    knob:draw();

    slider_v:setValue((slider_v.value + v_rate * dt()) % 1);
    slider_h:setValue((slider_h.value + h_rate * dt()) % 1);
    knob:setValue((knob.value + k_rate * dt()) % 1);
end