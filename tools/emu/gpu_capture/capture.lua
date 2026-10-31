-- T-39302: DrawOTag capture for the generic renderer (read-only observation of the original game,
-- PCSX-Redux/OpenBIOS). run.sh writes `JOB = {...}` in front of this file. The job loads a save
-- state on vsync 1, then at every DrawOTag entry (0x8009CB9C, libgpu) counts the call. For each call
-- number in JOB.picks it saves the 2 MiB RAM and a save state (VRAM + GPU status) before the call, and
-- a second save state when DrawOTag returns (the emulator runs the DMA list synchronously, so VRAM
-- then holds the drawn frame). Nothing is written to game RAM.
--
-- JOB fields:
--   out        output dir (container path)
--   load_state save state loaded on vsync 1
--   picks      { n, ... } DrawOTag call numbers (1 = first call after the load) to capture
--   frames     hard vsync limit
--   inject     optional INJECT table (from synth_ot.py): at the picked call the bytes are copied into RAM at
--              INJECT.addr and DrawOTag is pointed at them (a0), so the emulator draws a synthetic table
--   inputs     optional { {vsync, 'BUTTON', hold}, ... } pad overrides (vsyncs counted from the load vsync)
--
-- Outputs: <tag>.ram.bin, <tag>.before.sstate, <tag>.after.sstate, captures.jsonl, meta.jsonl.

local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local regs = PCSX.getRegisters()

local DRAW_OTAG = 0x8009CB9C
local frame = 0
local loaded = false
local done = false
local calls = 0
local pending = nil
local files = {}

local function u32(x) return tonumber(ffi.cast('uint32_t', x)) end
local function rw(a) local o = bit.band(a, 0x1fffff); return mem[o] + mem[o + 1] * 256 + mem[o + 2] * 65536 + mem[o + 3] * 16777216 end

local function out(name, line)
    local f = files[name]
    if f == nil then
        f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
        files[name] = f
    end
    f:write(line .. '\n')
end

local function writeFile(name, data)
    local f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
    f:write(data)
    f:close()
end

local function saveState(name)
    local f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
    f:writeMoveSlice(PCSX.createSaveState())
    f:close()
end


local picks = {}
for _, n in ipairs(JOB.picks) do picks[n] = true end
local GS = 0x800E6280

local bps = {}
local afterBp = nil

local function onAfter()
    if pending == nil then return true end
    local tag = pending.tag
    saveState(tag .. '.after.sstate')
    out('captures.jsonl', string.format('{"tag":"%s","call":%d,"vsync":%d,"ot":"%08X","ra":"%08X","mode":%d,"stage":%d,"step":%d}',
        tag, pending.call, pending.vsync, pending.ot, pending.ra, pending.mode, pending.stage, pending.step))
    pending = nil
    return true
end

bps[#bps + 1] = PCSX.addBreakpoint(DRAW_OTAG, 'Exec', 4, 'drawotag', function()
    if done or not loaded then return true end
    calls = calls + 1
    if picks[calls] then
        local tag = string.format('call%04d', calls)
        local ra = u32(regs.GPR.n.ra)
        if INJECT ~= nil then
            local bytes = INJECT.hex:gsub("%x%x", function(h) return string.char(tonumber(h, 16)) end)
            ffi.copy(mem + bit.band(INJECT.addr, 0x1fffff), bytes, #bytes)
            regs.GPR.n.a0 = INJECT.addr
        end
        writeFile(tag .. '.ram.bin', ffi.string(mem, 0x200000))
        saveState(tag .. '.before.sstate')
        pending = { tag = tag, call = calls, vsync = frame, ot = u32(regs.GPR.n.a0), ra = ra,
            mode = mem[bit.band(GS + 0x1108, 0x1fffff)], stage = mem[bit.band(GS + 0x1109, 0x1fffff)],
            step = mem[bit.band(GS + 0x110A, 0x1fffff)] }
        if afterBp == nil then
            afterBp = PCSX.addBreakpoint(ra, 'Exec', 4, 'drawotag-ret', onAfter)
            bps[#bps + 1] = afterBp
        end
    end
    return true
end)

local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]
local releases, inputAt = {}, {}
for _, e in ipairs(JOB.inputs or {}) do
    inputAt[e[1]] = inputAt[e[1]] or {}
    table.insert(inputAt[e[1]], e)
end

local function finish(reason)
    done = true
    out('meta.jsonl', string.format('{"reason":"%s","vsyncs":%d,"drawotag_calls":%d}', reason, frame, calls))
    for _, f in pairs(files) do f:close() end
    PCSX.quit(0)
end

local function lastPick()
    local m = 0
    for n in pairs(picks) do if n > m then m = n end end
    return m
end

local function onVsync()
    if done then return end
    frame = frame + 1
    if frame == 1 then
        PCSX.nextTick(function()
            PCSX.loadSaveState(Support.File.open(JOB.load_state))
            loaded = true
        end)
        return
    end
    if releases[frame] then
        for _, name in ipairs(releases[frame]) do pad.clearOverride(BUTTON[name]) end
        releases[frame] = nil
    end
    for _, e in ipairs(inputAt[frame] or {}) do
        pad.setOverride(BUTTON[e[2]])
        local at = frame + (e[3] or 2)
        releases[at] = releases[at] or {}
        table.insert(releases[at], e[2])
    end
    if calls >= lastPick() and pending == nil then finish('picks done') return end
    if frame >= JOB.frames then finish('frames') return end
end

GpuCapVsync = PCSX.Events.createEventListener('GPU::Vsync', onVsync)
GpuCapBps = bps
