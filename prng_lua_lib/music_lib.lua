function mtof(midi_note)
    return 440.0 * (2 ^ ((midi_note - 69) / 12))
end

function ftom(freq)
    return 69 + 12 * (math.log(freq / 440.0) / math.log(2))
end

function name_midi(note_name)
    --- converts a name (i.e. A4) to a midi note number
    --- i.e. A4 = 69, C4 = 60
    local note_postfix = {
        b = -1,
        ["#"] = 1,
        [""] = 0,
    }
    local note_base = {
        C = 0,
        D = 2,
        E = 4,
        F = 5,
        G = 7,
        A = 9,
        B = 11,
    }
    local note, octave = string.match(note_name, "([A-G][b#]?)(%d+)")
    if note == nil or octave == nil then
        return nil
    end
    octave = tonumber(octave)
    local base = note_base[note:sub(1, 1)]
    local postfix = note_postfix[note:sub(2)] or 0
    if base == nil or postfix == nil or octave == nil then
        return nil
    end
    return (octave + 1) * 12 + base + postfix
end

return {
    mtof = mtof,
    ftom = ftom,
}