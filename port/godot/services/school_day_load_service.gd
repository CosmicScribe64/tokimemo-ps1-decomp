extends RefCounted
## T-39002: the CD reads of the first school day on the installed route.
## school_day_screen_init substep 0 (school_day.c:142-182) issues
## dec_bg_cd_read for today's command background (10 sectors,
## cd_read_sectors_nowait(0xA, ...)) and waits in substep 1 for
## cd_read_sync_status bit 0, then dec_bg_decode_to_vram(0) picks the slot;
## school_day_sd_load reads the 41-sector SD set and checks it. The rules
## (scenes/school_day_controller.gd _setup) publish one school_day_load
## intent per read and take a "school_day.resources" completion.
##
## The completion is due on the logic frame whose handler sees the read done:
## the request frame's pre-increment frame_count plus the source read model
## (services/prologue_media_queue.gd model_read_vsyncs: 14 + 0.4 vsync per
## sector, 18 for the background, 30 for the SD set). Golden
## .asset-cache/emu/route-golden-smoke-v1: 40/0/0/1 at 21372 -> 40/0/0/2 at
## 21390 and 40/0/1/1 at 21392 -> 40/0/2/0 at 21422. A read is answered only
## when the installed command media (services/native_activity_media.gd) holds
## the exact background and SD set the rules asked for; the decode result is
## the cache slot the rules wrote (1 - dec_bg_show[0]), the SD checksum is the
## installed set's authentication. Nothing else is fabricated: a read the
## media cannot answer stays pending and is reported.

const Session = preload("res://core/game_session.gd")
const Queue = preload("res://services/prologue_media_queue.gd")
const ActorService = preload("res://presentation/school_day_actor_service.gd")
const SERVICE := "school_day.resources"
const ACTOR_SERVICE := "school_day.actor96"
## T-39002: the Sunday holiday (mode 0x46) and the next week's command setup
## (0x34 stage 0) on the same CD owner. holiday_open_init / holiday_open_wait_bg
## read the seasonal room, holiday_activity_load the activity background and
## the 41-sector holiday SD set (holiday.c:270-291, 934-992), and
## settings_command_screen_build the weekday room; scenes/holiday_controller.gd
## publishes one holiday_load intent per read and takes a "holiday.resources"
## completion, answered here from the installed holiday media
## (services/holiday_scene_adapter.gd open_fresh) and the command room.
const HOLIDAY_SERVICE := "holiday.resources"
const HOLIDAY_SD_SECTORS := 41
## cd_audio_command(9) (cd_queue.c:110) queues CdlPause on the command ring;
## a read pushed behind it starts once the pause has completed. Measured, one
## sample: golden route-golden-smoke-v1, the holiday click pushes the pause at
## frame 21763, the activity read pushed at 21764 is seen done at 21785, three
## frames after the 18-frame read model (46/5/0/1 -> 46/5/0/2). Ring busy until
## pause frame + 4. The first week's confirm pause (21363) precedes its first
## read by nine frames and does not move it (40/0/0/1 21372 -> 21390).
const CD_PAUSE_RING_VSYNCS := 4

var _capabilities: RefCounted
var _pending: Array[Dictionary] = []
var _actors := ActorService.new()
## Actor draw of the frame in flight (logical draw), taken on commit.
var _frame_commands: Array[Dictionary] = []
var _frame_services: Array[Dictionary] = []
## The committed frame's actor draw commands (presentation) and the actor96
## completion due on the next frame's BEFORE_MODE.
var actor_commands: Array[Dictionary] = []
var _actor_completions: Array[Dictionary] = []
var error := ""
## Holiday media: backgrounds (LBA -> draw commands) and the holiday SD set
## resources of the opened HolidaySceneAdapter; the room LBA the command HUD
## draws for the next-command read.
var _holiday_backgrounds: Dictionary = {}
var _holiday_resources: Dictionary = {}
var _next_command_lba := -1
## Frame of the last committed cd_audio_command(9), or -1.
var _pause_frame := -1


## Holiday reads and actors from the opened holiday scene adapter's media.
func prepare_holiday(scene: RefCounted, next_command_lba: int) -> String:
	if scene == null or not scene.get("backgrounds") is Dictionary or (scene.backgrounds as Dictionary).is_empty() or not scene.get("resources") is Dictionary or (scene.resources as Dictionary).is_empty():
		return "holiday reads need the opened holiday scene media"
	_holiday_backgrounds = scene.backgrounds
	_holiday_resources = scene.resources
	_next_command_lba = next_command_lba
	return ""


## Background draw commands of an installed holiday or retained background.
func holiday_background(lba: int) -> Array[Dictionary]:
	var out: Array[Dictionary] = []
	if _holiday_backgrounds.has(lba):
		out.assign(_holiday_backgrounds[lba])
	return out


func prepare(capabilities: RefCounted) -> String:
	if capabilities == null or capabilities.get("exact_media") == null:
		return "school-day reads need the installed ordinary command media"
	_capabilities = capabilities
	_pending.clear()
	return ""


## actor_draw_all for the school-day actors 96/97 inside the logical draw
## (presentation/school_day_actor_service.gd, the same adapter as the
## development route): the frame that runs school_day_sd_actors_init
## initializes them from the installed SD set, every later frame advances
## their sequences; actor96's terminal frame (flags5 cleared) is answered by a
## "school_day.actor96" completion on the next frame, which week_day_main0
## reads (scenes/school_day_controller.gd service_completion).
func draw(state: RefCounted, result: Session.FrameResult, iteration: int) -> String:
	_frame_commands = []
	_frame_services = []
	if state.game_mode == 0x46:
		return _draw_holiday(state, result, iteration)
	var data: Dictionary = state.rule_state
	var initialize: Variant = null
	for intent: Dictionary in result.presentation:
		if intent.get("kind") == "school_day_actors":
			initialize = intent
	if initialize == null and data.actor_state.is_empty():
		return ""
	if _capabilities == null:
		return "school-day actors have no installed command media"
	var selected: Dictionary = _capabilities.exact_media.resolve(state)
	if not str(selected.get("error", "")).is_empty():
		return "installed command media cannot draw the school-day actors: " + str(selected.error)
	if initialize != null:
		if initialize.get("sd_set_id") != selected.route.sd_set_id or data.get("scene_work") != selected.route.sd_set_id:
			return "school-day actors differ from the installed SD set"
		var prepared: Dictionary = _actors.initialize(initialize.actors, selected.resources)
		if not str(prepared.error).is_empty():
			return prepared.error
		data["actor_state"] = prepared.actor_state
	var drawn: Dictionary = _actors.native_draw(data.actor_state, selected.resources, iteration, Vector2i(160, 120))
	if not str(drawn.error).is_empty():
		return drawn.error
	data["actor_state"] = drawn.actor_state
	_frame_commands.assign(drawn.draw_commands)
	_frame_services.assign(drawn.services)
	for command: Dictionary in drawn.draw_commands:
		result.presentation.append({"kind": "native_actor_draw", "id": command.get("id"),
			"x": command.get("position", Vector2i.ZERO).x, "y": command.get("position", Vector2i.ZERO).y})
	return ""


## actor_draw_all for the holiday actor 96 (holiday_activity_sd_setup,
## holiday.c:1039-1052): the frame that publishes holiday_actors initializes
## it from the installed holiday SD set, later frames advance it.
func _draw_holiday(state: RefCounted, result: Session.FrameResult, iteration: int) -> String:
	var data: Dictionary = state.rule_state
	var initialize: Variant = null
	for intent: Dictionary in result.presentation:
		if intent.get("kind") == "holiday_actors":
			initialize = intent
	if initialize == null and data.actor_state.is_empty():
		return ""
	if _holiday_resources.is_empty():
		return "holiday actors have no installed holiday SD set"
	if initialize != null:
		if initialize.get("sd_set_id") != 1 or data.get("scene_work") != 1:
			return "holiday actors differ from the installed holiday SD set 1"
		var prepared: Dictionary = _actors.initialize(initialize.actors, _holiday_resources)
		if not str(prepared.error).is_empty():
			return prepared.error
		data["actor_state"] = prepared.actor_state
	var drawn: Dictionary = _actors.native_draw(data.actor_state, _holiday_resources, iteration, Vector2i(160, 120))
	if not str(drawn.error).is_empty():
		return drawn.error
	data["actor_state"] = drawn.actor_state
	_frame_commands.assign(drawn.draw_commands)
	_frame_services.assign(drawn.services)
	for command: Dictionary in drawn.draw_commands:
		result.presentation.append({"kind": "native_actor_draw", "id": command.get("id"),
			"x": command.get("position", Vector2i.ZERO).x, "y": command.get("position", Vector2i.ZERO).y})
	return ""


## Queues the reads a committed frame requested. state is the committed state
## (frame_count already incremented).
func committed(result: Session.FrameResult, state: RefCounted) -> String:
	actor_commands = _frame_commands.duplicate()
	_actor_completions = _frame_services.duplicate(true)
	_frame_commands = []
	_frame_services = []
	for intent: Dictionary in result.presentation:
		match intent.get("kind"):
			"school_day_load":
				var failure := _queue(intent, state)
				if not failure.is_empty(): return failure
			"holiday_load":
				var failure := _queue_holiday(intent, state)
				if not failure.is_empty(): return failure
			"native_background_reset":
				return "school-day background decode failure retry is not modeled"
	# A pause of this frame is pushed after its reads (the click frame has none).
	for intent: Dictionary in result.audio:
		if intent.get("kind") == "native_cd_audio_command" and int(intent.get("command", -1)) == 9:
			_pause_frame = int(state.frame_count) - 1
	return ""


## Frames a read pushed on frame `issued` waits behind a queued pause.
func _pause_delay(issued: int) -> int:
	if _pause_frame < 0:
		return 0
	return maxi(0, _pause_frame + CD_PAUSE_RING_VSYNCS - issued)


func _queue_holiday(intent: Dictionary, state: RefCounted) -> String:
	var request: String = str(intent.get("request", ""))
	var data: Dictionary = state.rule_state
	var pending: Variant = data.get("resources", {}).get("holiday_pending")
	if not pending is Dictionary or pending.get("request") != request or pending.get("lba") != intent.get("lba") or pending.get("ready") != false:
		return "holiday read has no matching pending record"
	for item: Dictionary in _pending:
		if item.payload.get("request") == request:
			return "holiday %s read requested twice before its completion" % request
	var lba: int = int(intent.get("lba", -1))
	var payload := {"request": request, "lba": lba, "ready": true}
	var sectors: int = int(intent.get("sectors", -1))
	if request == "sd":
		if sectors != HOLIDAY_SD_SECTORS or intent.get("sd_set_id") != 1 or _holiday_resources.is_empty():
			return "installed holiday media has no SD set %s at LBA %d" % [str(intent.get("sd_set_id")), lba]
	elif request in ["room", "activity", "next_command"]:
		var known: bool = _holiday_backgrounds.has(lba) if request != "next_command" else lba == _next_command_lba
		if not known or sectors != Queue.BACKGROUND_SECTORS:
			return "installed holiday media has no %s background at LBA %d" % [request, lba]
		var slot: Variant = pending.get("slot")
		if not slot is int or slot not in [0, 1] or int(data.background_cache[slot]) != lba:
			return "holiday background read differs from the cache slot the rules wrote"
		# dec_bg_decode_to_vram(0) decodes into 1 - dec_bg_show[0], the slot
		# dec_bg_cd_read wrote (background_images.c:81-91).
		payload["decode_result"] = slot
	else:
		return "holiday read has no source request: " + request
	var issued: int = int(state.frame_count) - 1
	var latency: int = Queue.model_read_vsyncs(sectors)
	if latency < 1:
		return "holiday read has no source latency for %d sectors" % sectors
	_pending.append({"due": issued + latency + _pause_delay(issued), "service": HOLIDAY_SERVICE, "payload": payload})
	return ""


func _queue(intent: Dictionary, state: RefCounted) -> String:
	if _capabilities == null:
		return "school-day reads are not prepared"
	var request: String = str(intent.get("request", ""))
	if request not in ["background", "sd"]:
		return "school-day read has no source request: " + request
	for item: Dictionary in _pending:
		if item.payload.request == request:
			return "school-day %s read requested twice before its completion" % request
	var selected: Dictionary = _capabilities.exact_media.resolve(state)
	if not str(selected.get("error", "")).is_empty():
		return "installed command media cannot answer the school-day read: " + str(selected.error)
	var route: Dictionary = selected.route
	var data: Dictionary = state.rule_state
	var issued: int = int(state.frame_count) - 1
	var payload := {"request": request, "ready": true}
	var sectors: int
	if request == "background":
		if intent.get("background_lba") != route.background_lba or (selected.get("background_commands", []) as Array).is_empty():
			return "installed command media has no background at LBA %s" % str(intent.get("background_lba"))
		var show: int = int(data.get("background_show", -1))
		if show not in [0, 1] or int(data.background_cache[1 - show]) != int(route.background_lba):
			return "school-day background read differs from the cache slot the rules wrote"
		payload["lba"] = int(route.background_lba)
		payload["decode_result"] = 1 - show
		sectors = Queue.BACKGROUND_SECTORS
	else:
		if intent.get("sd_lba") != route.sd_lba or intent.get("sd_set_id") != route.sd_set_id or (selected.get("resources", {}) as Dictionary).is_empty():
			return "installed command media has no SD set %s at LBA %s" % [str(intent.get("sd_set_id")), str(intent.get("sd_lba"))]
		payload["lba"] = int(route.sd_lba)
		payload["checksum_ok"] = true
		sectors = int(intent.get("sectors", -1))
	var latency: int = Queue.model_read_vsyncs(sectors)
	if latency < 1:
		return "school-day read has no source latency for %d sectors" % sectors
	_pending.append({"due": issued + latency + _pause_delay(issued), "payload": payload})
	return ""


## Completions due before the next step's handler. frame_count is the next
## step's pre-increment value.
func pending_events(tick: int, frame_count: int) -> Array[Session.ServiceEvent]:
	var events: Array[Session.ServiceEvent] = []
	for item: Dictionary in _actor_completions:
		if item.get("service") != ACTOR_SERVICE: continue
		events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
			Session.ServiceKind.COMPLETION, ACTOR_SERVICE, item.payload))
	for item: Dictionary in _pending:
		if int(item.due) <= frame_count:
			events.append(Session.ServiceEvent.new(tick, Session.Phase.BEFORE_MODE,
				Session.ServiceKind.COMPLETION, str(item.get("service", SERVICE)), item.payload))
	return events


## Drops the completions a committed step consumed.
func delivered(events: Array[Session.ServiceEvent]) -> void:
	for event: Session.ServiceEvent in events:
		if event.service == ACTOR_SERVICE:
			_actor_completions = _actor_completions.filter(func(item: Dictionary) -> bool: return item.payload != event.payload)
		if event.service != SERVICE and event.service != HOLIDAY_SERVICE: continue
		for index in range(_pending.size()):
			if str(_pending[index].get("service", SERVICE)) == event.service and _pending[index].payload == event.payload:
				_pending.remove_at(index)
				break


func pending_count() -> int:
	return _pending.size()
