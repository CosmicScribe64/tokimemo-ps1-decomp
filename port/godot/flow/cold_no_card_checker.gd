extends RefCounted
## T-29025: source-derived cold title/no-card checker prefix, not a capture.
## Counters change only at the two actual resident-loader draw callbacks.
const Session = preload("res://core/game_session.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Math = preload("res://core/native_math.gd")
const FLOW_SHA := "58a7e7b255c86a7f22101768a75d07335306f16fd821511cc965ec948979d53a"
const RECORD := "cold_no_card_checker_prefix"
var _session: WeakRef
var _owner := ""

func prepare(session: Session, route: Dictionary) -> String:
	if _session != null: return "cold checker prefix already owns its Session"
	var keys: Array[String] = ["source_flow_sha256","cold_boot","title_new_game","no_card_answer",
		"options_visited","retained_game","memory_card_load","attract_visited","debug_intervention"]
	if route.size() != keys.size() or not route.has_all(keys) or not route.source_flow_sha256 is String or route.source_flow_sha256 != FLOW_SHA:
		return "cold checker prefix requires its exact source-flow contract"
	for key: String in keys:
		if key != "source_flow_sha256" and not route[key] is bool: return "cold checker prefix needs explicit route observations"
	if not route.cold_boot or not route.title_new_game or not route.no_card_answer or route.options_visited or route.retained_game or route.memory_card_load or route.attract_visited or route.debug_intervention:
		return "checker prefix excludes non-cold, Options, retained, loaded, attract or debug history"
	if session == null or not session.state is State: return "checker prefix requires the actual interactive Session"
	var state: State = session.state
	if state.game_mode != 0x14 or state.mode_stage != 2 or state.mode_step != 5 or state.substep != 0 or state.mode_step_timer != 0 or not state.opening.get("origin") is String or state.opening.origin != "interactive-entry-owned-state-v1" or not state.opening.get("resident_overlay_lba") is int or state.opening.resident_overlay_lba != 0x95ea or not state.opening.get("overlay_phase") is int or state.opening.overlay_phase != 0 or state.opening.has(RECORD):
		return "checker prefix must bind at the actual fresh interactive-entry exit boundary"
	var brightness: Variant = state.opening.get("screen_brightness")
	if not brightness is int or brightness < 0 or brightness > 128 or not _default_scroll(state): return "checker prefix needs the source initial brightness and default player-flag branch"
	var owner := "cold-title-no-card-session:"+str(session.get_instance_id())
	# Before the first callback: 65 hold ticks, floor(brightness/4) fades,
	# one separate zero-brightness arm tick, then one game_mode_set tick.
	var first_frame := Math.u32(state.frame_count+67+int(brightness/4))
	state.opening[RECORD] = {"owner":owner,"source_flow_sha256":FLOW_SHA,"u":2,"v":255,
		"entry_frame":state.frame_count,"first_loader_frame":first_frame,"loader_calls":0,
		"last_loader_frame":-1,"handed_off":false}
	_owner = owner
	_session = weakref(session)
	return ""

func overlay_draw(state: State, result: Session.FrameResult) -> void:
	var error := _validate(state)
	if not error.is_empty():
		result.error = error
		return
	var record: Dictionary = state.opening[RECORD]
	var calls: int = record.loader_calls
	if record.handed_off or calls >= 2:
		result.error = "checker loader draw is duplicate or follows its HUD handoff"
		return
	var expected_mode := 2 if calls == 0 else 0x15
	var expected_phase := 3 if calls == 0 else 0
	if state.game_mode != expected_mode or not state.opening.get("overlay_phase") is int or state.opening.overlay_phase != expected_phase or state.ovl_target_game_mode != 0x15 or state.frame_count != Math.u32(record.first_loader_frame+calls):
		result.error = "checker loader draw is missing, stale or outside its genuine resident-loader phase"
		return
	var next: Dictionary = record.duplicate(true)
	var scroll := _updated(record.u,record.v,state.frame_count)
	next.u = scroll.u
	next.v = scroll.v
	next.loader_calls = calls+1
	next.last_loader_frame = state.frame_count
	state.opening[RECORD] = next
	result.presentation.append({"kind":"first_arrival_checker_prefix_update","owner":_owner,
		"loader_call":calls,"frame_count":state.frame_count,"u":next.u,"v":next.v})

func handoff(state: State) -> Dictionary:
	var error := _validate(state)
	if not error.is_empty(): return {"error":error}
	var record: Dictionary = state.opening[RECORD]
	if record.handed_off or record.loader_calls != 2 or state.game_mode != 0x15 or state.mode_stage != 0 or not state.opening.get("overlay_phase") is int or state.opening.overlay_phase != 0 or record.last_loader_frame != Math.u32(record.first_loader_frame+1):
		return {"error":"checker HUD handoff requires both actual resident-loader draws exactly once"}
	var transport := {"owner":_owner,"source_globals":["D_800B5BCC","D_800B5BD0"],
		"phase":"before_outer_scroll_update","u":record.u,"v":record.v}
	var next: Dictionary = record.duplicate(true)
	next.handed_off = true
	state.opening[RECORD] = next
	return {"error":"","transport":transport}

func _validate(state: State) -> String:
	var owner: Session = _session.get_ref() as Session if _session != null else null
	if owner == null or owner.state != state or not state.opening.get("origin") is String or state.opening.origin != "interactive-entry-owned-state-v1" or not state.opening.get("resident_overlay_lba") is int or state.opening.resident_overlay_lba != 0x95ea or not _default_scroll(state):
		return "checker prefix belongs to another Session, overlay or scroll branch"
	var record: Variant = state.opening.get(RECORD)
	var keys: Array[String] = ["owner","source_flow_sha256","u","v","entry_frame","first_loader_frame","loader_calls","last_loader_frame","handed_off"]
	if not record is Dictionary or record.size() != keys.size() or not record.has_all(keys): return "checker prefix state has malformed fields"
	if not record.owner is String or record.owner != _owner or not record.source_flow_sha256 is String or record.source_flow_sha256 != FLOW_SHA or not record.handed_off is bool:
		return "checker prefix state lost its source-flow owner"
	for key: String in ["u","v","entry_frame","first_loader_frame","loader_calls","last_loader_frame"]:
		if not record[key] is int: return "checker prefix counters and frames require exact integer types"
	if record.u < 0 or record.u > 255 or record.v < 0 or record.v > 255 or record.loader_calls < 0 or record.loader_calls > 2 or record.entry_frame < 0 or record.entry_frame > 0xffffffff or record.first_loader_frame < 0 or record.first_loader_frame > 0xffffffff or record.last_loader_frame < -1 or record.last_loader_frame > 0xffffffff:
		return "checker prefix counters or frames exceed native bounds"
	return ""

static func _default_scroll(state: State) -> bool:
	var flags: Variant = state.rule_state.get("player_flags")
	return flags is int and flags >= 0 and flags <= 0xffff and ((flags>>8)&15) == 0

static func _updated(u: int, v: int, frame_count: int) -> Dictionary:
	return {"u":(u+1)&255,"v":(v-(frame_count&1))&255}
