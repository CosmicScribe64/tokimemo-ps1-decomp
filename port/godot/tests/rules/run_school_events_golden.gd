extends SceneTree
## Compares the port's birthday rules with an original-program golden written
## by tools/port/school_events_golden.py from a PCSX-Redux breakpoint run.
## Inputs (both required; a missing one fails the run):
##   TOKIMEMO_SCHOOL_EVENTS_GOLDEN  golden JSON
##   TOKIMEMO_RULE_TABLES           school-day tables manifest.json, pinned by
##                                  GameplayContentIdentity.RULE_TABLES_SHA
## Each push_birthdays hit is replayed from its own GameState and must write
## exactly the birthday records the original queue holds at those indices.
## The GEKO player-birthday init and visitor selection are replayed from their
## hits and checked against the next observed state.
##
## What a golden proves depends on the state it holds. G1
## (school-events-golden-v1) has Shiori as the only met girl, at state 3, and
## no push_birthdays hit: it covers the step-0x17 default branch, done bits
## 0x01 -> 0x05 and the Shiori and player queue records only. It cannot tell
## the met bit (bit 1 vs bit 0), the visitor ordering or the girl-loop bound
## from wrong ones. G2 (school-events-golden-v2) is the run that separates
## them; see its README for the RAM pokes it starts from.

const BirthdayQueue = preload("res://rules/girl_encounters/birthday_queue.gd")
const PlayerBirthday = preload("res://rules/girl_encounters/player_birthday.gd")
const Identity = preload("res://services/gameplay_content_identity.gd")

const PC_PUSH := "0x8006D52C"
const PC_INIT := "0x80134538"
const PC_SELECT := "0x80134808"
const PC_DEFAULT_SCRIPT := "0x801349B8"
const PC_AUDIO_LOAD := "0x8013431C"
const SCHEMA_V1 := "tokimemo.school-events-golden.v1"
## v2 projects talk_girl_selector_plus_one (0x80122D40) into every state.
const SCHEMA_V2 := "tokimemo.school-events-golden.v2"

var failures: Array[String] = []
var projects_selector: bool = false
var checks: int = 0


func _initialize() -> void:
	var golden_path: String = OS.get_environment("TOKIMEMO_SCHOOL_EVENTS_GOLDEN")
	var tables_path: String = OS.get_environment("TOKIMEMO_RULE_TABLES")
	if golden_path.is_empty() or tables_path.is_empty():
		_finish("set TOKIMEMO_SCHOOL_EVENTS_GOLDEN and TOKIMEMO_RULE_TABLES")
		return
	var golden: Variant = JSON.parse_string(FileAccess.get_file_as_string(golden_path))
	if not golden is Dictionary or String(golden.get("schema", "")) not in [SCHEMA_V1, SCHEMA_V2]:
		_finish("golden is not a tokimemo.school-events-golden v1 or v2 document")
		return
	projects_selector = String(golden["schema"]) == SCHEMA_V2
	var pokes: Array = golden.get("pokes", [])
	if not pokes.is_empty():
		print("golden run wrote %d RAM pokes before the hits (constructed state, original code)" % pokes.size())
	var tables_bytes: PackedByteArray = FileAccess.get_file_as_bytes(tables_path)
	var digest: HashingContext = HashingContext.new()
	digest.start(HashingContext.HASH_SHA256)
	digest.update(tables_bytes)
	if digest.finish().hex_encode() != Identity.RULE_TABLES_SHA:
		_finish("rule tables manifest is not the pinned school-day tables pack")
		return
	var manifest: Variant = JSON.parse_string(tables_bytes.get_string_from_utf8())
	var tables: Dictionary = _integers(manifest["tables"])
	var records: Array = golden["records"]
	_compare(records, tables)
	_finish("")


func _finish(fatal: String) -> void:
	if not fatal.is_empty():
		failures.append(fatal)
	for failure: String in failures:
		printerr("FAIL: " + failure)
	print("school-events golden: %d checks, %d failures" % [checks, failures.size()])
	quit(1 if not failures.is_empty() else 0)


func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures.append(message)


## JSON numbers arrive as floats; the rules use ints.
func _integers(value: Variant) -> Variant:
	if value is float:
		return int(value)
	if value is Array:
		var out: Array = []
		for item: Variant in value:
			out.append(_integers(item))
		return out
	if value is Dictionary:
		var out_d: Dictionary = {}
		for key: Variant in value:
			out_d[key] = _integers(value[key])
		return out_d
	return value


func _state(record: Dictionary) -> Dictionary:
	var data: Dictionary = _integers(record["state"])
	# event_log entries are not projected; the GEKO steps compared here
	# never read existing entries, only the count.
	data["event_log"] = []
	return data


func _compare(records: Array, tables: Dictionary) -> void:
	var queue_after: Array = []
	for record: Dictionary in records:
		if String(record["pc"]) == PC_INIT:
			queue_after = _integers(record["state"]["day_events"])
			break
	_check(not queue_after.is_empty(), "golden reaches encounter_player_birthday_init (mode 0x56)")
	var pushes: int = 0
	var pushed_total: int = 0
	for i: int in range(records.size()):
		var record: Dictionary = records[i]
		var pc: String = String(record["pc"])
		if pc == PC_PUSH and not queue_after.is_empty():
			pushes += 1
			var data: Dictionary = _state(record)
			var index: int = int(record["queue_index"])
			var args: Dictionary = record["args"]
			var out: Dictionary = BirthdayQueue.push_birthdays(data, index, int(args["a1"]), int(args["a2"]))
			_check(String(out["error"]).is_empty(), "push %d: no refusal" % i)
			pushed_total += (out["pushed"] as Array).size()
			for k: int in range((out["pushed"] as Array).size()):
				var original: Array = queue_after[index + k]
				var port: Array = out["pushed"][k]
				_check(int(original[0]) == int(port[0]) and (int(port[0]) == 0x33 or int(original[1]) == int(port[1])),
					"push %d record %d: original %s, port %s" % [i, index + k, str(original), str(port)])
			# The record after the pushed ones is not a birthday the port missed.
			var after: Array = queue_after[int(out["index"])]
			_check(int(after[0]) not in [0x32, 0x33] or _pushed_by_later_hit(records, i, int(out["index"])),
				"push %d (month offset %d, day %d): original has a birthday at %d the port did not push" % [i, int(args["a1"]), int(args["a2"]), int(out["index"])])
		elif pc == PC_INIT:
			var data: Dictionary = _state(record)
			var event_index: int = int(data["day_event_index"])
			var event: Array = data["day_events"][event_index]
			_check(int(data["game_mode"]) == 0x56 and int(event[0]) == 0x33, "init runs in mode 0x56 on queue type 0x33")
			# The ordinary-week loop inlines the push checks; replay them for
			# today on a scratch queue and compare with the records ending at
			# the player-birthday record the original is running.
			var scratch: Dictionary = data.duplicate(true)
			var replay: Dictionary = BirthdayQueue.push_birthdays(scratch, 0, 0, int(data["day"]))
			var pushed: Array = replay["pushed"]
			pushed_total += pushed.size()
			_check(not pushed.is_empty() and int(pushed[-1][0]) == 0x33, "today's replay ends with the player birthday (%s)" % str(pushed))
			for k: int in range(pushed.size()):
				var original: Array = data["day_events"][event_index - pushed.size() + 1 + k]
				var port: Array = pushed[k]
				_check(int(original[0]) == int(port[0]) and (int(port[0]) == 0x33 or int(original[1]) == int(port[1])),
					"inline birthday record %d: original %s, port %s" % [k, str(original), str(port)])
			var init: Dictionary = PlayerBirthday.init_step(data)
			var next: Dictionary = _next_state(records, i, PC_SELECT)
			if not next.is_empty():
				_check(int(next["day_event_done_bits"]) == int(data["day_event_done_bits"]) and not bool(init["finish"]),
					"init done bits: port 0x%02X, original 0x%02X" % [int(data["day_event_done_bits"]), int(next["day_event_done_bits"])])
		elif pc == PC_SELECT:
			var data: Dictionary = _state(record)
			# A v1 golden does not project the selector word; select_girl reads
			# it only under done bit 0x80, and missing_input refuses that case.
			var missing: String = PlayerBirthday.missing_input(data, tables)
			_check(missing.is_empty(), "select inputs: " + missing)
			if not missing.is_empty():
				continue
			var sel: Dictionary = PlayerBirthday.select_girl(data, tables)
			var next: Dictionary = _next_state(records, i, "")
			if not next.is_empty():
				# The next hit names the branch the original took: the default
				# visitor script select (step 25, reached from step 0x17) or the
				# girl branch's audio load (step 2).
				var branch_pc: String = String(records[i + 1]["pc"])
				var expected_step: int = PlayerBirthday.STEP_DEFAULT_VISITOR if branch_pc == PC_DEFAULT_SCRIPT else (2 if branch_pc == PC_AUDIO_LOAD else -1)
				_check(expected_step == int(sel["next_step"]),
					"select: port next step %d, original branch %s" % [int(sel["next_step"]), branch_pc])
				_check(int(next["current_girl"]) == int(data["current_girl"]),
					"select: current girl port %d, original %d" % [int(data["current_girl"]), int(next["current_girl"])])
				if projects_selector:
					_check(int(next["talk_girl_selector_plus_one"]) == int(data["talk_girl_selector_plus_one"]),
						"select: talk_girl_selector_plus_one port %d, original %d" % [int(data["talk_girl_selector_plus_one"]), int(next["talk_girl_selector_plus_one"])])
			else:
				print("select replayed (girl %d, next step %d); no later hit to compare" % [int(sel["girl"]), int(sel["next_step"])])
	if pushes == 0:
		print("no push_birthdays hits: ordinary weeks run the inline checks only")
	_check(pushed_total > 0, "golden exercises at least one birthday push")


func _next_state(records: Array, index: int, pc: String) -> Dictionary:
	for j: int in range(index + 1, records.size()):
		if pc.is_empty() or String(records[j]["pc"]) == pc:
			return _integers(records[j]["state"])
	return {}


func _pushed_by_later_hit(records: Array, index: int, queue_index: int) -> bool:
	for j: int in range(index + 1, records.size()):
		if String(records[j]["pc"]) == PC_PUSH and records[j].get("queue_index") != null and int(records[j]["queue_index"]) == queue_index:
			return true
	return false
