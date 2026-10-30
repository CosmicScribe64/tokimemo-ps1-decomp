extends RefCounted
## T-39002: authenticated O10 animation tables. actor_sprites.c draws the
## current sequence row, then advances the logical actor for the next frame.
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
var _frames: Dictionary = {}
var _sequences: Dictionary = {}
var o11_bytes := PackedByteArray()

func prepare(bytes: PackedByteArray) -> String:
	_frames.clear()
	_sequences.clear()
	if bytes.size() != 51200 or Store._sha(bytes) != Board.O_09_SHA:
		return "Rei animation tables require original O10 bytes"
	var frames: Dictionary = {}
	var sequences: Dictionary = {}
	for spec: Array in [["body",0x4288,9,0x4280,2],["crowd",0x110,8,0x10c,1]]:
		var decoded: Array = []
		for index in int(spec[2]):
			var offset := int(bytes.decode_u32(spec[1]+index*4))-0x80180000
			if offset < 0 or offset+8 > bytes.size(): return "Rei frame pointer is outside original resource"
			var count := bytes.decode_s16(offset)
			if count < 1 or count > 4 or offset+8+count*14 > bytes.size(): return "Rei frame part extent differs"
			var parts: Array = []
			for part_index in count:
				var row := offset+8+part_index*14
				var texture := bytes.decode_u16(row+4)
				var page := texture >> 6
				if page >= int(spec[4]): return "Rei frame texture page is outside its native table"
				parts.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),
					"origin_x":bytes.decode_s16(offset+2),"origin_y":bytes.decode_s16(offset+4),
					"u":(texture&7)*16,"v":((texture>>3)&7)*16,
					"page_offset":int(bytes.decode_u32(spec[3]+page*4))-0x80180000,
					"palette_flags":bytes.decode_u16(row+6),"flip":bytes.decode_u16(row+8),
					"w":bytes.decode_s16(row+10),"h":bytes.decode_s16(row+12)})
			decoded.append(parts)
		frames[spec[0]] = decoded
	for spec: Array in [["blink",0x42ac,"body"],["crowd_left",0x130,"crowd"],["crowd_right",0x134,"crowd"]]:
		var offset := int(bytes.decode_u32(spec[1]))-0x80180000
		if offset < 0 or offset+2 > bytes.size(): return "Rei sequence pointer differs"
		var count := bytes.decode_s16(offset)
		if count < 1 or count > 5 or offset+2+count*4 > bytes.size(): return "Rei sequence extent differs"
		var rows: Array = []
		for index in count:
			var frame := bytes.decode_s16(offset+2+index*4)
			var duration := bytes.decode_s16(offset+4+index*4)
			if frame < 0 or frame >= frames[spec[2]].size() or duration <= 0: return "Rei sequence row differs"
			rows.append({"frame":frame,"duration":duration})
		sequences[spec[0]] = rows
	_frames = frames
	_sequences = sequences
	return ""

func prepare_lines15(bytes: PackedByteArray) -> String:
	# O11 after the stage9/substep11 memcpy: frames 0x150 (9), pages 0x140
	# (4). Stage10 actors use flags 0x40 only, so no sequence table is read.
	_frames.erase("o11"); o11_bytes = PackedByteArray()
	if bytes.size() != 0x10800 or Store._sha(bytes) != Board.O_11_SHA:
		return "Rei line15 tables require original O11 bytes"
	var decoded: Array = []
	for index in 9:
		var offset := int(bytes.decode_u32(0x150+index*4))-0x80180000
		if offset < 0 or offset+8 > bytes.size(): return "Rei O11 frame pointer is outside original resource"
		var count := bytes.decode_s16(offset)
		if count < 1 or count > 5 or offset+8+count*14 > bytes.size(): return "Rei O11 frame part extent differs"
		var parts: Array = []
		for part_index in count:
			var row := offset+8+part_index*14
			var texture := bytes.decode_u16(row+4)
			var page := texture >> 6
			if page >= 4: return "Rei O11 frame texture page is outside its native table"
			parts.append({"x":bytes.decode_s16(row),"y":bytes.decode_s16(row+2),
				"origin_x":bytes.decode_s16(offset+2),"origin_y":bytes.decode_s16(offset+4),
				"u":(texture&7)*16,"v":((texture>>3)&7)*16,
				"page_offset":int(bytes.decode_u32(0x140+page*4))-0x80180000,
				"palette_flags":bytes.decode_u16(row+6),"flip":bytes.decode_u16(row+8),
				"w":bytes.decode_s16(row+10),"h":bytes.decode_s16(row+12)})
		decoded.append(parts)
	_frames["o11"] = decoded
	o11_bytes = bytes.duplicate()
	return ""

var line17_bytes: Dictionary = {}
var line17_rows: Dictionary = {}

func prepare_line17(o12: PackedByteArray, o13: PackedByteArray) -> String:
	# Stage11 O12 (case8 copy) and O_13 (case6 copy) tables, read through
	# BoardStore.LINE17_ACTORS. Every sequence row must name a drawable frame.
	line17_bytes.clear(); line17_rows.clear()
	if o12.size() != 0x8800 or Store._sha(o12) != Board.O_12_SHA or o13.size() != 0x20800 or Store._sha(o13) != Board.O_13_SHA:
		return "Rei line17 tables require original O12 and O_13 bytes"
	for item: Array in [["o12_rei_line17",o12],["o13_rei_line17_exit",o13]]:
		var rows := Board.line17_sequence(item[0],item[1])
		if rows.is_empty(): return "Rei line17 sequence differs: "+str(item[0])
		for row: Dictionary in rows:
			if Board.line17_frame_parts(item[0],item[1],row.frame).is_empty() or Board.line17_tpage_slots(item[0],item[1],row.frame).is_empty():
				return "Rei line17 sequence frame has no source parts: "+str(item[0])
		line17_bytes[item[0]] = item[1].duplicate()
		line17_rows[item[0]] = rows
	return ""

func line17_parts(profile: String, frame: int) -> Array:
	if not line17_bytes.has(profile): return []
	return Board.line17_frame_parts(profile,line17_bytes[profile],frame)

func line17_slots(profile: String, frame: int) -> Dictionary:
	if not line17_bytes.has(profile): return {}
	return Board.line17_tpage_slots(profile,line17_bytes[profile],frame)

func draw_and_advance_rows(actor: Dictionary, rows: Array, frame_count: int) -> Dictionary:
	# actor_draw_all with flags&0x40 clear: draw sequence row seq_step, then
	# advance (unk_06 speed, unk_08 counter, unk_18 step) for the next frame.
	if rows.is_empty() or not actor.get("seq_step") is int or not actor.get("sequence_counter") is int or not actor.get("flags") is int or not actor.get("speed") is int:
		return {"error":"Rei actor has no native sequence state"}
	var step: int = actor.seq_step
	var counter: int = actor.sequence_counter
	var flags: int = actor.flags
	var speed: int = actor.speed
	if step < 0 or step >= rows.size() or counter < 0 or counter >= rows[step].duration or flags not in [0,1,3,5] or speed < 0 or speed > 255:
		return {"error":"Rei sequence cursor or flags are outside supported source states"}
	var drawn: int = rows[step].frame
	if (flags & 1) != 0:
		if (speed & 0x80) != 0 and (speed & 0x7f) != 0: counter += speed & 0x7f
		elif speed != 0 and frame_count % speed == 0: counter += 1
		if counter >= rows[step].duration:
			step += 1
			counter = 0
		if step >= rows.size():
			if (flags & 2) != 0:
				step = 0
				flags &= ~3
			elif (flags & 4) != 0:
				step -= 1
				flags &= ~5
			else: step = 0
	actor.seq_step = step
	actor.sequence_counter = counter
	actor.flags = flags
	actor["frame_index"] = int(rows[step].frame)
	return {"error":"","frame_index":drawn,"source_frame_count":frame_count}

func parts(group: String, frame: int) -> Array:
	if not _frames.has(group) or frame < 0 or frame >= _frames[group].size(): return []
	return _frames[group][frame].duplicate(true)

func draw_and_advance(actor: Dictionary, frame_count: int) -> Dictionary:
	var rows: Variant = _sequences.get(actor.get("sequence"))
	if not rows is Array or not actor.get("seq_step") is int or not actor.get("sequence_counter") is int or not actor.get("flags") is int or not actor.get("speed") is int:
		return {"error":"Rei actor has no native sequence state"}
	var step: int = actor.seq_step
	var counter: int = actor.sequence_counter
	var flags: int = actor.flags
	var speed: int = actor.speed
	if step < 0 or step >= rows.size() or counter < 0 or counter >= rows[step].duration or flags not in [0,1,3,5] or speed < 0 or speed > 255:
		return {"error":"Rei sequence cursor or flags are outside supported source states"}
	var drawn: int = rows[step].frame
	if (flags & 1) != 0:
		if (speed & 0x80) != 0 and (speed & 0x7f) != 0: counter += speed & 0x7f
		elif speed != 0 and frame_count % speed == 0: counter += 1
		if counter >= rows[step].duration:
			step += 1
			counter = 0
		if step >= rows.size():
			if (flags & 2) != 0:
				step = 0
				flags &= ~3
			elif (flags & 4) != 0:
				step -= 1
				flags &= ~5
			else: step = 0
	actor.seq_step = step
	actor.sequence_counter = counter
	actor.flags = flags
	actor["frame_index"] = int(rows[step].frame)
	return {"error":"","frame_index":drawn,"source_frame_count":frame_count}
