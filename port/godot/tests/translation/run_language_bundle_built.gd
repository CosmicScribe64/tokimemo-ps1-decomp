extends SceneTree
## T-30002 WP2 x Phase 4 WP-C: a bundle written by tools/port/build_english_bundle.py
## opens through LanguageBundleService exactly as the Language setting will
## use it. ENGLISH_BUNDLE=<bundle directory> (the builder's --output, holding
## english_bundle.json). With ENGLISH_BUNDLE_DRAFTS=1 the bundle must also carry
## a draft_images member, and the draft opt-in is checked. Without the env the
## test fails: it proves nothing on synthetic data.
const Bundle = preload("res://services/language_bundle_service.gd")
var failures: Array[String] = []

func check(condition: bool, label: String) -> void:
	if not condition: failures.append(label)

func _initialize() -> void:
	call_deferred("run")

func settle(service: RefCounted) -> void:
	var frames := 0
	while service.loading and frames < 1200:
		await process_frame
		service.poll()
		frames += 1

func run() -> void:
	var root := OS.get_environment("ENGLISH_BUNDLE")
	check(not root.is_empty(), "ENGLISH_BUNDLE names a built bundle directory")
	var path := root.path_join("english_bundle.json")
	check(FileAccess.file_exists(path), "built bundle manifest exists")
	if not failures.is_empty(): _finish(); return
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(path))
	var members: Dictionary = manifest.members
	var read := Bundle.read_bundle(path)
	check(read.ok, "builder manifest passes read_bundle: %s" % read.get("error", ""))
	# setup_content.py builds into <installed>/language/en by default.
	if root.ends_with("/language/en"):
		check(Bundle.bundle_path(root.get_base_dir().get_base_dir()) == path, "default location matches BUNDLE_RELATIVE")

	var service := Bundle.new()
	service.request("en", {"bundle_path": path, "allow_drafts": false}); await settle(service)
	check(service.choice == "en" and service.notice.is_empty(), "built bundle applies English: " + service.notice)
	check(service.disclosure == manifest.disclosure, "disclosure comes from the bundle")
	var coverage: Dictionary = service.coverage
	if members.has("ui_text"):
		check(service.text_layer != null and service.text_layer.entry_count() == int(members.ui_text.coverage.approved_records), "UI text opened with its coverage")
		check(coverage.text_records_representable == int(members.ui_text.coverage.representable_records), "representable count")
	if members.has("dialogue"):
		check(service.dialogue != null and service.dialogue.entries.size() == int(members.dialogue.coverage.lines), "dialogue opened with its coverage")
	if members.has("images"):
		check(service.images != null and service.images.asset_count() == int(members.images.coverage.reviewed_images), "reviewed art opened with its coverage")
		check(not coverage.draft_images_active, "no draft art without the opt-in")
	var omitted: Array = []
	for row: Dictionary in manifest.omitted: omitted.append(row.member)
	check(coverage.omitted_members == omitted, "omitted members reported: %s" % [coverage.omitted_members])
	check(not service.coverage_line().is_empty(), "coverage line for the Language page")

	if OS.get_environment("ENGLISH_BUNDLE_DRAFTS") == "1":
		check(members.has("draft_images"), "bundle carries draft art")
		service.request("en", {"bundle_path": path, "allow_drafts": true}); await settle(service)
		check(service.choice == "en", "English stays active with drafts")
		var expect_open := OS.get_environment("ENGLISH_BUNDLE_DRAFTS_OPEN") != "0"
		if expect_open:
			check(service.notice.is_empty() and service.coverage.draft_images_active, "draft art opens with the opt-in: " + service.notice)
			check(service.images.asset_count() >= int(members.images.coverage.reviewed_images), "draft art includes the reviewed art")
		else:
			check(service.notice.begins_with("Draft English artwork could not be loaded") and not service.coverage.draft_images_active, "unopenable draft art keeps the reviewed art: " + service.notice)
			check(service.images.asset_count() == int(members.images.coverage.reviewed_images), "reviewed art stays active")
	service.wait()
	_finish()

func _finish() -> void:
	if failures.is_empty():
		print("Language bundle (built): builder output opens through LanguageBundleService passed")
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: " + failure)
		quit(1)
