extends RefCounted
## T-39301: lint of engine/*.gd (the loop rule of wiki/engine-porting-contract.md). After a refusal call_c and the
## services return 0 at once, so a loop whose condition or body depends on them would spin forever. Every
## `while` loop that calls call_c, a drive/service method or call_address must therefore test engine.halted()
## (in its condition or body), and a `while true` must contain a break or return.

var failures: Array[String] = []

func run() -> Array[String]:
	failures.clear()
	var dir: DirAccess = DirAccess.open("res://engine")
	if dir == null:
		failures.append("cannot open res://engine")
		return failures
	var checked: int = 0
	for file_name: String in dir.get_files():
		var source: String = file_name.trim_suffix(".remap")
		if source.ends_with(".gd"):
			checked += _lint("res://engine/" + source)
	if checked == 0:
		failures.append("no engine file linted")
	_self_test()
	return failures

static func _indent(line: String) -> int:
	var n: int = 0
	while n < line.length() and line[n] == "\t":
		n += 1
	return n

## Returns the number of while loops checked.
func _lint(path: String) -> int:
	var lines: PackedStringArray = FileAccess.get_file_as_string(path).split("\n")
	var loops: int = 0
	for index: int in lines.size():
		var text: String = lines[index].strip_edges()
		if not text.begins_with("while "):
			continue
		loops += 1
		var depth: int = _indent(lines[index])
		var block: String = text
		var cursor: int = index + 1
		while cursor < lines.size() and (lines[cursor].strip_edges().is_empty() or _indent(lines[cursor]) > depth):
			block += "\n" + lines[cursor]
			cursor += 1
		for problem: String in problems_of(block):
			failures.append("%s:%d: %s" % [path, index + 1, problem])
	return loops

## The lint rules on the text of one loop (condition and body).
static func problems_of(block: String) -> Array[String]:
	var out: Array[String] = []
	var calls_out: bool = block.contains("call_c(") or block.contains("call_address(") or block.contains("cd_drive.") \
		or block.contains("renderer.") or block.contains("audio.") or block.contains("text_layer.")
	if calls_out and not block.contains("halted()"):
		out.append("a while loop that calls call_c or a service must test engine.halted()")
	if block.begins_with("while true") and not (block.contains("break") or block.contains("return")):
		out.append("while true without break or return")
	return out

func _self_test() -> void:
	if problems_of("while f() == 0:\n\tengine.call_c(\"x\")").is_empty():
		failures.append("lint self-test: an unguarded call_c loop must be flagged")
	if not problems_of("while engine.cd_drive.cd_control(1, 0, 0) == 0:\n\tif engine.halted():\n\t\tbreak").is_empty():
		failures.append("lint self-test: a guarded loop must pass")
	if problems_of("while true:\n\tpass").is_empty():
		failures.append("lint self-test: while true without break must be flagged")
