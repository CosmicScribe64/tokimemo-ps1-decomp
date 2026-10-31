extends RefCounted
## Memory-card image codec: the original's card file and its two banks, built and
## read with the source rules (m-flow 6.3-6.6; src/main/memory_card.c,
## include/game/save_image.h). It works on raw GameState and SystemData bytes at
## their source offsets, so a bank built here can be compared byte for byte with
## a card the original wrote, and a decoded bank with the original's RAM.
##
## Pure functions. Every function that the source lets change GameState or
## SystemData changes the byte array it is given, in the source order.

## One card block (0x2000 bytes): header 0x200, bank 0, bank 1.
const BLOCK_SIZE: int = 0x2000
const BANK_SIZE: int = 0xF00
const BANK_OFFSETS: Array[int] = [0x200, 0x1100]
## game_state is 0x1A90 bytes in RAM (0x800E6280..0x800E7D10); the port's raw
## entry image stops at 0x1A8C. The codec touches nothing past 0x1124.
const GAME_STATE_MIN_SIZE: int = 0x1125
const SYSTEM_DATA_SIZE: int = 0xEE0

## GameState offsets read or written outside the bank copies.
const GS_SAVED_SETTINGS: int = 0x668 # scene_flags[0xFC..0xFF]
const GS_DATE_GIRL: int = 0x75D
const GS_EVENT_LOG: int = 0x75E
const GS_EVENT_LOG_COUNT: int = 0xF5E
const GS_CURRENT_GIRL: int = 0xF5F
const GS_ALBUM_SLOT: int = 0x1117
const GS_OPTIONS_DIRTY: int = 0x1124

## memory_card_game_bank_build/_load, in source order:
## [image offset, GameState offset, size]. scene_flags (0x4E8) is copied after
## saved_settings are refreshed (build) and before they are applied (load).
const GAME_FIELDS: Array = [
	[0x004, 0x03D, 1], [0x005, 0x03E, 1], [0x006, 0x03F, 1], [0x007, 0x040, 1], [0x008, 0x041, 1],
	[0x00A, 0x042, 2], [0x00C, 0x044, 0x80], [0x08C, 0x0C4, 0x7C], [0x108, 0x1BC, 0x380],
	[0x488, 0x53C, 0x10], [0x498, 0x54C, 0x20], [0x4B8, 0x66C, 0x30], [0x4E8, 0x56C, 0x100],
	[0x5E8, 0x728, 4], [0x5EC, 0x72C, 0x10], [0x5FC, 0x73C, 0x10], [0x60C, 0x74C, 0x10],
	[0x61C, 0x75E, 0x800],
	[0xE1C, 0xF6C, 1], [0xE1D, 0xF6D, 1], [0xE1E, 0x1092, 1], [0xE1F, 0xF6E, 1], [0xE20, 0x03C, 1],
	[0xE24, 0xF68, 4], [0xE28, 0xF6F, 1], [0xE29, 0xF70, 1], [0xE2A, 0xF73, 1], [0xE2B, 0xF75, 1],
	[0xE2C, 0xF64, 4],
]

## SystemData offsets (include/game/system_data.h, SystemDataSaveView).
const SD_SLOT_STATUS: int = 0x04
const SD_KARAOKE: int = 0x24
const SD_SELECTED_SLOT: int = 0x26
const SD_CONFIG0: int = 0x2C
const SD_CONFIG0_PROTECT: int = 0x3F # config0.flags.b[3], bit 0
const SD_CONFIG1: int = 0x40 # bits.flag is bit 0 (bit-fields are LSB first)
const SD_LAYOUT: int = 0xDC8

## memory_card_system_bank_build's straight copies: [image, SystemData, size].
## selected_slot (0x26), karaoke (0x24/0x25) and the layout (0xDD8) have rules.
const SYSTEM_COPIES: Array = [
	[0x004, 0x04, 0x10], [0x014, 0x14, 0x10], [0x028, 0x2C, 0x14], [0x03C, 0x40, 0x18],
	[0x054, 0x27, 1], [0x055, 0x28, 1], [0x056, 0x58, 0x800],
	[0x856, 0x858, 0x10], [0x866, 0x868, 0x10], [0x876, 0x878, 0x10],
	[0x886, 0x888, 0x80], [0x906, 0x908, 0x80], [0x986, 0x988, 0x100],
	[0xA86, 0xEC8, 0x10], [0xA98, 0xA88, 0x240], [0xCD8, 0xCC8, 0x100],
]
const IMG_SELECTED_SLOT: int = 0x026
const IMG_LAYOUT: int = 0xDD8

## memory_card_file_write's title: strncpy(title, "ときめきメモリアル", 0x14) in
## Shift-JIS, then strncat of the slot label (6) or "・システム" (0xC).
const TITLE_SJIS: PackedByteArray = [0x82, 0xC6, 0x82, 0xAB, 0x82, 0xDF, 0x82, 0xAB, 0x83, 0x81, 0x83, 0x82, 0x83, 0x8A, 0x83, 0x41, 0x83, 0x8B]
const SYSTEM_LABEL_SJIS: PackedByteArray = [0x81, 0x45, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80]
const ICON_COUNT: int = 13
const ICON_PALETTE_SIZE: int = 0x20
const ICON_IMAGE_SIZE: int = 0x180


## Byte sum of the 0xEFC bytes after the checksum word, as the u32 the bank stores.
static func bank_checksum(block: PackedByteArray, bank_offset: int) -> int:
	var sum: int = 0
	for i in range(bank_offset + 4, bank_offset + BANK_SIZE):
		sum += block[i]
	return sum & 0xFFFFFFFF


## memory_card_bank_validate: bank 0 then bank 1; a bank is valid when its sum
## matches the stored word and one of its data bytes is not zero. Returns the
## bank index, or -1.
static func bank_validate(block: PackedByteArray) -> int:
	if block.size() != BLOCK_SIZE:
		return -1
	for b in 2:
		var base: int = BANK_OFFSETS[b]
		var sum: int = 0
		var nonzero: int = 0
		for i in range(base + 4, base + BANK_SIZE):
			sum += block[i]
			if block[i] != 0:
				nonzero += 1
		if (sum & 0xFFFFFFFF) == block.decode_u32(base) and nonzero != 0:
			return b
	return -1


## hud_event_log_scan (src/main/hud_screens.c): counts entries up to the first
## with year 0, zeroes the year of every later entry, and reports the girl of the
## latest scene 0/1/2/0xF entry (0xFF when none) and of the latest entry.
static func event_log_scan(state: PackedByteArray) -> Dictionary:
	var last_qualifying: int = 0xFF
	var final_girl: int = 0
	var count: int = 0
	for i in 0x100:
		var e: int = GS_EVENT_LOG + count * 8
		if state[e + 4] == 0:
			break
		var girl: int = state[e]
		if state[e + 2] in [0, 1, 2, 0xF]:
			last_qualifying = girl
		count += 1
		final_girl = girl
	for i in range(count, 0x100):
		state[GS_EVENT_LOG + i * 8 + 4] = 0
	return {"count": mini(count, 0xFF), "last_qualifying_girl": last_qualifying, "final_event_girl": final_girl}


## memory_card_game_bank_build into a cleared bank. Stores k_speed and
## cursor_style into saved_settings[3]/[2] of the state first, as the source does.
static func game_bank_build(state: PackedByteArray, k_speed: int, cursor_style: int) -> Dictionary:
	if state.size() < GAME_STATE_MIN_SIZE:
		return {"error": "game state image is too short"}
	if k_speed < 0 or k_speed > 0xFF or cursor_style < 0 or cursor_style > 0xFF:
		return {"error": "k_speed and cursor_style are bytes"}
	state[GS_SAVED_SETTINGS + 3] = k_speed
	state[GS_SAVED_SETTINGS + 2] = cursor_style
	var bank := PackedByteArray()
	bank.resize(BANK_SIZE)
	for f: Array in GAME_FIELDS:
		for i in f[2]:
			bank[f[0] + i] = state[f[1] + i]
	_store_checksum(bank)
	return {"error": "", "bank": bank}


## memory_card_game_bank_load. Returns k_speed and cursor_style to apply
## (k_speed_set, cursor_style) when the load succeeds. On failure the state is
## unchanged except album_slot, which the validator sets before the date test.
static func game_bank_load(block: PackedByteArray, state: PackedByteArray) -> Dictionary:
	if state.size() < GAME_STATE_MIN_SIZE:
		return {"error": "game state image is too short"}
	var b: int = bank_validate(block)
	if b < 0:
		return {"error": "no valid bank", "result": -1}
	state[GS_ALBUM_SLOT] = b
	var base: int = BANK_OFFSETS[b & 1]
	if block[base + 5] == 0:
		return {"error": "bank year is zero", "result": -1, "bank_index": b}
	if block[base + 6] == 0:
		return {"error": "bank month is zero", "result": -1, "bank_index": b}
	for f: Array in GAME_FIELDS:
		for i in f[2]:
			state[f[1] + i] = block[base + f[0] + i]
	var scan: Dictionary = event_log_scan(state)
	state[GS_EVENT_LOG_COUNT] = scan.count
	state[GS_CURRENT_GIRL] = scan.last_qualifying_girl
	state[GS_DATE_GIRL] = scan.final_event_girl
	return {"error": "", "result": 1, "bank_index": b,
		"k_speed": state[GS_SAVED_SETTINGS + 3], "cursor_style": state[GS_SAVED_SETTINGS + 2]}


## memory_card_system_bank_build into a cleared bank. Marks
## slot_status[selected_slot] = 1 in the system data. The layout block is taken
## from the live layout only while options_dirty is 0 and a slot is selected;
## otherwise the bank's layout stays zero. live_layout: {"icon_positions": 0x40
## bytes, "mascot_anchor": 4 bytes, "mascot_choice", "mascot_length",
## "mascot_swing"}.
static func system_bank_build(system: PackedByteArray, options_dirty: int, live_layout: Dictionary) -> Dictionary:
	if system.size() != SYSTEM_DATA_SIZE:
		return {"error": "system data image must be 0xEE0 bytes"}
	var selected: int = system[SD_SELECTED_SLOT]
	if selected > 0xF:
		return {"error": "selected_slot indexes slot_status[16]"}
	var layout_needed: bool = options_dirty == 0 and selected != 0
	if layout_needed:
		var error: String = _layout_error(live_layout)
		if not error.is_empty():
			return {"error": error}
	system[SD_SLOT_STATUS + selected] = 1
	var bank := PackedByteArray()
	bank.resize(BANK_SIZE)
	bank[0x24] = system[SD_KARAOKE]
	bank[0x25] = system[SD_KARAOKE + 1]
	if selected != 0:
		bank[IMG_SELECTED_SLOT] = selected % 15
	for f: Array in SYSTEM_COPIES:
		for i in f[2]:
			bank[f[0] + i] = system[f[1] + i]
	if layout_needed:
		var icons: PackedByteArray = live_layout.icon_positions
		var anchor: PackedByteArray = live_layout.mascot_anchor
		for i in 0x40:
			system[SD_LAYOUT + i] = icons[i]
		for i in 4:
			system[SD_LAYOUT + 0xF0 + i] = anchor[i]
		system[SD_LAYOUT + 0xF4] = live_layout.mascot_choice
		system[SD_LAYOUT + 0xF5] = live_layout.mascot_length
		system[SD_LAYOUT + 0xF6] = live_layout.mascot_swing
		for i in 0x100:
			bank[IMG_LAYOUT + i] = system[SD_LAYOUT + i]
	_store_checksum(bank)
	return {"error": "", "bank": bank}


## memory_card_state_reset's slot_status part (src/main/memory_card.c:1087):
## slot_status[0..14] are cleared; [15] is kept. memory_card_scan_poll then
## writes only [1..14], so [0] stays 0 until a build marks the selected slot.
static func slot_status_reset(system: PackedByteArray) -> void:
	for i in 15:
		system[SD_SLOT_STATUS + i] = 0


## memory_card_slot_summary_store (src/main/memory_card.c:720): the summary of
## slot selected_slot & 0xF: date, the three name buffers, the nine parameter
## words, girl record 15's first 16 counter bytes and the club.
static func slot_summary_store(system: PackedByteArray, state: PackedByteArray) -> String:
	if system.size() != SYSTEM_DATA_SIZE or state.size() < GAME_STATE_MIN_SIZE:
		return "system data or game state image has the wrong size"
	var slot: int = system[SD_SELECTED_SLOT] & 0xF
	system[0x858 + slot] = state[0x03E]
	system[0x868 + slot] = state[0x03F]
	system[0x878 + slot] = state[0x040]
	_copy(state, 0x0D4, system, 0x888 + slot * 8, 8)
	_copy(state, 0x0DC, system, 0x908 + slot * 8, 8)
	_copy(state, 0x0E4, system, 0x988 + slot * 0x10, 0x10)
	for i in 9:
		_copy(state, 0x0FC + i * 4, system, 0xA88 + (i * 16 + slot) * 4, 4)
	_copy(state, 0x518, system, 0xCC8 + slot * 0x10, 0x10)
	system[0xEC8 + slot] = state.decode_u16(0x0F4) >> 12
	return ""


## memory_card_system_bank_load: merges the valid bank into system data.
## slot_status is not read back (the source leaves it to the scan). karaoke byte
## 0 is OR-ed, byte 1 replaced; config0 is replaced only while its protection
## bit is clear and config1 only while its saved flag is clear. Returns the
## layout values the source pushes into the live icon positions and mascot.
static func system_bank_load(block: PackedByteArray, system: PackedByteArray, state: PackedByteArray) -> Dictionary:
	if system.size() != SYSTEM_DATA_SIZE or state.size() < GAME_STATE_MIN_SIZE:
		return {"error": "system data or game state image has the wrong size"}
	var b: int = bank_validate(block)
	if b < 0:
		return {"error": "no valid bank", "result": -1}
	state[GS_ALBUM_SLOT] = b
	var base: int = BANK_OFFSETS[b]
	_copy(block, base + 0x014, system, 0x14, 0x10)
	system[SD_KARAOKE] |= block[base + 0x24]
	system[SD_KARAOKE + 1] = block[base + 0x25]
	system[SD_SELECTED_SLOT] = block[base + IMG_SELECTED_SLOT]
	if system[SD_CONFIG0_PROTECT] & 1 == 0:
		_copy(block, base + 0x028, system, SD_CONFIG0, 0x14)
	if system[SD_CONFIG1] & 1 == 0:
		_copy(block, base + 0x03C, system, SD_CONFIG1, 0x18)
	for f: Array in SYSTEM_COPIES:
		if f[0] >= 0x054:
			_copy(block, base + f[0], system, f[1], f[2])
	_copy(block, base + IMG_LAYOUT, system, SD_LAYOUT, 0x100)
	return {"error": "", "result": 1, "bank_index": b, "layout": {
		"icon_positions": system.slice(SD_LAYOUT, SD_LAYOUT + 0x40),
		"mascot_anchor": system.slice(SD_LAYOUT + 0xF0, SD_LAYOUT + 0xF4),
		"mascot_choice": system[SD_LAYOUT + 0xF4],
		"mascot_length": system[SD_LAYOUT + 0xF5],
		"mascot_swing": system[SD_LAYOUT + 0xF6]}}


## The slot label of file n (1..14): D_800B328C[n - 1], the full-width decimal
## numeral of n (asm/data/data.data.s, 0x800B322C..0x800B3280).
static func slot_label(file_index: int) -> PackedByteArray:
	var out := PackedByteArray()
	for ch in str(file_index):
		out.append_array([0x82, 0x4F + int(ch)])
	return out


## memory_card_file_write's block, from a cleared block: header, title, icon of
## the latest date-like event-log girl (event_log_scan, icon 0 when none or 13 or
## more), the bank built from the state or system data, and bank 0 copied to
## bank 1. icons: {"palettes": 13 * 0x20 bytes, "images": 13 * 0x180 bytes}, the
## executable's memory_card_icon_palettes/_images. source: for file 0
## {"system", "options_dirty", "live_layout"}, else {"k_speed", "cursor_style"}.
## The scan runs on `state` for both kinds, as in the source.
static func file_build(file_index: int, state: PackedByteArray, icons: Dictionary, source: Dictionary) -> Dictionary:
	if file_index < 0 or file_index > 14:
		return {"error": "file index outside 0..14"}
	if state.size() < GAME_STATE_MIN_SIZE:
		return {"error": "game state image is too short"}
	if not icons.get("palettes") is PackedByteArray or icons.palettes.size() != ICON_COUNT * ICON_PALETTE_SIZE \
			or not icons.get("images") is PackedByteArray or icons.images.size() != ICON_COUNT * ICON_IMAGE_SIZE:
		return {"error": "card icons must be the 13 source palettes and images"}
	var block := PackedByteArray()
	block.resize(BLOCK_SIZE)
	block[0] = 0x53 # 'S'
	block[1] = 0x43 # 'C'
	block[2] = 0x13
	block[3] = 1
	# strncpy(title, ..., 0x14) zero-pads the 18-byte title; strncat then
	# writes the label from its strlen.
	var label: PackedByteArray = slot_label(file_index) if file_index != 0 else SYSTEM_LABEL_SJIS
	for i in TITLE_SJIS.size():
		block[4 + i] = TITLE_SJIS[i]
	for i in label.size():
		block[4 + TITLE_SJIS.size() + i] = label[i]
	var scan: Dictionary = event_log_scan(state)
	var idx: int = scan.last_qualifying_girl
	if idx >= 0xD:
		idx = 0
	_copy(icons.palettes, idx * ICON_PALETTE_SIZE, block, 0x60, ICON_PALETTE_SIZE)
	_copy(icons.images, idx * ICON_IMAGE_SIZE, block, 0x80, ICON_IMAGE_SIZE)
	var built: Dictionary
	if file_index != 0:
		built = game_bank_build(state, int(source.get("k_speed", -1)), int(source.get("cursor_style", -1)))
	else:
		if not source.get("system") is PackedByteArray:
			return {"error": "system file needs the system data image"}
		built = system_bank_build(source.system, int(source.get("options_dirty", -1)), source.get("live_layout", {}))
	if not built.error.is_empty():
		return built
	for b in 2:
		_copy(built.bank, 0, block, BANK_OFFSETS[b], BANK_SIZE)
	return {"error": "", "block": block, "icon": idx}


## Header test for a block read back: magic, icon flag, block count and title.
static func file_header_error(block: PackedByteArray, file_index: int) -> String:
	if block.size() != BLOCK_SIZE:
		return "card file must be 0x2000 bytes"
	if block[0] != 0x53 or block[1] != 0x43 or block[2] != 0x13 or block[3] != 1:
		return "card file header is not the game's"
	# strncat appends the label at the title's strlen; the rest of the 0x40
	# field and the 0x1C reserved bytes are zero.
	var title := TITLE_SJIS.duplicate()
	title.append_array(slot_label(file_index) if file_index != 0 else SYSTEM_LABEL_SJIS)
	title.resize(0x5C)
	for i in title.size():
		if block[4 + i] != title[i]:
			return "card file title does not name file %d" % file_index
	return ""


## Files of a raw 128 KiB PS1 card image (.mcd as PCSX-Redux writes it): the
## directory frames of block 0 name each one-block file. Returns
## {"error", "files": {name: block bytes}}. Only used to import cards the
## original wrote (emulator goldens, a player's own card).
static func raw_card_files(raw: PackedByteArray) -> Dictionary:
	if raw.size() != 0x20000 or raw[0] != 0x4D or raw[1] != 0x43:
		return {"error": "not a raw 128 KiB memory card image"}
	var files: Dictionary = {}
	for i in range(1, 16):
		var frame: int = i * 0x80
		if raw[frame] != 0x51:
			continue
		if raw.decode_u32(frame + 4) != BLOCK_SIZE:
			return {"error": "card file in block %d spans more than one block" % i}
		var name: String = raw.slice(frame + 0x0A, frame + 0x1E).get_string_from_ascii()
		files[name] = raw.slice(i * BLOCK_SIZE, (i + 1) * BLOCK_SIZE)
	return {"error": "", "files": files}


## memory_card_file_name_build's name without the "bu00:" device:
## BISLPS-00065 plus one hex digit of the file number.
static func file_name(file_index: int) -> String:
	return "BISLPS-00065" + "%X" % file_index


static func _store_checksum(bank: PackedByteArray) -> void:
	var sum: int = 0
	for i in range(4, BANK_SIZE):
		sum += bank[i]
	bank.encode_u32(0, sum & 0xFFFFFFFF)


static func _copy(src: PackedByteArray, src_off: int, dst: PackedByteArray, dst_off: int, n: int) -> void:
	for i in n:
		dst[dst_off + i] = src[src_off + i]


static func _layout_error(live: Dictionary) -> String:
	if not live.get("icon_positions") is PackedByteArray or live.icon_positions.size() != 0x40:
		return "live layout needs the 0x40 icon position bytes"
	if not live.get("mascot_anchor") is PackedByteArray or live.mascot_anchor.size() != 4:
		return "live layout needs the 4 mascot anchor bytes"
	for key: String in ["mascot_choice", "mascot_length", "mascot_swing"]:
		if not live.get(key) is int or live[key] < 0 or live[key] > 0xFF:
			return "live layout needs byte %s" % key
	return ""
