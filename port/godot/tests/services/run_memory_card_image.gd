extends SceneTree
## Memory-card image codec against a card the original wrote (emulator golden
## .asset-cache/emu/memcard-golden-v1: album 1 and the system file from the
## NAME_ENT slot-create chain) and against the original's RAM at the first
## command select of newgame-golden-v1 set A, which ran with that card.
## TOKIMEMO_MEMCARD_GOLDEN: directory holding memcard1.mcd.
## TOKIMEMO_NEWGAME_RAM: a7_command_select_ready.ram.bin of set A.
## Without them only the source-rule checks run, and the suite says so.
const Card = preload("res://services/memory_card_image.gd")

const GOLDEN_CARD_SHA256: String = "12ff9e4298149529737dbf4ab15ae5cd9411c352af7599655f95a2f7325869e3"
const GAME_STATE_RAM: int = 0xE6280
const SYSTEM_DATA_RAM: int = 0xE7D10

var failures: Array[String] = []
var checks: int = 0


func _initialize() -> void:
	_source_rules()
	var card_dir: String = OS.get_environment("TOKIMEMO_MEMCARD_GOLDEN")
	if card_dir.is_empty():
		print("SKIP golden card checks: TOKIMEMO_MEMCARD_GOLDEN is not set")
	else:
		_golden(card_dir.path_join("memcard1.mcd"), OS.get_environment("TOKIMEMO_NEWGAME_RAM"))
	var save_dir: String = OS.get_environment("TOKIMEMO_SAVE_GOLDEN")
	if save_dir.is_empty():
		print("SKIP notebook save/load goldens: TOKIMEMO_SAVE_GOLDEN is not set")
	else:
		_notebook_goldens(save_dir)
	if failures.is_empty():
		print("PASS: memory card image %d checks" % checks)
		quit(0)
	else:
		for message: String in failures:
			push_error(message)
		print("FAIL: memory card image %d of %d checks" % [failures.size(), checks])
		quit(1)


func _golden(card_path: String, ram_path: String) -> void:
	var raw: PackedByteArray = FileAccess.get_file_as_bytes(card_path)
	_check(_sha256(raw) == GOLDEN_CARD_SHA256, "golden card is the frozen original-written card")
	var listing: Dictionary = Card.raw_card_files(raw)
	_check(listing.error.is_empty(), "raw card directory parses")
	if not listing.error.is_empty():
		return
	var files: Dictionary = listing.files
	_check(files.has(Card.file_name(0)) and files.has(Card.file_name(1)) and files.size() == 2,
		"card holds exactly the system file and album 1 under the source names")
	var album: PackedByteArray = files.get(Card.file_name(1), PackedByteArray())
	var system_file: PackedByteArray = files.get(Card.file_name(0), PackedByteArray())
	_check(Card.file_header_error(album, 1).is_empty(), "album header and title match the source writer")
	_check(Card.file_header_error(system_file, 0).is_empty(), "system file header and title match the source writer")
	_check(not Card.file_header_error(album, 2).is_empty(), "album 1 is not accepted as file 2")
	for block: PackedByteArray in [album, system_file]:
		_check(Card.bank_validate(block) == 0, "bank 0 of an original file validates")
		_check(block.decode_u32(0x200) == Card.bank_checksum(block, 0x200), "stored checksum is the source byte sum")

	# Album: decode into an empty GameState, re-encode, require byte identity.
	var state := PackedByteArray()
	state.resize(0x1A90)
	var loaded: Dictionary = Card.game_bank_load(album, state)
	_check(loaded.error.is_empty() and loaded.bank_index == 0, "album bank loads from bank 0")
	_check(state[0x3E] == 0x5F and state[0x3F] == 4 and state[0x40] == 4 and state[0x3D] == 0,
		"album date is 1995-04-04, vacation 0")
	_check(state[0xF5E] == 0 and state[0xF5F] == 0xFF and state[0x75D] == 0,
		"event_log_scan recomputes count 0, current_girl 0xFF and date_girl 0 for the empty log")
	var icons: Dictionary = _icons_from(album, 0)
	var rebuilt: Dictionary = Card.file_build(1, state, icons, {"k_speed": loaded.k_speed, "cursor_style": loaded.cursor_style})
	_check(rebuilt.error.is_empty() and rebuilt.block == album, "album re-encodes byte for byte, header, icon 0 and both banks")
	_check(rebuilt.get("icon", -1) == 0, "empty event log selects icon 0")

	# System file: decode into empty system data, re-encode with the loaded layout.
	var system := PackedByteArray()
	system.resize(Card.SYSTEM_DATA_SIZE)
	var sys_loaded: Dictionary = Card.system_bank_load(system_file, system, state)
	_check(sys_loaded.error.is_empty() and sys_loaded.bank_index == 0, "system bank loads from bank 0")
	_check(system[0x26] == 1, "selected_slot 1 restored")
	_check(system[0x04] == 0 and system[0x05] == 0, "slot_status is not read back from the system bank")

	if ram_path.is_empty():
		print("SKIP system re-encode and RAM comparison: TOKIMEMO_NEWGAME_RAM is not set")
		return
	var ram: PackedByteArray = FileAccess.get_file_as_bytes(ram_path)
	_check(ram.size() == 0x200000, "newgame set A RAM dump is 2 MiB")
	if ram.size() != 0x200000:
		return
	var ram_system: PackedByteArray = ram.slice(SYSTEM_DATA_RAM, SYSTEM_DATA_RAM + Card.SYSTEM_DATA_SIZE)
	var ram_state: PackedByteArray = ram.slice(GAME_STATE_RAM, GAME_STATE_RAM + 0x1A90)
	# slot_status before the write: [1..14] are what memory_card_scan_poll finds
	# on this card; [0] is never written by the scan, so it is taken from the
	# original's RAM (the build marks only selected_slot 1).
	system[0x04] = ram_system[0x04]
	for i in range(1, 15):
		system[0x04 + i] = 1 if files.has(Card.file_name(i)) else 0
	var sys_rebuilt: Dictionary = Card.file_build(0, state, icons,
		{"system": system, "options_dirty": 0, "live_layout": sys_loaded.layout})
	_check(sys_rebuilt.error.is_empty() and sys_rebuilt.block == system_file,
		"system file re-encodes byte for byte (icon bytes come from the card itself; only the icon index is tested)")
	# Every saved system field equals the original's system_data after the write.
	var sys_mismatch: Array = []
	for f: Array in Card.SYSTEM_COPIES:
		if f[0] != 0x004 and system.slice(f[1], f[1] + f[2]) != ram_system.slice(f[1], f[1] + f[2]):
			sys_mismatch.append("0x%x" % f[1])
	if system.slice(Card.SD_LAYOUT, Card.SD_LAYOUT + 0x100) != ram_system.slice(Card.SD_LAYOUT, Card.SD_LAYOUT + 0x100):
		sys_mismatch.append("layout")
	_check(sys_mismatch.is_empty(), "decoded system bank equals the original's system_data: %s" % [sys_mismatch])
	var sys_from_ram: Dictionary = Card.system_bank_build(ram_system.duplicate(), ram_state[0x1124], sys_loaded.layout)
	_check(sys_from_ram.error.is_empty() and sys_from_ram.bank == system_file.slice(0x200, 0x1100),
		"system bank built from the original's RAM equals the card bank")
	# The album was written at slot create; by command select only the fields
	# the prologue writes differ: profile names, girl records, saved_settings and menu_mode.
	var changed: Array = []
	for f: Array in Card.GAME_FIELDS:
		if state.slice(f[1], f[1] + f[2]) != ram_state.slice(f[1], f[1] + f[2]):
			changed.append(f[1])
	_check(changed == [0x0C4, 0x1BC, 0x56C, 0x1092], "album decode differs from command-select RAM only in prologue-written fields: %s" % [changed])
	var flags_diff: Array = []
	for i in 0x100:
		if state[0x56C + i] != ram_state[0x56C + i]:
			flags_diff.append(i)
	_check(flags_diff == [0xFE, 0xFF], "scene_flags differ only in saved_settings[2..3]")


## endings-saves-golden-v1 (README there): S1 is a notebook save in the
## original (icon 13, save, slot 1, overwrite yes) on 1995-05-28; the card the
## original wrote and its RAM after the save. S2 loads that slot back
## (icon 13, load, slot 1, continue yes) to the reloaded command select.
func _notebook_goldens(dir: String) -> void:
	var raw: PackedByteArray = FileAccess.get_file_as_bytes(dir.path_join("s1-probe4/cards/memcard1.mcd"))
	var ram1: PackedByteArray = FileAccess.get_file_as_bytes(dir.path_join("s1-probe4/ram.bin"))
	var ram2: PackedByteArray = FileAccess.get_file_as_bytes(dir.path_join("s2-probe2/ram.bin"))
	_check(ram1.size() == 0x200000 and ram2.size() == 0x200000, "S1 and S2 RAM dumps are present")
	var listing: Dictionary = Card.raw_card_files(raw)
	_check(listing.error.is_empty() and listing.files.size() == 2, "S1 card lists the system file and album 1")
	if ram1.size() != 0x200000 or ram2.size() != 0x200000 or not listing.error.is_empty():
		return
	var album: PackedByteArray = listing.files[Card.file_name(1)]
	var system_file: PackedByteArray = listing.files[Card.file_name(0)]
	# S1 is rebuilt from the original's RAM before the save (base-0528: the
	# command select the S1 run starts from), so the builder's own writes
	# (saved_settings refresh, slot_status marks, slot summary, layout copy)
	# are tested, not read back from post-save RAM.
	var ram0: PackedByteArray = FileAccess.get_file_as_bytes(dir.path_join("base-0528/ram.bin"))
	var in_card: Dictionary = Card.raw_card_files(FileAccess.get_file_as_bytes(dir.path_join("inputs/memcard1.mcd")))
	_check(ram0.size() == 0x200000 and in_card.error.is_empty(), "pre-save RAM (base-0528) and the S1 input card are present")
	if ram0.size() != 0x200000 or not in_card.error.is_empty():
		return
	var state0: PackedByteArray = ram0.slice(GAME_STATE_RAM, GAME_STATE_RAM + 0x1A90)
	var system0: PackedByteArray = ram0.slice(SYSTEM_DATA_RAM, SYSTEM_DATA_RAM + Card.SYSTEM_DATA_SIZE)
	var state1: PackedByteArray = ram1.slice(GAME_STATE_RAM, GAME_STATE_RAM + 0x1A90)
	var system1: PackedByteArray = ram1.slice(SYSTEM_DATA_RAM, SYSTEM_DATA_RAM + Card.SYSTEM_DATA_SIZE)
	# Live globals the builders read (config/symbol_addrs*.txt): text_state.speed
	# (get_k_speed), cursor_style, D_80125C10 (icon positions), mascot_anchor_x,
	# hud_mascot_choice (hypothesis address), mascot_length, mascot_swing.
	var k_speed: int = ram0[0xB3F66]
	var cursor_style: int = ram0[0xB6708]
	var live: Dictionary = {"icon_positions": ram0.slice(0x125C10, 0x125C50), "mascot_anchor": ram0.slice(0x125C50, 0x125C54),
		"mascot_choice": ram0[0xB5C08], "mascot_length": ram0[0x125C54], "mascot_swing": ram0[0x125C55]}
	_check(state0.slice(0x668, 0x66C) != state1.slice(0x668, 0x66C)
		and system0.slice(0x04, 0x14) != system1.slice(0x04, 0x14)
		and system0.slice(Card.SD_LAYOUT, Card.SD_LAYOUT + 0x100) != system1.slice(Card.SD_LAYOUT, Card.SD_LAYOUT + 0x100)
		and system0.slice(0x858, 0xED8) != system1.slice(0x858, 0xED8),
		"pre-save RAM differs from post-save RAM in saved_settings, slot_status, layout and slot summaries")
	# The icon comes from the event-log scan, not current_girl: this log holds
	# two scene-8 entries only, so no date-like entry and icon 0 (current_girl is 8).
	var scan: Dictionary = Card.event_log_scan(state0.duplicate())
	_check(scan.count == 2 and scan.last_qualifying_girl == 0xFF and state0[0xF5F] == 8, "S1 event log has no date-like entry while current_girl is 8")
	# Icon palette and image bytes are copied from the card block: the port has
	# no executable icon producer yet, so only the icon index is verified.
	var icons: Dictionary = _icons_from(album, 0)
	var work: PackedByteArray = state0.duplicate()
	var built: Dictionary = Card.file_build(1, work, icons, {"k_speed": k_speed, "cursor_style": cursor_style})
	_check(built.error.is_empty() and built.icon == 0 and built.block == album,
		"S1: album built from pre-save RAM and the live text speed and cursor style equals the album the original wrote (icon index only)")
	_check(work.slice(0x668, 0x66C) == state1.slice(0x668, 0x66C), "S1: the build's saved_settings refresh equals the original's")
	# Data screen entry: memory_card_state_reset, then memory_card_scan_poll of
	# the input card. The write marks slot_status[save_slot]; step 4 stores
	# selected_slot = save_slot and the slot summary before the system write.
	var system: PackedByteArray = system0.duplicate()
	Card.slot_status_reset(system)
	for i in range(1, 15):
		system[0x04 + i] = 1 if in_card.files.has(Card.file_name(i)) else 0
	system[0x04 + 1] = 1
	system[Card.SD_SELECTED_SLOT] = 1
	_check(Card.slot_summary_store(system, work).is_empty(), "S1: slot summary stored")
	var sys_built: Dictionary = Card.file_build(0, work, icons, {"system": system, "options_dirty": state0[0x1124], "live_layout": live})
	_check(sys_built.error.is_empty() and sys_built.block == system_file,
		"S1: system file built from pre-save RAM and the live layout equals the system file the original wrote (icon index only)")
	# Saved system fields only: +0x00..0x03 and +0xED8.. are runtime words the
	# data screen changes outside the card code.
	var sys_ranges: Array = [[0x24, 3], [Card.SD_LAYOUT, 0x100]]
	for f: Array in Card.SYSTEM_COPIES:
		sys_ranges.append([f[1], f[2]])
	var sys_diff: Array = []
	for r: Array in sys_ranges:
		if system.slice(r[0], r[0] + r[1]) != system1.slice(r[0], r[0] + r[1]):
			sys_diff.append("0x%x" % r[0])
	_check(sys_diff.is_empty(), "S1: saved system_data fields after reset, scan, summary and build equal the original's post-save RAM: %s" % [sys_diff])
	# S2: the original's load of that album. The S1 RAM already equals S2 on
	# every saved field, so decoding over it would test nothing. The album is
	# decoded instead over (a) the S2 GameState with every byte inverted, which
	# differs from S2 in every byte of every saved field and scan output, and
	# (b) the original's set A GameState at 1995-04-04 when it is available.
	var state2: PackedByteArray = ram2.slice(GAME_STATE_RAM, GAME_STATE_RAM + 0x1A90)
	var inverted: PackedByteArray = state2.duplicate()
	for i in inverted.size():
		inverted[i] ^= 0xFF
	_notebook_load(album, inverted, state2, true, "inverted S2 base")
	var set_a_path: String = OS.get_environment("TOKIMEMO_NEWGAME_RAM")
	if set_a_path.is_empty():
		print("SKIP S2 load over set A RAM: TOKIMEMO_NEWGAME_RAM is not set")
	else:
		var set_a: PackedByteArray = FileAccess.get_file_as_bytes(set_a_path)
		_check(set_a.size() == 0x200000, "set A RAM dump is 2 MiB")
		if set_a.size() == 0x200000:
			_notebook_load(album, set_a.slice(GAME_STATE_RAM, GAME_STATE_RAM + 0x1A90), state2, false, "set A 1995-04-04 base")


## Decodes the S1 album over base and compares every saved field and the scan
## outputs with the original's S2 GameState. all_differ: the base must differ
## from S2 in every byte of those ranges before the load (otherwise the
## ranges it does differ on are listed, and at least one must).
func _notebook_load(album: PackedByteArray, base: PackedByteArray, state2: PackedByteArray, all_differ: bool, label: String) -> void:
	var ranges: Array = []
	for f: Array in Card.GAME_FIELDS:
		ranges.append([f[1], f[2]])
	for off: int in [0xF5E, 0xF5F, 0x75D, 0x1117]:
		ranges.append([off, 1])
	var same_bytes: int = 0
	var differing: int = 0
	for r: Array in ranges:
		var range_differs: bool = false
		for i in r[1]:
			if base[r[0] + i] == state2[r[0] + i]:
				same_bytes += 1
			else:
				range_differs = true
		if range_differs:
			differing += 1
	if all_differ:
		_check(same_bytes == 0, "S2 (%s): the base differs from S2 in every saved byte before the load" % label)
	else:
		_check(differing > 0, "S2 (%s): the base differs from S2 in %d of %d saved ranges before the load" % [label, differing, ranges.size()])
	var target: PackedByteArray = base.duplicate()
	var loaded: Dictionary = Card.game_bank_load(album, target)
	_check(loaded.error.is_empty() and loaded.k_speed == state2[0x66B] and loaded.cursor_style == state2[0x66A],
		"S2 (%s): load succeeds and returns the saved text speed and cursor style" % label)
	var mismatch: Array = []
	for r: Array in ranges:
		if target.slice(r[0], r[0] + r[1]) != state2.slice(r[0], r[0] + r[1]):
			mismatch.append("0x%x" % r[0])
	_check(mismatch.is_empty(), "S2 (%s): decoded album equals the original's GameState after its load on every saved field and scan output: %s" % [label, mismatch])


func _source_rules() -> void:
	_check(Card.slot_label(1) == PackedByteArray([0x82, 0x50]) and Card.slot_label(10) == PackedByteArray([0x82, 0x50, 0x82, 0x4F])
		and Card.slot_label(14) == PackedByteArray([0x82, 0x50, 0x82, 0x53]), "slot labels are the data table's full-width numerals")
	_check(Card.file_name(0) == "BISLPS-000650" and Card.file_name(14) == "BISLPS-00065E", "file names end in one hex digit")
	var state := PackedByteArray()
	state.resize(0x1A90)
	# Event log: three entries, a gap, a stray year after it.
	_entry(state, 0, 3, 5, 96)
	_entry(state, 1, 4, 0xF, 96)
	_entry(state, 2, 6, 7, 96)
	_entry(state, 4, 9, 1, 97)
	var scan: Dictionary = Card.event_log_scan(state)
	_check(scan.count == 3 and scan.last_qualifying_girl == 4 and scan.final_event_girl == 6, "scan counts to the first year 0 and picks scene 0/1/2/0xF")
	_check(state[0x75E + 4 * 8 + 4] == 0, "scan zeroes the year of entries after the count")
	var full := PackedByteArray()
	full.resize(0x1A90)
	for i in 0x100:
		_entry(full, i, 1, 5, 96)
	_check(Card.event_log_scan(full).count == 0xFF, "scan count is capped at 0xFF")

	# Game bank: build, validate, fallbacks and rejections.
	state[0x3E] = 96
	state[0x3F] = 7
	state[0x40] = 20
	state[0x1092] = 3
	state[0xF64] = 0xA5
	var built: Dictionary = Card.game_bank_build(state, 2, 0x40)
	_check(built.error.is_empty() and state[0x66B] == 2 and state[0x66A] == 0x40, "build stores k_speed and cursor_style into saved_settings first")
	_check(built.bank[0xE1E] == 3 and built.bank[0xE2C] == 0xA5 and built.bank[0x4E8 + 0xFF] == 2, "menu_mode, quarter_flags and saved_settings reach the image")
	var block: PackedByteArray = _block(built.bank)
	_check(Card.bank_validate(block) == 0, "fresh block validates bank 0")
	var damaged: PackedByteArray = block.duplicate()
	damaged[0x200 + 0x10] ^= 0xFF
	_check(Card.bank_validate(damaged) == 1, "corrupt bank 0 falls back to bank 1")
	var target := PackedByteArray()
	target.resize(0x1A90)
	var res: Dictionary = Card.game_bank_load(damaged, target)
	_check(res.error.is_empty() and res.bank_index == 1 and target[0x1117] == 1 and target[0x3F] == 7, "load reads bank 1 and records it in album_slot")
	_check(res.k_speed == 2 and res.cursor_style == 0x40, "load returns the saved text speed and cursor style")
	damaged[0x1100 + 0x10] ^= 0xFF
	_check(Card.bank_validate(damaged) == -1, "both banks corrupt is invalid")
	var zero := PackedByteArray()
	zero.resize(Card.BANK_SIZE)
	_check(Card.bank_validate(_block(zero)) == -1, "an all-zero bank with sum 0 is invalid")
	for field: int in [5, 6]:
		var dated: PackedByteArray = built.bank.duplicate()
		dated[field] = 0
		Card._store_checksum(dated)
		var fresh := PackedByteArray()
		fresh.resize(0x1A90)
		var r: Dictionary = Card.game_bank_load(_block(dated), fresh)
		var rest_untouched: bool = true
		for i in fresh.size():
			if i != 0x1117 and fresh[i] != 0:
				rest_untouched = false
		_check(r.result == -1 and rest_untouched, "a bank with %s 0 is rejected before any field is copied" % ("year" if field == 5 else "month"))
	_check(not Card.game_bank_build(PackedByteArray([1, 2]), 0, 0).error.is_empty(), "short state refused")

	# System bank rules.
	var system := PackedByteArray()
	system.resize(Card.SYSTEM_DATA_SIZE)
	system[0x26] = 16 + 3
	_check(not Card.system_bank_build(system, 0, {}).error.is_empty(), "selected_slot past slot_status is refused")
	system[0x26] = 3
	_check(not Card.system_bank_build(system, 0, {}).error.is_empty(), "layout is required when options are clean and a slot is selected")
	var dirty: Dictionary = Card.system_bank_build(system, 1, {})
	_check(dirty.error.is_empty() and system[0x04 + 3] == 1 and dirty.bank[0x26] == 3, "build marks the selected slot and stores it modulo 15")
	var layout_zero: bool = true
	for i in 0x100:
		if dirty.bank[0xDD8 + i] != 0:
			layout_zero = false
	system[Card.SD_LAYOUT] = 0x77
	var dirty2: Dictionary = Card.system_bank_build(system, 1, {})
	_check(layout_zero and dirty2.bank[0xDD8] == 0, "options_dirty leaves the bank layout zero")
	var icons := PackedByteArray()
	icons.resize(0x40)
	icons.fill(9)
	var clean: Dictionary = Card.system_bank_build(system, 0, {"icon_positions": icons, "mascot_anchor": PackedByteArray([1, 2, 3, 4]),
		"mascot_choice": 5, "mascot_length": 6, "mascot_swing": 7})
	_check(clean.error.is_empty() and clean.bank[0xDD8] == 9 and clean.bank[0xDD8 + 0xF6] == 7 and system[Card.SD_LAYOUT + 0xF4] == 5,
		"clean options copy the live layout into system data and the bank")
	system[0x26] = 0
	_check(Card.system_bank_build(system, 0, {}).bank[0x26] == 0, "slot 0 leaves the image's selected_slot zero")
	# Load merge rules.
	clean.bank[0x24] = 0x02
	clean.bank[0x25] = 0x10
	clean.bank[0x028] = 0xAA
	clean.bank[0x03C + 2] = 0xBB
	Card._store_checksum(clean.bank)
	var live := PackedByteArray()
	live.resize(Card.SYSTEM_DATA_SIZE)
	live[0x24] = 0x01
	live[0x25] = 0x20
	live[0x3F] = 1 # config0 protected
	live[0x40] = 1 # config1 saved this session
	var gs := PackedByteArray()
	gs.resize(0x1A90)
	var merged: Dictionary = Card.system_bank_load(_block(clean.bank), live, gs)
	_check(merged.error.is_empty() and live[0x24] == 0x03 and live[0x25] == 0x10, "karaoke byte 0 is OR-ed and byte 1 replaced")
	_check(live[0x2C] == 0 and live[0x42] == 0, "protected config0 and saved config1 are kept")
	live[0x3F] = 0
	live[0x40] = 0
	Card.system_bank_load(_block(clean.bank), live, gs)
	_check(live[0x2C] == 0xAA and live[0x42] == 0xBB, "unprotected config records are replaced")

	# File build refusals.
	var none := PackedByteArray()
	none.resize(0x1A90)
	_check(not Card.file_build(15, none, {}, {}).error.is_empty(), "file 15 refused")
	_check(not Card.file_build(1, none, {"palettes": PackedByteArray(), "images": PackedByteArray()}, {"k_speed": 0, "cursor_style": 0}).error.is_empty(),
		"missing source icons refused")
	_check(not Card.raw_card_files(PackedByteArray([0x4D, 0x43])).error.is_empty(), "short raw card refused")


func _icons_from(block: PackedByteArray, index: int) -> Dictionary:
	var palettes := PackedByteArray()
	palettes.resize(Card.ICON_COUNT * Card.ICON_PALETTE_SIZE)
	palettes.fill(0xEE)
	var images := PackedByteArray()
	images.resize(Card.ICON_COUNT * Card.ICON_IMAGE_SIZE)
	images.fill(0xEE)
	for i in Card.ICON_PALETTE_SIZE:
		palettes[index * Card.ICON_PALETTE_SIZE + i] = block[0x60 + i]
	for i in Card.ICON_IMAGE_SIZE:
		images[index * Card.ICON_IMAGE_SIZE + i] = block[0x80 + i]
	return {"palettes": palettes, "images": images}


func _block(bank: PackedByteArray) -> PackedByteArray:
	var block := PackedByteArray()
	block.resize(Card.BLOCK_SIZE)
	for b in 2:
		for i in Card.BANK_SIZE:
			block[Card.BANK_OFFSETS[b] + i] = bank[i]
	return block


func _entry(state: PackedByteArray, i: int, girl: int, scene: int, year: int) -> void:
	var e: int = 0x75E + i * 8
	state[e] = girl
	state[e + 2] = scene
	state[e + 4] = year
	state[e + 5] = 4


func _sha256(bytes: PackedByteArray) -> String:
	var ctx := HashingContext.new()
	ctx.start(HashingContext.HASH_SHA256)
	ctx.update(bytes)
	return ctx.finish().hex_encode()


func _check(ok: bool, message: String) -> void:
	checks += 1
	if not ok:
		failures.append(message)
