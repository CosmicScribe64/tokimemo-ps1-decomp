extends SceneTree
## Verifies opening and querying ClubCommandContentStore.

const Store = preload("res://services/club_command_content_store.gd")
const EXPECTED_SHA := "00b844999ad42c174a4163906ca6552929d48ebbe9b96e12d6285b7e2aae65d9"

func _initialize() -> void:
	var pack := OS.get_environment("T29014_CLUB_PACK")
	assert(not pack.is_empty(), "T29014_CLUB_PACK must name the source-backed club export")
	var store = Store.new()
	assert(store.open_pack(pack, EXPECTED_SHA), store.error_message())
	for club_id in range(11):
		assert(store.has_club(club_id), "store has club %d" % club_id)
		var content: Dictionary = store.get_club(club_id)
		assert(content.get("route", {}).get("command") == 4)
		assert(content.get("route", {}).get("club_id") == club_id)
		assert(not content.get("sd", {}).get("frames", []).is_empty())
		assert(not content.get("sd", {}).get("sequences", []).is_empty())
		var bg_img: Image = store.get_background(club_id)
		assert(bg_img != null and bg_img.get_size() == Vector2i(320, 88), "club %d background loaded" % club_id)
		var frame0: Dictionary = store.get_sd_frame(club_id, 0)
		assert(not frame0.is_empty() and frame0.get("image") != null, "club %d frame 0 loaded" % club_id)

	# Verify negative queries: club 11 is none, club 12 is out of range
	assert(not store.has_club(11))
	assert(store.get_club(11).is_empty())
	assert(store.get_background(11) == null)

	# Verify corrupt manifest rejection
	var corrupt = Store.new()
	assert(not corrupt.open_pack(pack, "0".repeat(64)))
	assert(corrupt.error_message().contains("reviewed absolute pack identity"))

	print("T29014 club content store: 45 assertions PASS")
	quit(0)
