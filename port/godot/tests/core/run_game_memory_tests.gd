extends RefCounted
## T-39300 (M1): core/game_memory.gd with synthetic bytes only. The layout checks
## against the C headers live in tools/port/test_gen_game_memory.py; these tests cover
## the model's access semantics, the PS-X EXE loader (a synthetic image) and the views.

const Memory = preload("res://core/game_memory.gd")
const Layout = preload("res://core/game_layout.gd")

var failures: Array[String] = []

func run() -> Array[String]:
	failures.clear()
	_test_scalars()
	_test_aliases_and_faults()
	_test_ranges()
	_test_executable_loader()
	_test_game_state_views()
	_test_nested_and_bit_fields()
	_test_globals()
	_test_snapshot()
	return failures

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func _test_scalars() -> void:
	var mem := Memory.new()
	_check(mem.ram.size() == 0x200000 and mem.u32(0x80000000) == 0, "RAM starts zeroed at 2 MiB")
	mem.write_u32(0x80001000, 0x8899aabb)
	_check(mem.u8(0x80001000) == 0xbb and mem.u8(0x80001003) == 0x88, "little-endian bytes")
	_check(mem.u16(0x80001000) == 0xaabb and mem.s16(0x80001002) == -30567, "halfword views")
	_check(mem.s8(0x80001000) == -69 and mem.s8(0x80001001) == -86, "signed byte views")
	_check(mem.u32(0x80001000) == 0x8899aabb and mem.s32(0x80001000) == -2003195205, "word views")
	mem.write_s16(0x80001004, -1)
	_check(mem.u16(0x80001004) == 0xffff and mem.u8(0x80001006) == 0, "sh keeps the low 16 bits only")
	mem.write_s32(0x80001008, 0x1_0000_0001)
	_check(mem.u32(0x80001008) == 1, "sw keeps the low 32 bits only")
	mem.write_u8(0x8000100c, 0x1ff)
	_check(mem.u8(0x8000100c) == 0xff and mem.error.is_empty(), "sb keeps the low byte")
	_check(mem.read_int(0x80001000, 2, true) == -21829 and mem.read_int(0x80001000, 1, false) == 0xbb, "width-generic read")

func _test_aliases_and_faults() -> void:
	var mem := Memory.new()
	mem.write_u32(0x80002000, 0x12345678)
	_check(mem.u32(0xa0002000) == 0x12345678 and mem.u32(0x2000) == 0x12345678, "KSEG1 and KUSEG alias KSEG0")
	_check(mem.error.is_empty(), "aliases are not faults")
	_check(mem.u32(0x80200000) == 0 and mem.error.contains("outside main RAM"), "read past RAM latches a fault")
	var first: String = mem.error
	mem.write_u32(0x801ffffe, 5)
	_check(mem.error == first, "the first fault is kept")
	mem.clear_error()
	_check(mem.u16(0x80002001) == 0 and mem.error.contains("aligned"), "misaligned halfword faults")
	mem.clear_error()
	mem.write_u32(0x80002002, 0xffffffff)
	_check(mem.error.contains("aligned") and mem.u32(0x80002000) == 0x12345678, "misaligned store writes nothing")
	mem.clear_error()
	_check(mem.u8(0xc0000000) == 0 and not mem.error.is_empty(), "KSEG2 is refused")
	mem.clear_error()
	_check(mem.u8(-1) == 0 and not mem.error.is_empty(), "negative address is refused")

func _test_ranges() -> void:
	var mem := Memory.new()
	mem.write_bytes(0x80003001, PackedByteArray([1, 2, 3]))
	_check(mem.read_bytes(0x80003000, 8) == PackedByteArray([0, 1, 2, 3, 0, 0, 0, 0]), "byte range round trip")
	mem.fill(0x80003000, 4, 0x1aa)
	_check(mem.u32(0x80003000) == 0xaaaaaaaa, "fill keeps the low byte")
	mem.clear_error()
	mem.write_bytes(0x801ffffe, PackedByteArray([1, 2, 3]))
	_check(not mem.error.is_empty() and mem.u8(0x801ffffe) == 0, "a range past RAM is refused whole")

func _exe(text: PackedByteArray, t_addr: int) -> PackedByteArray:
	var image := PackedByteArray()
	image.resize(Memory.EXE_HEADER_SIZE + text.size())
	image.fill(0)
	for i: int in 8:
		image[i] = "PS-X EXE".unicode_at(i)
	image.encode_u32(0x10, t_addr + 8)
	image.encode_u32(0x14, 0x800eb670)
	image.encode_u32(0x18, t_addr)
	image.encode_u32(0x1c, text.size())
	for i: int in text.size():
		image[Memory.EXE_HEADER_SIZE + i] = text[i]
	return image

func _test_executable_loader() -> void:
	var mem := Memory.new()
	var text := PackedByteArray([0xde, 0xad, 0xbe, 0xef, 1, 2, 3, 4])
	_check(mem.load_executable(_exe(text, 0x80041000)).is_empty(), "synthetic image loads")
	_check(mem.u32(0x80041000) == 0xefbeadde and mem.u32(0x80041004) == 0x04030201, "text lands at t_addr")
	_check(mem.executable.get("pc") == 0x80041008 and mem.executable.get("text_size") == 8, "entry facts")
	_check(mem.u32(0x80041008) == 0 and mem.u32(0x80040ffc) == 0, "bytes around the image stay zero (bss)")
	_check(not mem.load_executable(PackedByteArray([1, 2, 3])).is_empty(), "short image refused")
	var bad := _exe(text, 0x80041000)
	bad[0] = 0x58
	_check(not mem.load_executable(bad).is_empty(), "bad magic refused")
	_check(not mem.load_executable(_exe(text, 0x801ffffc)).is_empty(), "text past RAM refused")
	var short := _exe(text, 0x80041000)
	short.resize(short.size() - 4)
	_check(not mem.load_executable(short).is_empty(), "truncated text refused")
	_check(not mem.load_executable_file("/nonexistent/executable.bin").is_empty(), "missing file refused")

func _test_game_state_views() -> void:
	var mem := Memory.new()
	var state: Memory.View = mem.game_state()
	_check(state.base == 0x800e6280 and Layout.SIZEOF_GAME_STATE == 0x1a90, "GameState base and size")
	state.set_field("year", 0x5f)
	state.set_field("month", 4)
	state.set_field("day_count", 0x1234)
	_check(mem.u8(0x800e6280 + 0x3e) == 0x5f and mem.u8(0x800e6280 + 0x3f) == 4, "scalar fields at their offsets")
	_check(mem.u16(0x800e6280 + 0x42) == 0x1234 and state.get_field("day_count") == 0x1234, "u16 field")
	state.set_field("mouse_x", -5)
	_check(state.get_field("mouse_x") == -5 and mem.u16(0x800e6280 + 0xf7a) == 0xfffb, "s16 field sign")
	state.set_field("click_timer", -3)
	_check(state.get_field("click_timer") == -3 and mem.u8(0x800e6280 + 0xf71) == 0xfd, "s8 field sign")
	state.set_field("pad_held", -2)
	_check(state.get_field("pad_held") == -2 and state.get_field("pad_prev") == 0, "s32 field")
	state.set_field("event_log_count", 7)
	state.set_field("current_girl", 3)
	_check(mem.u8(0x800e6280 + 0xf5e) == 7 and mem.u8(0x800e6280 + 0xf5f) == 3, "neighbouring bytes stay separate")
	state.set_field("scene_flags", 9, 251)
	_check(mem.u8(0x800e6280 + 0x56c + 251) == 9 and state.count_of("scene_flags") == 0xfc, "byte array element")
	_check(state.get_field("scene_flags", 252) == 0 and not mem.error.is_empty(), "index past the array faults")
	mem.clear_error()
	_check(state.get_field("no_such_field") == 0 and mem.error.contains("no field"), "unknown field faults")
	mem.clear_error()
	state.set_field("player_full_name", 0x81, 15)
	_check(state.bytes_of("player_full_name")[15] == 0x81 and state.bytes_of("player_full_name").size() == 16, "bytes_of")
	_check(Layout.GAME_STATE_GIRLS == 0x1bc and Layout.GAME_STATE_EVENT_LOG_COUNT == 0xf5e, "generated offset constants")

func _test_nested_and_bit_fields() -> void:
	var mem := Memory.new()
	var state: Memory.View = mem.game_state()
	var girl: Memory.View = state.at("girls", 3)
	_check(girl.base == 0x800e6280 + 0x1bc + 3 * 0x38, "girls[3] address")
	girl.set_field("tokimeki", 55)
	girl.set_field("affection", -2)
	_check(mem.s16(girl.base + 2) == 55 and mem.s16(girl.base + 6) == -2, "nested fields")
	girl.set_field("scene_counters", 200, 35)
	_check(mem.u8(girl.base + 0x14 + 35) == 200, "nested byte array")
	var bits: Memory.View = girl.at("flags").at("f")
	bits.set_field("story_stage", 5)
	bits.set_field("met", 1)
	_check(mem.u32(girl.base + 0xc) == (5 << 9) | 2, "bit-field set keeps the other bits clear")
	bits.set_field("b12", 1)
	bits.set_field("story_stage", 2)
	_check(bits.get_field("story_stage") == 2 and bits.get_field("b12") == 1 and bits.get_field("met") == 1, "bit-field read-modify-write")
	_check(girl.at("flags").get_field("w") == (1 << 12) | (2 << 9) | 2, "word view of the flag union")
	bits.set_field("story_stage", 0xf)
	_check(bits.get_field("story_stage") == 7 and bits.get_field("b12") == 1, "bit-field store is masked")
	_check(state.at("girls", 16).base == 0 and not mem.error.is_empty(), "girl index past 16 faults")
	mem.clear_error()
	_check(girl.at("tokimeki").base == 0 and not mem.error.is_empty(), "at() on a scalar faults")

func _test_globals() -> void:
	var mem := Memory.new()
	_check(mem.has_global("schedule_last_picked_girl") and not mem.has_global("nonsense"), "global lookup")
	_check(mem.global_address("schedule_last_picked_girl") == 0x800b6740, "global address from symbol_addrs")
	mem.write_global("schedule_last_picked_girl", -1)
	_check(mem.u8(0x800b6740) == 0xff and mem.read_global("schedule_last_picked_girl") == -1, "s8 global")
	mem.write_global("bg_lba_cache", 70000, 2)
	_check(mem.read_global("bg_lba_cache", 2) == 70000 and mem.s32(mem.global_address("bg_lba_cache") + 8) == 70000, "s32 array global")
	_check(mem.read_global("bg_lba_cache", 4) == 0 and not mem.error.is_empty(), "array global bounds")
	mem.clear_error()
	var gs: Memory.View = mem.global_view("game_state")
	_check(gs.base == 0x800e6280 and gs.struct_name == "GameState", "struct global view")
	_check(mem.read_global("game_state") == 0 and not mem.error.is_empty(), "struct global is not a scalar")

func _test_snapshot() -> void:
	var mem := Memory.new()
	mem.write_u32(0x80004000, 0xcafef00d)
	var saved: PackedByteArray = mem.snapshot()
	mem.write_u32(0x80004000, 1)
	mem.restore(saved)
	_check(mem.u32(0x80004000) == 0xcafef00d, "restore returns the saved bytes")
	mem.restore(PackedByteArray([1]))
	_check(not mem.error.is_empty(), "wrong-size snapshot refused")
