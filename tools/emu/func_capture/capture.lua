-- T-39301: function-differential capture (read-only observation of the original game, PCSX-Redux /
-- OpenBIOS). run.sh writes `JOB = {...}` in front of this file. The job loads a save state on vsync 1,
-- then at every entry of a target function records, for the first JOB hits (see per_key):
--   * the arguments a0..a3, the stack words sp+16..sp+47 (o32 stack arguments), sp and ra,
--   * the full 2 MiB RAM at entry (<fn>/hitNNNN.entry.ram),
--   * at the return (the breakpoint at ra with the entry sp restored): v0, v1 and the RAM as a
--     byte-run diff against the entry RAM (<fn>/hitNNNN.json, "exit_diff").
-- Callees that the port has not ported yet are listed in JOB.callees. For each call of such a callee
-- made while a target is running (the outermost one per target), the capture records its arguments, the
-- RAM at its entry as a diff against the target's entry RAM ("pre_diff"), v0/v1 and its RAM effect
-- ("effect_diff": RAM at its return against RAM at its entry). The GDScript runner replays them as test
-- doubles (tests/engine/engine_diff_runner.gd): it checks that the port's memory equals entry + pre_diff
-- when the callee is reached, then applies effect_diff and returns v0.
--
-- JOB fields:
--   out        output dir (container path)
--   load_state save state loaded on vsync 1
--   frames     hard vsync limit
--   targets    { {name=, addr=, max=N, key=addr_or_nil, key_max=M}, ... } functions to capture.
--              max: hits to keep. key (optional RAM address of a byte, e.g. 0x800E7388): keep at most
--              key_max hits per distinct value of that byte, so a function called every frame still
--              covers different modes. from (optional): ignore hits before this vsync.
--   callees    { {name=, addr=} | {name=, ptr=addr_of_a_u32_holding_the_code_address}, ... }
--   inputs     optional { {vsync, 'BUTTON', hold}, ... } pad overrides (vsyncs counted from the load vsync)
--   pokes      optional { {vsync, address, 'hexbytes'}, ... } RAM writes at the start of that vsync (a constructed
--              state, e.g. a bad game mode to reach game_mode_reset_to_title; say so wherever the capture is used)
--   ptr_bytes  optional bytes recorded behind a callee's pointer argument that points into the stack (default 64)
--   cover      optional: true records the executed instruction addresses of each target hit (an Exec breakpoint over
--              the function's code range, targets[i].size); pack.py/coverage.py report unreached blocks
--   max_diff   optional cap on the bytes of one diff (default 0x40000); a hit above it is dropped and
--              counted in meta.jsonl ("dropped_diff")
--
-- Outputs: <fn>/hitNNNN.json, <fn>/hitNNNN.entry.ram (pack.py gzips these), meta.jsonl.
-- All of it is game data: it stays under .asset-cache and is never committed.

local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local regs = PCSX.getRegisters()

local RAM = 0x200000
local frame = 0
local loaded = false
local done = false
local files = {}
local maxDiff = JOB.max_diff or 0x40000
local dropped = 0

local function u32(x) return tonumber(ffi.cast('uint32_t', x)) end
local function ramByte(a) return mem[bit.band(a, 0x1fffff)] end
local function ramWord(a)
    local o = bit.band(a, 0x1fffff)
    return mem[o] + mem[o + 1] * 256 + mem[o + 2] * 65536 + mem[o + 3] * 16777216
end

local function writeFile(name, data)
    local f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
    f:write(data)
    f:close()
end

local function appendLine(name, line)
    local f = files[name]
    if f == nil then
        f = Support.File.open(JOB.out .. '/' .. name, 'TRUNCATE')
        files[name] = f
    end
    f:write(line .. '\n')
end

local hexOf = {}
for i = 0, 255 do hexOf[i] = string.format('%02x', i) end

local function hex(ptr, off, len)
    local t = {}
    for i = 0, len - 1 do t[i + 1] = hexOf[ptr[off + i]] end
    return table.concat(t)
end

-- Byte runs where `b` differs from `a` (both 2 MiB uint8_t*), as a JSON array of [offset, "hex"].
-- Returns the JSON text and the number of differing bytes, or nil when above the cap.
local function diffRuns(a, b)
    local a32, b32 = ffi.cast('uint32_t*', a), ffi.cast('uint32_t*', b)
    local runs, total = {}, 0
    local startOff, endOff = nil, nil
    local function flush()
        if startOff then
            runs[#runs + 1] = '[' .. startOff .. ',"' .. hex(b, startOff, endOff - startOff) .. '"]'
            total = total + (endOff - startOff)
            startOff = nil
        end
    end
    for w = 0, RAM / 4 - 1 do
        if a32[w] ~= b32[w] then
            local base = w * 4
            for k = 0, 3 do
                if a[base + k] ~= b[base + k] then
                    local o = base + k
                    if startOff and o - endOff <= 3 then
                        endOff = o + 1
                    else
                        flush()
                        startOff, endOff = o, o + 1
                    end
                end
            end
            if total > maxDiff then return nil, total end
        end
    end
    flush()
    if total > maxDiff then return nil, total end
    return '[' .. table.concat(runs, ',') .. ']', total
end

local function snapshot()
    local copy = ffi.new('uint8_t[?]', RAM)
    ffi.copy(copy, mem, RAM)
    return copy
end

local STACK_LO, STACK_HI = 0x801E0000, 0x80200000
local function calleeExtraJson()
    local g = regs.GPR.n
    local sp = u32(g.sp)
    local words = {}
    for i = 0, 7 do words[#words + 1] = tostring(ramWord(sp + 16 + i * 4)) end
    local mem_parts = {}
    local n = JOB.ptr_bytes or 64
    for i, name in ipairs({'a0', 'a1', 'a2', 'a3'}) do
        local v = u32(g[name])
        if v >= STACK_LO and v < STACK_HI then
            local len = math.min(n, STACK_HI - v)
            local o = bit.band(v, 0x1fffff)
            mem_parts[#mem_parts + 1] = string.format('"a%d":"%s"', i - 1, hex(mem, o, len))
        end
    end
    return '"stack":[' .. table.concat(words, ',') .. '],"ptr_mem":{' .. table.concat(mem_parts, ',') .. '}'
end

local function regsJson()
    local g = regs.GPR.n
    return string.format('"a0":%d,"a1":%d,"a2":%d,"a3":%d,"v0":%d,"v1":%d,"sp":%d,"ra":%d',
        u32(g.a0), u32(g.a1), u32(g.a2), u32(g.a3), u32(g.v0), u32(g.v1), u32(g.sp), u32(g.ra))
end

local targets = {}   -- by name
local byAddr = {}    -- code address -> list of {kind='target'|'callee', def=}
local pendings = {}  -- targets in flight (a stack; the newest last)
local retBps = {}    -- return address -> breakpoint
local bps = {}
local seq = {}       -- per target: hits kept
local perKey = {}    -- per target: value -> count

for _, t in ipairs(JOB.targets) do
    targets[t.name] = t
    seq[t.name] = 0
    perKey[t.name] = {}
    byAddr[t.addr] = byAddr[t.addr] or {}
    table.insert(byAddr[t.addr], { kind = 'target', def = t })
end
local calleeDefs = {}
for _, c in ipairs(JOB.callees or {}) do
    calleeDefs[#calleeDefs + 1] = c
    if c.addr then
        byAddr[c.addr] = byAddr[c.addr] or {}
        table.insert(byAddr[c.addr], { kind = 'callee', def = c })
    end
end

local function finishPending(p)
    -- the target returned: v0/v1, exit diff, write the hit
    local g = regs.GPR.n
    local exitDiff, n = diffRuns(p.entry, mem)
    if exitDiff == nil then
        dropped = dropped + 1
        return
    end
    local callees = {}
    for _, c in ipairs(p.callees) do callees[#callees + 1] = c end
    local executed = {}
    if p.exec then
        for pc, _ in pairs(p.exec) do executed[#executed + 1] = pc end
        table.sort(executed)
    end
    local body = string.format('{"function":"%s","address":%d,"hit":%d,"vsync":%d,%s,"stack":[',
        p.def.name, p.def.addr, p.hit, p.vsync, p.argsJson)
    local st = {}
    for i = 0, 7 do st[#st + 1] = tostring(p.stack[i]) end
    body = body .. table.concat(st, ',') .. '],"ret_v0":' .. u32(g.v0) .. ',"ret_v1":' .. u32(g.v1)
        .. ',"exit_diff_bytes":' .. n .. ',"exit_diff":' .. exitDiff
        .. ',"executed":[' .. table.concat(executed, ',') .. '],"callees":[' .. table.concat(callees, ',') .. ']}'
    local dir = p.def.name
    writeFile(string.format('%s/hit%04d.json', dir, p.hit), body)
    appendLine('meta.jsonl', string.format('{"event":"hit","function":"%s","hit":%d,"vsync":%d,"exit_diff_bytes":%d,"callees":%d}',
        p.def.name, p.hit, p.vsync, n, #p.callees))
end

local function onReturn()
    local sp = u32(regs.GPR.n.sp)
    local pc = u32(regs.pc)
    -- callees first: a callee returns before its target does
    for _, p in ipairs(pendings) do
        local c = p.callee
        if c and c.ra == pc and c.sp == sp then
            local effect, n = diffRuns(c.before, mem)
            if effect == nil then
                p.failed = true
            else
                local g = regs.GPR.n
                p.callees[#p.callees + 1] = string.format('{"name":"%s",%s,"ret_v0":%d,"ret_v1":%d,"pre_diff":%s,"effect_diff":%s}',
                    c.def.name, c.argsJson, u32(g.v0), u32(g.v1), c.pre, effect)
            end
            p.callee = nil
        end
    end
    for i = #pendings, 1, -1 do
        local p = pendings[i]
        if p.ra == pc and p.sp == sp then
            table.remove(pendings, i)
            if not p.failed then finishPending(p) else dropped = dropped + 1 end
        end
    end
    return true
end

local function returnBreakpoint(ra)
    if retBps[ra] == nil then
        retBps[ra] = PCSX.addBreakpoint(ra, 'Exec', 4, 'func-capture-ret', onReturn)
        bps[#bps + 1] = retBps[ra]
    end
end

local function onTargetEntry(t)
    if seq[t.name] >= t.max then return end
    if t.from and frame < t.from then return end
    if t.key then
        local v = ramByte(t.key)
        local used = perKey[t.name][v] or 0
        if used >= (t.key_max or 1) then return end
        perKey[t.name][v] = used + 1
    end
    seq[t.name] = seq[t.name] + 1
    local g = regs.GPR.n
    local sp = u32(g.sp)
    local stack = {}
    for i = 0, 7 do stack[i] = ramWord(sp + 16 + i * 4) end
    local hit = seq[t.name]
    local p = { def = t, hit = hit, exec = JOB.cover and {} or nil, vsync = frame, sp = sp, ra = u32(g.ra), stack = stack, callees = {},
        argsJson = regsJson():gsub('"v0":%d+,"v1":%d+,', ''), entry = snapshot() }
    writeFile(string.format('%s/hit%04d.entry.ram', t.name, hit), ffi.string(p.entry, RAM))
    pendings[#pendings + 1] = p
    returnBreakpoint(p.ra)
    -- a callee given by a pointer is resolved now
    for _, c in ipairs(calleeDefs) do
        if c.ptr then
            local a = ramWord(c.ptr)
            if a ~= 0 and a >= 0x80000000 and byAddr[a] == nil then
                byAddr[a] = {}
                bps[#bps + 1] = PCSX.addBreakpoint(a, 'Exec', 4, 'func-capture-ptr', function()
                    if done or not loaded then return true end
                    for _, e in ipairs(byAddr[a]) do if e.kind == 'callee' then GCalleeEntry(e.def) end end
                    return true
                end)
            end
            if a ~= 0 and a >= 0x80000000 then
                local present = false
                for _, e in ipairs(byAddr[a]) do if e.def == c then present = true end end
                if not present then table.insert(byAddr[a], { kind = 'callee', def = c }) end
            end
        end
    end
end

function GCalleeEntry(c)
    local g = regs.GPR.n
    local sp, ra = u32(g.sp), u32(g.ra)
    for _, p in ipairs(pendings) do
        if p.callee == nil and not p.failed then
            local pre, n = diffRuns(p.entry, mem)
            if pre == nil then
                p.failed = true
            else
                p.callee = { def = c, sp = sp, ra = ra, pre = pre, before = snapshot(),
                    argsJson = '"address":' .. u32(regs.pc) .. ',' .. calleeExtraJson() .. ',' .. regsJson():gsub('"v0":%d+,"v1":%d+,', '') }
                returnBreakpoint(ra)
            end
        end
    end
end

-- coverage: one Exec breakpoint over each target's code range records the executed addresses of pending hits
if JOB.cover then
    for _, t in ipairs(JOB.targets) do
        if t.size and t.size > 4 then
            bps[#bps + 1] = PCSX.addBreakpoint(t.addr, 'Exec', t.size, 'func-capture-cover', function()
                local pc = u32(regs.pc)
                for _, p in ipairs(pendings) do
                    if p.exec and pc >= p.def.addr and pc < p.def.addr + p.def.size then p.exec[pc] = true end
                end
                return true
            end)
        end
    end
end

local registered = {}
local function addEntryBreakpoint(addr)
    if registered[addr] then return end
    registered[addr] = true
    bps[#bps + 1] = PCSX.addBreakpoint(addr, 'Exec', 4, 'func-capture', function()
        if done or not loaded then return true end
        for _, e in ipairs(byAddr[addr]) do
            if e.kind == 'callee' then GCalleeEntry(e.def) end
        end
        for _, e in ipairs(byAddr[addr]) do
            if e.kind == 'target' then onTargetEntry(e.def) end
        end
        return true
    end)
end
for addr, _ in pairs(byAddr) do addEntryBreakpoint(addr) end

local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]
local releases, inputAt = {}, {}
for _, e in ipairs(JOB.inputs or {}) do
    inputAt[e[1]] = inputAt[e[1]] or {}
    table.insert(inputAt[e[1]], e)
end

local function finish(reason)
    done = true
    local kept = {}
    for name, n in pairs(seq) do kept[#kept + 1] = string.format('"%s":%d', name, n) end
    appendLine('meta.jsonl', string.format('{"event":"end","reason":"%s","vsyncs":%d,"hits":{%s},"dropped_diff":%d}',
        reason, frame, table.concat(kept, ','), dropped))
    for _, f in pairs(files) do f:close() end
    PCSX.quit(0)
end

local function allDone()
    if #pendings > 0 then return false end
    for name, t in pairs(targets) do
        if seq[name] < t.max then return false end
    end
    return true
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
    for _, e in ipairs(JOB.pokes or {}) do
        if e[1] == frame then
            local bytes = e[3]:gsub('%x%x', function(h) return string.char(tonumber(h, 16)) end)
            ffi.copy(mem + bit.band(e[2], 0x1fffff), bytes, #bytes)
        end
    end
    if allDone() then finish('all hits') return end
    if frame >= JOB.frames then finish('frames') return end
end

FuncCapVsync = PCSX.Events.createEventListener('GPU::Vsync', onVsync)
FuncCapBps = bps
