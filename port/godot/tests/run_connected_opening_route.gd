extends "res://tests/run_new_game_entry.gd"

const Adapter = preload("res://flow/new_game_state_adapter.gd")
const State = preload("res://rules/new_game_route_state.gd")
const Arrival = preload("res://flow/prologue_arrival.gd")
const Reload = preload("res://flow/new_game_command_reload.gd")
const Memory = preload("res://services/native_vab_memory.gd")
const AudioService = preload("res://services/audio_service.gd")
const PlayableBoot = preload("res://app/playable_boot.gd")
const Controller = preload("res://scenes/school_day_controller.gd")
const HolidayController = preload("res://scenes/holiday_controller.gd")
const HolidaySceneAdapter = preload("res://services/holiday_scene_adapter.gd")
const HolidayHud = preload("res://presentation/holiday_hud_adapter.gd")
const LiveHud = preload("res://presentation/live_hud_adapter.gd")
const CommandHudPresenter = preload("res://presentation/command_hud_presenter.gd")
const HudSeason = preload("res://services/native_hud_season.gd")
const ProloguePresenter = preload("res://presentation/prologue_presenter.gd")
const MediaAdapter = preload("res://services/ordinary_command_media_adapter.gd")
const ExactMedia = preload("res://services/native_activity_media.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const SaveService = preload("res://services/save_service.gd")
const NativePad = preload("res://ui/native_pad.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Math = preload("res://core/native_math.gd")

const CONFIRM_BUTTON_MASK: int = 0x20
const RIGHT_BUTTON_MASK: int = 0x2000
const LEFT_BUTTON_MASK: int = 0x8000
const SFX_ID_14_CONFIRM_CMD: int = 0x050e


static func _resolve_repo_root() -> String:
	for env_key: String in ["TOKIMEMO_ROOT"]:
		var env_val: String = OS.get_environment(env_key).simplify_path()
		if not env_val.is_empty() and DirAccess.dir_exists_absolute(env_val.path_join(".asset-cache")):
			return env_val
	for candidate: String in [
		"/workspace",
		ProjectSettings.globalize_path("res://../..").simplify_path(),
		ProjectSettings.globalize_path("res://../../../../..").simplify_path(),
		"/repo",
		"/work",
	]:
		if DirAccess.dir_exists_absolute(candidate.path_join(".asset-cache")):
			return candidate
	return ProjectSettings.globalize_path("res://../..").simplify_path()


static func _resolve_opening_pack(repo_root: String) -> String:
	var env_pack: String = OS.get_environment("TOKIMEMO_OPENING_PACK").simplify_path()
	if not env_pack.is_empty() and FileAccess.file_exists(env_pack.path_join("opening.json")):
		return env_pack
	for rel: String in [
		".asset-cache/port/installed-clean-v1/packs/opening_Arrival",
		".asset-cache/port/installed-clean-v1/packs/opening_pack",
		".asset-cache/port/connected-opening-v2",
	]:
		var cand: String = repo_root.path_join(rel)
		if FileAccess.file_exists(cand.path_join("opening.json")):
			return cand
	return env_pack


static func _resolve_tables_path(repo_root: String) -> String:
	var env_tables: String = OS.get_environment("TOKIMEMO_TABLES").simplify_path()
	if not env_tables.is_empty() and FileAccess.file_exists(env_tables):
		return env_tables
	var fallback: String = repo_root.path_join(".asset-cache/port/school-day-tables-v1-final/manifest.json")
	if FileAccess.file_exists(fallback):
		return fallback
	return env_tables


static func _resolve_installed_root(repo_root: String) -> String:
	var env_inst: String = OS.get_environment("TOKIMEMO_INSTALLED_ROOT").simplify_path()
	if not env_inst.is_empty() and DirAccess.dir_exists_absolute(env_inst):
		return env_inst
	return repo_root.path_join(".asset-cache/port/installed-clean-v1")


static func _make_holiday_extension_dict() -> Dictionary:
	return {
		"icon_mode": 0,
		"icon_count": 14,
		"input_device": 1,
		"screen_brightness": 128,
		"tile_layer_enabled": 0,
		"boot_hold": 0,
		"background_brightness": 128,
		"clubs": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
		"dial_keys": [255, 255, 255, 255, 255, 255, 255, 0],
		"dial_pos": 0,
	}


static func _attach_week_controllers(
	session: Session,
	route_state: State,
	common_tables: Dictionary,
	holiday_tables: Dictionary,
	audio_svc: AudioService,
	pending_services: Array,
	exact_media: ExactMedia = null,
	actor_service: ActorService = null,
	media_adapter: MediaAdapter = null,
	holiday_scene: HolidaySceneAdapter = null,
	staged_commands: Array = []
) -> Dictionary:
	var controller: Controller = Controller.new(common_tables)
	if not controller.error.is_empty():
		return {"error": controller.error}
	controller.command_capabilities.assign([0])
	controller.opening_services = PlayableBoot.build_opening_services(audio_svc)
	route_state.rule_state["allow_multi_week"] = true
	route_state.rule_state["resources"]["available_commands"] = [0]
	route_state.rule_state["resources"]["available_holiday_commands"] = [0]
	if not (route_state.rule_state.get("holiday") is Dictionary):
		route_state.rule_state["holiday"] = holiday_scene.initial_extension() if holiday_scene != null else _make_holiday_extension_dict()

	var holiday_ctrl: HolidayController = HolidayController.new(common_tables, holiday_tables)
	if not holiday_ctrl.error.is_empty():
		return {"error": holiday_ctrl.error}

	var attach_err: String = controller.attach_multi_week(session, route_state)
	if not attach_err.is_empty():
		return {"error": attach_err}
	var h_attach_err: String = holiday_ctrl.attach(session, route_state)
	if not h_attach_err.is_empty():
		return {"error": h_attach_err}

	session.draw_logic = func(base, _r, res: Session.FrameResult) -> void:
		var cur: State = base as State
		res.error = HudSeason.draw_step(cur, res)
		if not res.error.is_empty():
			return
		staged_commands.clear()
		if cur.game_mode == 0x46 or (holiday_scene != null and holiday_scene.handles(cur)):
			if holiday_scene != null:
				var staged: Dictionary = holiday_scene.stage(cur, session.clock.logic_iterations, res)
				if not str(staged.get("error", "")).is_empty():
					res.error = str(staged["error"])
					return
				staged_commands.append_array(staged.get("draw_commands", []))
				for svc_desc: Dictionary in staged.get("services", []):
					pending_services.append(svc_desc)
				return
			for intent: Dictionary in res.presentation:
				if intent.get("kind") == "holiday_load":
					var req: String = str(intent.get("request", ""))
					var payload: Dictionary = {
						"request": req,
						"lba": int(intent.get("lba", 0)),
						"ready": true,
					}
					if req != "sd":
						payload["decode_result"] = 1
					pending_services.append({"service": "holiday.resources", "payload": payload})
			return

		var data: Dictionary = cur.rule_state
		for intent: Dictionary in res.presentation:
			var kind: String = str(intent.get("kind", ""))
			if kind == "school_day_load":
				var req: String = str(intent.get("request", ""))
				pending_services.append({
					"service": "school_day.resources",
					"payload": {
						"request": req,
						"ready": true,
						"lba": int(intent["background_lba"]) if req == "background" else int(intent["sd_lba"]),
						"decode_result": 1,
						"checksum_ok": true,
					},
				})
			elif kind == "school_day_actors" and exact_media != null and actor_service != null:
				var selected: Dictionary = exact_media.resolve(cur)
				if str(selected.get("error", "")).is_empty():
					var init_out: Dictionary = actor_service.initialize(intent["actors"], selected["resources"])
					if str(init_out.get("error", "")).is_empty():
						data["actor_state"] = init_out["actor_state"]
			elif kind == "holiday_load":
				var h_req: String = str(intent.get("request", ""))
				var h_payload: Dictionary = {
					"request": h_req,
					"lba": int(intent.get("lba", 0)),
					"ready": true,
				}
				if h_req != "sd":
					h_payload["decode_result"] = 1
				pending_services.append({"service": "holiday.resources", "payload": h_payload})

		if cur.game_mode in [0x40, 0x31, 0x32, 0x38] and media_adapter != null:
			var shown_lba: int = int(data["background_cache"][data["background_show"]])
			if media_adapter.command_backgrounds.has(0) and shown_lba == MediaAdapter.MEDIA_ROUTES[0]["background_lba"]:
				staged_commands.append_array(media_adapter.command_backgrounds[0])
			elif holiday_scene != null and holiday_scene.backgrounds.has(shown_lba):
				staged_commands.append_array(holiday_scene.backgrounds[shown_lba])

		if not data["actor_state"].is_empty():
			if exact_media != null and actor_service != null and cur.game_mode == 0x40:
				var selected: Dictionary = exact_media.resolve(cur)
				if str(selected.get("error", "")).is_empty():
					var drawn: Dictionary = actor_service.native_draw(
						data["actor_state"],
						selected["resources"],
						session.clock.logic_iterations,
						Vector2i(160, 120)
					)
					if str(drawn.get("error", "")).is_empty():
						data["actor_state"] = drawn["actor_state"]
						staged_commands.append_array(drawn.get("draw_commands", []))
						for svc_desc: Dictionary in drawn.get("services", []):
							pending_services.append(svc_desc)
						return
			if cur.game_mode == 0x40 and cur.mode_stage == 1 and data["actor_state"].has("96"):
				var actor96: Dictionary = data["actor_state"]["96"]
				if (int(actor96.get("flags", 0)) & 1) != 0:
					pending_services.append({
						"service": "school_day.actor96",
						"payload": {
							"active": false,
							"counter": 0,
							"sequence_slot": int(actor96.get("sequence_slot", 0)),
						},
					})

	session.cd_logic = func(_s, _r, _res) -> void:
		pass
	session.audio_logic = func(_s, _r, _res) -> void:
		pass

	return {
		"error": "",
		"controller": controller,
		"holiday_controller": holiday_ctrl,
	}


static func _detach_session(session: Session, attach_dict: Dictionary) -> void:
	if session != null:
		session.draw_logic = Callable()
		session.cd_logic = Callable()
		session.audio_logic = Callable()
		session.service_completion = Callable()
		session.final_precommit = Callable()
		session.router = Session.ModeRouter.new()
	if attach_dict.get("controller") != null:
		attach_dict["controller"].opening_services.clear()
	attach_dict.clear()


static func _step_session_with_pending(
	session: Session,
	held_mask: int,
	pending_services: Array
) -> Session.FrameResult:
	var tick: int = session.clock.logic_iterations
	var events: Array[Session.ServiceEvent] = []
	for desc: Dictionary in pending_services:
		events.append(Session.ServiceEvent.new(
			tick,
			Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION,
			str(desc["service"]),
			desc["payload"]
		))
	pending_services.clear()
	return session.step(InputFrame.new(tick, held_mask), events)


static func _non_zero_pcm_samples(data: PackedByteArray) -> int:
	var count: int = 0
	var limit: int = mini(data.size() - 1, 8192)
	for offset: int in range(0, limit, 2):
		if data[offset] != 0 or data[offset + 1] != 0:
			count += 1
	return count


static func _non_black_pixels(image: Image) -> int:
	if image == null or image.is_empty():
		return 0
	var count: int = 0
	for y: int in range(0, image.get_height(), 4):
		for x: int in range(0, image.get_width(), 4):
			var c: Color = image.get_pixel(x, y)
			if c.r8 > 4 or c.g8 > 4 or c.b8 > 4:
				count += 1
	return count


func _initialize() -> void:
	print("--- Running Connected Opening Route Test Suite ---")

	var repo_root: String = _resolve_repo_root()
	var installed_root: String = _resolve_installed_root(repo_root)
	var pack_path: String = _resolve_opening_pack(repo_root)
	var table_path: String = _resolve_tables_path(repo_root)
	var receipt_path: String = OS.get_environment("TOKIMEMO_WITNESS_RECEIPT")
	var xa_audio_pack: String = OS.get_environment("TOKIMEMO_ORIGINAL_OPENING_AUDIO_PACK").simplify_path()
	if xa_audio_pack.is_empty() or not FileAccess.file_exists(xa_audio_pack.path_join("opening-audio.json")):
		xa_audio_pack = installed_root.path_join("packs/opening_audio")
	var xa_audio_sha: String = OS.get_environment("TOKIMEMO_ORIGINAL_OPENING_AUDIO_SHA")
	if xa_audio_sha.is_empty():
		xa_audio_sha = AudioService.DEFAULT_OPENING_AUDIO_SHA

	check(not pack_path.is_empty() and FileAccess.file_exists(pack_path.path_join("opening.json")), "Auto-resolved opening pack path: " + pack_path)
	check(not table_path.is_empty() and FileAccess.file_exists(table_path), "Auto-resolved school-day rule tables path: " + table_path)
	check(DirAccess.dir_exists_absolute(installed_root), "Auto-resolved installed-clean-v1 root: " + installed_root)
	# T-39002: unavailable original content is a failed prerequisite, not a
	# synthetic route pass or a later JSON error that leaves Godot running.
	check(FileAccess.file_exists(xa_audio_pack.path_join("opening-audio.json")), "Original opening XA audio pack is required; set TOKIMEMO_ORIGINAL_OPENING_AUDIO_PACK")
	var installed_manifest_path := installed_root.path_join("manifest.json")
	var installed_manifest: Variant = JSON.parse_string(FileAccess.get_file_as_string(installed_manifest_path)) if FileAccess.file_exists(installed_manifest_path) else null
	check(installed_manifest is Dictionary, "Installed runtime manifest must be present and valid JSON")
	if installed_manifest is Dictionary:
		var installed_roles: Dictionary = installed_manifest.get("roles", {})
		for role: String in ["rule_tables", "school_day_media", "spring_media", "hud_manifest", "hud_context", "holiday_scene", "holiday_text"]:
			check(installed_roles.has(role), "Installed runtime requires role: " + role)
	if failures:
		printerr("Connected opening prerequisites failed; gameplay was not exercised.")
		quit(1)
		return

	var rng: Rng = Rng.new()
	rng.configure(Rng.OPENBIOS_PROFILE, 19950404)

	# =========================================================================
	# Step 1: Committed Name Entry
	# =========================================================================
	var entry: Entry = Entry.new()
	entry.begin(invented_tables(), invented_content(), rng)
	finish_names(entry, rng, true)

	for kind: String in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(entry, kind)

	check(entry.stage == 2 and entry.step_id == 5, "Name entry reached committed confirmation stage (stage 2, step 5)")

	# =========================================================================
	# Step 2: Fresh Route State Adapter
	# =========================================================================
	var initial_mascot: int = 0
	var opening_pack: Dictionary = {}
	if not pack_path.is_empty():
		var json_text: String = FileAccess.get_file_as_string(pack_path + "/opening.json")
		if not json_text.is_empty():
			opening_pack = JSON.parse_string(json_text)
			if opening_pack.has("constants") and opening_pack["constants"].has("initial_mascot"):
				initial_mascot = int(opening_pack["constants"]["initial_mascot"])

	var adapter_outcome: Adapter.Outcome = Adapter.from_entry(entry, 1000, 2000, {"initial_mascot": initial_mascot})
	check(adapter_outcome.error.is_empty(), "State adapter successfully materialized fresh route: " + adapter_outcome.error)
	var state: State = adapter_outcome.state
	check(state != null, "State adapter produced valid State object")
	check(state.game_mode == 0x15 and state.mode_stage == 0 and state.mode_step == 0, "Initial game mode is 0x15 stage 0 step 0")
	check(state.rule_state["year"] == 95 and state.rule_state["month"] == 4 and state.rule_state["day"] == 4, "Initial rule date is 1995-04-04")
	check(state.opening["hud_mascot_choice"] == initial_mascot, "Initial mascot matches authenticated opening constant")
	check(not state.opening["command_ready"], "Command ready is false at beginning of opening")

	# =========================================================================
	# Step 3: Prologue Arrival (Stage 0) with Native VAB Memory (START Skip Path)
	# =========================================================================
	var memory: Memory = Memory.new()
	var service_calls: Array = []
	var bank_installed: Dictionary = {
		"prologue_arrival": false,
		"normal_gameplay": false,
	}

	var install_bank_role: Callable = func(role: String) -> bool:
		if not opening_pack.is_empty() and opening_pack.has("bank_roles") and opening_pack["bank_roles"].has(role):
			var assets: Dictionary = opening_pack["bank_roles"][role]["assets"]
			var se_h: Dictionary = assets["se_header"]
			var se_b: Dictionary = assets["se_body"]
			var mu_h: Dictionary = assets["music_header"]
			var mu_b: Dictionary = assets["music_body"]

			var se_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-se_header.bin").slice(0, int(se_h["size"]))
			var se_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-se_body.bin").slice(0, int(se_b["size"]))
			var mu_header_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-music_header.bin").slice(0, int(mu_h["size"]))
			var mu_body_bytes: PackedByteArray = FileAccess.get_file_as_bytes(pack_path + "/reads/" + role + "-music_body.bin").slice(0, int(mu_b["size"]))

			var res0: Memory.Outcome = memory.install(0, 0x1010, 0x10000, se_header_bytes, se_body_bytes, {"header_sha256": se_h["sha256"], "body_sha256": se_b["sha256"]})
			var res1: Memory.Outcome = memory.install(1, 0x11010, 0x28000, mu_header_bytes, mu_body_bytes, {"header_sha256": mu_h["sha256"], "body_sha256": mu_b["sha256"]})
			if res0.installed and res1.installed:
				bank_installed[role] = true
				return true
			return false
		bank_installed[role] = true
		return true

	var cd_sync_done: bool = true
	var services: Dictionary = {
		"bank_load_start": func(_s: State, role: String, _r: Session.FrameResult) -> void:
			service_calls.append(["bank_load_start", role])
			install_bank_role.call(role),
		"bank_poll": func(s: State, _r: Session.FrameResult) -> int:
			service_calls.append("bank_poll")
			if s.game_mode == 0x15:
				return 1 if bank_installed["prologue_arrival"] else 0
			return 1 if bank_installed["normal_gameplay"] else 0,
		"bank_start": func(_s: State, a: int, b: int, _r: Session.FrameResult) -> void:
			service_calls.append(["bank_start", a, b]),
		"scene_reset": func(_s: State, _r: Session.FrameResult) -> void:
			service_calls.append("scene_reset"),
		"background_read": func(_s: State, lba: int, arg: int, _r: Session.FrameResult, retry: bool = false) -> void:
			service_calls.append(["bg_read", lba, arg, retry]),
		"cd_sync": func(_s: State, _r: Session.FrameResult) -> int:
			service_calls.append("cd_sync")
			return 1 if cd_sync_done else 0,
		"background_decode": func(_s: State, arg: int, _r: Session.FrameResult) -> int:
			service_calls.append(["bg_decode", arg])
			return 0,
		"actor_read": func(_s: State, count: int, dest: int, lba: int, _r: Session.FrameResult) -> void:
			service_calls.append(["actor_read", count, dest, lba]),
		"actor_palette_setup": func(_s: State, _r: Session.FrameResult) -> void:
			service_calls.append("actor_palette_setup"),
		"line_start": func(_s: State, line: int, _r: Session.FrameResult) -> void:
			service_calls.append(["line_start", line]),
		"line_run": func(s: State, _r: Session.FrameResult) -> void:
			service_calls.append("line_run")
			s.substep += 1,
		"arrival_draw": func(_s: State, arg: int, _r: Session.FrameResult) -> void:
			service_calls.append(["arrival_draw", arg]),
		"outer_draw": func(_s: State, a: Variant = null, _b: Variant = null) -> void:
			service_calls.append(["outer_draw", a]),
		"prologue_sine": Math.rsin,
		"finish_cd_reset": func(_s: State, _r: Session.FrameResult) -> void:
			service_calls.append("finish_cd_reset"),
		"finish_bank_reset": func(_s: State, _r: Session.FrameResult) -> void:
			service_calls.append("finish_bank_reset"),
		"audio_command": func(_s: State, cmd: int, _r: Session.FrameResult) -> void:
			service_calls.append(["audio_command", cmd]),
		"setup_reset": func(s: State, _r: Session.FrameResult) -> void:
			service_calls.append("setup_reset")
			s.next_step(),
		"hud_read_start": func(_s: State, n: int, d: int, l: int, _r: Session.FrameResult) -> void:
			service_calls.append(["hud_read_start", n, d, l]),
		"calendar_upload": func(_s: State, y: int, m: int, b: int, _r: Session.FrameResult) -> void:
			service_calls.append(["calendar_upload", y, m, b]),
		"mascot_set": func(_s: State, n: int, on: int, _r: Session.FrameResult) -> void:
			service_calls.append(["mascot_set", n, on]),
		"command_build": func(s: State, _r: Session.FrameResult) -> void:
			service_calls.append("command_build")
			Reload.build_command_screen(s),
		"room_finish": func(s: State, _r: Session.FrameResult) -> void:
			service_calls.append("room_finish")
			Reload.finish_room_load(s),
		"frame_draw": func(_s: State, title: bool = false, _r: Variant = null) -> void:
			service_calls.append(["frame_draw", title]),
	}

	var res: Session.FrameResult = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty(), "Prologue step 0 succeeded: " + res.error)
	check(state.mode_step == 1 and state.substep == 0, "Advanced to step 1 substep 0")
	check(bank_installed["prologue_arrival"], "Prologue arrival VAB sound bank installed into software sound RAM")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue step 1 substep 0 reset scene and advanced to substep 1")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 2, "Prologue step 1 substep 1 confirmed bank ready and started bg read")

	while state.substep == 2:
		Arrival.step(state, rng, 0, services)
	check(state.substep == 3, "Prologue step 1 substep 2 timer elapsed and advanced to substep 3")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 4, "Prologue step 1 substep 3 decoded background and started actor read")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.mode_step == 2 and state.substep == 0, "Prologue step 1 substep 4 completed and advanced to step 2")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue step 2 substep 0 started dialogue line")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 2, "Prologue step 2 substep 1 completed line execution")

	res = Arrival.step(state, rng, 0x800, services)
	check(res.error.is_empty() and state.mode_stage == 21, "START button pressed transitioned to prologue finish stage 21")

	# =========================================================================
	# Step 4: Prologue Finish (Stage 21) -> Command Screen Transition
	# =========================================================================
	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty() and state.substep == 1, "Prologue finish substep 0 initiated normal gameplay bank install")
	check(bank_installed["normal_gameplay"], "Normal gameplay bank COM_MIX + SPRING successfully installed into software sound RAM")

	res = Arrival.step(state, rng, 0, services)
	check(res.error.is_empty(), "Prologue finish substep 1 succeeded: " + res.error)
	check(state.game_mode == 0x34 and state.mode_stage == 0xff and state.mode_step == 0, "Transitioned to command screen mode 0x34 stage FF step 0")

	# =========================================================================
	# Step 5: Command Screen Mode Stage Reload (0x34 Stage 0xFF)
	# =========================================================================
	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 1 and state.substep == 0, "Command reload step 0 reset setup and advanced to step 1")

	res = Reload.step(state, services)
	check(res.error.is_empty() and state.substep == 1, "Command reload step 1 verified normal gameplay bank ready and advanced")

	res = Reload.step(state, services)
	check(res.error.is_empty() and state.substep == 2, "Command reload step 1 started HUD read")

	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 2, "Command reload step 1 completed calendar upload and advanced to step 2")
	check((int(state.rule_state["menu_mode"]) & ~3) == 0 and state.opening["D_800B5BC8"] == 0xfe, "Calendar upload applied exact menu mode mask and D_800B5BC8")

	res = Reload.step(state, services)
	check(res.error.is_empty() and state.mode_step == 3, "Command reload step 2 completed command build and advanced to step 3")

	res = Reload.step(state, services)
	check(res.error.is_empty(), "Command reload step 3 room finish executed without error")
	check(state.mode_stage == 1, "Command screen transitioned to Stage 1 (April 4 Command Screen Ready)")

	# =========================================================================
	# Step 6: Final Destination Invariants & Sound RAM Verification
	# =========================================================================
	check(state.game_mode == 0x34, "Final game mode is 0x34 (COMMAND_SCREEN)")
	check(state.mode_stage == 1, "Final mode stage is 1 (Active room command menu)")
	check(state.rule_state["year"] == 95, "Rule state year is 95")
	check(state.rule_state["month"] == 4, "Rule state month is 4 (April)")
	check(state.rule_state["day"] == 4, "Rule state day is 4 (April 4)")
	check(bool(state.opening["command_ready"]), "Command ready flag is true after room finish")
	check(state.rule_state["icon_enabled"] == [1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0], "April 4 command icons match native settings_command_screen_icons_reset(14)")

	if not opening_pack.is_empty():
		check(memory.banks.has(0) and memory.banks.has(1), "Both sound bank slots (SE COM_MIX, BGM SPRING) resident in sound RAM")
		check(memory.banks[0]["address"] == 0x1010 and memory.banks[0]["sample_bytes"] == 0xf3c0, "SE bank slot 0 has COM_MIX at 0x1010 (62,400 bytes)")
		check(memory.banks[1]["address"] == 0x11010 and memory.banks[1]["sample_bytes"] == 0x26d10, "BGM bank slot 1 has SPRING at 0x11010 (158,992 bytes)")

	var checkpoint: Dictionary = state.projection()
	state.opening["screen_brightness"] = 200
	state.restore(checkpoint)
	check(state.projection() == checkpoint, "Full connected route state supports deterministic transaction rollback")

	# =========================================================================
	# Step 7: Full Non-Skip 22-Stage Prologue -> Command Reload -> Playable Week 1 -> First Result -> Sunday Holiday -> Next Selectable Command
	# =========================================================================
	var audio_svc: AudioService = AudioService.new()
	if FileAccess.file_exists(xa_audio_pack.path_join("opening-audio.json")):
		var xa_cfg_err: String = audio_svc.configure_original_opening_audio_pack(xa_audio_pack, xa_audio_sha)
		check(xa_cfg_err.is_empty(), "Configured AudioService with original native XA opening voice pack: " + xa_cfg_err)

	var full_out: Dictionary = PlayableBoot.run_new_game_opening_to_command_ready(
		entry,
		rng,
		pack_path,
		receipt_path,
		false,
		audio_svc
	)
	check(str(full_out.get("error", "")).is_empty(), "PlayableBoot full non-skip 22-stage opening join succeeded: " + str(full_out.get("error", "")))
	if str(full_out.get("error", "")).is_empty():
		var joined_state: State = full_out["state"]
		var visited: Array = full_out["visited_stages"]
		check(visited.size() == 22 and visited[0] == 0 and visited[-1] == 21, "PlayableBoot non-skip opening visited all 22 prologue stages (0..21)")
		check(joined_state.game_mode == 0x34 and joined_state.mode_stage == 1 and bool(joined_state.opening["command_ready"]), "PlayableBoot non-skip opening reached 0x34 stage 1 command_ready")

		# Verify AudioService produced real non-zero PCM audio bytes for PRO_VOL, native XA voiced lines, SfxId 14, and COM_MIX + SPRING
		var pro_vol_stream: AudioStreamWAV = audio_svc.last_decoded_streams.get("bgm.1.0")
		var spring_stream: AudioStreamWAV = audio_svc.last_decoded_streams.get("bgm.0.0")
		check(pro_vol_stream != null and pro_vol_stream.data.size() > 0 and _non_zero_pcm_samples(pro_vol_stream.data) > 100, "AudioService decoded non-zero PCM stream for PRO_VOL prologue BGM")
		check(spring_stream != null and spring_stream.data.size() > 0 and _non_zero_pcm_samples(spring_stream.data) > 100, "AudioService decoded non-zero PCM stream for SPRING normal gameplay BGM")
		check(audio_svc.vab_memory.banks.has(0) and audio_svc.vab_memory.banks[0]["sample_bytes"] == 0xf3c0 and audio_svc.vab_memory.banks[1]["sample_bytes"] == 0x26d10, "AudioService sound RAM holds COM_MIX (0xf3c0) and SPRING (0x26d10)")
		var voiced_count: int = 0
		for v_entry: Dictionary in audio_svc.voice_log:
			if int(v_entry.get("decoded_pcm_bytes", 0)) > 0 and str(v_entry.get("source", "")) == "original_xa_pcm":
				voiced_count += 1
		check(voiced_count >= 16, "AudioService decoded >= 16 native XA voiced lines with non-zero PCM bytes (got %d)" % voiced_count)

		var sfx14_tick: int = int(joined_state.frame_count)
		check(audio_svc.queue_se_play(SFX_ID_14_CONFIRM_CMD, sfx14_tick, AudioService.Phase.AUDIO).is_empty(), "Queued SfxId 14 (0x050e) confirm chime on COM_MIX SE bank")
		var sfx14_drain: Dictionary = audio_svc.drain_phase(sfx14_tick, AudioService.Phase.AUDIO)
		var sfx14_stream: AudioStreamWAV = audio_svc.last_decoded_streams.get("se.0x50e")
		check(str(sfx14_drain.get("error", "")).is_empty() and sfx14_stream != null and sfx14_stream.data.size() > 0 and _non_zero_pcm_samples(sfx14_stream.data) > 20, "AudioService decoded non-zero PCM bytes for SfxId 14 confirm chime")

		var holiday_tables_path: String = repo_root.path_join(".asset-cache/port/holiday-tables-v1/manifest.json")
		var holiday_pack: Dictionary = PlayableBoot._integers(JSON.parse_string(FileAccess.get_file_as_string(holiday_tables_path)))
		var holiday_tables: Dictionary = holiday_pack.get("tables", {})

		if not table_path.is_empty() and FileAccess.file_exists(table_path):
			var table_pack: Variant = PlayableBoot._integers(JSON.parse_string(FileAccess.get_file_as_string(table_path)))
			var pending_step7: Array = []
			var session: Session = Session.new()
			session.rng.configure(Rng.OPENBIOS_PROFILE, rng.state)
			var attach_out: Dictionary = _attach_week_controllers(
				session,
				joined_state,
				table_pack["tables"],
				holiday_tables,
				audio_svc,
				pending_step7
			)
			check(str(attach_out.get("error", "")).is_empty(), "Attached joined New Game state to SchoolDayController + HolidayController session: " + str(attach_out.get("error", "")))

			# Verify held confirm (0x20) across 21 -> 0x34 handoff requires a clean release before latching command selection
			session.latched_input = InputFrame.new(-1, CONFIRM_BUTTON_MASK, CONFIRM_BUTTON_MASK)
			var held_handoff_step: Session.FrameResult = _step_session_with_pending(session, CONFIRM_BUTTON_MASK, pending_step7)
			check(held_handoff_step.committed and joined_state.game_mode == 0x34 and joined_state.mode_stage == 1, "Held confirm (0x20) across 21 -> 0x34 handoff does not auto-confirm command 0")
			var release_handoff_step: Session.FrameResult = _step_session_with_pending(session, 0, pending_step7)
			check(release_handoff_step.committed and joined_state.game_mode == 0x34 and joined_state.mode_stage == 1, "Clean button release on 0x34 stage 1 clears held state without premature confirmation")

			# Press confirm button (0x20) after release; next iteration confirms command 0 -> mode 0x30 (schedule_init)
			var step_a: Session.FrameResult = _step_session_with_pending(session, CONFIRM_BUTTON_MASK, pending_step7)
			check(step_a.committed and joined_state.game_mode == 0x34, "Fresh confirm press latched on command screen")
			var step_b: Session.FrameResult = _step_session_with_pending(session, 0, pending_step7)
			check(step_b.committed and joined_state.game_mode == 0x30, "Next tick confirmed command 0 and transitioned to scheduler mode 0x30")

			# Advance scheduler mode 0x30 steps 0..4 -> mode 0x31 -> mode 0x40 (April 5 weekday activity!)
			var safety: int = 0
			while joined_state.game_mode in [0x30, 0x31] and safety < 20:
				var sched_res: Session.FrameResult = _step_session_with_pending(session, 0, pending_step7)
				check(sched_res.committed, "Scheduler/day-screen tick committed: %s" % sched_res.error)
				safety += 1
			check(joined_state.game_mode == 0x40 and joined_state.rule_state["month"] == 4 and joined_state.rule_state["day"] == 5, "Joined New Game state entered playable weekday activity mode 0x40 on April 5, 1995")

			# Continue past April 5 (0x40) through First Result (0x31 day_event_index >= 1 and 0x32 Saturday week-end result) to Sunday 0x46 -> Next Selectable Command 0x34
			var baseline_params_step7: Array = joined_state.rule_state["params"].duplicate()
			var reached_first_weekday_result: bool = false
			var reached_saturday_week_end_0x32: bool = false
			var weekday_activity_days: Array[int] = [5]
			var step7_guard: int = 0
			while not (joined_state.game_mode == 0x46 and joined_state.mode_stage == 1) and step7_guard < 400:
				var held_in: int = CONFIRM_BUTTON_MASK if joined_state.rule_state.get("overlay_state") is Dictionary and not (joined_state.rule_state["overlay_state"] as Dictionary).is_empty() else 0
				var w_res: Session.FrameResult = _step_session_with_pending(session, held_in, pending_step7)
				if not w_res.committed:
					check(false, "Step 7 week progression failed: " + w_res.error)
					break
				if joined_state.game_mode == 0x40 and joined_state.mode_stage == 0 and joined_state.mode_step == 0 and joined_state.substep == 0:
					var d_val: int = int(joined_state.rule_state["day"])
					if not weekday_activity_days.has(d_val):
						weekday_activity_days.append(d_val)
				if not reached_first_weekday_result and joined_state.game_mode == 0x31 and int(joined_state.rule_state["day_event_index"]) >= 1:
					reached_first_weekday_result = true
					check(joined_state.rule_state["params"] != baseline_params_step7, "Step 7 first ordinary command (April 5) committed parameter deltas at first result")
				if joined_state.game_mode == 0x32:
					reached_saturday_week_end_0x32 = true
				step7_guard += 1

			check(weekday_activity_days == [5, 6, 7, 8], "Step 7 stepped all 4 Week 1 weekday activities (April 5 Wed, 6 Thu, 7 Fri, 8 Sat): " + str(weekday_activity_days))
			check(reached_first_weekday_result, "Step 7 reached first result boundary after April 5 activity")
			check(reached_saturday_week_end_0x32, "Step 7 stepped through Saturday April 8 week-end result screen (mode 0x32)")
			check(joined_state.game_mode == 0x46 and joined_state.mode_stage == 1 and joined_state.rule_state["month"] == 4 and joined_state.rule_state["day"] == 9 and joined_state.rule_state["weekday"] == 6, "Step 7 reached Sunday April 9, 1995 selectable holiday command screen (0x46 stage 1)")

			# Confirm Sunday holiday command 0 -> 0x46 stage 5 -> 0x31 -> 0x34 stage 1 (Next Selectable Weekday Command)
			var sun_press: Session.FrameResult = _step_session_with_pending(session, CONFIRM_BUTTON_MASK, pending_step7)
			check(sun_press.committed and joined_state.game_mode == 0x46 and joined_state.mode_stage == 1, "Step 7 latched confirm on Sunday holiday command screen")
			var sun_confirm: Session.FrameResult = _step_session_with_pending(session, 0, pending_step7)
			check(sun_confirm.committed and joined_state.game_mode == 0x46 and joined_state.mode_stage == 5, "Step 7 confirmed Sunday holiday command 0 and entered holiday activity (0x46 stage 5)")

			var post_sun_guard: int = 0
			while not (joined_state.game_mode == 0x34 and joined_state.mode_stage == 1) and post_sun_guard < 200:
				var ps_res: Session.FrameResult = _step_session_with_pending(session, 0, pending_step7)
				if not ps_res.committed:
					check(false, "Step 7 post-Sunday progression failed: " + ps_res.error)
					break
				post_sun_guard += 1
			check(joined_state.game_mode == 0x34 and joined_state.mode_stage == 1 and joined_state.mode_step == 0, "Step 7 reached next selectable weekday command screen (0x34 stage 1 step 0)")
			_detach_session(session, attach_out)

	# =========================================================================
	# Step 8: Full G1 Route with Imported Name Entry (installed-clean-v1), Input Release/Repeat/Focus, Save/Relaunch, and Real 320x240 Captures
	# =========================================================================
	var name_rules_path: String = installed_root.path_join("packs/name_entry_rules/manifest.json")
	var name_content_path: String = installed_root.path_join("packs/name_entry_content/entry.json")
	var imported_name_tables: Dictionary = Rules.load_tables(name_rules_path)
	var imported_name_content: Dictionary = Entry.load_content(name_content_path)
	check(not imported_name_tables.is_empty() and not imported_name_content.is_empty(), "Loaded imported name_entry_rules and name_entry_content from installed-clean-v1")

	var g1_rng: Rng = Rng.new()
	g1_rng.configure(Rng.OPENBIOS_PROFILE, 19950404)
	var g1_entry: Entry = Entry.new()
	check(g1_entry.begin(imported_name_tables, imported_name_content, g1_rng).is_empty(), "Imported name entry began cleanly")
	finish_names(g1_entry, g1_rng, false)
	for kind: String in ["tick", "start", "tick", "yes", "tick", "start", "tick", "yes", "tick"]:
		action(g1_entry, kind)
	check(g1_entry.stage == 2 and g1_entry.step_id == 5, "Imported name entry committed at stage 2 step 5")

	# Render Capture 01: Prologue Arrival (320x240)
	var captures_dir: String = repo_root.path_join(".claude/orchestrator/state/T-29025/connected-opening-to-next-command-and-save-v1/captures")
	DirAccess.make_dir_recursive_absolute(captures_dir)
	var font_root_path: String = installed_root.path_join(".asset-cache/port/fonts/native-font-v2")
	if not FileAccess.file_exists(font_root_path.path_join("font.json")):
		font_root_path = repo_root.path_join(".asset-cache/port/fonts/native-font-v2")
	var hud_manifest_path: String = repo_root.path_join(".asset-cache/port/hud-production-v1/manifest.json")
	var scene_plates_dir: String = repo_root.path_join(".claude/orchestrator/state/T-29012/prologue-visual-v1/scene_plates")
	var prologue_pres: ProloguePresenter = ProloguePresenter.new()
	var pp_err: String = prologue_pres.init_resources(font_root_path, hud_manifest_path, scene_plates_dir)
	check(pp_err.is_empty(), "Initialized ProloguePresenter with font root, HUD manifest, and scene plates: " + pp_err)
	var g1_prologue_preview: Adapter.Outcome = Adapter.from_entry(g1_entry, 1000, 2000, {"initial_mascot": initial_mascot})
	check(g1_prologue_preview.error.is_empty() and g1_prologue_preview.state != null, "Materialized prologue state from imported name entry")
	var cap01_img: Image = prologue_pres.render_stage0_arrival(false)
	var cap01_path: String = captures_dir.path_join("01-prologue-arrival-320x240.png")
	check(cap01_img != null and cap01_img.get_width() == 320 and cap01_img.get_height() == 240 and _non_black_pixels(cap01_img) > 200 and cap01_img.save_png(cap01_path) == OK, "Saved real 320x240 capture 01-prologue-arrival-320x240.png")

	# Run full 22-stage opening to April 4 0x34 stage 1 with AudioService
	var g1_audio: AudioService = AudioService.new()
	if FileAccess.file_exists(xa_audio_pack.path_join("opening-audio.json")):
		g1_audio.configure_original_opening_audio_pack(xa_audio_pack, xa_audio_sha)
	var g1_open_out: Dictionary = PlayableBoot.run_new_game_opening_to_command_ready(
		g1_entry,
		g1_rng,
		pack_path,
		receipt_path,
		false,
		g1_audio
	)
	check(str(g1_open_out.get("error", "")).is_empty(), "Imported G1 route completed 22-stage prologue and command reload to 0x34 stage 1: " + str(g1_open_out.get("error", "")))
	var g1_state: State = g1_open_out["state"]
	if not g1_state.rule_state.get("resources") is Dictionary:
		g1_state.rule_state["resources"] = {"hud_season_fade": 0}

	# Open real media, LiveHud, HolidayHud, and HolidaySceneAdapter for G1 gameplay + captures
	var hud_context_path: String = repo_root.path_join(".claude/orchestrator/state/T-29010/app-integration-v1/proof/development-hud-context.json")
	var media_pack_path: String = repo_root.path_join(".asset-cache/port/holiday-command0-current-964ce")
	var spring_pack_path: String = repo_root.path_join(".asset-cache/port/spring-command-current-236ab3")
	var weekday56_pack_path: String = repo_root.path_join(".asset-cache/port/weekday56-current-e593")
	var club_pack_path: String = repo_root.path_join(".asset-cache/port/t29014-command4-club-v1")
	var holiday_ext_path: String = repo_root.path_join(".claude/orchestrator/state/T-29013/evidence/holiday-boundary-extension.json")
	var holiday_text_path: String = repo_root.path_join(".asset-cache/port/holiday-hud-text-v1/manifest.json")

	var media_adapter: MediaAdapter = MediaAdapter.new()
	var ma_err: String = media_adapter.open_packs(media_pack_path, spring_pack_path, weekday56_pack_path, club_pack_path)
	check(ma_err.is_empty(), "Opened OrdinaryCommandMediaAdapter packs: " + ma_err)
	var exact_media: ExactMedia = ExactMedia.new()
	var em_err: String = exact_media.open(media_adapter)
	check(em_err.is_empty(), "Opened NativeActivityMedia: " + em_err)
	for cmd_id: int in [0, 7]:
		if media_adapter.has_command(cmd_id):
			media_adapter.command_resources[cmd_id] = exact_media.get_resources(cmd_id)
	var actor_service: ActorService = ActorService.new()

	var live_hud: LiveHud = LiveHud.new()
	check(live_hud.open(hud_manifest_path, LiveHud.MANIFEST_SHA, font_root_path, hud_context_path, LiveHud.CONTEXT_SHA), "Opened LiveHud with production HUD manifest and font root: " + live_hud.error)
	live_hud.reset(g1_state.rule_state)

	var holiday_hud: HolidayHud = HolidayHud.new()
	var media_manifest_sha: String = FileAccess.get_sha256(media_pack_path.path_join("manifest.json"))
	check(holiday_hud.open(hud_manifest_path, LiveHud.MANIFEST_SHA, font_root_path, media_pack_path, media_manifest_sha, holiday_text_path, HolidayHud.TEXT_SHA), "Opened HolidayHud: " + holiday_hud.error)
	var holiday_profile: Dictionary = {
		"family_name_hex": live_hud.profile["family_name_hex"],
		"icon_mode": live_hud.profile["icon_mode"],
		"icon_count": live_hud.profile["icon_count"],
		"cursor_style": live_hud.profile["cursor_style"],
	}
	var next_room_bg: Dictionary = holiday_hud.get_background("next_command", HolidaySceneAdapter.ROUTE["next_command"])
	var holiday_scene: HolidaySceneAdapter = HolidaySceneAdapter.new()
	var hs_err: String = holiday_scene.open(media_adapter.media, holiday_ext_path, next_room_bg.get("image_data"))
	check(hs_err.is_empty(), "Opened HolidaySceneAdapter: " + hs_err)

	# Render Capture 02: April 4 First Command Screen (320x240)
	var empty_trace: Array[Dictionary] = []
	var empty_cmds: Array[Dictionary] = []
	var cap02_cmd: Dictionary = live_hud.compose(g1_state.rule_state, g1_state.projection(), empty_trace, empty_cmds)
	var cap02_img: Image = cap02_cmd.get("image")
	var cap02_path: String = captures_dir.path_join("02-april04-first-command-320x240.png")
	check(cap02_img != null and cap02_img.get_width() == 320 and cap02_img.get_height() == 240 and _non_black_pixels(cap02_img) > 200 and cap02_img.save_png(cap02_path) == OK, "Saved real 320x240 capture 02-april04-first-command-320x240.png")

	var table_pack_g1: Dictionary = PlayableBoot._integers(JSON.parse_string(FileAccess.get_file_as_string(table_path)))
	var g1_pending: Array = []
	var g1_staged_cmds: Array = []
	var g1_session: Session = Session.new()
	g1_session.rng.configure(Rng.OPENBIOS_PROFILE, g1_rng.state)
	var g1_attach: Dictionary = _attach_week_controllers(
		g1_session,
		g1_state,
		table_pack_g1["tables"],
		holiday_scene.tables,
		g1_audio,
		g1_pending,
		exact_media,
		actor_service,
		media_adapter,
		holiday_scene,
		g1_staged_cmds
	)
	check(str(g1_attach.get("error", "")).is_empty(), "Attached imported G1 state to SchoolDayController + HolidayController: " + str(g1_attach.get("error", "")))

	# Test NativePad focus pause/resume clearing latched edges and directional repeat suppression on 0x34 stage 1
	var pad: NativePad = NativePad.new()
	var key_right_down := InputEventKey.new()
	key_right_down.physical_keycode = KEY_RIGHT
	key_right_down.pressed = true
	pad._input(key_right_down)
	var sample_t0: InputFrame = pad.sample(0)
	check(sample_t0.held == RIGHT_BUTTON_MASK and sample_t0.pressed == RIGHT_BUTTON_MASK, "NativePad latched rising edge for KEY_RIGHT (0x2000)")
	var sample_t1_repeat: InputFrame = pad.sample(1)
	check(sample_t1_repeat.held == RIGHT_BUTTON_MASK and sample_t1_repeat.pressed == 0, "NativePad suppresses repeat edge while KEY_RIGHT remains held")
	# Simulate focus loss and neutral focus resume
	pad.clear_devices()
	pad.set_process_input(false)
	var sample_after_focus_loss: InputFrame = pad.sample(2)
	check(sample_after_focus_loss.held == 0 and sample_after_focus_loss.pressed == 0 and sample_after_focus_loss.released == 0, "NativePad focus loss clears held device state and suppresses phantom edges")
	pad.set_process_input(true)
	pad.free()

	# Save at legitimate April 4 command boundary (0x34 stage 1) into an empty temporary user-data directory
	var saves_dir: String = repo_root.path_join(".claude/orchestrator/state/T-29025/connected-opening-to-next-command-and-save-v1/saves")
	DirAccess.make_dir_recursive_absolute(saves_dir)
	for stale_file: String in DirAccess.get_files_at(saves_dir):
		DirAccess.remove_absolute(saves_dir.path_join(stale_file))
	check(DirAccess.get_files_at(saves_dir).is_empty(), "Temporary user-data save directory starts empty: " + saves_dir)

	var save_svc: SaveService = SaveService.new(saves_dir)
	var april04_snapshot: Dictionary = {
		"boundary": "stable",
		"source_version": "t29025-g1-connected-opening-v1",
		"content_version": "installed-clean-v1",
		"behavior_version": "g1-opening-to-next-command-v1",
		"gameplay_state": {
			"checkpoint_id": "command",
			"state": g1_state.projection(),
			"input_latch": g1_session.latched_input.snapshot(),
			"session_hash": g1_session.state_hash(),
		},
		"native_rng": g1_session.rng.checkpoint(),
		"native_clocks": g1_session.clock.snapshot(),
		"queued_service_events": [],
	}
	var save_res_04: Dictionary = save_svc.save_slot("april04_command", april04_snapshot)
	check(str(save_res_04.get("error", "")).is_empty() and not str(save_res_04.get("checksum_sha256", "")).is_empty(), "Saved April 4 command boundary slot via SaveService: " + str(save_res_04.get("error", "")))

	# Step uninterrupted G1 session across Week 1 -> First Result -> Sunday 0x46 -> Next Selectable Command 0x34
	var g1_trace: Array[Dictionary] = []
	var g1_hashes: Array[String] = []
	var cap03_saved: bool = false
	var g1_initial_params: Array = g1_state.rule_state["params"].duplicate()

	# Confirm command 0 on April 4 (press on tick 0, release on tick 1)
	for init_held: int in [CONFIRM_BUTTON_MASK, 0]:
		var c_res: Session.FrameResult = _step_session_with_pending(g1_session, init_held, g1_pending)
		check(c_res.committed, "G1 April 4 command confirmation step committed: " + c_res.error)
		g1_hashes.append(c_res.state_hash)
		g1_trace.append({
			"mode": g1_state.game_mode,
			"stage": g1_state.mode_stage,
			"step": g1_state.mode_step,
			"substep": g1_state.substep,
			"frame_count": g1_state.frame_count,
			"params": g1_state.rule_state["params"].duplicate(),
			"presentation": c_res.presentation,
		})

	var g1_guard: int = 0
	while not (g1_state.game_mode == 0x46 and g1_state.mode_stage == 1) and g1_guard < 400:
		var held_mask: int = CONFIRM_BUTTON_MASK if g1_state.rule_state.get("overlay_state") is Dictionary and not (g1_state.rule_state["overlay_state"] as Dictionary).is_empty() else 0
		var s_res: Session.FrameResult = _step_session_with_pending(g1_session, held_mask, g1_pending)
		if not s_res.committed:
			check(false, "G1 Week 1 step failed: " + s_res.error)
			break
		g1_hashes.append(s_res.state_hash)
		g1_trace.append({
			"mode": g1_state.game_mode,
			"stage": g1_state.mode_stage,
			"step": g1_state.mode_step,
			"substep": g1_state.substep,
			"frame_count": g1_state.frame_count,
			"params": g1_state.rule_state["params"].duplicate(),
			"presentation": s_res.presentation,
		})
		# Render Capture 03 on April 5 when parameters first update during weekday activity (0x40 stage 1)
		if not cap03_saved and g1_state.game_mode == 0x40 and g1_state.mode_stage == 1 and g1_state.rule_state["day"] == 5 and g1_state.rule_state["params"] != g1_initial_params:
			var typed_cmds: Array[Dictionary] = []
			typed_cmds.assign(g1_staged_cmds)
			var cap03_cmd: Dictionary = live_hud.compose(g1_state.rule_state, g1_state.projection(), g1_trace, typed_cmds)
			var cap03_img: Image = cap03_cmd.get("image")
			var cap03_path: String = captures_dir.path_join("03-april05-first-activity-320x240.png")
			check(cap03_img != null and cap03_img.get_width() == 320 and cap03_img.get_height() == 240 and _non_black_pixels(cap03_img) > 200 and cap03_img.save_png(cap03_path) == OK, "Saved real 320x240 capture 03-april05-first-activity-320x240.png")
			cap03_saved = true
		g1_guard += 1

	check(cap03_saved, "Captured April 5 first activity frame with parameter delta")
	check(g1_state.game_mode == 0x46 and g1_state.mode_stage == 1 and g1_state.rule_state["day"] == 9, "G1 route reached Sunday April 9 holiday command screen (0x46 stage 1)")

	# Verify held-input release across 0x40 -> 0x32 -> 0x46: if confirm (0x20) is already held entering 0x46 stage 1, holding 0x20 does NOT auto-confirm holiday command
	g1_session.latched_input = InputFrame.new(g1_session.clock.logic_iterations - 1, CONFIRM_BUTTON_MASK, CONFIRM_BUTTON_MASK)
	var held_0x46_res: Session.FrameResult = _step_session_with_pending(g1_session, CONFIRM_BUTTON_MASK, g1_pending)
	check(held_0x46_res.committed and g1_state.game_mode == 0x46 and g1_state.mode_stage == 1, "Held confirm (0x20) on 0x46 stage 1 does not auto-confirm Sunday command without release")
	g1_hashes.append(held_0x46_res.state_hash)
	g1_trace.append({
		"mode": g1_state.game_mode,
		"stage": g1_state.mode_stage,
		"step": g1_state.mode_step,
		"substep": g1_state.substep,
		"frame_count": g1_state.frame_count,
		"params": g1_state.rule_state["params"].duplicate(),
		"presentation": held_0x46_res.presentation,
	})
	var rel_0x46_res: Session.FrameResult = _step_session_with_pending(g1_session, 0, g1_pending)
	check(rel_0x46_res.committed and g1_state.game_mode == 0x46 and g1_state.mode_stage == 1, "Clean release on 0x46 stage 1 prepares for fresh confirmation")
	g1_hashes.append(rel_0x46_res.state_hash)
	g1_trace.append({
		"mode": g1_state.game_mode,
		"stage": g1_state.mode_stage,
		"step": g1_state.mode_step,
		"substep": g1_state.substep,
		"frame_count": g1_state.frame_count,
		"params": g1_state.rule_state["params"].duplicate(),
		"presentation": rel_0x46_res.presentation,
	})

	# Confirm Sunday holiday command 0 and advance to post-Sunday 0x34 stage 1 (Next Selectable Command)
	for sun_in: int in [CONFIRM_BUTTON_MASK, 0]:
		var sc_res: Session.FrameResult = _step_session_with_pending(g1_session, sun_in, g1_pending)
		check(sc_res.committed, "G1 Sunday holiday command confirm step committed: " + sc_res.error)
		g1_hashes.append(sc_res.state_hash)
		g1_trace.append({
			"mode": g1_state.game_mode,
			"stage": g1_state.mode_stage,
			"step": g1_state.mode_step,
			"substep": g1_state.substep,
			"frame_count": g1_state.frame_count,
			"params": g1_state.rule_state["params"].duplicate(),
			"presentation": sc_res.presentation,
		})

	var g1_post_guard: int = 0
	while not (g1_state.game_mode == 0x34 and g1_state.mode_stage == 1) and g1_post_guard < 200:
		var ps_res: Session.FrameResult = _step_session_with_pending(g1_session, 0, g1_pending)
		if not ps_res.committed:
			check(false, "G1 post-Sunday step failed: " + ps_res.error)
			break
		g1_hashes.append(ps_res.state_hash)
		g1_trace.append({
			"mode": g1_state.game_mode,
			"stage": g1_state.mode_stage,
			"step": g1_state.mode_step,
			"substep": g1_state.substep,
			"frame_count": g1_state.frame_count,
			"params": g1_state.rule_state["params"].duplicate(),
			"presentation": ps_res.presentation,
		})
		g1_post_guard += 1

	check(g1_state.game_mode == 0x34 and g1_state.mode_stage == 1 and g1_state.mode_step == 0, "G1 route reached next selectable weekday command screen (0x34 stage 1)")

	# Render Capture 04: April 9 Next Selectable Command Screen (320x240)
	var typed_holiday_cmds: Array[Dictionary] = []
	typed_holiday_cmds.assign(g1_staged_cmds)
	var active_lba_04: int = int(g1_state.rule_state["background_cache"][g1_state.rule_state["background_show"]])
	var core_04: Dictionary = g1_state.projection()
	core_04.erase("opening")
	var composed_04: Dictionary = holiday_hud.compose(g1_state.rule_state, core_04, holiday_profile, g1_trace, typed_holiday_cmds, active_lba_04)
	check(str(composed_04.get("error", "")).is_empty() and composed_04.get("commands") is Array and not (composed_04["commands"] as Array).is_empty(), "Composed April 9 next command HUD frame: " + str(composed_04.get("error", "")))
	var cap04_img: Image = composed_04["commands"][0].get("image") if composed_04.get("commands") is Array and not (composed_04["commands"] as Array).is_empty() else null
	var cap04_path: String = captures_dir.path_join("04-april09-next-command-320x240.png")
	check(cap04_img != null and cap04_img.get_width() == 320 and cap04_img.get_height() == 240 and _non_black_pixels(cap04_img) > 200 and cap04_img.save_png(cap04_path) == OK, "Saved real 320x240 capture 04-april09-next-command-320x240.png")

	# Verify held-input release on post-Sunday 0x34 stage 1 before saving next_command boundary
	var pre_hold_checkpoint: Dictionary = g1_state.projection()
	var pre_hold_rng: Dictionary = g1_session.rng.checkpoint()
	var pre_hold_clock: Dictionary = g1_session.clock.snapshot()
	var pre_hold_latch: InputFrame = g1_session.latched_input
	g1_session.latched_input = InputFrame.new(g1_session.clock.logic_iterations - 1, CONFIRM_BUTTON_MASK, CONFIRM_BUTTON_MASK)
	var held_next_cmd: Session.FrameResult = _step_session_with_pending(g1_session, CONFIRM_BUTTON_MASK, g1_pending)
	check(held_next_cmd.committed and g1_state.game_mode == 0x34 and g1_state.mode_stage == 1, "Held confirm (0x20) on post-Sunday 0x34 stage 1 does not auto-confirm next week command")
	g1_state.restore(pre_hold_checkpoint)
	g1_session.rng.restore(pre_hold_rng)
	g1_session.clock.restore(pre_hold_clock)
	g1_session.latched_input = pre_hold_latch

	# Also save at post-Sunday next_command boundary (0x34 stage 1)
	var april09_snapshot: Dictionary = {
		"boundary": "stable",
		"source_version": "t29025-g1-connected-opening-v1",
		"content_version": "installed-clean-v1",
		"behavior_version": "g1-opening-to-next-command-v1",
		"gameplay_state": {
			"checkpoint_id": "next_command",
			"state": g1_state.projection(),
			"input_latch": g1_session.latched_input.snapshot(),
			"session_hash": g1_session.state_hash(),
		},
		"native_rng": g1_session.rng.checkpoint(),
		"native_clocks": g1_session.clock.snapshot(),
		"queued_service_events": [],
	}
	var save_res_09: Dictionary = save_svc.save_slot("april09_next_command", april09_snapshot)
	check(str(save_res_09.get("error", "")).is_empty(), "Saved April 9 post-Sunday next_command boundary slot via SaveService: " + str(save_res_09.get("error", "")))

	# Simulate cold process relaunch from April 4 save slot and verify identical trajectory across the entire week
	var relaunch_save_svc: SaveService = SaveService.new(saves_dir)
	var loaded_04: Dictionary = relaunch_save_svc.load_slot("april04_command")
	check(str(loaded_04.get("error", "")).is_empty() and loaded_04.get("snapshot") is Dictionary, "Relaunched SaveService loaded april04_command slot cleanly: " + str(loaded_04.get("error", "")))
	var snap_04: Dictionary = loaded_04["snapshot"]
	var relaunch_state: State = State.new()
	relaunch_state.restore(snap_04["gameplay_state"]["state"])
	var relaunch_session: Session = Session.new()
	var relaunch_pending: Array = []
	var relaunch_staged: Array = []
	var relaunch_attach: Dictionary = _attach_week_controllers(
		relaunch_session,
		relaunch_state,
		table_pack_g1["tables"],
		holiday_scene.tables,
		g1_audio,
		relaunch_pending,
		exact_media,
		actor_service,
		media_adapter,
		holiday_scene,
		relaunch_staged
	)
	check(str(relaunch_attach.get("error", "")).is_empty(), "Attached relaunched session to fresh SchoolDayController + HolidayController")
	relaunch_session.clock.restore(snap_04["native_clocks"])
	relaunch_session.rng.restore(snap_04["native_rng"])
	var latch_04: Dictionary = snap_04["gameplay_state"]["input_latch"]
	var prev_04: int = (int(latch_04["held"]) & ~int(latch_04["pressed"])) | int(latch_04["released"])
	relaunch_session.latched_input = InputFrame.new(int(latch_04["tick"]), int(latch_04["held"]), prev_04)
	check(relaunch_session.state_hash() == str(snap_04["gameplay_state"]["session_hash"]), "Relaunched session state_hash matches pre-save session_hash")

	var relaunch_hashes: Array[String] = []
	for init_held: int in [CONFIRM_BUTTON_MASK, 0]:
		var rc_res: Session.FrameResult = _step_session_with_pending(relaunch_session, init_held, relaunch_pending)
		relaunch_hashes.append(rc_res.state_hash)
	var r_guard: int = 0
	while not (relaunch_state.game_mode == 0x46 and relaunch_state.mode_stage == 1) and r_guard < 400:
		var r_held: int = CONFIRM_BUTTON_MASK if relaunch_state.rule_state.get("overlay_state") is Dictionary and not (relaunch_state.rule_state["overlay_state"] as Dictionary).is_empty() else 0
		var rs_res: Session.FrameResult = _step_session_with_pending(relaunch_session, r_held, relaunch_pending)
		relaunch_hashes.append(rs_res.state_hash)
		r_guard += 1
	relaunch_session.latched_input = InputFrame.new(relaunch_session.clock.logic_iterations - 1, CONFIRM_BUTTON_MASK, CONFIRM_BUTTON_MASK)
	relaunch_hashes.append(_step_session_with_pending(relaunch_session, CONFIRM_BUTTON_MASK, relaunch_pending).state_hash)
	relaunch_hashes.append(_step_session_with_pending(relaunch_session, 0, relaunch_pending).state_hash)
	for sun_in: int in [CONFIRM_BUTTON_MASK, 0]:
		relaunch_hashes.append(_step_session_with_pending(relaunch_session, sun_in, relaunch_pending).state_hash)
	var r_post_guard: int = 0
	while not (relaunch_state.game_mode == 0x34 and relaunch_state.mode_stage == 1) and r_post_guard < 200:
		relaunch_hashes.append(_step_session_with_pending(relaunch_session, 0, relaunch_pending).state_hash)
		r_post_guard += 1

	check(relaunch_hashes.size() == g1_hashes.size() and relaunch_hashes == g1_hashes, "Relaunched session from April 4 save produced identical state_hash sequence across all %d steps" % g1_hashes.size())
	check(relaunch_session.rng.state == g1_session.rng.state and relaunch_session.rng.draw_count == g1_session.rng.draw_count, "Relaunched session RNG state and draw_count match uninterrupted session")
	check(relaunch_session.clock.snapshot() == g1_session.clock.snapshot(), "Relaunched session clock counters match uninterrupted session")
	check(relaunch_state.rule_state["params"] == g1_state.rule_state["params"], "Relaunched session final parameter outcomes match uninterrupted session")

	# Simulate cold process relaunch from April 9 next_command save slot and step both sessions into Week 2 (Monday April 10 0x40)
	var loaded_09: Dictionary = relaunch_save_svc.load_slot("april09_next_command")
	check(str(loaded_09.get("error", "")).is_empty(), "Relaunched SaveService loaded april09_next_command slot cleanly")
	var snap_09: Dictionary = loaded_09["snapshot"]
	var relaunch09_state: State = State.new()
	relaunch09_state.restore(snap_09["gameplay_state"]["state"])
	var relaunch09_session: Session = Session.new()
	var relaunch09_pending: Array = []
	var relaunch09_staged: Array = []
	var relaunch09_attach: Dictionary = _attach_week_controllers(
		relaunch09_session,
		relaunch09_state,
		table_pack_g1["tables"],
		holiday_scene.tables,
		g1_audio,
		relaunch09_pending,
		exact_media,
		actor_service,
		media_adapter,
		holiday_scene,
		relaunch09_staged
	)
	check(str(relaunch09_attach.get("error", "")).is_empty(), "Attached April 9 relaunched session to fresh SchoolDayController + HolidayController")
	relaunch09_session.clock.restore(snap_09["native_clocks"])
	relaunch09_session.rng.restore(snap_09["native_rng"])
	var latch_09: Dictionary = snap_09["gameplay_state"]["input_latch"]
	var prev_09: int = (int(latch_09["held"]) & ~int(latch_09["pressed"])) | int(latch_09["released"])
	relaunch09_session.latched_input = InputFrame.new(int(latch_09["tick"]), int(latch_09["held"]), prev_09)
	check(relaunch09_session.state_hash() == g1_session.state_hash(), "Relaunched April 9 next_command session state_hash matches uninterrupted session")

	var w2_guard: int = 0
	while g1_state.game_mode != 0x40 and w2_guard < 25:
		var w2_in: int = CONFIRM_BUTTON_MASK if w2_guard == 0 else 0
		var u_res: Session.FrameResult = _step_session_with_pending(g1_session, w2_in, g1_pending)
		var r_res: Session.FrameResult = _step_session_with_pending(relaunch09_session, w2_in, relaunch09_pending)
		check(u_res.committed and r_res.committed and u_res.state_hash == r_res.state_hash, "Week 2 continuation step %d matches between uninterrupted and April 9 relaunched sessions" % w2_guard)
		w2_guard += 1
	check(g1_state.game_mode == 0x40 and g1_state.rule_state["day"] == 10 and relaunch09_state.game_mode == 0x40 and relaunch09_state.rule_state["day"] == 10, "Both uninterrupted and relaunched sessions entered Week 2 Monday April 10 activity (0x40)")

	_detach_session(g1_session, g1_attach)
	_detach_session(relaunch_session, relaunch_attach)
	_detach_session(relaunch09_session, relaunch09_attach)
	services.clear()

	print("--------------------------------------------------")
	print(("PASS" if failures == 0 else "FAIL") + ": %d connected opening route checks" % checks)
	quit(0 if failures == 0 else 1)
