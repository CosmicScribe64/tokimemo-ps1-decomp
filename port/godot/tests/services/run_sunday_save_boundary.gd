extends SceneTree
## Focused role and bounds checks for dynamic Sunday save positions.
const State = preload("res://rules/school_day_state.gd")
const Adapter = preload("res://services/playable_save_adapter.gd")
var checks: int = 0
var failures: Array[String] = []

func _initialize() -> void:
	var adapter: Adapter = Adapter.new({})
	var apr9: State = _state(9, 95, 4, 9, 6, 8, 0xfb, 9, 0x10)
	_check(adapter._sunday_command_index(apr9) == 9, "first Sunday role locates command at native queue index 9")
	_check(adapter._boundary_error(apr9, "holiday_command", true).is_empty(), "first Sunday command is accepted")
	var returned9: State = _state(10, 95, 4, 9, 6, 9, 0x10, 10, 0xff)
	returned9.game_mode = 0x34
	_check(adapter._sunday_command_index(returned9) == 9, "returned Sunday command resolves prior native 0x10 role")
	_check(adapter._boundary_error(returned9, "next_command", true).is_empty(), "first returned command is accepted")
	var apr16: State = _state(15, 95, 4, 16, 6, 14, 0xfb, 15, 0x10)
	apr16.game_mode = 0x46
	_check(adapter._sunday_command_index(apr16) == 15, "second Sunday command is located from its queue role, not fixed index")
	_check(adapter._boundary_error(apr16, "holiday_command", true).is_empty(), "second Sunday current 0x10 command is accepted")
	var returned16: State = _state(16, 95, 4, 16, 6, 15, 0x10, 16, 0xff)
	returned16.game_mode = 0x34
	_check(adapter._boundary_error(returned16, "next_command", true).is_empty(), "second returned Sunday current 0xff after 0x10 is accepted")
	var wrong_index: State = _state(14, 95, 4, 16, 6, 14, 0xfb, 15, 0x10)
	wrong_index.game_mode = 0x46
	_check(adapter._sunday_command_index(wrong_index) == -1, "wrong current index cannot borrow adjacent Sunday opcode")
	_check(not adapter._boundary_error(wrong_index, "holiday_command", true).is_empty(), "wrong Sunday index is rejected")
	var wrong_opcode: State = _state(15, 95, 4, 16, 6, 14, 0xfb, 15, 0xff)
	wrong_opcode.game_mode = 0x46
	_check(adapter._sunday_command_index(wrong_opcode) == -1, "padding opcode cannot impersonate current Sunday menu")
	var wrong_day: State = _state(15, 95, 4, 15, 5, 14, 0xfb, 15, 0x10)
	wrong_day.game_mode = 0x46
	_check(adapter._sunday_command_index(wrong_day) == -1, "wrong weekday/date cannot impersonate Sunday")
	var wrong_return: State = _state(16, 95, 4, 16, 6, 15, 0x11, 16, 0xff)
	wrong_return.game_mode = 0x34
	_check(adapter._sunday_command_index(wrong_return) == -1, "returned command requires previous opcode 0x10")
	var bad_record: State = _state(15, 95, 4, 16, 6, 14, 0xfb, 15, 0x10)
	bad_record.game_mode = 0x46
	bad_record.rule_state["day_events"][20] = [0x10, 0, 0]
	_check(adapter._sunday_command_index(bad_record) == -1, "malformed queue records reject boundary classification")
	var bad_byte: State = _state(15, 95, 4, 16, 6, 14, 0xfb, 15, 0x10)
	bad_byte.rule_state["day_events"][20][1] = 256
	_check(adapter._sunday_command_index(bad_byte) == -1, "out-of-range opcode record rejects boundary classification")
	var bad_index_type: State = _state(15, 95, 4, 16, 6, 14, 0xfb, 15, 0x10)
	bad_index_type.rule_state["day_event_index"] = "15"
	_check(adapter._sunday_command_index(bad_index_type) == -1, "noninteger current index rejects boundary classification")
	var unbounded_year: State = _state(15, 100000000, 4, 16, 6, 14, 0xfb, 15, 0x10)
	_check(adapter._sunday_command_index(unbounded_year) == -1, "year outside native byte bounds rejects before calendar traversal")
	print(JSON.stringify({"checks": checks, "failures": failures}))
	quit(1 if not failures.is_empty() else 0)

func _state(index: int, year: int, month: int, day: int, weekday: int, prev_index: int, prev_opcode: int, cur_index: int, cur_opcode: int) -> State:
	var state: State = State.new()
	state.game_mode = 0x46
	state.mode_stage = 1
	var events: Array = []
	for i: int in range(32): events.append([0xff, 0, 0, 0])
	events[prev_index] = [prev_opcode, 0, 0, 0]
	events[cur_index] = [cur_opcode, 0, 0, 0]
	state.rule_state = {"year": year, "month": month, "day": day, "weekday": weekday,
		"day_event_index": index, "day_events": events}
	return state

func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok: failures.append(message)
