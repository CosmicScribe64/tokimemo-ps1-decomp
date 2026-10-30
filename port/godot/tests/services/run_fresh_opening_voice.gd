extends SceneTree
## T-39002: decoded original voice acceptance, not an audible timing witness.
const Service = preload("res://services/audio_service.gd")
const Bridge = preload("res://services/opening_audio_bridge.gd")
const EXPECTED_SHA := "90c2277e4fc0c6bedf1f2dfc55cc6740be889f28669c8dfa58a81d92d213daf3"

func _initialize() -> void:
	var root := OS.get_environment("TOKIMEMO_ORIGINAL_OPENING_AUDIO_PACK")
	var service = Service.new()
	var error: String = service.configure_original_opening_audio_pack(root, EXPECTED_SHA)
	if not error.is_empty():
		printerr(error)
		quit(1)
		return
	var voiced := 0
	var bridge := Bridge.new()
	bridge.audio = service
	for line: int in range(43):
		var event: Dictionary = service.original_opening_line_event(line)
		if event.get("kind") == "voice_silent":
			continue
		for request: Dictionary in event["xa_requests"]:
			var schema_error := bridge._schema({"kind":"voice_play", "voice_id":line,
				"speaker":0, "xa_request":request, "request_key":"voice-test",
				"epoch":1, "line_id":line, "clip_index":0})
			if not schema_error.is_empty():
				printerr(schema_error)
				quit(1)
				return
		var resolved: Dictionary = service.resolve_original_audio_event(event)
		if not str(resolved.get("error", "")).is_empty():
			printerr(resolved)
			quit(1)
			return
		for stream: AudioStreamWAV in resolved["streams"]:
			if stream.data.is_empty() or stream.get_length() <= 0.0:
				printerr("Original voice is empty")
				quit(1)
				return
		voiced += 1
	if voiced < 16:
		printerr("Expected original voiced lines were not registered")
		quit(1)
		return
	var bad: Dictionary = service.original_opening_line_event(5)
	bad["xa_requests"][0]["channel"] = 255
	if str(service.resolve_original_audio_event(bad).get("error", "")).is_empty():
		printerr("Unknown native request was accepted")
		quit(1)
		return
	print("PASS: 43 original lines, %d voiced, incorrect request rejected" % voiced)
	quit(0)
