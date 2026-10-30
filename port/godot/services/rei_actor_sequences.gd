extends RefCounted
## T-39002: authenticated O10 animation tables. actor_sprites.c draws the
## current sequence row, then advances the logical actor for the next frame.
const Board = preload("res://services/first_arrival_board_store.gd")
const Store = preload("res://services/first_arrival_media_store.gd")
var _frames: Dictionary = {}
var _sequences: Dictionary = {}

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
