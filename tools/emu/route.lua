-- Route player loaded into PCSX-Redux by tools/emu/route.py (T-39504).
-- route.py writes `JOB = {...}` in front of this file and runs it with -dofile. JOB.steps is a
-- list of steps run one after another, each driven from the vsync event:
--   press  buttons, hold, gap      press pad 1 buttons for `hold` vsyncs, then idle `gap` vsyncs
--   idle   frames                  do nothing for `frames` vsyncs
--   wait   all, timeout, stable,   wait until every condition in `all` ({addr, values, neg}) holds
--          mash, shot_every, name  for `stable` vsyncs; meanwhile press `mash` every few vsyncs and
--                                  take a screenshot every `shot_every` vsyncs (name_01, name_02 ...)
--   steer  x, y, tol, boost,       hold the d-pad (and `boost`, which doubles the speed of the free
--          xaddr, yaddr, timeout   cursor) until the s16 values at xaddr/yaddr are within tol of x, y
--   talk   all, name, buttons,     dialogue: whenever the typewriter (text_state pos == total) has
--          settle, max, repress,   shown a whole page for `settle` vsyncs, screenshot it (name_01 ...)
--          timeout, text_addr      and press `buttons`; ends when every condition in `all` holds
--   goto   target, all, max        jump to step `target` when every condition in `all` holds, at
--                                  most `max` times (a retry loop); `label` steps do nothing
--   shot   name                    screenshot (a name taken before, in a loop, gets _r2, _r3 ...)
--   save   path                    write a save state
-- A wait that times out ends the run (reason "timeout <label>") unless it is `optional`.
-- Outputs: JOB.out/route_result.json (raw: shots, step log, mode log) and shot_<n>.raw files,
-- which route.py turns into PNGs.

local ffi = require('ffi')
local mem = PCSX.getMemPtr()

local frame = 0
local finishing = nil
local result = { shots = {}, steps = {}, mode_log = {}, notes = {} }

local function rb(a) return mem[bit.band(a, 0x1fffff)] end
local function rs16(a)
    local v = rb(a) + rb(a + 1) * 256
    if v >= 0x8000 then v = v - 0x10000 end
    return v
end

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

local function openOut(path)
    local f = Support.File.open(path, 'TRUNCATE')
    if f:failed() then
        note('cannot write ' .. path)
        return nil
    end
    return f
end

-- the game's scene key: GameState game_mode, mode_stage, mode_step, substep, overlay target mode
local function key()
    local k = {}
    for i, a in ipairs(JOB.key_addrs) do k[i] = rb(a) end
    return k
end

-- memory the job asks to record with every screenshot ({addr, len} -> hex)
local function probeValues()
    local out = {}
    for i, p in ipairs(JOB.probes or {}) do
        local parts = {}
        for k = 0, p[2] - 1 do parts[#parts + 1] = string.format('%02x', rb(p[1] + k)) end
        out[i] = table.concat(parts)
    end
    return out
end

local function finish(reason)
    if finishing == nil then finishing = reason end
end

local usedNames = {}

local function screenshot(name)
    usedNames[name] = (usedNames[name] or 0) + 1
    if usedNames[name] > 1 then name = string.format('%s_r%d', name, usedNames[name]) end
    local ss = PCSX.GPU.takeScreenShot()
    local n = #result.shots + 1
    local path = string.format('%s/shot_%03d.raw', JOB.out, n)
    local f = openOut(path)
    if f == nil then return end
    f:writeMoveSlice(ss.data)
    f:close()
    result.shots[n] = { name = name, frame = frame, step = JOB.step_index, key = key(), raw = path,
        cursor = { rs16(JOB.cursor_addrs[1]), rs16(JOB.cursor_addrs[2]) },
        probes = probeValues(),
        width = tonumber(ss.width), height = tonumber(ss.height), bpp = (tonumber(ss.bpp) == 0) and 16 or 24 }
end

local BUTTON = PCSX.CONSTS.PAD.BUTTON
local pad = PCSX.SIO0.slots[1].pads[1]
local held = {} -- button name -> release frame

local function press(buttons, hold)
    for _, name in ipairs(buttons) do
        pad.setOverride(BUTTON[name])
        held[name] = frame + hold
    end
end

local function releaseDue()
    for name, at in pairs(held) do
        if frame >= at then
            pad.clearOverride(BUTTON[name])
            held[name] = nil
        end
    end
end

local function holds(c)
    local v = rb(c.addr)
    local any = false
    for _, x in ipairs(c.values) do
        if v == x then any = true end
    end
    if c.neg then return not any end
    return any
end

local function allHold(conds)
    for _, c in ipairs(conds or {}) do
        if not holds(c) then return false end
    end
    return true
end

-- step state
local idx = 0
local st = nil -- { step, start, until_frame, stable, shots, next_mash, next_shot }

local function logStep(ok, why)
    local s = st.step
    result.steps[#result.steps + 1] = { index = idx, op = s.op, label = s.label, start = st.start, finish = frame,
        ok = ok, why = why }
end

local jumps = {} -- goto step index -> jumps taken

local function begin(nextIdx)
    idx = nextIdx or (idx + 1)
    JOB.step_index = idx
    local s = JOB.steps[idx]
    if s == nil then
        st = nil
        finish('done')
        return
    end
    st = { step = s, start = frame, stable = 0, shots = 0 }
    if s.op == 'press' then
        press(s.buttons, s.hold)
        st.until_frame = frame + s.hold + s.gap
    elseif s.op == 'idle' then
        st.until_frame = frame + s.frames
    elseif s.op == 'steer' then
        st.dirs = {}
    elseif s.op == 'talk' then
        st.phase = 'reveal'
        st.since = frame
    elseif s.op == 'wait' then
        st.next_mash = frame + (s.mash_delay or 0)
        st.next_shot = frame + (s.shot_every or 0)
    elseif s.op == 'shot' then
        screenshot(s.name)
        st.until_frame = frame
    elseif s.op == 'label' or s.op == 'goto' then
        st.until_frame = frame
    elseif s.op == 'save' then
        local f = openOut(s.path)
        if f ~= nil then
            f:writeMoveSlice(PCSX.createSaveState())
            f:close()
        end
        st.until_frame = frame
    else
        note('unknown op ' .. tostring(s.op))
        finish('bad step ' .. idx)
    end
end

local function steerHold(want)
    for _, name in ipairs({ 'UP', 'DOWN', 'LEFT', 'RIGHT' }) do
        if want[name] and not st.dirs[name] then pad.setOverride(BUTTON[name]) end
        if not want[name] and st.dirs[name] then pad.clearOverride(BUTTON[name]) end
        st.dirs[name] = want[name]
    end
end

-- d-pad towards the target, one axis at a time; done after 3 vsyncs inside the window
local function steerTick(s)
    local dx, dy = s.x - rs16(s.xaddr), s.y - rs16(s.yaddr)
    local want = {}
    if math.abs(dx) > s.tol then
        want[dx > 0 and 'RIGHT' or 'LEFT'] = true
    elseif math.abs(dy) > s.tol then
        want[dy > 0 and 'DOWN' or 'UP'] = true
    end
    -- the boost button doubles the step; near the target move at the normal speed
    local far = math.abs(dx) > 4 * s.tol + 8 or (math.abs(dx) <= s.tol and math.abs(dy) > 4 * s.tol + 8)
    for _, b in ipairs(s.boost) do
        if far and not st.boosting then pad.setOverride(BUTTON[b]) end
        if not far and st.boosting then pad.clearOverride(BUTTON[b]) end
    end
    st.boosting = far
    steerHold(want)
    if next(want) == nil then
        st.stable = st.stable + 1
        if st.stable >= 3 then
            logStep(true)
            return true
        end
    else
        st.stable = 0
    end
    if frame - st.start >= s.timeout then
        steerHold({})
        for _, b in ipairs(s.boost) do pad.clearOverride(BUTTON[b]) end
        logStep(false, string.format('timeout at %d,%d', rs16(s.xaddr), rs16(s.yaddr)))
        finish('timeout ' .. s.label)
        return true
    end
    return false
end

-- typewriter state: text_state.total (+2) and text_state.pos (+8), s16 (include/main/text.h)
local function textDone(s)
    local total, pos = rs16(s.text_addr + 2), rs16(s.text_addr + 8)
    return total > 0 and pos == total, total, pos
end

local function talkTick(s)
    if allHold(s.all) then
        logStep(true)
        return true
    end
    if frame - st.start >= s.timeout then
        logStep(false, 'timeout')
        if not s.optional then finish('timeout ' .. (s.label or tostring(idx))) end
        return true
    end
    local done, total, pos = textDone(s)
    if st.phase == 'reveal' then
        if done then
            st.stable = st.stable + 1
        else
            st.stable = 0
        end
        if st.stable >= s.settle and next(held) == nil then
            if st.shots < s.max then
                st.shots = st.shots + 1
                screenshot(string.format('%s_%02d', s.name, st.shots))
            end
            press(s.buttons, 4)
            st.phase, st.since, st.last = 'advance', frame, total
        elseif frame - st.since >= s.repress and next(held) == nil then
            -- no text on screen for a while (a picture or a fade waiting for a button): press on
            press(s.buttons, 4)
            st.since = frame
        end
    else
        -- the next page has started once the reveal restarts or the text changes
        if not done or total ~= st.last then
            st.phase, st.since, st.stable = 'reveal', frame, 0
        elseif frame - st.since >= s.repress and next(held) == nil then
            press(s.buttons, 4)
            st.since = frame
        end
    end
    return false
end

-- one vsync of the current step; true when the step is over
local function tick()
    local s = st.step
    if s.op == 'steer' then return steerTick(s) end
    if s.op == 'talk' then return talkTick(s) end
    if s.op ~= 'wait' then return frame >= st.until_frame end
    local elapsed = frame - st.start
    if elapsed >= (s.min or 0) and allHold(s.all) then
        st.stable = st.stable + 1
        if st.stable >= (s.stable or 1) then
            logStep(true)
            return true
        end
    else
        st.stable = 0
    end
    if elapsed >= s.timeout then
        logStep(false, 'timeout')
        if not s.optional then finish('timeout ' .. (s.label or tostring(idx))) end
        return true
    end
    if s.mash and frame >= st.next_mash and next(held) == nil then
        press(s.mash.buttons, s.mash.hold)
        st.next_mash = frame + s.mash.every
    end
    if (s.shot_every or 0) > 0 and frame >= st.next_shot and st.shots < (s.max_shots or 99) then
        st.shots = st.shots + 1
        screenshot(string.format('%s_%02d', s.name, st.shots))
        st.next_shot = frame + s.shot_every
    end
    return false
end

local lastKey = ''
local function logKey()
    local k = key()
    local h = table.concat(k, ',')
    if h ~= lastKey then
        lastKey = h
        result.mode_log[#result.mode_log + 1] = { frame = frame, key = k }
    end
end

local function writeOutputs()
    result.reason = finishing
    result.frames = frame
    result.last_step = idx
    if JOB.final_shot then screenshot('final') end
    if JOB.save_state then
        local f = openOut(JOB.save_state)
        if f ~= nil then
            f:writeMoveSlice(PCSX.createSaveState())
            f:close()
            result.saved_state = JOB.save_state
        end
    end
    local f = openOut(JOB.out .. '/route_result.json')
    if f ~= nil then
        f:write(jsonEncode(result))
        f:close()
    end
end

local started = (JOB.load_state == nil)

local function onVsync()
    frame = frame + 1
    if frame == 1 and JOB.load_state then
        PCSX.nextTick(function()
            PCSX.loadSaveState(Support.File.open(JOB.load_state))
            started = true
            note('loaded state ' .. JOB.load_state)
        end)
        return
    end
    if not started then return end
    logKey()
    releaseDue()
    if finishing == nil then
        if st == nil then begin() end
        -- several instant steps (shot, save) may finish on one vsync
        while st ~= nil and finishing == nil and tick() do
            local s = st.step
            if s.op ~= 'wait' and s.op ~= 'steer' and s.op ~= 'talk' then logStep(true) end
            local target = nil
            if s.op == 'goto' and (jumps[idx] or 0) < s.max and allHold(s.all) then
                jumps[idx] = (jumps[idx] or 0) + 1
                result.steps[#result.steps].why = 'jump ' .. jumps[idx]
                target = s.target
            end
            begin(target)
            if st ~= nil and (st.step.op == 'wait' or st.step.op == 'steer' or st.step.op == 'talk') then break end
        end
    end
    if finishing == nil and frame >= JOB.frames then finish('frames') end
    if finishing ~= nil then
        writeOutputs()
        PCSX.quit(0)
    end
end

RouteVsync = PCSX.Events.createEventListener('GPU::Vsync', onVsync)
