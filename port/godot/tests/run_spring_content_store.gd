extends SceneTree

const Store = preload("res://services/spring_school_day_content_store.gd")
const EXPECTED_SHA := "b26ef7e5c0301633c45d9a6a898ca4bdb7be68ee30f163825eed827e4f5c0c28"

func _initialize() -> void:
	var pack := OS.get_environment("T29014_SPRING_PACK")
	assert(not pack.is_empty(), "T29014_SPRING_PACK must name the ignored source-backed export")
	var cache_root := ProjectSettings.globalize_path("res://../../.asset-cache/port").simplify_path()
	assert(pack.begins_with(cache_root.trim_suffix("/") + "/"), "test pack must be inside repository .asset-cache/port")
	var store = Store.new()
	assert(store.open_pack(pack, EXPECTED_SHA), store.error_message())
	for command in [1, 2, 3]:
		var content: Dictionary = store.get_command(command)
		assert(content.get("route", {}).get("command") == command)
		assert(not content.get("sd", {}).get("frames", []).is_empty())
		assert(not content.get("sd", {}).get("sequences", []).is_empty())
	assert(store.get_command(0).is_empty() and store.get_command(4).is_empty())

	var corrupt := Store.new()
	assert(not corrupt.open_pack(pack, "0".repeat(64)))
	assert(corrupt.error_message().contains("reviewed manifest identity"))

	var embedded := Store.new()
	assert(not embedded.open_pack("res://embedded-pack"))
	assert(embedded.error_message().contains("canonical absolute POSIX"))
	var project_local := Store.new()
	assert(not project_local.open_pack(ProjectSettings.globalize_path("res://project-pack")))
	assert(project_local.error_message().contains("outside res://"))
	var double_slash := Store.new()
	assert(not double_slash.open_pack("//" + pack.trim_prefix("/")))
	assert(double_slash.error_message().contains("canonical absolute POSIX"))

	var linked_root := cache_root.path_join("t29014-test-linked-pack")
	var disk := DirAccess.open(cache_root)
	assert(disk != null and disk.create_link(pack, linked_root) == OK, "test setup must create pack symlink")
	var symlinked := Store.new()
	assert(not symlinked.open_pack(linked_root))
	assert(symlinked.error_message().contains("symlink"))
	assert(DirAccess.remove_absolute(linked_root) == OK, "test cleanup must remove pack symlink")
	print("T29014 spring content store: 25 assertions PASS")
	quit(0)
