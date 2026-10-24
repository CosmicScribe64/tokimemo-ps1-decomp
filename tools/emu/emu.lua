-- Job runner loaded into PCSX-Redux by tools/emu/emu.py (T-20500).
-- emu.py writes `JOB = {...}` in front of this file and runs it with -dofile. The job says what
-- to do (breakpoints, watches, pad input, screenshots, save states, how long to run); this file
-- records what happened into JOB.out/result.json and quits the emulator. Raw outputs only:
-- emu.py turns them into the report (symbols, call stack, PNG).

local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local scratch = PCSX.getScratchPtr()
local rom = PCSX.getRomPtr()
local regs = PCSX.getRegisters()

local GPR = { 'r0', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
    's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra' }

local frame = 0
local finishing = nil -- stop reason, set once; the next vsync writes the outputs and quits
local releases = {}
local result = { hits = {}, counts = {}, shots = {}, notes = {} }

local function u32(x) return tonumber(ffi.cast('uint32_t', x)) end

-- 2 MB main RAM through any of its mirrors, the 1 KB scratchpad and the 512 KB BIOS ROM
local function region(a, n)
    local p = bit.band(a, 0x1fffffff)
    if p < 0x00800000 then
        local o = bit.band(p, 0x1fffff)
        if o + n <= 0x200000 then return mem + o end
    elseif p >= 0x1f800000 and p + n <= 0x1f800400 then
        return scratch + (p - 0x1f800000)
    elseif p >= 0x1fc00000 and p + n <= 0x1fc80000 then
        return rom + (p - 0x1fc00000)
    end
    return nil
end

local function readRaw(a, n)
    local ptr = region(a, n)
    if ptr == nil then return nil end
    return ffi.string(ptr, n)
end

-- like readRaw, but cut at the end of the region instead of failing
local function readUpTo(a, n)
    while n > 0 do
        local s = readRaw(a, n)
        if s ~= nil then return s end
        n = n - 0x100
    end
    return nil
end

local function hex(s)
    if s == nil then return nil end
    return (s:gsub('.', function(c) return string.format('%02x', c:byte()) end))
end

local function unhex(h) return (h:gsub('..', function(x) return string.char(tonumber(x, 16)) end)) end

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

local function note(msg) result.notes[#result.notes + 1] = string.format('frame %d: %s', frame, msg) end

-- an output file, or nil (with a note) when it cannot be created; writing to a failed file crashes
local function openOut(path)
    local f = Support.File.open(path, 'TRUNCATE')
    if f:failed() then
        note('cannot write ' .. path)
        return nil
    end
    return f
end

local function writeFile(path, data)
    local f = openOut(path)
    if f == nil then return false end
    f:write(data)
    f:close()
    return true
end

local function snapshot(bp, address, width)
    local s = { frame = frame, cycles = tonumber(PCSX.getCPUCycles()), pc = u32(regs.pc), address = address,
        width = width, label = bp.label, regs = {}, args = {}, dumps = {}, slots = {} }
    for i = 0, 31 do s.regs[GPR[i + 1]] = u32(regs.GPR.r[i]) end
    s.regs.lo = u32(regs.GPR.n.lo)
    s.regs.hi = u32(regs.GPR.n.hi)
    s.insn = hex(readRaw(s.pc, 4))
    s.sp = s.regs.sp
    s.stack = hex(readUpTo(s.regs.sp, JOB.stack_bytes))
    for _, r in ipairs({ 'a0', 'a1', 'a2', 'a3' }) do
        local v = s.regs[r]
        -- only values that look like pointers: KSEG0/KSEG1 RAM or the scratchpad
        if v >= 0x80000000 or (v >= 0x1f800000 and v < 0x1f800400) then
            s.args[r] = hex(readRaw(v, JOB.arg_bytes))
        end
    end
    for _, d in ipairs(bp.dumps or JOB.dumps or {}) do
        s.dumps[#s.dumps + 1] = { addr = d[1], data = hex(readRaw(d[1], d[2])) }
    end
    for _, a in ipairs(JOB.slot_probes) do s.slots[string.format('%08X', a)] = hex(readRaw(a, 32)) end
    return s
end

local function finish(reason)
    if finishing == nil then finishing = reason end
end

-- Breakpoints and watches. `expect` (hex) guards overlay code: a hit counts only when the
-- bytes at the breakpoint address are that overlay's code, since all overlays share one slot.
local bps = {}
for i, b in ipairs(JOB.breaks) do
    local expect = b.expect and unhex(b.expect) or nil
    result.counts[b.label] = 0
    local function invoker(address, width, cause)
        if finishing ~= nil then return true end
        if expect ~= nil and readRaw(b.addr, #expect) ~= expect then return true end
        result.counts[b.label] = result.counts[b.label] + 1
        local n = result.counts[b.label]
        if n <= b.record then result.hits[#result.hits + 1] = snapshot(b, address, width) end
        if b.stop_after > 0 and n >= b.stop_after then finish('break ' .. b.label) end
        return true
    end
    bps[i] = PCSX.addBreakpoint(b.addr, b.kind, b.width, 'emu.py', invoker, b.label)
end

local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]

local function press(buttons, hold)
    for _, name in ipairs(buttons) do
        pad.setOverride(BUTTON[name])
        local at = frame + hold
        releases[at] = releases[at] or {}
        table.insert(releases[at], name)
    end
end

local function screenshot(tag)
    local ss = PCSX.GPU.takeScreenShot()
    local path = string.format('%s/shot_%s.raw', JOB.out, tag)
    local f = openOut(path)
    if f == nil then return end
    f:writeMoveSlice(ss.data)
    f:close()
    result.shots[#result.shots + 1] = { tag = tag, frame = frame, path = path, width = tonumber(ss.width),
        height = tonumber(ss.height), bpp = (tonumber(ss.bpp) == 0) and 16 or 24 }
end

local function saveState(path)
    local f = openOut(path)
    if f == nil then return false end
    f:writeMoveSlice(PCSX.createSaveState())
    f:close()
    return true
end

local function writeOutputs()
    result.reason = finishing
    result.frames = frame
    result.mode = readRaw(JOB.mode_addr, 1):byte()
    result.slots = {}
    for _, a in ipairs(JOB.slot_probes) do result.slots[string.format('%08X', a)] = hex(readRaw(a, 32)) end
    result.final_dumps = {}
    for _, d in ipairs(JOB.final_dumps) do
        result.final_dumps[#result.final_dumps + 1] = { addr = d[1], data = hex(readRaw(d[1], d[2])) }
    end
    if JOB.final_shot then screenshot('final') end
    if JOB.save_state and saveState(JOB.save_state) then result.saved_state = JOB.save_state end
    if JOB.ram_dump and writeFile(JOB.ram_dump, ffi.string(mem, 0x200000)) then result.ram_dump = JOB.ram_dump end
    writeFile(JOB.out .. '/result.json', jsonEncode(result))
end

local shotAt = {}
for _, f in ipairs(JOB.shots) do shotAt[f] = true end
local inputAt = {}
for _, e in ipairs(JOB.inputs) do
    inputAt[e.frame] = inputAt[e.frame] or {}
    table.insert(inputAt[e.frame], e)
end

-- which overlay sits in the slot, and the loader's mode byte, logged on change
local lastSlot, lastMode = {}, nil
result.slot_log, result.mode_log = {}, {}
local function logSlots()
    for _, a in ipairs(JOB.slot_probes) do
        local h = hex(readRaw(a, 32))
        if h ~= lastSlot[a] then
            lastSlot[a] = h
            result.slot_log[#result.slot_log + 1] = { frame = frame, addr = a, data = h }
        end
    end
    -- an overlay counts as loaded once its first and its last bytes (checksum trailer) are in
    local u = JOB.until_overlay
    if u and hex(readRaw(u.addr, 32)) == u.head and hex(readRaw(u.tail_addr, 16)) == u.tail then
        finish('overlay ' .. u.name)
    end
    local m = readRaw(JOB.mode_addr, 1):byte()
    if m ~= lastMode then
        lastMode = m
        result.mode_log[#result.mode_log + 1] = { frame = frame, mode = m }
    end
end

local function onVsync()
    frame = frame + 1
    logSlots()
    if frame == 1 and JOB.load_state then
        PCSX.nextTick(function()
            PCSX.loadSaveState(Support.File.open(JOB.load_state))
            note('loaded state ' .. JOB.load_state)
        end)
    end
    if releases[frame] then
        for _, name in ipairs(releases[frame]) do pad.clearOverride(BUTTON[name]) end
        releases[frame] = nil
    end
    for _, e in ipairs(inputAt[frame] or {}) do press(e.buttons, e.hold) end
    if shotAt[frame] then screenshot(tostring(frame)) end
    if finishing == nil and frame >= JOB.frames then finish('frames') end
    if finishing ~= nil then
        writeOutputs()
        PCSX.quit(0)
    end
end

EmuVsyncListener = PCSX.Events.createEventListener('GPU::Vsync', onVsync)
EmuBreakpoints = bps
