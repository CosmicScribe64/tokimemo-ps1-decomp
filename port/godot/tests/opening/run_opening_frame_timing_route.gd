extends "res://tests/opening/run_installed_rei_banner.gd"
## T-39002 whole-opening frame timing route. Runs the actual installed entry and
## opening to REI_TARGET_STAGE=12 with scripted entry input and asserts the game
## frames (state.frame_count deltas) of every opening substep from 0/0/0 to
## 12/0/0 against the original's table in tests/fixtures/opening_frame_timing.json.
##
## Entry input matches the emulator reference (name-shiori-confirm-v1.sstate):
## family, given and nickname "い", January / 1 / blood type A for both
## birthdays, both confirmations. The actions go into the entry's own logical
## queue before its first tick, so each lands on a fixed logic tick.
## TIMED_ENTRY_PAD idle ticks before the last confirmation set the frame_count
## phase the opening starts on: 2 matches the original run with START at vsync
## 20, 3 the run with START at vsync 33. Only pads present in the fixture run.
## Runner: tools/port/run_opening_frame_timing.sh <out-dir> <pad>.
const FIXTURE := "res://tests/fixtures/opening_frame_timing.json"
var scripted_entry := false
var transitions: Dictionary = {}
var transition_order: Array[String] = []
var reference: Dictionary = {}
var pad := -1

func _initialize() -> void:
	var raw := OS.get_environment("TIMED_ENTRY_PAD")
	var fixture = JSON.parse_string(FileAccess.get_file_as_string(FIXTURE))
	var pads: Dictionary = fixture.get("pads",{}) if fixture is Dictionary else {}
	if raw.is_valid_int() and pads.has(raw):
		pad = int(raw); reference = pads[raw]
	check(not reference.is_empty(),"TIMED_ENTRY_PAD must be one of the fixture pads %s, got '%s'" % [pads.keys(),raw])
	check(_target_ok(),"frame timing route needs REI_TARGET_STAGE=12, got %d" % target)
	node_added.connect(_node_added)
	super._initialize()

## Observers that extend this route past 12/0/0 accept their own target.
func _target_ok() -> bool:
	return target == 12

func _node_added(node: Node) -> void:
	if scripted_entry or node.get_script() == null or node.get_script().resource_path != "res://app/new_game_entry_boot.gd": return
	scripted_entry = true
	node.ready.connect(_script_entry.bind(node),CONNECT_ONE_SHOT)

func _script_entry(entry: Node) -> void:
	var c = entry.controller
	var at := Vector2i(-1,-1)
	for row in 6:
		for column in 17:
			if at.x < 0 and c.cell(column,row) == PackedByteArray([0x82,0xa2]): at = Vector2i(column,row)
	check(at.x >= 0 and entry.tick == 0 and entry.queue.is_empty(),"scripted entry could not bind before the first entry tick")
	var birthday := [{"kind":"select","value":0},{"kind":"circle"},{"kind":"select","value":0},{"kind":"circle"},{"kind":"select","value":0},{"kind":"circle"}]
	var actions: Array = []
	for part in 3: actions.append_array([{"kind":"cell","column":at.x,"row":at.y},{"kind":"start"}])
	actions.append_array(birthday); actions.append({"kind":"yes"}); actions.append_array(birthday)
	for i in maxi(pad,0): actions.append({"kind":"tick"})
	actions.append({"kind":"yes"})
	entry.queue.append_array(actions)

func _frame_published(tick: int, app: App) -> void:
	var state = app.consumer.opening.state
	if state.game_mode == 0x15:
		var key := "%d/%d/%d" % [state.mode_stage,state.mode_step,state.substep]
		if not transitions.has(key):
			transitions[key] = int(state.frame_count); transition_order.append(key)
	super._frame_published(tick,app)

func _finish_rei(app: App) -> void:
	_check_timing()
	super._finish_rei(app)

func _check_timing() -> void:
	var file := FileAccess.open("/out/transitions.json",FileAccess.WRITE)
	if file != null:
		file.store_string(JSON.stringify({"transitions":transitions},"\t")); file.close()
	if reference.is_empty(): return
	check(transition_order.size() > 0 and transition_order[0] == "0/0/0","opening did not start at 0/0/0: %s" % [transition_order.slice(0,1)])
	check(transitions.has("12/0/0"),"opening never reached 12/0/0")
	if transition_order.is_empty() or not transitions.has("12/0/0"): return
	var start: int = transitions["0/0/0"]
	check(posmod(start - 1,6) == int(reference["start_phase"]),"opening started on frame_count phase %d, reference phase %d" % [posmod(start - 1,6),int(reference["start_phase"])])
	var expected: Array = reference["substeps"]
	var mismatches: Array = []
	for i in expected.size():
		var key: String = expected[i][0]
		var next: String = expected[i + 1][0] if i + 1 < expected.size() else "12/0/0"
		var want := int(expected[i][1])
		var got: int = int(transitions[next]) - int(transitions[key]) if transitions.has(key) and transitions.has(next) else -1
		if got != want: mismatches.append("%s port %d original %d" % [key,got,want])
	var port_keys := transition_order.slice(0,transition_order.find("12/0/0"))
	check(port_keys == expected.map(func(row): return row[0]),"port substep order differs from the original (%d port, %d original)" % [port_keys.size(),expected.size()])
	check(mismatches.is_empty(),"substep frame counts differ from %s: %s" % [reference["source"],mismatches])
	var total: int = int(transitions["12/0/0"]) - start
	check(total == int(reference["frames_to_12_0_0"]),"opening to 12/0/0 took %d frames, original %d" % [total,int(reference["frames_to_12_0_0"])])
	print("opening frame timing: pad %d, %d substeps, %d frames to 12/0/0, %d mismatches" % [pad,expected.size(),total,mismatches.size()])
