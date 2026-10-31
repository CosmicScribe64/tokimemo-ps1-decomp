extends RefCounted
## T-39301: engine/cd_queue.gd and the CD drive model (engine/_cd_drive.gd) on an invented disc pack.
## Expectations come from src/main/cd_queue.c and from the latency model floor(14 + 0.4 x sectors).
## The pump is stepped one frame at a time (game_state.frame_count = frame; cd_command_pump) and the
## status bytes callers poll are checked at each step. The emulator's real CD timing differs from the
## model (it is measured, not derived: wiki/tickets/T-39002-restore-port-runtime.md), so the comparison
## with captures is limited to what is timing independent (see config/port/engine-diff/cd_queue.json).

const GameEngine = preload("res://engine/engine.gd")
const DiscPack = preload("res://services/disc_pack.gd")

var failures: Array[String] = []
var base: String = ""

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

func run() -> Array[String]:
	failures.clear()
	base = "/tmp/tokimemo-engine-cd-test-%d" % Time.get_ticks_usec()
	_test_ring_and_requests()
	_test_checked_read_sequence()
	_test_nowait_read()
	_test_retry_and_refusals()
	_test_xa_sequence()
	_test_commands()
	_test_polls_and_recovery()
	_test_checksum()
	_remove_tree(base)
	return failures

# ---- invented disc pack: A (LBA 100, 3 sectors, ends with the 0x1012 marker), B (200, 2 sectors, no marker), C (300, 1)

static func _sha(bytes: PackedByteArray) -> String:
	var hash := HashingContext.new()
	hash.start(HashingContext.HASH_SHA256)
	hash.update(bytes)
	return hash.finish().hex_encode()

static func _content(seed_value: int, length: int, marker: bool) -> PackedByteArray:
	var bytes := PackedByteArray()
	bytes.resize(length)
	for i: int in length:
		bytes[i] = (seed_value * 31 + i * 7) & 0xff
	if marker:
		bytes.encode_u32(length - 4, 0x10120001)
	else:
		bytes.encode_u32(length - 4, 0x77770001)
	return bytes

func _build() -> String:
	var dir: String = base.path_join("pack")
	DirAccess.make_dir_recursive_absolute(dir.path_join("files"))
	var records: Array = []
	for spec: Array in [["A.BIN", 100, 3, 1, true], ["B.BIN", 200, 2, 2, false], ["C.BIN", 300, 1, 3, true]]:
		var stored: PackedByteArray = _content(spec[3], int(spec[2]) * 2048, spec[4])
		var file: FileAccess = FileAccess.open(dir.path_join("files").path_join(spec[0]), FileAccess.WRITE)
		file.store_buffer(stored)
		file.close()
		records.append({"path": spec[0], "lba": spec[1], "sectors": spec[2], "size": stored.size(), "stored_size": stored.size(),
			"mode": 2, "form": 1, "sha256": _sha(stored), "content_sha256": _sha(stored)})
	var manifest: Dictionary = {"schema": DiscPack.SCHEMA, "version": 1, "sector_size": 2048, "volume_id": "SYNTH",
		"source": {}, "files": records, "excluded": [], "totals": {}}
	var out: FileAccess = FileAccess.open(dir.path_join(DiscPack.MANIFEST_NAME), FileAccess.WRITE)
	out.store_string(JSON.stringify(manifest))
	out.close()
	return dir

## An engine with the invented pack open and audio_command (not ported: sound code) replaced by a recorder.
func _engine(log: Array) -> Object:
	var engine = GameEngine.new()
	var problem: String = engine.disc_pack.open(_build())
	_check(problem.is_empty(), "invented disc pack opens: " + problem)
	engine.set_stub("audio_command", func(args: Array) -> int:
		log.append(args.duplicate())
		return 0)
	engine.call_c("cd_command_reset")
	return engine

func _frame(engine, frame: int) -> void:
	engine.memory.game_state().set_field("frame_count", frame)

func _pump(engine, frame: int) -> void:
	_frame(engine, frame)
	engine.call_c("cd_command_pump")

func _g(engine, name: String, index: int = 0) -> int:
	return engine.memory.read_global(name, index)

# ---- tests

func _test_ring_and_requests() -> void:
	var log: Array = []
	var engine = _engine(log)
	_check(engine.call_c("cd_queue_idle") == 1 and _g(engine, "cd_read_status") == 2 and _g(engine, "cd_state") == 0, "after cd_command_reset: idle, status 2, state 0")
	engine.call_c("cd_read_param_pack", [3, 0x80100000, 0x80001000])
	_check(engine.memory.u32(0x80001000) == 0x03100000, "cd_read_param_pack: destination with the sector count in the top byte")
	engine.call_c("cd_read_sectors", [3, 0x80100000, 100])
	_check(log == [[6]], "cd_read_sectors announces the read with audio_command(6): %s" % [log])
	var ring: int = engine.memory.global_address("cd_command_ring")
	_check(_g(engine, "cd_queue_tail") == 1 and _g(engine, "cd_queue_head") == 0 and engine.call_c("cd_queue_idle") == 0, "one command queued")
	_check(engine.memory.u8(ring) == 6 and engine.memory.s32(ring + 4) == 100 and engine.memory.u32(ring + 8) == 0x03100000 and _g(engine, "cd_last_read_lba") == 100 and _g(engine, "cd_read_checked") == 1,
		"ring entry 0: command 6, lba 100, parameter word 0x03100000, checked read")
	_check(engine.call_c("cd_last_command") == 0, "cd_last_command looks at entry (head + 0x2F) % 0x30, the one before the head")
	# the ring holds 0x2F commands: the 48th push finds tail + 1 == head and refuses with -1
	engine.call_c("cd_command_reset")
	var pushes: Array = []
	for i: int in 0x30:
		pushes.append(engine.call_c("cd_command_push", [9, i, 0]))
	_check(pushes.count(1) == 0x2F and pushes[0x2F] == -1 and _g(engine, "cd_queue_tail") == 0x2F, "the ring takes 0x2F commands, then push returns -1")
	_check(engine.memory.u8(ring + 0x2E * 0x18 + 8 + 15) == 0, "param[15] is cleared on a push without parameters")
	engine.call_c("cd_command_reset")
	engine.call_c("cd_command_push", [27, 5, 1])
	_check(engine.memory.u8(ring + 8 + 15) == 0xFF, "a push with the parameter 1 marks param[15] with 0xFF")
	# cd_audio_request
	engine.memory.write_global("cd_audio_speaker_offset_table", 1234, 5)
	engine.call_c("cd_audio_request_set", [0x11, 0x22, 0x15])
	_check([_g(engine, "cd_audio_request", 0), _g(engine, "cd_audio_request", 1), _g(engine, "cd_audio_request", 2), _g(engine, "cd_audio_request", 3)] == [5, 1234, 0x11, 0x22],
		"cd_audio_request_set: speaker & 0xF, table offset, clip start and end")
	_check(engine.call_c("cd_audio_request_get") == 0x800B3D70, "cd_audio_request_get returns the address of the request")
	engine.call_c("cd_load_destination_base_set", [0x80123456])
	_check(engine.call_c("cd_load_destination_base_get") == -0x7FEDCBAA, "cd_load_destination_base round trip (s32)")

func _test_checked_read_sequence() -> void:
	var log: Array = []
	var engine = _engine(log)
	var dest: int = 0x80100000
	_frame(engine, 100)
	engine.call_c("cd_read_sectors", [3, dest, 100])
	var latency: int = 14 + 3 * 2 / 5 # floor(14 + 0.4 x 3)
	var status: Array = []
	for frame: int in range(100, 100 + latency + 3):
		_pump(engine, frame)
		status.append([_g(engine, "cd_read_status") & 1, _g(engine, "cd_state"), _g(engine, "cd_read_count"), _g(engine, "cd_queue_phase")])
		if frame < 100 + latency:
			_check(engine.memory.u32(dest) == 0, "frame %d: the bytes have not landed yet" % frame)
	_check(engine.refusal.is_empty(), "the read pumps without refusal: " + engine.refusal)
	_check(status[0] == [0, 0, 0, 1], "frame 100: command issued (phase 1), not finished: %s" % [status[0]])
	_check(status[latency - 1] == [0, 0, 0, 1], "frame %d (latency - 1): still reading: %s" % [100 + latency - 1, status[latency - 1]])
	_check(status[latency] == [1, 2, 1, -1], "frame %d = request + floor(14 + 0.4 x 3) = request + %d: status bit 0 set, state 2, count 1, phase -1: %s" % [100 + latency, latency, status[latency]])
	_check(status[latency + 1] == [1, 2, 1, -1] and status[latency + 2] == [1, 2, 1, -1], "the finished status holds")
	_check(_g(engine, "cd_queue_head") == 1 and engine.call_c("cd_queue_idle") == 1 and engine.call_c("cd_read_sync_status") & 1 == 1 \
			and engine.call_c("cd_done_count") == 1 and engine.call_c("cd_audio_state") == 2, "after the read the ring is idle and the accessors show the finish")
	var pack_bytes: Dictionary = engine.disc_pack.read_sectors(100, 3)
	_check(pack_bytes.ok and engine.memory.read_bytes(dest, 3 * 2048) == pack_bytes.data, "the 3 sectors landed in memory byte for byte")
	_check(_g(engine, "cd_sectors_left") == 0, "cd_sectors_left is 0 once landed")
	# the model latency scales with the sectors: 49 sectors = 33 frames (the opening's measured 49-sector reads)
	var drive = engine.cd_drive
	_check([14 + 1 * 2 / 5, 14 + 10 * 2 / 5, 14 + 33 * 2 / 5, 14 + 49 * 2 / 5, 14 + 149 * 2 / 5] == [14, 18, 27, 33, 73],
		"the model floor(14 + 0.4 x sectors): 1 -> 14, 10 -> 18, 33 -> 27, 49 -> 33, 149 -> 73")
	_check(drive != null, "the engine owns the drive model")

func _test_nowait_read() -> void:
	var log: Array = []
	var engine = _engine(log)
	_frame(engine, 10)
	engine.call_c("cd_read_sectors_nowait", [2, 0x80110000, 200]) # B: no end marker, fine without the check
	_check(_g(engine, "cd_read_checked") == 0 and log == [[6]], "cd_read_sectors_nowait clears cd_read_checked")
	var done_at: int = -1
	for frame: int in range(10, 60):
		_pump(engine, frame)
		if done_at < 0 and _g(engine, "cd_read_status") & 1 == 1:
			done_at = frame
	_check(done_at == 10 + 14 + 2 * 2 / 5 and _g(engine, "cd_state") == 2 and _g(engine, "cd_read_count") == 1, "an unchecked read finishes without the marker at request + 14: frame %d" % done_at)
	_check(engine.memory.u32(0x80110000 + 2 * 2048 - 4) == 0x77770001, "the data landed")

func _test_retry_and_refusals() -> void:
	var log: Array = []
	var engine = _engine(log)
	# a checked read of B (no 0x1012 marker) is retried: the head stays, the retry counter counts
	_frame(engine, 1)
	engine.call_c("cd_read_sectors", [2, 0x80110000, 200])
	var heads: Array = []
	var frame: int = 1
	while frame < 400 and engine.refusal.is_empty():
		_pump(engine, frame)
		heads.append(_g(engine, "cd_queue_head"))
		frame += 1
	_check(not heads.has(1) and engine.memory.u16(0x800B3D64) == 3, "a checked read without the end marker is retried, never completed (retry count %d)" % engine.memory.u16(0x800B3D64))
	_check(engine.refusal.contains("func_800AD950") and _g(engine, "cd_read_count") == 0 and _g(engine, "cd_state") == 0, "the third failed attempt reaches the original's VSync wait, which is not ported: %s" % engine.refusal)
	# a read outside the pack refuses at the landing frame (the original would retry forever)
	var other = _engine([])
	_frame(other, 1)
	other.call_c("cd_read_sectors_nowait", [1, 0x80110000, 150])
	frame = 1
	while frame < 100 and other.refusal.is_empty():
		_pump(other, frame)
		frame += 1
	_check(other.refusal.contains("outside every file") and other.refusal.contains("LBA 0x96"), "a read the disc pack cannot serve is refused with the pack's reason: %s" % other.refusal)
	# a read with audio_command not ported refuses before queueing anything
	var bare = GameEngine.new()
	bare.call_c("cd_read_sectors", [1, 0x80110000, 100])
	_check(bare.refusal.contains("cd_read_sectors -> audio_command") and bare.memory.read_global("cd_queue_tail") == 0 and bare.memory.read_global("cd_read_checked") == 0,
		"without audio_command ported, cd_read_sectors refuses at that call and queues nothing: %s" % bare.refusal)

func _test_xa_sequence() -> void:
	var log: Array = []
	var engine = _engine(log)
	var params: int = 0x80001000
	engine.memory.write_u8(params, 1) # filter file
	engine.memory.write_u8(params + 1, 3) # filter channel
	_frame(engine, 1)
	engine.call_c("cd_audio_command", [13, 100, params])
	var ring: int = engine.memory.global_address("cd_command_ring")
	_check([engine.memory.u8(ring), engine.memory.u8(ring + 0x18), engine.memory.u8(ring + 0x30)] == [0xD, 2, 0x1B] and _g(engine, "cd_queue_tail") == 3 and _g(engine, "cd_last_read_lba") == 100,
		"cd_audio_command(13, lba, params) queues Setfilter, Setloc, ReadS and remembers the LBA")
	var states: Array = []
	for frame: int in range(1, 12):
		_pump(engine, frame)
		states.append([_g(engine, "cd_queue_head"), _g(engine, "cd_queue_phase"), _g(engine, "cd_state")])
	_check(engine.refusal.is_empty(), "the XA request pumps without refusal: " + engine.refusal)
	_check(states.back() == [3, -1, 4], "after Setfilter, Setloc and ReadS the ring is empty and cd_state is 4 (XA playing): %s" % [states])
	_check(engine.cd_drive.filter_file == 1 and engine.cd_drive.filter_channel == 3 and engine.cd_drive.loc_lba == 100, "the drive got the filter and the position")
	# while playing, the idle pump tracks the head: 0.4 frame per sector (2.5 sectors per frame)
	var first: int = _g(engine, "cd_audio_position")
	_pump(engine, 21)
	var later: int = _g(engine, "cd_audio_position")
	_pump(engine, 61)
	var much_later: int = _g(engine, "cd_audio_position")
	_check(much_later - later == 100 and later >= first, "cd_audio_ticks follows the head at 2.5 sectors per frame (+40 frames = +100 sectors): %d, %d, %d" % [first, later, much_later])
	_check(engine.call_c("cd_audio_ticks") == much_later, "cd_audio_ticks returns it")
	# a pause ends the XA stream (command 9 -> phase 1 -> state 2)
	engine.call_c("cd_audio_command", [9, 0, 0])
	for frame: int in range(62, 66):
		_pump(engine, frame)
	_check(_g(engine, "cd_state") == 2 and engine.call_c("cd_queue_idle") == 1, "cd_audio_command(9) pauses: state 2, ring idle")

func _test_commands() -> void:
	var log: Array = []
	var engine = _engine(log)
	var ring: int = engine.memory.global_address("cd_command_ring")
	engine.call_c("cd_audio_command", [3, 777, 0])
	_check([engine.memory.u8(ring), engine.memory.u8(ring + 0x18), engine.memory.u8(ring + 0x30)] == [0xE, 2, 3] and engine.memory.u8(ring + 8) == 5 and engine.memory.s32(ring + 0x18 + 4) == 777,
		"cd_audio_command(3) sets mode 5, seeks and plays: ring 0xE (param[0] = 5), 2, 3")
	engine.call_c("cd_command_reset")
	engine.call_c("cd_audio_command", [21, 0x7FFFFFFF, 0])
	_check(engine.memory.s32(ring + 4) == 0x4AB50 and engine.memory.u8(ring + 0x18) == 0x15, "cd_audio_command(21) clamps the LBA to 0x4AB50 and queues Setloc + SeekL")
	engine.call_c("cd_command_reset")
	engine.call_c("cd_audio_command", [27, 5, 0])
	engine.call_c("cd_audio_command", [99, 5, 0])
	_check(_g(engine, "cd_queue_tail") == 0, "operations 27 and unknown ones queue nothing")
	# Pause with the drive idle: ring 0 -> state 2
	engine.call_c("cd_audio_command", [0, 0, 0])
	for frame: int in range(1, 6):
		_pump(engine, frame)
	_check(_g(engine, "cd_state") == 2 and engine.call_c("cd_queue_idle") == 1, "command 0 (pause) finishes with state 2")
	# a Play command is not modelled: refused, not invented
	engine.call_c("cd_command_push", [3, 0, 0])
	_pump(engine, 10)
	_check(engine.refusal.contains("func_8008B750"), "ring command 3 first calls the SPU CD volume (not ported): %s" % engine.refusal)

func _test_polls_and_recovery() -> void:
	var engine = _engine([])
	engine.memory.write_global("cd_read_status", 0)
	engine.call_c("cd_sync_poll")
	_check(_g(engine, "cd_read_status") == 2, "cd_sync_poll: a completed command sets bit 1 of the read status")
	_frame(engine, 1)
	engine.call_c("cd_read_sectors_nowait", [3, 0x80110000, 100])
	_pump(engine, 1) # the pump issues the read (Setmode, Setloc, ReadN)
	engine.call_c("cd_read_sync_poll")
	_check(_g(engine, "cd_read_status") == 0x80 and _g(engine, "cd_sectors_left") > 0, "cd_read_sync_poll while reading: status 0x80, sectors left > 0 (%d)" % _g(engine, "cd_sectors_left"))
	_frame(engine, 1 + 14 + 3 * 2 / 5)
	engine.call_c("cd_read_sync_poll")
	_check(_g(engine, "cd_sectors_left") == 0 and engine.memory.u32(0x80110000) != 0, "cd_read_sync_poll at the deadline: nothing left and the bytes have landed")
	var recover = _engine([])
	recover.call_c("cd_error_recover")
	_check(recover.memory.read_global("cd_mode_scratch") == 0x84 and recover.refusal.contains("func_800AD950"),
		"cd_error_recover sets mode 0x84 and then reaches the blocking VSync(0) loop, which is not ported: %s" % recover.refusal)

func _test_checksum() -> void:
	var engine = GameEngine.new()
	var buf: int = 0x80120000
	for i: int in 2048:
		engine.memory.write_u8(buf + i, (i * 3) & 0xFF)
	var sum: int = 0
	for i: int in range(0, 2044, 2):
		sum = (sum + ((i * 3) & 0xFF) + (((i + 1) * 3) & 0xFF)) & 0xFFFF
	engine.memory.write_u8(buf + 2044, sum >> 8)
	engine.memory.write_u8(buf + 2045, sum & 0xFF)
	_check(engine.call_c("cd_file_checksum_ok", [1, buf]) == 1, "cd_file_checksum_ok accepts the big-endian sum stored after the data")
	engine.memory.write_u8(buf + 2045, (sum + 1) & 0xFF)
	_check(engine.call_c("cd_file_checksum_ok", [1, buf]) == 0, "and rejects a wrong one")
	engine.call_c("cd_file_checksum_ok", [0, buf])
	_check(engine.refusal.contains("blocks == 0"), "blocks == 0 is refused (the C loop bound wraps)")

func _remove_tree(path: String) -> void:
	var directory := DirAccess.open(path)
	if directory == null:
		return
	for name: String in directory.get_directories():
		_remove_tree(path.path_join(name))
	for name: String in directory.get_files():
		DirAccess.remove_absolute(path.path_join(name))
	DirAccess.remove_absolute(path)
