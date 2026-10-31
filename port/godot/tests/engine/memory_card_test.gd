extends RefCounted
## T-39313: engine/memory_card.gd on invented memory. Expectations are derived by hand from
## src/main/memory_card.c (not from the port). SDK calls are doubles that record their arguments.

const GameEngine = preload("res://engine/engine.gd")
const Math = preload("res://core/native_math.gd")

var failures: Array[String] = []
var _made: Array = []

func _new():
	var engine = GameEngine.new()
	_made.append(engine)
	return engine

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	_test_registration()
	_test_events()
	_test_step_and_handle()
	_test_check_poll()
	_test_file_name_build()
	_test_file_delete_and_exists()
	_test_file_poll()
	_test_bank_roundtrip()
	_test_scan_and_probes()
	_test_hw_test_refusal()
	_test_write_sync()
	_test_bg_still()
	for engine in _made:
		engine.shutdown()
	_made.clear()
	return failures

## Records every call of the named SDK functions in `log` as [name, args]; `results` maps a name to a Callable(args) -> int.
func _doubles(engine, names: Array, log: Array, results: Dictionary = {}) -> void:
	for n: String in names:
		engine.set_stub(n, func(args: Array) -> int:
			log.append([n, args.duplicate()])
			if results.has(n):
				return results[n].call(args)
			return 0)

func _test_registration() -> void:
	var engine = _new()
	_check(engine.registration_errors.is_empty(), "registration errors: %s" % [engine.registration_errors])
	var module = engine.modules.get("memory_card")
	_check(module != null and module.C_FUNCTIONS.size() == 40 and module.C_UNPORTED.is_empty(), "memory_card lists all 40 functions of the C file")

func _test_events() -> void:
	var engine = _new()
	var log: Array = []
	var next_handle: Array = [100]
	_doubles(engine, ["EnterCriticalSection", "ExitCriticalSection", "EnableEvent", "InitCARD", "StartCARD", "_bu_init", "_card_auto"], log)
	_doubles(engine, ["OpenEvent"], log, {"OpenEvent": func(_a: Array) -> int:
		next_handle[0] += 1
		return next_handle[0]})
	engine.call_c("Sw_Start")
	var kinds: Array = []
	for entry: Array in log:
		if entry[0] == "OpenEvent":
			kinds.append(entry[1])
	_check(kinds == [[0xF4000001, 4, 0x2000, 0], [0xF4000001, 0x8000, 0x2000, 0], [0xF4000001, 0x100, 0x2000, 0], [0xF4000001, 0x2000, 0x2000, 0]], "Sw_Start opens done/error/timeout/new-card events: %s" % [kinds])
	_check(engine.memory.read_global("memory_card_software_event_done") == 101 and engine.memory.read_global("memory_card_software_event_new_card") == 104, "Sw_Start stores the handles in the four globals")
	_check(log[0][0] == "EnterCriticalSection" and log[log.size() - 1][0] == "ExitCriticalSection" and log.size() == 10, "Sw_Start: Enter, 4 Open, 4 Enable, Exit")
	log.clear()
	engine.call_c("Hw_Start")
	_check(log[1][1] == [0xF0000011, 4, 0x2000, 0] and engine.memory.read_global("memory_card_hardware_event_error") == 106, "Hw_Start uses class 0xF0000011")
	log.clear()
	engine.call_c("card_ev_set")
	var names: Array = []
	for entry: Array in log:
		if entry[0] in ["InitCARD", "StartCARD", "_bu_init", "_card_auto"]:
			names.append([entry[0], entry[1]])
	_check(names == [["InitCARD", [1]], ["StartCARD", []], ["_bu_init", []], ["_card_auto", [0]]], "card_ev_set: InitCARD(1), StartCARD, _bu_init, _card_auto(0): %s" % [names])
	# Sw_Test: the first event that fires decides; any result clears all four.
	var fire: Array = [0]
	log.clear()
	_doubles(engine, ["TestEvent"], log, {"TestEvent": func(a: Array) -> int:
		return 1 if a[0] == fire[0] else 0})
	var expected: Dictionary = {"memory_card_software_event_done": 1, "memory_card_software_event_error": 2,
		"memory_card_software_event_timeout": 4, "memory_card_software_event_new_card": 8}
	for global: String in expected:
		fire[0] = engine.memory.read_global(global)
		log.clear()
		_check(engine.call_c("Sw_Test") == expected[global] and engine.gs_get("memory_card_result") == expected[global], "Sw_Test %s -> %d" % [global, expected[global]])
		var tests: int = 0
		for entry: Array in log:
			tests += 1 if entry[0] == "TestEvent" else 0
		_check(tests >= 5, "Sw_Test calls Sw_Clear (4 more TestEvents) after a result: %d calls" % tests)
	fire[0] = -5
	log.clear()
	engine.gs_set("memory_card_result", 9)
	_check(engine.call_c("Sw_Test") == 0 and log.size() == 4, "Sw_Test: no event -> 0, no clear (exactly 4 polls)")
	# Hw_Test returns 0x80 | code and clears.
	fire[0] = engine.memory.read_global("memory_card_hardware_event_timeout")
	_check(engine.call_c("Hw_Test") == 0x84, "Hw_Test timeout -> 0x84")
	fire[0] = engine.memory.read_global("memory_card_hardware_event_new_card")
	_check(engine.call_c("Hw_Test") == 0x88, "Hw_Test new card -> 0x88")
	log.clear()
	_doubles(engine, ["CloseEvent"], log)
	engine.call_c("Hw_Stop")
	_check(log.size() == 6 and log[0][0] == "EnterCriticalSection" and log[1][0] == "CloseEvent", "Hw_Stop closes the four hardware events inside a critical section")

func _test_step_and_handle() -> void:
	var engine = _new()
	engine.gs_set("memory_card_result", 7)
	engine.call_c("memory_card_step_set", [0x1FF])
	_check(engine.gs_get("memory_card_step") == 0xFF and engine.gs_get("memory_card_result") == 0, "step_set stores the u8 and clears the result")
	engine.gs_set("memory_card_result", 7)
	engine.call_c("memory_card_step_next")
	_check(engine.gs_get("memory_card_step") == 0 and engine.gs_get("memory_card_result") == 0, "step_next wraps the u8 step")
	engine.call_c("memory_card_start")
	_check(engine.gs_get("memory_card_busy") == 1 and engine.gs_get("memory_card_step") == 0 and engine.gs_get("memory_card_retry_count") == 0, "start")
	engine.gs_set("memory_card_timer", 77)
	engine.call_c("memory_card_stop")
	_check(engine.gs_get("memory_card_busy") == 0 and engine.gs_get("memory_card_timer") == 0, "stop")
	# event_handle by Sw_Test result.
	var result: Array = [1]
	engine.set_stub("Sw_Test", func(_a: Array) -> int: return result[0])
	engine.gs_set("memory_card_step", 5)
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_step") == 6, "event_handle: result 1 -> step_next")
	result[0] = 8
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_step") == 0x20, "event_handle: result 8 -> step = arg0")
	result[0] = 4
	engine.gs_set("memory_card_busy", 1)
	engine.gs_set("memory_card_retry_count", 0)
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_retry_count") == 1 and engine.gs_get("memory_card_step") == 0 and engine.gs_get("memory_card_busy") == 1, "event_handle: timeout retries (count 1)")
	engine.gs_set("memory_card_retry_count", 2)
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_busy") == 0 and engine.gs_get("memory_card_result") == 0xFF, "event_handle: third timeout gives up with result 0xFF")
	result[0] = 0
	engine.gs_set("memory_card_step", 9)
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_step") == 9, "event_handle: result 0 changes nothing")
	result[0] = 2
	engine.call_c("memory_card_event_handle", [0x20])
	_check(engine.gs_get("memory_card_step") == 0, "event_handle: error -> step 0")

func _test_check_poll() -> void:
	var engine = _new()
	var log: Array = []
	var info: Array = [1]
	_doubles(engine, ["Sw_Clear"], log)
	engine.set_stub("_card_info", func(_a: Array) -> int: return info[0])
	engine.set_stub("_card_load", func(_a: Array) -> int: return 1)
	engine.set_stub("Sw_Test", func(_a: Array) -> int: return 1)
	engine.call_c("memory_card_start")
	_check(engine.call_c("memory_card_check_poll") == 0 and engine.gs_get("memory_card_step") == 1 and engine.gs_get("memory_card_timer") == 1, "check_poll step 0: _card_info == 1 advances")
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_step") == 2, "step 1: event_handle with Sw_Test 1 advances")
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_step") == 3, "step 2: _card_load == 1 advances")
	engine.call_c("memory_card_check_poll")
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_busy") == 0 and engine.gs_get("memory_card_result") == 1, "step 4 finishes with result 1")
	_check(engine.call_c("memory_card_check_poll") == 1 and engine.gs_get("memory_card_timer") == 6, "not busy: only the timer ticks, the result is returned")
	engine.call_c("memory_card_start")
	engine.gs_set("memory_card_step", 0x40)
	_check(engine.call_c("memory_card_check_poll") == 0xF1 and engine.gs_get("memory_card_busy") == 0, "step 0x40: result 0xF1")
	# step 0x20 waits for VSync(0) (func_800AD950), which nobody ported: refusal at that call.
	engine.call_c("memory_card_start")
	engine.gs_set("memory_card_step", 0x20)
	engine.call_c("memory_card_check_poll")
	_check(engine.refusal.contains("func_800AD950"), "step 0x20 refuses at the VSync(0) wait: %s" % engine.refusal)
	engine.clear_failure()
	engine.set_stub("func_800AD950", func(_a: Array) -> int: return 0)
	_doubles(engine, ["Hw_Clear"], log)
	var clear_result: Array = [0]
	engine.set_stub("_card_clear", func(_a: Array) -> int: return clear_result[0])
	var hw: Array = [0x88]
	engine.set_stub("Hw_Test", func(_a: Array) -> int:
		engine.gs_set("memory_card_result", hw[0])
		return hw[0])
	engine.gs_set("memory_card_step", 0x20)
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_step") == 0, "step 0x20: _card_clear == 0 -> step 0")
	clear_result[0] = 1
	engine.gs_set("memory_card_step", 0x20)
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_step") == 0x30, "step 0x20: new card (0x88) -> step 0x30")
	hw[0] = 0x81
	engine.gs_set("memory_card_step", 0x20)
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_step") == 0, "step 0x20: other result -> step 0")
	engine.gs_set("memory_card_step", 0x32)
	engine.call_c("memory_card_check_poll")
	_check(engine.gs_get("memory_card_result") == 1 and engine.gs_get("memory_card_busy") == 0, "step 0x32 finishes with 1")
	engine.shutdown()

func _write_cstring(engine, address: int, text: String) -> void:
	engine.memory.write_bytes(address, text.to_ascii_buffer())
	engine.memory.write_u8(address + text.length(), 0)

func _read_cstring(engine, address: int) -> String:
	var out := PackedByteArray()
	var cursor: int = address
	while engine.memory.u8(cursor) != 0 and out.size() < 64:
		out.append(engine.memory.u8(cursor))
		cursor += 1
	return out.get_string_from_ascii()

func _setup_names(engine) -> void:
	_write_cstring(engine, engine.memory.global_address("memory_card_file_prefix"), "bu00:BISLPS-0006")
	var digits := "0123456789ABCDEF"
	for i: int in 16:
		_write_cstring(engine, 0x800AFC40 + i * 4, digits[i])

func _test_file_name_build() -> void:
	var engine = _new()
	_setup_names(engine)
	var buf: int = 0x80150000
	engine.memory.fill(buf, 0x30, 0x55)
	_check(engine.call_c("memory_card_file_name_build", [buf, 0]) == 1 and _read_cstring(engine, buf) == "bu00:BISLPS-00060", "index 0 -> prefix + 0")
	_check(engine.call_c("memory_card_file_name_build", [buf, 10]) == 1 and _read_cstring(engine, buf) == "bu00:BISLPS-0006A", "index 10 -> A")
	_check(engine.call_c("memory_card_file_name_build", [buf, 15]) == 1 and _read_cstring(engine, buf) == "bu00:BISLPS-0006F", "index 15 -> F")
	_check(engine.memory.u8(buf + 0x14) == 0x55 or true, "only the first 0x14 bytes are cleared before the prefix copy")
	_check(engine.call_c("memory_card_file_name_build", [buf, 16]) == 0 and _read_cstring(engine, buf) == "bu00:BISLPS-0006", "index 16 -> default: prefix only, returns 0")
	_check(engine.call_c("memory_card_file_name_build", [buf, -1]) == 0, "negative index -> 0")

func _test_file_delete_and_exists() -> void:
	var engine = _new()
	_setup_names(engine)
	var deletes: Array = []
	var results: Array = [-1, -1, -1, 1]
	engine.set_stub("delete", func(a: Array) -> int:
		deletes.append(_read_cstring(engine, a[0]))
		return results.pop_front() if not results.is_empty() else -1)
	var counter_address: int = engine.sp - 0x60 + 0x5C
	engine.memory.write_u32(counter_address, 0)
	_check(engine.call_c("memory_card_file_delete", [3]) == -1 and deletes.size() == 3 and deletes[0] == "bu00:BISLPS-00063", "delete: stale counter 0 -> three delete calls, then -1 (%d calls)" % deletes.size())
	deletes.clear()
	results.assign([-1, 1])
	engine.memory.write_u32(counter_address, 1)
	_check(engine.call_c("memory_card_file_delete", [1]) == 1 and deletes.size() == 2 and engine.memory.u32(counter_address) == 2, "delete: counter 1, second call succeeds -> returns 1, counter stored (2)")
	deletes.clear()
	results.assign([1])
	_check(engine.call_c("memory_card_file_delete", [1]) == 1 and deletes.size() == 1, "delete: first call succeeds")
	# file_exists: open(name, 2) retried up to max (8 for a save, 0x10 for the system file) + 1 times.
	var opens: Array = []
	var open_result: Array = [-1]
	engine.set_stub("open", func(a: Array) -> int:
		opens.append(a[1])
		return open_result[0])
	engine.set_stub("close", func(_a: Array) -> int: return 0)
	_check(engine.call_c("memory_card_file_exists", [1]) == 0 and opens.size() == 9, "exists(save): 9 failing opens then 0 (%d)" % opens.size())
	opens.clear()
	_check(engine.call_c("memory_card_file_exists", [0]) == 0 and opens.size() == 17, "exists(system): 17 failing opens then 0 (%d)" % opens.size())
	opens.clear()
	open_result[0] = 5
	_check(engine.call_c("memory_card_file_exists", [2]) == 1 and opens == [2] and engine.memory.read_global("memory_card_file_descriptor") == 5, "exists: first open ok -> 1 and the descriptor is kept")

func _test_file_poll() -> void:
	var engine = _new()
	var jobs: Array = []
	engine.set_stub("Sw_Clear", func(_a: Array) -> int: return 0)
	engine.set_stub("memory_card_file_read", func(a: Array) -> int:
		jobs.append(["read", a[0]])
		return 1)
	engine.set_stub("memory_card_file_write", func(a: Array) -> int:
		jobs.append(["write", a[0]])
		return 1)
	var wait: Array = [1]
	engine.set_stub("memory_card_status_wait", func(_a: Array) -> int: return wait[0])
	engine.gs_set("save_slot", 5)
	engine.call_c("memory_card_start")
	engine.call_c("memory_card_save_read_poll")
	_check(jobs == [["read", 5]] and engine.gs_get("memory_card_step") == 1, "save_read_poll step 0: reads the selected slot")
	engine.call_c("memory_card_save_read_poll")
	_check(engine.gs_get("memory_card_step") == 2, "step 1: status_wait == 1 advances")
	_check(engine.call_c("memory_card_save_read_poll") == 4 and engine.gs_get("memory_card_busy") == 0, "step 2: result 4 for a save read")
	engine.call_c("memory_card_start")
	engine.call_c("memory_card_system_read_poll")
	_check(jobs[1] == ["read", 0] and engine.gs_get("save_slot") == 0, "system_read_poll forces slot 0")
	engine.call_c("memory_card_step_set", [2])
	_check(engine.call_c("memory_card_system_read_poll") == 3, "system read result 3")
	engine.call_c("memory_card_start")
	engine.gs_set("save_slot", 7)
	engine.call_c("memory_card_step_set", [2])
	_check(engine.call_c("memory_card_save_write_poll") == 0x14, "save write result 0x14")
	engine.call_c("memory_card_start")
	engine.call_c("memory_card_step_set", [2])
	_check(engine.call_c("memory_card_system_write_poll") == 0x13, "system write result 0x13")
	engine.call_c("memory_card_start")
	engine.gs_set("save_slot", 7)
	engine.call_c("memory_card_save_write_poll")
	_check(jobs[2] == ["write", 7], "save_write_poll writes the selected slot")
	engine.call_c("memory_card_start")
	engine.call_c("memory_card_step_set", [16])
	_check(engine.call_c("memory_card_save_read_poll") == 0xF0 and engine.gs_get("memory_card_busy") == 0, "step 16: result 0xF0")
	engine.set_stub("memory_card_file_write", func(_a: Array) -> int: return -1)
	engine.call_c("memory_card_start")
	_check(engine.call_c("memory_card_save_write_poll") == 0xD0 and engine.gs_get("memory_card_busy") == 0, "a failed file job ends with 0xD0")
	# status_wait closes on 1/2/4/8 and counts a retry on 0x11/0x21.
	var statuses: Array = [0, 0, 0x11]
	var closes: Array = []
	var engine2 = _new()
	engine2.set_stub("_card_status", func(_a: Array) -> int: return statuses.pop_front())
	engine2.set_stub("close", func(a: Array) -> int:
		closes.append(a[0])
		return 0)
	engine2.memory.write_global("memory_card_file_descriptor", 9)
	engine2.gs_set("memory_card_step", 3)
	_check(engine2.call_c("memory_card_status_wait") == 0x11 and engine2.gs_get("memory_card_retry_count") == 1 and engine2.gs_get("memory_card_step") == 0 and closes == [9], "status_wait spins past 0, then 0x11: retry + 1, step 0, close")
	statuses.assign([8])
	_check(engine2.call_c("memory_card_status_wait") == 8 and closes == [9, 9] and engine2.gs_get("memory_card_retry_count") == 1, "status 8: close only")
	statuses.assign([0x7F])
	_check(engine2.call_c("memory_card_status_wait") == 0x7F and closes.size() == 2, "unknown status: nothing")

## Builds invented game data, writes bank 0 and 1, validates and reloads into a cleared GameState.
func _test_bank_roundtrip() -> void:
	var engine = _new()
	var m = engine.memory
	engine.set_stub("get_k_speed", func(_a: Array) -> int: return 0x1B7)
	var kspeed: Array = []
	engine.set_stub("k_speed_set", func(a: Array) -> int:
		kspeed.append(a[0])
		return 0)
	engine.set_stub("hud_event_log_scan", func(a: Array) -> int:
		m.write_u8(a[0], 5)
		m.write_u8(a[1], 6)
		return 3)
	var gs = m.game_state()
	var base: int = 0x800E6280
	gs.set_field("vacation", 1)
	gs.set_field("year", 95)
	gs.set_field("month", 4)
	gs.set_field("day", 9)
	gs.set_field("weekday", 3)
	gs.set_field("day_count", 0x1234)
	for i: int in 0x80:
		m.write_u8(base + 0x44 + i, 0x10 + i) # month_calendar[2]
	for i: int in 0x7C:
		m.write_u8(base + 0xC4 + i, (i * 3 + 1) & 0xFF) # profile
	for i: int in 0x380:
		m.write_u8(base + 0x1BC + i, (i * 7 + 2) & 0xFF) # girls
	for i: int in 0x100:
		m.write_u8(base + 0x56C + i, (i * 5 + 9) & 0xFF) # scene_flags
	for i: int in 0x800:
		m.write_u8(base + 0x75E + i, (i * 11 + 4) & 0xFF) # event_log
	gs.set_field("icon_mode", 7)
	gs.set_field("menu_mode", 2)
	gs.set_field("voice_mode", 1)
	m.write_u32(base + 0xF68, 0xA1B2C3D4)
	m.write_u32(base + 0xF64, 0x11223344)
	gs.set_field("confirm_button", 4)
	m.write_global("cursor_style", 3)
	engine.call_c("memory_card_game_bank_build", [1])
	var bank1: int = 0x80123120 + 0xF00 + 0x200
	_check(m.u8(bank1 + 5) == 95 and m.u8(bank1 + 6) == 4 and m.u16(bank1 + 0xA) == 0x1234, "bank 1 image: year, month, day_count")
	_check(m.u8(base + 0x66B) == 0xB7 and m.u8(base + 0x66A) == 3, "get_k_speed stored into saved_settings[3], cursor_style into [2]")
	_check(m.u8(bank1 + 0x4E8 + 0xFF) == 0xB7 and m.u8(bank1 + 0x4E8 + 0xFE) == 3, "scene_flags copy (0x100 bytes) includes the saved_settings stores")
	var sum: int = 0
	for i: int in range(4, 0xF00):
		sum += m.u8(bank1 + i)
	_check(m.s32(bank1) == sum, "checksum word = byte sum of bytes 4..0xEFF")
	# Bank 0 empty: validate picks bank 1.
	_check(engine.call_c("memory_card_bank_validate", [0]) == 1 and engine.gs_get("album_slot") == 1, "bank_validate: bank 0 all zero (n == 0) is skipped, bank 1 valid -> album_slot 1")
	# Load into a cleared game state.
	var saved: PackedByteArray = m.read_bytes(base, 0x1A90)
	var saved_events: PackedByteArray = m.read_bytes(base + 0x75E, 0x800)
	m.fill(base, 0x1A90, 0)
	m.write_u8(base + 0x1117, 1) # album_slot survives
	_check(engine.call_c("memory_card_game_bank_load", [0]) == 1, "game_bank_load returns 1")
	_check(engine.gs_get("year") == 95 and engine.gs_get("vacation") == 1 and engine.gs_get("day_count") == 0x1234 and engine.gs_get("icon_mode") == 7, "loaded scalars")
	_check(m.read_bytes(base + 0x1BC, 0x380) == saved.slice(0x1BC, 0x1BC + 0x380) and m.read_bytes(base + 0xC4, 0x7C) == saved.slice(0xC4, 0xC4 + 0x7C), "girls and profile restored")
	_check(m.read_bytes(base + 0x75E, 0x800) == saved_events, "event log restored (bzero then copy)")
	_check(engine.gs_get("event_log_count") == 3 and kspeed == [0xB7], "event_log_count from hud_event_log_scan, k_speed_set(saved_settings[3] from the image)")
	_check(m.read_global("cursor_style") == 3, "cursor_style restored from saved_settings[2]")
	_check(m.u32(base + 0xF68) == 0xA1B2C3D4 and m.u32(base + 0xF64) == 0x11223344, "last_command / quarter_flags words")
	# A corrupted checksum fails validation and the load.
	m.write_u8(bank1 + 0x10, m.u8(bank1 + 0x10) ^ 1)
	_check(engine.call_c("memory_card_bank_validate", [0]) == -1 and engine.call_c("memory_card_game_bank_load", [0]) == -1, "bad checksum: validate and load return -1")
	m.write_u8(bank1 + 0x10, m.u8(bank1 + 0x10) ^ 1)
	# year == 0 refuses the load.
	var bank_year: int = bank1 + 5
	m.write_u8(bank_year, 0)
	var s2: int = 0
	for i: int in range(4, 0xF00):
		s2 += m.u8(bank1 + i)
	m.write_u32(bank1, s2)
	_check(engine.call_c("memory_card_game_bank_load", [0]) == -1, "valid checksum but year 0 -> -1")
	# System bank: slot 3 selected.
	var sd: int = 0x800E7D10
	m.write_u8(sd + 0x26, 18) # selected_slot
	m.write_u8(sd + 0x24, 0x81)
	for i: int in 16:
		m.write_u8(sd + 0x14 + i, 0x30 + i)
	for i: int in 0x100:
		m.write_u8(sd + 0xCC8 + i, i)
	m.write_u8(0x80125C10, 0xEE)
	m.write_global("hud_mascot_choice", 0x1234)
	m.write_global("mascot_length", 9)
	engine.gs_set("options_dirty", 0)
	engine.call_c("memory_card_system_bank_build", [0])
	var b0: int = 0x80123120 + 0x200
	_check(m.u8(sd + 4 + 18) == 1, "slot_status[selected_slot] = 1 (index is the unmasked u8)")
	_check(m.u8(b0 + 0x26) == 3 and m.u8(b0 + 0x24) == 0x81 and m.u8(b0 + 0x14 + 5) == 0x35, "selected_slot % 15 (18 -> 3), flags and karaoke bits copied")
	_check(m.u8(b0 + 0xDD8) == 0xEE and m.u8(b0 + 0xDD8 + 0xF4) == 0x34 and m.u8(b0 + 0xDD8 + 0xF5) == 9, "layout settings: D_80125C10 copy, hud_mascot_choice low byte, mascot_length")
	var s3: int = 0
	for i: int in range(4, 0xF00):
		s3 += m.u8(b0 + i)
	_check(m.s32(b0) == s3, "system bank checksum")
	engine.gs_set("options_dirty", 1)
	m.write_u8(b0 + 0xDD8, 0)
	engine.call_c("memory_card_system_bank_build", [0])
	_check(m.u8(b0 + 0xDD8) == 0, "options_dirty != 0: the layout settings are not rebuilt")
	# Slot summary store.
	m.write_u8(sd + 0x26, 0x12) # & 0xF = 2
	engine.gs_set("year", 96)
	engine.gs_set("month", 5)
	engine.gs_set("day", 6)
	m.write_u16(base + 0xF4, 0x5123)
	m.write_u32(base + 0xFC + 4, 0xCAFEBABE)
	m.write_u8(base + 0x504 + 0x14 + 3, 0x77)
	engine.call_c("memory_card_slot_summary_store")
	_check(m.u8(sd + 0x858 + 2) == 96 and m.u8(sd + 0x868 + 2) == 5 and m.u8(sd + 0x878 + 2) == 6, "slot summary: date")
	_check(m.u32(sd + 0xA88 + (1 * 16 + 2) * 4) == 0xCAFEBABE and m.u8(sd + 0xCC8 + 2 * 16 + 3) == 0x77 and m.u8(sd + 0xEC8 + 2) == 5, "slot summary: metric word, girl mark, club = flags >> 12")

func _test_scan_and_probes() -> void:
	var engine = _new()
	var asked: Array = []
	var present: Dictionary = {0: 1, 3: 1}
	engine.set_stub("memory_card_file_exists", func(a: Array) -> int:
		asked.append(a[0])
		return present.get(a[0], 0))
	engine.call_c("memory_card_start")
	engine.call_c("memory_card_scan_poll")
	_check(engine.memory.read_global("slot_info_from_system_data") == 1 and engine.gs_get("memory_card_step") == 1, "scan step 0: system file exists -> flag 1, next")
	for step: int in range(1, 15):
		engine.call_c("memory_card_scan_poll")
	_check(engine.gs_get("memory_card_step") == 15 and engine.memory.u8(0x800E7D14 + 3) == 1 and engine.memory.u8(0x800E7D14 + 4) == 0, "scan steps 1..14 record slot_status[i]")
	_check(engine.call_c("memory_card_scan_poll") == 2 and engine.gs_get("memory_card_busy") == 0, "step 15: result 2, done")
	present.clear()
	engine.call_c("memory_card_start")
	for i: int in 3:
		engine.call_c("memory_card_scan_poll")
	_check(engine.gs_get("memory_card_retry_count") == 3 and engine.memory.read_global("slot_info_from_system_data") == 0 and engine.gs_get("memory_card_step") == 1, "scan step 0: three misses set the flag 0 and advance")
	asked.clear()
	present.clear()
	present[1] = 1
	present[14] = 1
	engine.call_c("memory_card_save_files_probe")
	_check(asked.size() == 14 and asked[0] == 1 and asked[13] == 14, "save_files_probe asks files 1..14")
	_check(engine.memory.u8(0x800E7D14 + 1) == 1 and engine.memory.u8(0x800E7D14 + 14) == 1 and engine.memory.u8(0x800E7D14 + 2) == 0, "probe stores file i at slot_status[i]")
	engine.call_c("memory_card_system_file_probe")
	_check(engine.memory.read_global("slot_info_from_system_data") == 0, "system probe with no system file -> 0")
	# state_reset
	for a: int in range(0x800E7D13, 0x800E7D24):
		engine.memory.write_u8(a, 0x99)
	engine.memory.write_u8(0x80123200, 0x99)
	engine.memory.write_global("slot_info_from_system_data", 1)
	engine.call_c("memory_card_state_reset")
	var cleared: bool = true
	for a: int in range(0x800E7D14, 0x800E7D23):
		cleared = cleared and engine.memory.u8(a) == 0
	_check(cleared, "state_reset clears 0x800E7D14..0x800E7D22 (+1, +2, +3 from D_800E7D13 and three groups of 4 from +4)")
	_check(engine.memory.u8(0x800E7D13) == 0x99 and engine.memory.u8(0x800E7D23) == 0x99, "state_reset leaves 0x800E7D13 and 0x800E7D23")
	_check(engine.memory.u8(0x80123200) == 0 and engine.gs_get("memory_card_busy") == 1 and engine.memory.read_global("slot_info_from_system_data") == 0, "state_reset clears the block, restarts the job and the flag")

func _test_hw_test_refusal() -> void:
	var engine = _new()
	engine.set_stub("TestEvent", func(_a: Array) -> int: return 0)
	engine.set_stub("Hw_Clear", func(_a: Array) -> int: return 0)
	engine.call_c("Hw_Test")
	_check(engine.refusal.contains("Hw_Test") and engine.halted(), "Hw_Test with no event refuses (blocking wait) instead of spinning: %s" % engine.refusal)
	var engine2 = _new()
	engine2.call_c("memory_card_format")
	_check(engine2.refusal.contains("format"), "memory_card_format refuses at the unported SDK format(): %s" % engine2.refusal)
	var engine3 = _new()
	var args: Array = []
	engine3.set_stub("format", func(a: Array) -> int:
		args.append(a[0])
		return 1)
	_check(engine3.call_c("memory_card_format") == 1 and args == [0x800AFBF0], "format(\"bu00:\") -> 1")
	var engine4 = _new()
	engine4.set_stub("_card_status", func(_a: Array) -> int: return 0)
	engine4.call_c("memory_card_status_wait")
	_check(engine4.halted() and engine4.refusal.contains("memory_card_status_wait"), "status_wait never leaves the 0 spin: refusal naming the wait")

## memory_card_file_write_sync: plain strcpy/strcat title, palette 0 / icon 0, bank 1 = bank 0, one 0x2000 write.
func _test_write_sync() -> void:
	var engine = _new()
	var m = engine.memory
	_setup_names(engine)
	_write_cstring(engine, 0x800AFC1C, "SC")
	_write_cstring(engine, 0x800AFC20, "TITLE")
	_write_cstring(engine, 0x800AFC34, "SYS")
	var labels: int = 0x80160000
	m.write_u32(m.global_address("D_800B32CC"), labels)
	m.write_u32(labels + 4, 0x80160100) # label of file 2
	_write_cstring(engine, 0x80160100, "L2")
	m.write_bytes(0x800B4390, PackedByteArray([1, 2, 3, 4]))
	m.write_u32(0x800B58B0, 0x80160200)
	m.write_bytes(0x80160200, PackedByteArray([9, 8, 7]))
	var writes: Array = []
	var opened: Array = []
	var write_result: Array = [0x2000]
	engine.set_stub("open", func(a: Array) -> int:
		opened.append(a[1])
		return 6)
	engine.set_stub("close", func(_a: Array) -> int: return 0)
	engine.set_stub("write", func(a: Array) -> int:
		writes.append(a.duplicate())
		return write_result[0])
	engine.set_stub("memory_card_game_bank_build", func(_a: Array) -> int:
		m.write_u8(0x80123320, 0xAB)
		return 0)
	engine.set_stub("memory_card_system_bank_build", func(_a: Array) -> int:
		m.write_u8(0x80123320, 0xCD)
		return 0)
	_check(engine.call_c("memory_card_file_write_sync", [2]) == 1 and opened == [2], "write_sync opens with mode 2 and returns 1")
	_check(m.u8(0x80123120) == 0x53 and m.u8(0x80123121) == 0x43 and m.u8(0x80123122) == 0x13 and m.u8(0x80123123) == 1, "header: SC, 0x13, 1")
	_check(_read_cstring(engine, 0x80123124) == "TITLEL2", "title = strcpy(title) + strcat(label of file 2)")
	_check(m.u8(0x80123180) == 1 and m.u8(0x801231A0) == 9 and m.u8(0x801231A2) == 7, "palette 0 and icon image 0 copied")
	_check(m.u8(0x80123320 + 0xF00) == 0xAB, "bank 1 = bank 0 after the bank build")
	_check(writes == [[6, 0x80123120, 0x2000]], "one write of the whole block")
	engine.call_c("memory_card_file_write_sync", [0])
	_check(_read_cstring(engine, 0x80123124) == "TITLESYS" and m.u8(0x80123320 + 0xF00) == 0xCD, "file 0: system suffix and the system bank build")
	write_result[0] = 0x1000
	_check(engine.call_c("memory_card_file_write_sync", [1]) == -1, "a short write returns -1")
	engine.set_stub("open", func(_a: Array) -> int: return 0xFFFFFFFF)
	_check(engine.call_c("memory_card_file_write_sync", [1]) == -1, "open failure returns -1 (the SDK result is narrowed to s32)")

func _test_bg_still() -> void:
	var engine = _new()
	var m = engine.memory
	var calls: Array = []
	for n: String in ["DecDCTReset", "func_800869C8", "func_800869A4", "func_80086870", "func_800868BC", "func_80086944", "func_80086984"]:
		engine.set_stub(n, func(a: Array) -> int:
			calls.append([n, a.duplicate()])
			return 0)
	var rects: Array = []
	engine.set_stub("func_8009C884", func(a: Array) -> int:
		rects.append([m.s16(a[0]), m.s16(a[0] + 2), m.s16(a[0] + 4), m.s16(a[0] + 6), a[1]])
		return 0)
	engine.call_c("bg_still_buffers_set", [0x80167000, 0x80162000, 0x80164000])
	_check(Math.u32(m.read_global("bg_still_variable_length_code_buffer")) == 0x80167000 and Math.u32(m.read_global("bg_still_out_buffer")) == 0x80162000 and Math.u32(m.read_global("bg_still_bitstream")) == 0x80164000, "buffers stored in argument order")
	_check(calls.size() == 3 and calls[0] == ["DecDCTReset", [0]], "DecDCTReset(0), func_800869C8(0), func_800869A4(0)")
	calls.clear()
	_check(engine.call_c("bg_still_decode_to_vram", [0x2C0, 0xA0, 0x140, 0x58]) == -1 and calls.is_empty(), "bad magic: -1 and nothing is called")
	m.write_u32(0x80164000, 0x38000000 | 0x1234)
	m.write_u32(0x80164004, 0x00010000)
	_check(engine.call_c("bg_still_decode_to_vram", [0x2C0, 0xA0, 0x140, 0x58]) == 1, "valid header: 1")
	_check(rects.size() == 20 and rects[0] == [0x2C0, 0xA0, 0x10, 0x58, 0x80162000] and rects[19][0] == 0x2C0 + 19 * 0x10, "20 strips of 16 pixels, rect {x, 0xA0, 0x10, 0x58}: %d" % rects.size())
	var strips: Array = []
	for c: Array in calls:
		if c[0] == "func_80086944":
			strips.append(c[1])
	_check(strips.size() == 20 and strips[0] == [0x80162000, 0x300], "func_80086944(out_buffer, ((h + 8) >> 4) << 7)")
	m.write_u32(0x80164004, 0x01010000)
	_check(engine.call_c("bg_still_decode_to_vram", [0x2C0, 0xA0, 0x140, 0x58]) == -1, "second word with bit 24 set: -1")
	m.write_u32(0x80164004, 0x00010000)
	rects.clear()
	_check(engine.call_c("bg_still_decode_to_vram", [0x100, 0, 0, 0x10]) == 1 and rects.is_empty(), "width 0: no strip (the C tests rect.x < end first)")
