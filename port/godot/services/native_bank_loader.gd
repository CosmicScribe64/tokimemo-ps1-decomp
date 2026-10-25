extends RefCounted
## T-29012: bank0 control rules from frame_render_audio.c, not an audio renderer.
## Original bank payload/LBA bindings are required external data. Completion and
## driver readiness are explicit inputs; no elapsed-frame readiness is invented.
const Math = preload("res://core/native_math.gd")
var data: Dictionary = {}

class Outcome extends RefCounted:
	var error: String = ""
	var status: int = -1
	var status_known: bool = false
	var operations: Array[Dictionary] = []

func reset() -> void:
	# audio_bank_state_reset writes these fields only; baseline/bank/playing persist.
	if data.is_empty():
		data = {"cd_baseline":null,"bank":null,"playing":null}
	data.merge({"load_flags":[0,0],"bgm_set":255,"se_header_0":0,"music_header_0":0,
		"se_header_1":0,"music_header_1":0,"skip_se":0,"skip_music":0,
		"transfer_polls":0,"load_requested":0,"sequence":0},true)

func restore(value: Dictionary) -> String:
	var previous: Dictionary = data
	data = value.duplicate(true)
	if not initialized():
		data = previous
		return "invalid native bank-loader checkpoint"
	return ""

func checkpoint() -> Dictionary:
	return data.duplicate(true)

func initialized() -> bool:
	if data.size() != 14: return false
	for key in ["cd_baseline","bank","playing"]:
		if not data.has(key): return false
		if data[key] != null and not _counter_valid(data[key]): return false
	for key in ["bgm_set","skip_se","skip_music","transfer_polls","load_requested","sequence"]:
		if not _counter_valid(data.get(key)): return false
	for key in ["se_header_0","music_header_0","se_header_1","music_header_1"]:
		if typeof(data.get(key)) != TYPE_INT or data[key] < -2147483648 or data[key] > 2147483647: return false
	if not data.get("load_flags") is Array or data.load_flags.size() != 2: return false
	for value in data.load_flags:
		if not _counter_valid(value): return false
	return true

static func _counter_valid(value: Variant) -> bool:
	return typeof(value) == TYPE_INT and value >= 0 and value <= 255

static func _binding_valid(binding: Dictionary) -> bool:
	if binding.size() != 6: return false
	for key in ["bank","se_header","se_body","music_header","music_body","sequence"]:
		if typeof(binding.get(key)) != TYPE_INT or binding[key] < 0 or binding[key] > 0x7fffffff: return false
	return binding.bank == 0

static func _command(outcome: Outcome, command: int) -> void:
	outcome.operations.append({"kind":"audio_command","command":command})

func load_start(binding: Dictionary, done_count: Variant) -> Outcome:
	var out := Outcome.new()
	if not initialized() or not _binding_valid(binding) or not _counter_valid(done_count):
		out.error = "bank0 start requires initialized source-owned state, authenticated binding and explicit byte CD done counter"
		return out
	data.bgm_set = 255; data.skip_se = 0; data.skip_music = 0; data.bank = 0
	data.transfer_polls = 0; data.load_requested = 1; data.cd_baseline = done_count
	out.operations.append({"kind":"cd_destination_base","role":"bank0_se_body"})
	if data.se_header_0 == binding.se_header:
		data.cd_baseline = Math.u8(data.cd_baseline-2); data.skip_se = 1
	else:
		if binding.se_header != 0:
			out.operations.append({"kind":"cd_read_nowait","sectors":3,"destination":"bank0_se_header","lba":binding.se_header})
			data.se_header_0 = binding.se_header
		else: data.cd_baseline = Math.u8(data.cd_baseline-1); data.skip_se = 1
		if binding.se_body != 0: out.operations.append({"kind":"cd_read_nowait","sectors":32,"destination":"bank0_se_body","lba":binding.se_body})
		else: data.cd_baseline = Math.u8(data.cd_baseline-1); data.skip_se = 1
	if data.music_header_0 == binding.music_header:
		data.cd_baseline = Math.u8(data.cd_baseline-3); data.skip_music = 1
		return out
	# Native order is sequence, body, then header for the music half.
	if binding.sequence != 0:
		out.operations.append({"kind":"cd_read_nowait","sectors":37,"destination":"bank0_sequence","lba":binding.sequence})
		data.music_header_0 = binding.music_header
	else: data.cd_baseline = Math.u8(data.cd_baseline-1); data.skip_music = 1
	if binding.music_body != 0: out.operations.append({"kind":"cd_read_nowait","sectors":80,"destination":"bank0_music_body","lba":binding.music_body})
	else: data.cd_baseline = Math.u8(data.cd_baseline-1); data.skip_music = 1
	if binding.music_header != 0:
		out.operations.append({"kind":"cd_read_nowait","sectors":6,"destination":"bank0_music_header","lba":binding.music_header})
		data.music_header_0 = binding.music_header
	else: data.cd_baseline = Math.u8(data.cd_baseline-1); data.skip_music = 1
	return out

func load_poll(done_count: Variant, bank0_ready: Variant, bank1_ready: Variant) -> Outcome:
	var out := Outcome.new()
	if not initialized() or not _counter_valid(done_count) or typeof(bank0_ready) != TYPE_BOOL or typeof(bank1_ready) != TYPE_BOOL:
		out.error = "bank0 poll requires explicit CD count and both actual driver-ready values"
		return out
	if data.bank != 0:
		out.error = "only source bank0 loading is implemented"
		return out
	if typeof(data.cd_baseline) != TYPE_INT:
		out.error = "bank0 poll requires its actual load-start CD baseline"
		return out
	out.status_known = true
	if ((int(done_count)-int(data.cd_baseline)+256)&255) < 5: return out
	if data.transfer_polls == 0:
		if data.skip_se == 0 and data.skip_music == 0:
			out.operations.append({"kind":"cd_destination_base","role":"bank0_se_body"}); _command(out,0x11)
		elif data.skip_se == 0:
			out.operations.append({"kind":"cd_destination_base","role":"bank0_se_body"}); _command(out,0x13)
		elif data.skip_music == 0:
			out.operations.append({"kind":"cd_destination_base","role":"bank0_music_body"}); _command(out,0x14)
		data.transfer_polls = Math.u8(data.transfer_polls+1)
	else:
		data.transfer_polls = Math.u8(data.transfer_polls+1)
		if data.transfer_polls > 192 or (bank0_ready and bank1_ready):
			_command(out,0x22); _command(out,0x21); _command(out,0x2f)
			data.load_flags[0] = 0; out.status = 1
	return out

func start_sequence(sequence_word0: Variant, sequence_id: Variant) -> Outcome:
	var out := Outcome.new()
	if not initialized() or typeof(sequence_word0) != TYPE_INT or typeof(sequence_id) != TYPE_INT or sequence_id < -2147483648 or sequence_id > 2147483647:
		out.error = "bank0 sequence start requires source-owned bank and authenticated header word/int sequence ID"
		return out
	if (int(sequence_word0)&0xffff) != 0x40:
		out.status_known = true
		return out
	_command(out,0x22); _command(out,0x21); _command(out,0x2f)
	_command(out,(int(sequence_id)|0x200)&0xffff)
	data.bgm_set = 0; data.sequence = Math.u8(sequence_id); data.playing = 1
	# Original valid-header return is the last audio_command result, not a proved
	# success constant. Callers here use the effects; status remains unknown (-1).
	return out

const Rules = preload("res://rules/new_game.gd")
const BANK_CONTENT_SHA: String = "1aea3228d41a007f6044ef8b6661e8fe498fc35178cde0f0838c5ebd37557bf7"

static func load_bindings(path: String) -> Dictionary:
	if not Rules._external_manifest_path_valid(path): return {}
	var file: FileAccess = FileAccess.open(path,FileAccess.READ)
	if file == null or file.get_length() <= 0 or file.get_length() > 65536: return {}
	var length: int = file.get_length()
	var raw: PackedByteArray = file.get_buffer(length)
	if raw.size() != length or file.get_length() != length or not Rules._external_manifest_path_valid(path): return {}
	var hash := HashingContext.new(); hash.start(HashingContext.HASH_SHA256); hash.update(raw)
	if hash.finish().hex_encode() != BANK_CONTENT_SHA: return {}
	var parsed: Variant = JSON.parse_string(raw.get_string_from_utf8())
	if not parsed is Dictionary or parsed.size() != 2: return {}
	for role in ["prologue_arrival","normal_gameplay"]:
		if not parsed.get(role) is Dictionary or not parsed[role].get("binding") is Dictionary or not parsed[role].get("assets") is Dictionary: return {}
		var binding: Dictionary = parsed[role].binding
		for key in binding:
			if typeof(binding[key]) not in [TYPE_FLOAT,TYPE_INT] or binding[key] != int(binding[key]): return {}
			binding[key] = int(binding[key])
		if not _binding_valid(binding): return {}
		var first_word: Variant = parsed[role].assets.get("sequence",{}).get("word0")
		if typeof(first_word) not in [TYPE_FLOAT,TYPE_INT] or first_word != int(first_word) or first_word < 0 or first_word > 0xffffffff: return {}
		parsed[role].assets.sequence.word0 = int(first_word)
	return parsed
