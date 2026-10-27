extends RefCounted
## T-29025: open only the current installed pack's inventoried original data.
## Prepared resources do not grant BGM, full-scene or gameplay readiness.
const Profile = preload("res://services/installed_launch_profile.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const Text = preload("res://services/prologue_text_store.gd")
const MediaStore = preload("res://services/first_arrival_media_store.gd")
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const Lines = preload("res://services/prologue_line_services.gd")
var text: Text
var media_store: MediaStore
var hud: Hud
var painter: Painter
var audio: Audio
var lines: Lines
var launch_profile: Dictionary = {}

func prepare(profile: Dictionary) -> String:
	if not launch_profile.is_empty(): return "installed opening resources already own their authenticated pack"
	var checked := current_profile(profile)
	if not checked.error.is_empty(): return checked.error
	var fresh: Dictionary = checked.profile
	var next_text := Text.new()
	var next_media := MediaStore.new()
	var next_hud := Hud.new()
	var next_painter := Painter.new()
	var next_audio := Audio.new()
	var next_lines := Lines.new()
	var error: String = next_text.open(fresh.paths.prologue_text,fresh.role_manifests.prologue_text.sha256)
	if error.is_empty(): error = next_media.open(fresh.paths.first_arrival_media,fresh.role_manifests.first_arrival_media.sha256)
	if error.is_empty(): error = next_hud.call("open_installed",fresh.profile_root,fresh.role_manifests.prologue_hud.relative,fresh.role_manifests.prologue_hud.sha256)
	if error.is_empty(): error = next_hud.snapshot_runtime_assets()
	if error.is_empty(): error = next_painter.open_font(fresh.paths.font_root)
	if error.is_empty(): error = next_audio.configure_opening_pack(fresh.paths.opening_pack)
	if error.is_empty(): error = next_audio.prepare_opening_bank_resources()
	if error.is_empty() and fresh.paths.has("opening_audio"):
		error = next_audio.configure_original_opening_audio_pack(fresh.paths.opening_audio,fresh.role_manifests.opening_audio.sha256)
		if error.is_empty(): error = next_lines.prepare(next_text,next_painter,next_audio)
	if not error.is_empty(): return "Installed opening data could not be prepared: "+error
	text = next_text
	media_store = next_media
	hud = next_hud
	painter = next_painter
	audio = next_audio
	lines = next_lines
	launch_profile = fresh.duplicate(true)
	return ""

static func current_profile(profile: Dictionary) -> Dictionary:
	var root: Variant = profile.get("profile_root")
	var pin: Variant = profile.get("installation_manifest_sha256")
	if not profile.get("valid") is bool or not profile.valid or not root is String or not ExternalPath.is_external_path(root) or not pin is String or not Profile._sha(pin):
		return {"error":"New Game requires the current validated installed pack"}
	var manifest_path: String = root.path_join("manifest.json")
	if not ExternalPath.is_external_file(manifest_path) or FileAccess.get_size(manifest_path) > 8*1024*1024 or FileAccess.get_sha256(manifest_path) != pin:
		return {"error":"Installed New Game data changed after launch validation"}
	var manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(manifest_path))
	if not manifest is Dictionary: return {"error":"Installed New Game manifest is malformed"}
	var fresh := Profile.read_pack(root,manifest)
	if not fresh.get("valid",false): return {"error":str(fresh.get("message","Installed New Game pack could not be validated"))}
	if not fresh.available_capabilities.has(Profile.ENTRY_CAP) or not fresh.available_capabilities.has(Profile.OPENING_RESOURCES_CAP):
		return {"error":"Installed game data lacks the original opening resources; import the game again"}
	for key: String in ["profile_root","paths","role_manifests","installation_manifest_sha256"]:
		if not profile.has(key) or not _same(profile[key],fresh[key]): return {"error":"Opening resource locators belong to another or stale installed pack"}
	return {"error":"","profile":fresh}

static func _same(a: Variant, b: Variant) -> bool:
	if typeof(a) != typeof(b): return false
	if a is Dictionary:
		if a.size() != b.size(): return false
		for key: Variant in a:
			if not b.has(key) or not _same(a[key],b[key]): return false
		return true
	if a is Array:
		if a.size() != b.size(): return false
		for index in a.size():
			if not _same(a[index],b[index]): return false
		return true
	return a == b
