extends RefCounted
## T-29025: open only the current installed pack's inventoried original data.
## Prepared resources do not grant BGM, full-scene or gameplay readiness.
const Profile = preload("res://services/installed_launch_profile.gd")
const ExternalPath = preload("res://services/external_content_path.gd")
const Text = preload("res://services/prologue_text_store.gd")
const MediaStore = preload("res://services/first_arrival_media_store.gd")
const BoardStore = preload("res://services/first_arrival_board_store.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
const CommandReload = preload("res://services/command_reload_store.gd")
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Painter = preload("res://presentation/prologue_text_presenter.gd")
const Audio = preload("res://services/audio_service.gd")
const Lines = preload("res://services/prologue_line_services.gd")
const CommandCapabilities = preload("res://services/school_day_command_capabilities.gd")
var text: Text
var media_store: MediaStore
var board_store: BoardStore
## Stage21 COM_MIX/SPRING sector reads (installed first_arrival_finish_bank
## role); null without that role, which keeps prologue_finish_run case 0 an
## explicit rejection.
var finish_bank: FinishBank
## Mode 0x34 command reload reads (installed first_arrival_command_reload
## role); null without that role, which keeps the reload's CALROM and room
## reads explicit rejections.
var command_reload: CommandReload
## Inventoried command HUD manifest (with its identity), font root and rule
## tables (FILE_ROLES hud_manifest / rule_tables); empty without them.
var hud_manifest: Dictionary = {}
## Ordinary weekly command capabilities (school_day_media, spring_media,
## weekday56_media, club_media roles) for the school-day controller after
## command select. Opened at launch but not fatal: a closed capability lists
## no command, and its reason stays in school_day_commands.error.
var school_day_commands: CommandCapabilities
## T-39002: installed sources of the Sunday holiday after command select: the
## school_day_media pack (a v5 pack carries the holiday components) and the
## holiday_scene_source profile. Empty without either role.
var holiday_sources: Dictionary = {}
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
	var next_board: BoardStore = null
	var next_finish: FinishBank = null
	var next_reload: CommandReload = null
	var next_hud := Hud.new()
	var next_painter := Painter.new()
	var next_audio := Audio.new()
	var next_lines := Lines.new()
	var error: String = next_text.open(fresh.paths.prologue_text,fresh.role_manifests.prologue_text.sha256)
	if error.is_empty(): error = next_media.open(fresh.paths.first_arrival_media,fresh.role_manifests.first_arrival_media.sha256)
	if error.is_empty() and fresh.paths.has("first_arrival_board"):
		next_board = BoardStore.new()
		error = next_board.open_pack(fresh.paths.first_arrival_board,fresh.role_manifests.first_arrival_board.sha256)
	if error.is_empty() and fresh.paths.has("first_arrival_finish_bank"):
		# The inventoried finish_bank.json must be the file the store parses.
		if FileAccess.get_sha256(fresh.role_manifests.first_arrival_finish_bank.path) != fresh.role_manifests.first_arrival_finish_bank.sha256:
			error = "first_arrival_finish_bank manifest differs from the installed inventory"
		else:
			next_finish = FinishBank.new()
			error = next_finish.open_pack(fresh.paths.first_arrival_finish_bank)
	if error.is_empty() and fresh.paths.has("first_arrival_command_reload"):
		if FileAccess.get_sha256(fresh.role_manifests.first_arrival_command_reload.path) != fresh.role_manifests.first_arrival_command_reload.sha256:
			error = "first_arrival_command_reload manifest differs from the installed inventory"
		else:
			next_reload = CommandReload.new()
			error = next_reload.open_pack(fresh.paths.first_arrival_command_reload)
	var next_hud_manifest: Dictionary = {}
	if error.is_empty() and fresh.paths.has("hud_manifest") and fresh.paths.has("font_root") and fresh.paths.has("rule_tables") and fresh.file_identities.has("hud_manifest") and fresh.file_identities.has("rule_tables"):
		next_hud_manifest = {"manifest":fresh.paths.hud_manifest,"sha256":fresh.file_identities.hud_manifest.sha256,
			"font_root":fresh.paths.font_root,"rule_tables":fresh.paths.rule_tables,"rule_tables_sha256":fresh.file_identities.rule_tables.sha256}
		# Scaled hover-pulse texels (func_80098680 path); optional role.
		if fresh.paths.has("command_hover_texels"):
			next_hud_manifest["hover_texels"] = {"path":fresh.role_manifests.command_hover_texels.path,
				"sha256":fresh.role_manifests.command_hover_texels.sha256}
	if error.is_empty(): error = next_hud.call("open_installed",fresh.profile_root,fresh.role_manifests.prologue_hud.relative,fresh.role_manifests.prologue_hud.sha256)
	if error.is_empty(): error = next_hud.snapshot_runtime_assets()
	if error.is_empty(): error = next_painter.open_font(fresh.paths.font_root)
	if error.is_empty(): error = next_audio.configure_opening_pack(fresh.paths.opening_pack)
	if error.is_empty(): error = next_audio.prepare_opening_bank_resources()
	if error.is_empty() and fresh.paths.has("opening_audio"):
		error = next_audio.configure_original_opening_audio_pack(fresh.paths.opening_audio,fresh.role_manifests.opening_audio.sha256)
		if error.is_empty():
			error = _prepare_first_arrival_voiced_lines(next_lines,next_text,next_painter,next_audio)
	if not error.is_empty(): return "Installed opening data could not be prepared: "+error
	text = next_text
	media_store = next_media
	board_store = next_board
	finish_bank = next_finish
	command_reload = next_reload
	hud_manifest = next_hud_manifest
	school_day_commands = CommandCapabilities.new()
	school_day_commands.open_installed(fresh.paths)
	holiday_sources = {}
	if fresh.paths.has("school_day_media") and fresh.paths.has("holiday_scene_source"):
		holiday_sources = {"school_day_media":fresh.paths.school_day_media,"holiday_scene_source":fresh.paths.holiday_scene_source}
	hud = next_hud
	painter = next_painter
	audio = next_audio
	lines = next_lines
	launch_profile = fresh.duplicate(true)
	return ""

static func _prepare_first_arrival_voiced_lines(next_lines: Lines, next_text: Text, next_painter: Painter, next_audio: Audio) -> String:
	var lines: Dictionary = {}
	var line_levels: Dictionary = {}
	var clip_levels_by_key: Dictionary = {}
	# Every prologue line through prologue_finish_run, as Lines.prepare:
	# 0..42, and the stage15 built answers 100/101 (prologue_line keeps 0x18).
	var ids: Array = range(43)
	ids.append_array([100,101])
	for line: int in ids:
		var prepared: Dictionary = next_audio.prepare_original_line(line)
		if not str(prepared.get("error", "")).is_empty():
			return str(prepared.error)
		if prepared.get("line_id") != line or not prepared.get("clips") is Array:
			return "prepared original audio line does not match the source identity"
		var per_clip_levels: Array = []
		for clip: Dictionary in prepared["clips"]:
			var req_key: String = str(clip.get("request_key", ""))
			var levels: Array[int] = []
			if clip_levels_by_key.has(req_key):
				levels = clip_levels_by_key[req_key]
			else:
				var resolved: Dictionary = next_audio.resolve_original_audio_event(clip)
				if not str(resolved.get("error", "")).is_empty():
					return str(resolved["error"])
				var stream: AudioStreamWAV = resolved.get("stream")
				levels = Lines.VoicePlayback.compute_stream_cd_levels(stream)
				if levels.is_empty():
					return "prepared original audio clip produced empty getCDlevel() envelope: " + req_key
				clip_levels_by_key[req_key] = levels
			per_clip_levels.append(levels)
		lines[line] = prepared.duplicate(true)
		line_levels[line] = per_clip_levels
	Lines.AudioRules.prepare_mid_prologue_bank_roles(next_audio)
	next_lines._store = next_text
	next_lines._painter = next_painter
	next_lines._audio_lines = lines
	next_lines._line_clip_levels = line_levels
	next_lines._clip_cd_levels = clip_levels_by_key
	next_lines._voiced_scope_prepared = true
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
	if is_same(a, b): return true
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
