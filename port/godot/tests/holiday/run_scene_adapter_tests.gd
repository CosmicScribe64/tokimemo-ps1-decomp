extends SceneTree
## Original-content preflight only. UI readiness remains a separate capability.

const Media = preload("res://services/school_day_media_store.gd")
const SceneAdapter = preload("res://services/holiday_scene_adapter.gd")

func _initialize() -> void:
	var media: Media = Media.new()
	assert(media.open_pack(OS.get_environment("TOKIMEMO_MEDIA_PACK")), media.error_message())
	var room: Image = Image.new()
	assert(room.load(OS.get_environment("TOKIMEMO_NEXT_ROOM")) == OK)
	var adapter: SceneAdapter = SceneAdapter.new()
	var error: String = adapter.open(media, OS.get_environment("TOKIMEMO_HOLIDAY_EXTENSION"), room)
	assert(error.is_empty(), error)
	assert(adapter.tables.size() == 6 and not adapter.resources["frames"].is_empty())
	assert(adapter.backgrounds.keys().size() == 5)
	assert(adapter.initial_extension()["icon_count"] == 14)
	var changed: Image = room.duplicate()
	changed.set_pixel(0, 0, Color.MAGENTA)
	assert(not adapter.open(media, OS.get_environment("TOKIMEMO_HOLIDAY_EXTENSION"), changed).is_empty())
	assert(adapter.initial_extension().is_empty() and adapter.resources.is_empty())
	print("PASS: actual holiday v3/next-room/April4 extension preflight and failed-reopen clearing")
	quit()
