-- Title and file-select golden capture (read-only observation of the original game, PCSX-Redux/OpenBIOS).
-- run.sh writes `JOB = {...}` in front of this file. The job boots from power-on (no save state), drives
-- the pad only through overrides and never writes game RAM.
--
-- JOB fields:
--   out          output dir (container path)
--   inputs       { {vsync, 'BUTTON', hold}, ... }  vsyncs counted from power-on (the shared input-script format)
--   frames       hard vsync limit
--   no_cards     true: report both memory-card slots as empty (Mcd1Inserted/Mcd2Inserted = false)
--   after_entry  logic frames to keep logging after the first dispatch in mode 0x14 (name entry)
--   final_state  optional save state written when the job ends
--   load_state   optional save state loaded on vsync 1 (a continuation run; then vsyncs count from the load)
--   stop_mode    optional: stop after_entry logic frames after the first dispatch in this mode instead of 0x14
--   vsync_log    optional vsync limit: log frame_count/timer_ticks/seed at every vsync up to it (vsync.jsonl)
--
-- Outputs (JSON lines):
--   logic.jsonl   one record per game_mode_dispatch entry (pre-logic state of that logic frame)
--   rng.jsonl     every rand()/srand() call: caller ra, srand argument, the BIOS rand word before the call
--   inputs.jsonl  every pad override
--   checkpoints.jsonl and <tag>.ram.bin  2 MiB RAM at the first mode 0x13 and 0x14 dispatches
--   meta.jsonl    stop reason and counts

local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local regs = PCSX.getRegisters()

local frame = 0
local logic = 0
local done = false
local loaded = (JOB.load_state == nil)

local function u32(x) return tonumber(ffi.cast('uint32_t', x)) end
local function rb(a) return mem[bit.band(a, 0x1fffff)] end
local function rw(a) local o = bit.band(a, 0x1fffff); return mem[o] + mem[o + 1] * 256 + mem[o + 2] * 65536 + mem[o + 3] * 16777216 end
local function cyc() return tonumber(PCSX.getCPUCycles()) end

local function jsonEncode(v)
    local t = type(v)
    if t == 'nil' then return 'null' end
    if t == 'boolean' then return v and 'true' or 'false' end
    if t == 'number' then
        if v == math.floor(v) then return string.format('%d', v) end
        return string.format('%.17g', v)
    end
    if t == 'string' then
        return '"' .. v:gsub('[%c"\\]', function(c) return string.format('\\u%04x', c:byte()) end) .. '"'
    end
    local parts = {}
    if v[1] ~= nil then
        for _, x in ipairs(v) do parts[#parts + 1] = jsonEncode(x) end
        return '[' .. table.concat(parts, ',') .. ']'
    end
    for k, x in pairs(v) do parts[#parts + 1] = jsonEncode(tostring(k)) .. ':' .. jsonEncode(x) end
    return '{' .. table.concat(parts, ',') .. '}'
end

local files = {}
local function out(name, rec)
    local f = files[name]
    if f == nil then
        f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
        files[name] = f
    end
    f:write(jsonEncode(rec) .. '\n')
end
local function writeBin(name, data)
    local f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
    f:write(data)
    f:close()
end

-- Addresses (config/symbol_addrs*.txt, include/game/game_state.h, include/game/system_data.h)
local GS = 0x800E6280
local A = {
    timer_ticks = GS + 0x10F4, frame_count = GS + 0x10F8, game_mode_timer = GS + 0x10FC,
    mode_stage_timer = GS + 0x1100, mode_step_timer = GS + 0x1104,
    mode = GS + 0x1108, stage = GS + 0x1109, step = GS + 0x110A, substep = GS + 0x110D,
    pad_pressed = GS + 0xF88, pad_held = GS + 0xF80,
    card_result = GS + 0x1115, card_timer = GS + 0x1120,
    seed = 0x800E7D10, scene_work = 0x801230D0,
    rand_word = 0x8000850C, -- OpenBIOS rand state (kernel RAM), T-39002
}

local function key()
    return { m = rb(A.mode), s = rb(A.stage), st = rb(A.step), ss = rb(A.substep) }
end

local function capture(tag)
    local k = key()
    writeBin(tag .. '.ram.bin', ffi.string(mem, 0x200000))
    out('checkpoints.jsonl', { tag = tag, vsync = frame, logic = logic, cycles = cyc(), m = k.m, s = k.s, st = k.st,
        ss = k.ss, fc = rw(A.frame_count), tt = rw(A.timer_ticks), seed = rw(A.seed), rw = rw(A.rand_word),
        sw = rw(A.scene_work) })
end

if JOB.no_cards then
    local ok, err = pcall(function()
        PCSX.settings.emulator.Mcd1Inserted = false
        PCSX.settings.emulator.Mcd2Inserted = false
    end)
    out('meta.jsonl', { no_cards = ok, error = ok and nil or tostring(err) })
end

local seen = {}
local entryLogic = nil
local bps = {}
local function bp(addr, label, fn)
    bps[#bps + 1] = PCSX.addBreakpoint(addr, 'Exec', 4, 'tfs', function()
        if done or not loaded then return true end
        fn()
        return true
    end, label)
end

bp(0x80042540, 'game_mode_dispatch', function()
    logic = logic + 1
    local k = key()
    out('logic.jsonl', { l = logic, v = frame, fc = rw(A.frame_count), tt = rw(A.timer_ticks), seed = rw(A.seed),
        m = k.m, s = k.s, st = k.st, ss = k.ss, gmt = rw(A.game_mode_timer), sgt = rw(A.mode_stage_timer),
        stt = rw(A.mode_step_timer), pp = rw(A.pad_pressed), sw = rw(A.scene_work), rw = rw(A.rand_word),
        cr = rb(A.card_result), ct = rw(A.card_timer) })
    if not seen[k.m] then
        seen[k.m] = logic
        if k.m == 0x12 or k.m == 0x13 or k.m == 0x14 or k.m == 0x15 then capture(string.format('mode%02x_first', k.m)) end
        if k.m == (JOB.stop_mode or 0x14) then entryLogic = logic end
    end
end)
-- pad_update entry while mode 0x10 plays the boot movie: movie_play polls the pad once per decoded frame
bp(0x80042A00, 'pad_update', function()
    if rb(A.mode) == 0x10 then
        out('movie.jsonl', { v = frame, fc = rw(A.frame_count), tt = rw(A.timer_ticks), seed = rw(A.seed),
            pp = rw(A.pad_pressed) })
    end
end)
bp(0x800AE0D0, 'rand', function()
    out('rng.jsonl', { l = logic, v = frame, f = 'rand', ra = string.format('%08X', u32(regs.GPR.n.ra)),
        before = rw(A.rand_word) })
end)
bp(0x800AE120, 'srand', function()
    out('rng.jsonl', { l = logic, v = frame, f = 'srand', ra = string.format('%08X', u32(regs.GPR.n.ra)),
        a0 = u32(regs.GPR.n.a0), before = rw(A.rand_word) })
end)

local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]
local releases = {}
local inputAt = {}
for _, e in ipairs(JOB.inputs or {}) do
    inputAt[e[1]] = inputAt[e[1]] or {}
    table.insert(inputAt[e[1]], e)
end

local function finish(reason)
    done = true
    local k = key()
    out('meta.jsonl', { reason = reason, vsyncs = frame, logic_frames = logic, first_dispatch = seen, final = k })
    for _, f in pairs(files) do f:close() end
    if JOB.final_state then
        local f = Support.File.open(JOB.final_state, 'TRUNCATE')
        f:writeMoveSlice(PCSX.createSaveState())
        f:close()
    end
    PCSX.quit(0)
end

local function onVsync()
    if done then return end
    frame = frame + 1
    if frame == 1 and JOB.load_state then
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
        out('inputs.jsonl', { v = frame, button = e[2], hold = e[3] or 2, logic = logic })
    end
    if JOB.vsync_log and frame <= JOB.vsync_log then
        local k = key()
        out('vsync.jsonl', { v = frame, l = logic, m = k.m, fc = rw(A.frame_count), tt = rw(A.timer_ticks), seed = rw(A.seed) })
    end
    if entryLogic ~= nil and logic >= entryLogic + (JOB.after_entry or 120) then finish('entry') return end
    if frame >= JOB.frames then finish('frames') return end
end

TfsVsync = PCSX.Events.createEventListener('GPU::Vsync', onVsync)
TfsBps = bps
