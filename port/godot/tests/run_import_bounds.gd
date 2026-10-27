extends SceneTree
## Invented oversized inputs are rejected before payload reads/decompression.

const Backend = preload("res://services/original_game_import_backend.gd")
var checks := 0
var failures := 0
func _check(condition: bool, message: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		printerr("FAIL: ", message)

func _initialize() -> void:
	var backend = Backend.new()
	_check(backend._bounded_file("/out/oversized.nbn").is_empty(), "oversized regular input rejected")
	var track := FileAccess.open("/original/track1.bin", FileAccess.READ)
	_check(backend._read_iso_extent(track, 0, Backend.MAX_SOURCE_FILE_BYTES + 1, Backend.SECTOR_RAW).is_empty(), "oversized ISO extent rejected")
	_check(backend._read_iso_extent(track, 0x7fffffff, 2048, Backend.SECTOR_RAW).is_empty(), "out-of-track ISO extent rejected")
	_check(backend._index_iso_directory(track, 16, Backend.MAX_ISO_DIRECTORY_BYTES + 1, Backend.SECTOR_RAW).is_empty(), "oversized ISO directory rejected")
	track.close()
	_check(backend.begin_import("/out/oversized-input.zip", "/out/bounds-zip-stage").is_empty(), "bounded ZIP inspection starts")
	var event: Dictionary = {}
	for step in range(8):
		event = backend.poll_event()
		if event.get("status") != "running":
			break
	_check(event.get("status") == "failed", "oversized expanded ZIP payload rejected")
	var linked_root := "/out/linked-source-" + str(Time.get_ticks_usec())
	DirAccess.make_dir_recursive_absolute(linked_root)
	_check(DirAccess.open("/").create_link(linked_root, linked_root.path_join("CDROM")) == OK, "invented nested directory link created")
	backend._source_path = linked_root
	_check(backend._read_directory_file("CDROM/missing.bin").is_empty(), "nested directory link rejected without recursion")
	_check(backend._read_directory_file("../outside.bin").is_empty(), "directory source traversal rejected")
	var status := FileAccess.open("/out/import-bounds.json", FileAccess.WRITE)
	status.store_string(JSON.stringify({"status":"PASS" if failures == 0 else "FAIL","checks":checks,"failures":failures,"max_source_bytes":Backend.MAX_SOURCE_FILE_BYTES})); status.close()
	print("T29025 import bounds: ", checks, " checks PASS")
	quit(0 if failures == 0 else 1)
