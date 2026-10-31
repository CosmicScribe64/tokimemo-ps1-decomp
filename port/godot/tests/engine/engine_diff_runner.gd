extends SceneTree
## T-39301: runs the function-differential tests of one or all engine modules against emulator captures.
## Not part of the core suite (the captures are game data under .asset-cache); run it through
## tools/port/run_engine_diff.sh, which mounts the captures read-only. Exit code 1 on any failure.
##
## usage (after `--`): --root <captures dir (.asset-cache/m2-engine)> --specs <config/port/engine-diff>
##                     (--file <c file stem> | --all) [--function <C name>] [--show N] [--ranges]
## --show N prints N failing hits per function (default 4); --ranges lists, per function, every RAM range
## that differed in any hit (to decide what an exclusion needs to say).
##
## A spec (config/port/engine-diff/<stem>.json) names the captures that cover a module and the RAM
## exclusions (each with a reason):
##   {"captures": ["game-mode-v1"], "exclude": [{"field": "...", "reason": "..."}],
##    "functions": {"<C name>": {"exclude": [...], "not_comparable": {"<capture>:<hit>": "reason"}}},
##    "untested": {"<C name>": "reason"}}
## not_comparable (keys "<capture>:<hit>", "<capture>:<first>-<last>", "<capture>:*"): a hit whose result is known to differ for a stated reason (state the replay cannot load
## because it lives outside game memory, a timing the model approximates); it is run, counted and printed
## separately, never silently dropped. Hits replay in capture order on one engine per capture and function.
## Every function of the module's C_FUNCTIONS needs hits in a capture unless the spec lists it under
## "untested" with a reason (for example a function the emulator never reached from the saved states).

const Differ = preload("res://tests/engine/engine_diff.gd")
const GameEngine = preload("res://engine/engine.gd")
const MAX_FAILURES_SHOWN: int = 4 ## failing hits printed per function (the tally counts them all; --show N changes it)

var show_limit: int = MAX_FAILURES_SHOWN
var args_verbose: bool = false

func _initialize() -> void:
	call_deferred("_run")

func _arg(args: PackedStringArray, name: String, fallback: String = "") -> String:
	var at: int = args.find(name)
	return args[at + 1] if at >= 0 and at + 1 < args.size() else fallback

func _run() -> void:
	var args: PackedStringArray = OS.get_cmdline_user_args()
	var root: String = _arg(args, "--root")
	var specs: String = _arg(args, "--specs")
	var only_function: String = _arg(args, "--function")
	if root.is_empty() or specs.is_empty() or (not args.has("--all") and _arg(args, "--file").is_empty()):
		printerr("usage: -- --root <captures> --specs <specs dir> (--file <stem> | --all) [--function <name>]")
		quit(2)
		return
	var engine = GameEngine.new()
	if not engine.registration_errors.is_empty():
		printerr("engine registration errors: ", engine.registration_errors)
		quit(2)
		return
	var stems: Array[String] = []
	if args.has("--all"):
		for file_name: String in DirAccess.get_files_at(specs):
			if file_name.ends_with(".json"):
				stems.append(file_name.get_basename())
		stems.sort()
	else:
		stems.append(_arg(args, "--file"))
	var failed: bool = false
	var differ = Differ.new()
	show_limit = int(_arg(args, "--show", str(MAX_FAILURES_SHOWN)))
	args_verbose = args.has("--ranges")
	for stem: String in stems:
		if not _run_module(engine, differ, stem, root, specs, only_function):
			failed = true
	print("RESULT: ", "FAIL" if failed else "PASS")
	quit(1 if failed else 0)

func _run_module(engine, differ, stem: String, root: String, specs: String, only_function: String) -> bool:
	var module: RefCounted = engine.modules.get(stem)
	if module == null:
		printerr("%s: no engine module" % stem)
		return false
	var spec_text: String = FileAccess.get_file_as_string(specs.path_join(stem + ".json"))
	var json := JSON.new()
	if json.parse(spec_text) != OK or not json.data is Dictionary:
		printerr("%s: spec is missing or not JSON" % stem)
		return false
	var spec: Dictionary = json.data
	var ok: bool = true
	var signatures: Dictionary = module.get("C_SIGNATURES")
	var untested: Dictionary = spec.get("untested", {})
	var tallies: Dictionary = {}
	for capture: String in spec.get("captures", []):
		var index_text: String = FileAccess.get_file_as_string(root.path_join(capture).path_join("index.json"))
		if json.parse(index_text) != OK or not json.data is Dictionary:
			printerr("%s: capture %s has no index.json (run tools/emu/func_capture/pack.sh)" % [stem, capture])
			ok = false
			continue
		var index: Dictionary = json.data
		for function_name: String in module.get("C_FUNCTIONS"):
			if (not only_function.is_empty() and function_name != only_function) or not index.has(function_name):
				continue
			# One engine per capture and function: the hits replay in capture order, so state that lives
			# outside game memory (the CD drive model) evolves as it did between the captured calls.
			engine = GameEngine.new()
			var function_spec: Dictionary = spec.get("functions", {}).get(function_name, {})
			var known: Dictionary = function_spec.get("not_comparable", {})
			var exclude: Array = (spec.get("exclude", []) as Array) + (function_spec.get("exclude", []) as Array)
			var tally: Dictionary = tallies.get(function_name, {"hits": 0, "passed": 0, "compared": 0, "callees": 0})
			for entry: Dictionary in index[function_name]:
				var dir: String = root.path_join(capture).path_join(function_name)
				var hit: Dictionary = Differ.parse_hit(FileAccess.get_file_as_string(dir.path_join(str(entry.file))))
				var ram_path: String = dir.path_join(str(entry.file).get_basename() + ".entry.ram.gz")
				var entry_ram: PackedByteArray = Differ.read_entry_ram(ram_path)
				if hit.is_empty() or entry_ram.size() != engine.memory.RAM_SIZE:
					printerr("%s %s hit %s: unreadable capture" % [stem, function_name, entry.file])
					ok = false
					continue
				var result: Dictionary = differ.run_hit(engine, function_name, hit, entry_ram, signatures.get(function_name, {}), exclude)
				tally.hits += 1
				tally.callees += int(result.callees_replayed)
				if result.ok:
					tally.passed += 1
					tally.compared += int(result.compared_bytes)
				elif not _known_reason(known, capture, int(entry.hit)).is_empty():
					tally.not_comparable = int(tally.get("not_comparable", 0)) + 1
					_note_runs(tally, result.runs)
				else:
					ok = false
					tally.failed = int(tally.get("failed", 0)) + 1
					_note_runs(tally, result.runs)
					if int(tally.failed) > show_limit:
						continue
					print("FAIL %s.%s %s/%s (vsync %d, a0=0x%X)" % [stem, function_name, capture, entry.file, int(entry.vsync), int(hit.a0)])
					for problem: String in result.problems:
						print("    ", problem)
			tallies[function_name] = tally
	for function_name: String in module.get("C_FUNCTIONS"):
		if not only_function.is_empty() and function_name != only_function:
			continue
		if tallies.has(function_name):
			var t: Dictionary = tallies[function_name]
			print("%-34s %3d hit(s), %3d passed, %d callee call(s) replayed, %d bytes compared%s%s" % [
				function_name, t.hits, t.passed, t.callees, t.compared,
				", %d not comparable (spec)" % int(t.not_comparable) if int(t.get("not_comparable", 0)) > 0 else "",
				", %d FAILED" % int(t.failed) if int(t.get("failed", 0)) > 0 else ""])
			if args_verbose and t.has("ranges"):
				_print_ranges(t.ranges)
		elif untested.has(function_name) and not str(untested[function_name]).is_empty():
			print("%-34s untested: %s" % [function_name, untested[function_name]])
		else:
			print("%-34s NO CAPTURE (add one or list it under \"untested\" with a reason)" % function_name)
			ok = false
	return ok

func _note_runs(tally: Dictionary, runs: Array) -> void:
	var ranges: Dictionary = tally.get("ranges", {})
	for run: Array in runs:
		var key: int = int(run[0]) & ~0xF
		ranges[key] = int(ranges.get(key, 0)) + 1
	tally.ranges = ranges

func _print_ranges(ranges: Dictionary) -> void:
	var keys: Array = ranges.keys()
	keys.sort()
	var parts: PackedStringArray = []
	for key: int in keys:
		parts.append("0x%08X x%d" % [0x80000000 + key, ranges[key]])
	print("    differing 16-byte blocks: ", ", ".join(parts))

## The spec's reason for a hit that may differ: keys are "<capture>:<hit>", "<capture>:<first>-<last>" or
## "<capture>:*". Returns "" when none applies.
func _known_reason(known: Dictionary, capture: String, hit: int) -> String:
	for key: String in known:
		var parts: PackedStringArray = key.split(":")
		if parts.size() != 2 or parts[0] != capture:
			continue
		var span: String = parts[1]
		var matches: bool = span == "*" or span == str(hit)
		if not matches and span.contains("-"):
			var bounds: PackedStringArray = span.split("-")
			matches = bounds.size() == 2 and hit >= int(bounds[0]) and hit <= int(bounds[1])
		if matches:
			return str(known[key])
	return ""
