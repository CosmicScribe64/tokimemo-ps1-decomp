extends SceneTree
## T-39314: DrawOTag through the SDK layer against the emulator's VRAM. Not part of the core suite (it needs the
## GDExtension and the emulator captures of T-39302, both outside Git); run it with tools/port/run_sdk_render_check.sh.
##
## For every captured DrawOTag call (<root>/f-*/a/callNNNN.{ram.bin,vram_before.bin,vram_after.bin,cmp.args}):
## the entry RAM goes into the engine's memory, VRAM before into the renderer, the GPU environment is rebuilt the
## way tools/port/native_render/compare_capture.cpp does (the GPUSTAT word seeds the draw mode and mask, then the
## E1..E6 words of the PutDrawEnv packet the library keeps at 0x800DD5B8), and the game's own call
## `engine.call_c("func_8009CB9C", [ot])` (libgpu DrawOTag) draws. The renderer's VRAM is compared with the
## emulator's VRAM after the call, pixel by pixel. A frame passes with at most MAX_DIFFERENT_PIXELS differences
## (the renderer's documented interpolation rounding: wiki/ps1-renderer.md); the count is printed per frame.
##
## usage (after `--`): --root <.asset-cache/m2-renderer> [--limit N]

const GameEngine = preload("res://engine/engine.gd")
const MAX_DIFFERENT_PIXELS: int = 1
const ENV_PACKET: int = 0x800DD5B8

func _initialize() -> void:
	call_deferred("_run")

func _arg(args: PackedStringArray, name: String, fallback: String = "") -> String:
	var at: int = args.find(name)
	return args[at + 1] if at >= 0 and at + 1 < args.size() else fallback

func _run() -> void:
	var args: PackedStringArray = OS.get_cmdline_user_args()
	var root: String = _arg(args, "--root")
	var limit: int = int(_arg(args, "--limit", "1000"))
	if root.is_empty():
		printerr("usage: -- --root <.asset-cache/m2-renderer> [--limit N]")
		quit(2)
		return
	var checked: int = 0
	var failed: int = 0
	var dirs: PackedStringArray = DirAccess.get_directories_at(root)
	dirs.sort()
	for dir_name: String in dirs:
		if not dir_name.begins_with("f-"):
			continue
		var frames: String = root.path_join(dir_name).path_join("a")
		var files: PackedStringArray = DirAccess.get_files_at(frames)
		files.sort()
		for file_name: String in files:
			if not file_name.ends_with(".ram.bin") or checked >= limit:
				continue
			var tag: String = file_name.trim_suffix(".ram.bin")
			var result: Dictionary = _check_frame(frames, tag)
			checked += 1
			var ok: bool = result.error.is_empty() and int(result.different) <= MAX_DIFFERENT_PIXELS
			if not ok:
				failed += 1
			print("%s %s/%s: %s" % ["ok  " if ok else "FAIL", dir_name, tag,
				result.error if not result.error.is_empty() else "%d of %d pixels differ" % [result.different, result.compared]])
	print("%d frame(s) checked, %d failed" % [checked, failed])
	print("RESULT: ", "FAIL" if failed > 0 or checked == 0 else "PASS")
	quit(1 if failed > 0 or checked == 0 else 0)

func _check_frame(frames: String, tag: String) -> Dictionary:
	var ram: PackedByteArray = FileAccess.get_file_as_bytes(frames.path_join(tag + ".ram.bin"))
	var before: PackedByteArray = FileAccess.get_file_as_bytes(frames.path_join(tag + ".vram_before.bin"))
	var after: PackedByteArray = FileAccess.get_file_as_bytes(frames.path_join(tag + ".vram_after.bin"))
	var arguments: String = FileAccess.get_file_as_string(frames.path_join(tag + ".cmp.args")).strip_edges()
	var parts: PackedStringArray = arguments.split(" ", false)
	var ot: int = 0
	var status: int = 0
	for i: int in range(0, parts.size() - 1, 2):
		if parts[i] == "--ot":
			ot = ("0x" + parts[i + 1]).hex_to_int()
		elif parts[i] == "--status":
			status = ("0x" + parts[i + 1]).hex_to_int()
	if ram.size() != 0x200000 or before.size() != 0x100000 or after.size() != 0x100000 or ot == 0:
		return {"error": "unreadable capture files", "different": 0, "compared": 0}
	var engine = GameEngine.new()
	engine.memory.restore(ram)
	if not engine.renderer_ready():
		engine.shutdown()
		return {"error": "renderer unavailable: " + engine.renderer.last_error, "different": 0, "compared": 0}
	engine.renderer.set_vram(before)
	# the GPU environment at the call: E1 from the GPUSTAT word, E6 from its mask bits, then the library's DR_ENV words
	engine.renderer.gp0_word(0xE1000000 | (status & 0x7FF))
	engine.renderer.gp0_word(0xE6000000 | (1 if status & 0x800 else 0) | (2 if status & 0x1000 else 0))
	var at: int = ENV_PACKET
	while at < ENV_PACKET + 0x40:
		var word: int = engine.memory.u32(at)
		if (word >> 24) < 0xE1 or (word >> 24) > 0xE6:
			break
		engine.renderer.gp0_word(word)
		at += 4
	engine.call_c("func_8009CB9C", [ot])
	var problem: String = engine.failure()
	var drawn: PackedByteArray = engine.renderer.vram()
	engine.shutdown()
	if not problem.is_empty():
		return {"error": "engine: " + problem, "different": 0, "compared": 0}
	var different: int = 0
	for i: int in range(0, 0x100000, 2):
		if drawn[i] != after[i] or drawn[i + 1] != after[i + 1]:
			different += 1
	return {"error": "", "different": different, "compared": 0x80000}
