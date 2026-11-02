-- T-39510: observe every original-font upload, including hidden/preloaded text.
-- This is a superset of text drawn through vload_font, not an artwork detector.
-- Store glyph codes/counts and scene keys, never complete game strings.
return function(config, sceneKey, currentFrame, enabled)
    local ffi = require('ffi')
    local mem, regs = PCSX.getMemPtr(), PCSX.getRegisters()
    local trace = { calls = 0, unknown_calls = 0, overflow_calls = 0, entries = {},
        load_address = config.load_address, ranges_address = config.ranges_address,
        coverage = 'vload_font uploads; includes preloaded text, excludes sprite artwork',
        frame_origin = 'start of this emulator run' }
    local count = 0
    local function word(address)
        local offset = bit.band(address, 0x1fffff)
        return mem[offset] + mem[offset + 1] * 256
    end
    local function unsigned(value) return tonumber(ffi.cast('uint32_t', value)) end
    local function observe()
        if not enabled() then return true end
        local glyph, offset = unsigned(regs.GPR.r[4]), unsigned(regs.GPR.r[5])
        local code = -1
        -- Both original load_string and english_load_string pass the glyph
        -- index in a0 and the offset within its Shift-JIS range in a1.
        for i = 0, 44 do
            local address = config.ranges_address + i * 4
            local first, base, nextFirst = word(address), word(address + 2), word(address + 4)
            if base + offset == glyph and first + offset < nextFirst then
                code = first + offset
                break
            end
        end
        local scene, frame = sceneKey(), currentFrame()
        local caller = unsigned(regs.GPR.r[31])
        local id = tostring(code) .. ':' .. tostring(caller) .. ':' .. table.concat(scene, ',')
        trace.calls = trace.calls + 1
        if code < 0 then trace.unknown_calls = trace.unknown_calls + 1 end
        local entry = trace.entries[id]
        if entry == nil then
            if count >= 4096 then
                trace.overflow_calls = trace.overflow_calls + 1
                return true
            end
            entry = { code = code, glyph = glyph, offset = offset, caller = caller,
                key = scene, count = 0, first_frame = frame, last_frame = frame }
            trace.entries[id] = entry
            count = count + 1
        end
        entry.count = entry.count + 1
        entry.last_frame = frame
        return true
    end
    local breakpoint = PCSX.addBreakpoint(config.load_address, 'Exec', 4, 'font_trace', observe, 'vload_font')
    return trace, breakpoint
end
