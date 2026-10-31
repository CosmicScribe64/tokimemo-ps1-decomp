-- Route driver, emulator side: plays an expanded tokimemo.input-script.v2 on the original game
-- (PCSX-Redux, OpenBIOS, interpreter). Read-only observation: no RAM writes; input only through pad
-- overrides. run.sh writes `JOB = {...}` and `SCRIPT = {...}` (tools/emu/route_script.py lua) in front.
--
-- Boundary: a breakpoint at 0x800421F0 in boot_loop, the instruction after game_mode_dispatch (and
-- boot_paused_step) returns and before `frame_count += 1` (asm main_loop/boot_loop.s). There the
-- iteration's logic is finished; its post-update frame_count is RAM frame_count + 1. pad_update for the
-- next iteration runs later in draw_frame, after the VSync wait whose interrupt polls the pad, so an
-- override set here is read by the next iteration. The rule (identical to tools/emu/route_script.py
-- Rule and port/godot/tests/route/route_script_player.gd) checks that: a press must be in GameState
-- pad_held at the boundary of the iteration it was due for, else "input slip" stops the run, except a
-- pulse press after a boundary state listed in SCRIPT.unsampled_ok (a loading state in which the game
-- skips pad_update), which is recorded in route.json unsampled. The run passes only on script
-- completion or a stop pinned by an unsupported row with the same side, stop text and at_segment.
--
-- JOB fields:
--   out          output dir (container path)
--   load_state   save state loaded on vsync 1 (e.g. newgame-golden-v1 setA command-stage1.sstate)
--   frames       hard vsync limit
--   trace        true: route-trace.jsonl, one row per boundary
--   shots        true: screenshot at each checkpoint (default true)
--
-- Outputs: route.json (anchors, skipped, delivered, checkpoints, transitions, stop), route-trace.jsonl,
-- checkpoints.jsonl + <id>.ram.bin/.scratch.bin/.shot.raw (decode.py and shots.py inputs), <id>.sstate
-- for checkpoints with save_state.

local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local scratch = PCSX.getScratchPtr()

local GS = 0x800E6280
local A = { frame_count = GS + 0x10F8, mode = GS + 0x1108, stage = GS + 0x1109, step = GS + 0x110A,
    substep = GS + 0x110D, pad_held = GS + 0xF80, year = GS + 0x03E, month = GS + 0x03F, day = GS + 0x040,
    weekday = GS + 0x041, day_count = GS + 0x042, menu_cursor = GS + 0x1093, icon_count = GS + 0xF6D }
local BOUNDARY = 0x800421F0
local MASKS = { L2 = 0x1, R2 = 0x2, L1 = 0x4, R1 = 0x8, TRIANGLE = 0x10, CIRCLE = 0x20, CROSS = 0x40, SQUARE = 0x80,
    SELECT = 0x100, START = 0x800, UP = 0x1000, RIGHT = 0x2000, DOWN = 0x4000, LEFT = 0x8000 }
local PREDICATE_KEYS = { 'game_mode', 'mode_stage', 'mode_step', 'substep', 'year', 'month', 'day', 'weekday',
    'day_count', 'menu_cursor', 'icon_count' }
local EXPECT_LIMIT = 600

local function rb(a) return mem[bit.band(a, 0x1fffff)] end
local function rh(a) local o = bit.band(a, 0x1fffff); return mem[o] + mem[o + 1] * 256 end
local function rw(a) local o = bit.band(a, 0x1fffff); return mem[o] + mem[o + 1] * 256 + mem[o + 2] * 65536 + mem[o + 3] * 16777216 end
local function s8(v) if v >= 128 then return v - 256 end return v end

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
    if v[1] ~= nil or next(v) == nil and getmetatable(v) == 'array' then
        for _, x in ipairs(v) do parts[#parts + 1] = jsonEncode(x) end
        return '[' .. table.concat(parts, ',') .. ']'
    end
    if next(v) == nil then return '{}' end
    local keys = {}
    for k in pairs(v) do keys[#keys + 1] = tostring(k) end
    table.sort(keys)
    for _, k in ipairs(keys) do parts[#parts + 1] = jsonEncode(k) .. ':' .. jsonEncode(v[k]) end
    return '{' .. table.concat(parts, ',') .. '}'
end
local function array(t) return setmetatable(t, { __metatable = 'array' }) end

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

-- shared rule -----------------------------------------------------------------------------------------
local function matches(p, s)
    for _, k in ipairs(PREDICATE_KEYS) do
        if p[k] ~= nil and s[k] ~= p[k] then return false end
    end
    return true
end
local function any(list, s)
    for _, p in ipairs(list or {}) do if matches(p, s) then return true end end
    return false
end
local function ptext(p)
    local parts = {}
    for _, k in ipairs(PREDICATE_KEYS) do
        if p[k] ~= nil then
            parts[#parts + 1] = k .. '=' .. (k == 'game_mode' and string.format('0x%02X', p[k]) or string.format('%d', p[k]))
        end
    end
    return table.concat(parts, ',')
end
local function keyText(s) return string.format('%02x/%d/%d/%d', s.game_mode, s.mode_stage, s.mode_step, s.substep) end

local R = { index = 1, anchor = -1, search_from = -1, waiting_since = -1, releases = {}, expect = nil, last = -1,
    pulse_k = 0, pulse_last = -1, failure = '', stop = '', done = false, anchors = {}, skipped = array({}),
    delivered = array({}), fired = array({}), prev_when = {}, counts = {}, held_due = {}, unsampled = array({}),
    prev = nil }
local segments = SCRIPT.segments
local checkpoints = SCRIPT.checkpoints
local unsampledOk = {}
for _, row in ipairs(SCRIPT.unsampled_ok or {}) do unsampledOk[#unsampledOk + 1] = row.when end

local function fail(msg) R.failure = msg; R.done = true end

local function checkpointsFire(s)
    local now_fired = {}
    for _, c in ipairs(checkpoints) do
        local now = matches(c.when, s)
        local before = R.prev_when[c.id] or false
        R.prev_when[c.id] = now
        if now and not before then
            local count = R.counts[c.id] or 0
            local name = nil
            if not c.recurring then
                if count == 0 then name = c.id end
            elseif c.limit == nil or count < c.limit then
                name = string.format('%s-%03d', c.id, count + 1)
            end
            if name then
                R.counts[c.id] = count + 1
                local rec = { id = name, checkpoint = c.id, frame_count = s.frame_count, ram = c.ram or false, save_state = c.save_state or false }
                R.fired[#R.fired + 1] = rec
                now_fired[#now_fired + 1] = rec
            end
        end
    end
    return now_fired
end

local function checkExpect(s)
    if R.expect == nil then return end
    if matches(R.expect.after, s) then
        if s.frame_count - R.expect.since > EXPECT_LIMIT then
            fail(string.format('segment %s: still in its own state %d iterations after its last press (expected %s)', R.expect.segment, EXPECT_LIMIT, ptext(R.expect.want)))
        end
        return
    end
    if not matches(R.expect.want, s) then
        fail(string.format('segment %s: expected %s next, observed %s', R.expect.segment, ptext(R.expect.want), keyText(s)))
        return
    end
    R.expect = nil
end

local function edgeDueBetween(first, last)
    for _, at in pairs(R.releases) do if at >= first and at <= last then return true end end
    local seg = segments[R.index]
    if seg ~= nil and R.anchor >= 0 then
        if seg.kind == 'pulse' then
            local p = seg.pulse
            local due = R.anchor + (p.offset or 1) + R.pulse_k * p.every
            return R.pulse_k < p.max and due >= first and due <= last
        end
        for _, e in ipairs(seg.events or {}) do
            local at = R.anchor + e.at
            if at >= first and at <= last then return true end
        end
    end
    return false
end

-- Returns edges {{button, pressed}, ...} for iteration s.frame_count + 1 and the checkpoints fired now.
local function feed(s)
    if R.done then return {}, {} end
    local observed = s.frame_count
    if observed == R.last then return {}, {} end
    if R.last >= 0 and observed > R.last + 1 and edgeDueBetween(R.last + 2, observed) then
        fail(string.format('iterations %d..%d ran without a boundary while an edge was due', R.last + 2, observed))
        return {}, {}
    end
    local names = {}
    for b in pairs(R.held_due) do names[#names + 1] = b end
    table.sort(names)
    for _, b in ipairs(names) do
        local span = R.held_due[b]
        if observed >= span[1] and observed <= span[2] and bit.band(s.pad_held, MASKS[b]) == 0 then
            if span[3] or R.prev == nil or not any(unsampledOk, R.prev) then
                fail(string.format('input slip: %s due for iteration %d is not in the sampled pad %04x (previous boundary %s)',
                    b, observed, s.pad_held, R.prev == nil and 'none' or keyText(R.prev)))
                return {}, {}
            end
            -- a pulse press after an unsampled_ok loading state (no pad_update there)
            R.unsampled[#R.unsampled + 1] = { frame_count = observed, button = b }
        end
        if observed >= span[2] then R.held_due[b] = nil end
    end
    R.last = observed
    R.prev = s
    local nxt = observed + 1
    local fired = checkpointsFire(s)
    checkExpect(s)
    if R.done then return {}, fired end
    local edges = {}
    local rel = {}
    for b, at in pairs(R.releases) do if at == nxt then rel[#rel + 1] = b end end
    table.sort(rel)
    for _, b in ipairs(rel) do edges[#edges + 1] = { b, false }; R.releases[b] = nil end
    while R.index <= #segments do
        local seg = segments[R.index]
        local kind = seg.kind or 'input'
        local stop_loop = false
        if R.anchor < 0 then
            if R.waiting_since < 0 then R.waiting_since = observed end
            if observed < R.search_from or not matches(seg.wait_until, s) then
                local timeout = seg.timeout_iterations or 0
                if timeout > 0 and observed - R.waiting_since > timeout then
                    fail(string.format('segment %s: wait_until not reached within %d iterations', seg.id, timeout))
                end
                break
            end
            R.anchor = observed
            R.anchors[seg.id] = observed
            R.pulse_k = 0
            R.pulse_last = -1
            if seg.only_if ~= nil and not any(seg.only_if, s) then
                R.skipped[#R.skipped + 1] = seg.id
                R.index = R.index + 1
                R.search_from = observed
                R.anchor = -1
                R.waiting_since = -1
                goto continue
            end
        end
        if kind == 'save_relaunch' then
            R.stop = string.format('segment %s: save_relaunch slot %d', seg.id, seg.slot)
            R.done = true
            break
        end
        do
            local last = observed
            if kind == 'pulse' then
                local p = seg.pulse
                if not matches(p['until'], s) then
                    local due = R.anchor + (p.offset or 1) + R.pulse_k * p.every
                    if R.pulse_k >= p.max then
                        if observed >= R.pulse_last + p.every then
                            fail(string.format('segment %s: pulse exhausted after %d presses without %s', seg.id, p.max, ptext(p['until'])))
                        end
                        break
                    end
                    if due == nxt then
                        R.pulse_k = R.pulse_k + 1
                        R.pulse_last = nxt
                        if not any(p.skip_when, s) then
                            local buttons = p.buttons
                            for _, c in ipairs(p.choose or {}) do
                                if matches(c.when, s) then buttons = c.buttons; break end
                            end
                            for _, b in ipairs(buttons) do
                                edges[#edges + 1] = { b, true }
                                R.releases[b] = nxt + p.hold
                                R.held_due[b] = { nxt, nxt + p.hold - 1, false }
                            end
                        end
                    end
                    break
                end
            else
                last = R.anchor + (seg.run_for or 0)
                for _, e in ipairs(seg.events or {}) do
                    local at = R.anchor + e.at
                    if at > last then last = at end
                    if at == nxt then
                        for _, b in ipairs(e.buttons) do
                            edges[#edges + 1] = { b, true }
                            R.releases[b] = at + e.hold
                            R.held_due[b] = { at, at + e.hold - 1, true }
                        end
                    end
                end
                if nxt <= last then break end
            end
            if seg.expect_next ~= nil then
                R.expect = { segment = seg.id, after = seg.wait_until, want = seg.expect_next, since = observed }
                checkExpect(s)
                if R.done then return {}, fired end
            end
            R.index = R.index + 1
            R.search_from = last
            R.anchor = -1
            R.waiting_since = -1
        end
        ::continue::
    end
    for _, e in ipairs(edges) do
        R.delivered[#R.delivered + 1] = { frame_count = nxt, button = e[1], pressed = e[2] }
    end
    if not R.done and R.index > #segments and next(R.releases) == nil and R.expect == nil then R.done = true end
    return edges, fired
end

-- emulator glue ---------------------------------------------------------------------------------------
local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]
local vsync = 0
local loaded = (JOB.load_state == nil)
local finished = false
local boundaries = 0
local transitions = array({})
local lastKey = ''
local captured = array({})

local function observe()
    return { frame_count = rw(A.frame_count) + 1, game_mode = rb(A.mode), mode_stage = rb(A.stage), mode_step = rb(A.step),
        substep = rb(A.substep), year = rb(A.year), month = rb(A.month), day = rb(A.day), weekday = rb(A.weekday),
        day_count = rh(A.day_count), menu_cursor = s8(rb(A.menu_cursor)), icon_count = rb(A.icon_count),
        pad_held = bit.band(rw(A.pad_held), 0xFFFF) }
end

local pendingStates = {}
local function saveState(path)
    local f = Support.File.open(path, 'TRUNCATE')
    f:writeMoveSlice(PCSX.createSaveState())
    f:close()
end

local function capture(rec, s)
    local tag = rec.id
    local row = { tag = tag, checkpoint = rec.checkpoint, vsync = vsync, frame_count = s.frame_count,
        mode = s.game_mode, stage = s.mode_stage, step = s.mode_step, substep = s.substep, year = s.year, month = s.month,
        day = s.day, weekday = s.weekday, day_count = s.day_count, menu_cursor = s.menu_cursor,
        where = 'boundary 0x800421F0 (post-dispatch, before frame_count += 1)' }
    if rec.ram then
        writeBin(tag .. '.ram.bin', ffi.string(mem, 0x200000))
        writeBin(tag .. '.scratch.bin', ffi.string(scratch, 0x400))
    end
    if JOB.shots ~= false then
        local ok, ss = pcall(PCSX.GPU.takeScreenShot)
        if ok and ss ~= nil then
            local f = Support.File.open(JOB.out .. '/' .. tag .. '.shot.raw', 'TRUNCATE')
            f:writeMoveSlice(ss.data)
            f:close()
            row.shot = { width = tonumber(ss.width), height = tonumber(ss.height), bpp = (tonumber(ss.bpp) == 0) and 16 or 24 }
            row.shot_note = 'display at the boundary: the last frame drawn before this iteration'
        else
            row.shot_error = tostring(ss)
        end
    end
    if rec.save_state then
        -- Written at the next vsync callback, not inside the breakpoint (the newgame golden's pattern);
        -- states.jsonl records that vsync.
        pendingStates[#pendingStates + 1] = tag
        row.sstate = tag .. '.sstate'
    end
    out('checkpoints.jsonl', row)
    captured[#captured + 1] = { id = tag, frame_count = s.frame_count, vsync = vsync, ram = rec.ram, sstate = row.sstate }
    -- Progress record, so an emulator crash still leaves the rule's state up to this checkpoint.
    local f = Support.File.open(JOB.out .. '/route-progress.json', 'TRUNCATE')
    f:write(jsonEncode({ checkpoint = tag, vsync = vsync, boundaries = boundaries, segment_index = R.index - 1,
        segment = segments[R.index] and segments[R.index].id or '', anchors = R.anchors, unsampled = R.unsampled }))
    f:close()
end

local function finish(reason, s)
    if finished then return end
    finished = true
    for b in pairs(R.releases) do pad.clearOverride(BUTTON[b]) end
    local stop = ''
    if reason == 'player_failure' then stop = 'input script: ' .. R.failure
    elseif reason == 'save_relaunch' then stop = 'save_relaunch: emulator card save and power-cycle reload not scripted'
    elseif reason == 'frames' then stop = 'vsync limit ' .. JOB.frames end
    local seg = segments[R.index]
    local segAtStop = seg and seg.id or ''
    local pinned = false
    for _, u in ipairs(SCRIPT.unsupported or {}) do
        if stop ~= '' and u.side == 'native' and u.stop == stop and u.at_segment == segAtStop then pinned = true end
    end
    local rec = { schema = 'tokimemo.route-run.v1', side = 'native', script = SCRIPT.name, reason = reason, stop = stop,
        stop_pinned = pinned, passed = (reason == 'script_complete') or pinned, save_relaunch_segment = R.stop,
        player_failure = R.failure, vsyncs = vsync, boundaries = boundaries, segment_index = R.index - 1,
        segment_at_stop = segAtStop, segments_total = #segments, final_key = lastKey,
        final_calendar = s, anchors = R.anchors, skipped = R.skipped, unsampled = R.unsampled, delivered = R.delivered, checkpoints = captured,
        transitions = transitions }
    local f = Support.File.open(JOB.out .. '/route.json', 'TRUNCATE')
    f:write(jsonEncode(rec))
    f:close()
    saveState(JOB.out .. '/final.sstate')
    for _, fh in pairs(files) do fh:close() end
    PCSX.quit(0)
end

RouteBp = PCSX.addBreakpoint(BOUNDARY, 'Exec', 4, 'route', function()
    if finished or not loaded then return true end
    local s = observe()
    boundaries = boundaries + 1
    local key = keyText(s)
    if key ~= lastKey then
        lastKey = key
        transitions[#transitions + 1] = array({ key, s.frame_count, s.day_count, vsync })
    end
    if JOB.trace then out('route-trace.jsonl', s) end
    local edges, fired = feed(s)
    for _, rec in ipairs(fired) do capture(rec, s) end
    for _, e in ipairs(edges) do
        if e[2] then pad.setOverride(BUTTON[e[1]]) else pad.clearOverride(BUTTON[e[1]]) end
    end
    if R.done then
        local reason = R.failure ~= '' and 'player_failure' or (R.stop ~= '' and 'save_relaunch' or 'script_complete')
        finish(reason, s)
    end
    return true
end, 'route_boundary')

RouteVsync = PCSX.Events.createEventListener('GPU::Vsync', function()
    if finished then return end
    vsync = vsync + 1
    if vsync == 1 and JOB.load_state then
        PCSX.nextTick(function()
            PCSX.loadSaveState(Support.File.open(JOB.load_state))
            loaded = true
        end)
        return
    end
    for _, tag in ipairs(pendingStates) do
        saveState(JOB.out .. '/' .. tag .. '.sstate')
        out('states.jsonl', { tag = tag, vsync = vsync, frame_count = rw(A.frame_count) })
    end
    pendingStates = {}
    if vsync >= JOB.frames then finish('frames', observe()) end
end)
