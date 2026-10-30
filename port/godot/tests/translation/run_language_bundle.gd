extends SceneTree
## T-30002 WP2: semantic language bundle. 'en' resolves the English bundle
## manifest (schema tokimemo.english-bundle.v1) and opens its members on a
## worker; failures keep the previous language with a visible notice; drafts
## open only on opt-in. Synthetic packs in user://, no game data.
const Bundle = preload("res://services/language_bundle_service.gd")
const TextLayer = preload("res://services/text_layer_store.gd")
const Dialogue = preload("res://services/text_translation_store.gd")
const Preferences = preload("res://services/user_preferences_service.gd")
var failures: Array[String] = []
var folder := ""

func check(condition: bool, label: String = "unlabelled check") -> void:
	if not condition: failures.append(label)

func write_json(path: String, value: Variant) -> String:
	var file := FileAccess.open(path, FileAccess.WRITE); file.store_string(JSON.stringify(value)); file.close()
	return FileAccess.get_sha256(path)

func write_png(path: String, shade: int) -> String:
	var image := Image.create(4, 2, false, Image.FORMAT_RGBA8); image.fill(Color8(shade, 0, 0, 255))
	image.save_png(path)
	return FileAccess.get_sha256(path)

func image_pack(name: String, count: int, drafts: bool, first_shade: int = 10) -> Dictionary:
	var assets := []
	for index: int in count:
		var png := "%s-%d.png" % [name, index]
		var sha := write_png(folder.path_join(png), first_shade + index)
		assets.append({"source_container_sha256": "e".repeat(64), "source_png_sha256": ("%064x" % (index + 1)), "output_sha256": sha,
			"frame": index, "palette": "p0", "output": png, "style": "draft" if drafts else "approve", "dimensions": [4, 2]})
	var path := folder.path_join(name + ".json")
	var sha := write_json(path, {"schema": "tokimemo.translated-image-pack.v1", "language": "en", "development_drafts": drafts,
		"snapshot_sha256": "9".repeat(64), "assets": assets})
	return {"path": name + ".json", "sha256": sha}

func text_layer_pack() -> Dictionary:
	var entries := {}
	for index: int in 3:
		entries["talk:CDROM/X.BIN:main:g0000:l%04d" % index] = {"text": "Hi", "segments": [["g", "82678289"]], "context_status": "ready_for_review"}
	var sha := write_json(folder.path_join("ui-text.json"), {"schema": TextLayer.SCHEMA, "language": "en",
		"approval": {"approver": "user", "date": "2026-10-29", "instruction": "x", "scope": "x", "source": "x"},
		"font": {"source_path": "SLPM_86.053", "source_sha256": "a".repeat(64), "glyph_bank_sha256": "b".repeat(64)},
		"containers": {"CDROM/X.BIN": {"sha256": "c".repeat(64), "size": 10}}, "entries": entries})
	return {"path": "ui-text.json", "sha256": sha}

func dialogue_pack(text: String) -> Dictionary:
	var sha := write_json(folder.path_join("dialogue.json"), {"schema": Dialogue.SCHEMA, "language": "en", "source_payload_sha256": "a".repeat(64),
		"entries": {"0": {"speaker": "Player", "text": text}, "1": {"speaker": "Player", "text": "A second line."}}})
	return {"path": "dialogue.json", "sha256": sha}

func bundle(members: Dictionary, extra: Dictionary = {}) -> String:
	var rows := {}
	var schemas := Bundle.MEMBER_SCHEMAS
	for name: String in members:
		rows[name] = {"path": members[name].path, "sha256": members[name].sha256, "schema": members[name].get("schema", schemas[name][0]), "coverage": members[name].coverage}
	var value := {"schema": Bundle.SCHEMA, "language": "en", "generator": "tools/port/build_english_bundle.py",
		"disclosure": "English text is AI-translated and not yet human-reviewed; voices remain Japanese.",
		"members": rows, "omitted": []}
	for key: String in extra: value[key] = extra[key]
	var path := folder.path_join("english_bundle.json")
	write_json(path, value)
	return path

func members_standard() -> Dictionary:
	var ui := text_layer_pack(); ui.coverage = {"approved_records": 3, "representable_records": 2}
	var lines := dialogue_pack("A test translation."); lines.coverage = {"lines": 2}
	var approved := image_pack("approved", 1, false); approved.coverage = {"reviewed_images": 1, "original_japanese_images": 5}
	var drafts := image_pack("drafts", 2, true); drafts.coverage = {"draft_images": 2}
	return {"ui_text": ui, "dialogue": lines, "images": approved, "draft_images": drafts}

## Polls once per frame; returns frames until the switch settles.
func settle(service: RefCounted, limit: int = 600) -> int:
	var frames := 0
	while service.loading and frames < limit:
		await process_frame
		var started := Time.get_ticks_usec()
		service.poll()
		check(Time.get_ticks_usec() - started < 16000, "poll never blocks a frame")
		frames += 1
	return frames

func _initialize() -> void:
	call_deferred("run")

func run() -> void:
	folder = ProjectSettings.globalize_path("user://language-bundle-tests")
	DirAccess.make_dir_recursive_absolute(folder)
	var members := members_standard()
	var path := bundle(members)
	var service := Bundle.new()
	var applied := []
	var notices := []
	var warnings := []
	service.warning.connect(func(notice: String) -> void: warnings.append(notice))
	service.applied.connect(func(choice: String, coverage: Dictionary) -> void: applied.append([choice, coverage]))
	service.fallback.connect(func(notice: String) -> void: notices.append(notice))
	check(service.choice == "ja" and service.images == null and service.text_layer == null and service.dialogue == null, "starts in Japanese with no packs")
	check(Bundle.bundle_path("/install/profile") == "/install/profile/" + Bundle.BUNDLE_RELATIVE, "bundle path inside the installed profile")

	# 'en' opens asynchronously and applies in one step on poll().
	var started := Time.get_ticks_usec()
	check(service.request("en", {"bundle_path": path}), "en request accepted")
	check(Time.get_ticks_usec() - started < 16000, "request returns within a frame")
	check(service.loading and service.choice == "ja" and applied.is_empty(), "nothing applies before poll")
	var frames := await settle(service)
	check(frames >= 1 and not service.loading, "en settles on a later frame")
	check(service.choice == "en" and applied.size() == 1 and service.notice.is_empty(), "en applied: %s" % [service.notice])
	check(service.text_layer != null and service.text_layer.entry_count() == 3, "UI text layer opened")
	check(service.dialogue != null and service.dialogue.replacement("a".repeat(64), 0).text == "A test translation.", "dialogue opened")
	check(service.images != null and service.images.asset_count() == 1, "only reviewed art without the opt-in")
	var coverage: Dictionary = service.coverage
	check(coverage.text_records_approved == 3 and coverage.text_records_representable == 2 and coverage.images_reviewed == 1 and coverage.images_original_japanese == 5 and coverage.dialogue_lines == 2 and coverage.draft_images == 2 and not coverage.draft_images_active, "coverage matches the manifest: %s" % [coverage])
	check(service.coverage_line().contains("1 reviewed images") and service.coverage_line().contains("5 images stay in original Japanese"), "coverage line")
	check(service.disclosure.contains("AI-translated"), "disclosure carried")
	var active_layer: RefCounted = service.text_layer
	var active_images: RefCounted = service.images

	# Draft art: never read without the opt-in, used only with it.
	var draft_member: Dictionary = members.draft_images
	var draft_file := FileAccess.open(folder.path_join(draft_member.path), FileAccess.READ_WRITE); draft_file.seek_end(); draft_file.store_string(" "); draft_file.close()
	check(service.request("en", {"bundle_path": path, "allow_drafts": false}), "reload without drafts")
	await settle(service)
	check(service.choice == "en" and service.notice.is_empty() and service.images.asset_count() == 1, "a damaged draft pack is never opened without the opt-in")
	check(service.request("en", {"bundle_path": path, "allow_drafts": true}), "drafts requested")
	await settle(service)
	check(service.choice == "en" and service.text_layer != null and service.dialogue != null, "a damaged draft pack keeps English text and dialogue")
	check(service.images != null and service.images.asset_count() == 1 and not service.coverage.draft_images_active, "a damaged draft pack keeps the reviewed art")
	check(service.notice.begins_with("Draft English artwork could not be loaded") and warnings.size() == 1 and notices.is_empty(), "a damaged draft pack shows a warning, not a fallback: " + service.notice)
	members = members_standard(); path = bundle(members)
	check(service.request("en", {"bundle_path": path, "allow_drafts": true}), "drafts requested again")
	await settle(service)
	check(service.notice.is_empty() and service.images.asset_count() == 2 and service.coverage.draft_images_active, "drafts open only with the opt-in")
	# A draft pack that lacks a reviewed asset would hide reviewed art: refused.
	var thin := members.duplicate(true)
	thin.draft_images = image_pack("thin-drafts", 2, true, 50); thin.draft_images.coverage = {"draft_images": 2}
	check(service.request("en", {"bundle_path": bundle(thin), "allow_drafts": true}), "draft pack without the reviewed asset requested")
	await settle(service)
	check(service.choice == "en" and service.images.asset_count() == 1 and not service.coverage.draft_images_active and service.notice.contains("lacks 1 reviewed images") and warnings.size() == 2, "a draft pack missing reviewed art keeps the reviewed pack: " + service.notice)
	path = bundle(members)

	# Failures keep the previous language and its packs.
	service.request("en", {"bundle_path": path}); await settle(service)
	active_layer = service.text_layer; active_images = service.images
	var broken := members.duplicate(true); broken.dialogue.sha256 = "0".repeat(64)
	check(service.request("en", {"bundle_path": bundle(broken)}), "mismatch request accepted")
	await settle(service)
	check(service.choice == "en" and service.text_layer == active_layer and service.images == active_images, "sha mismatch keeps the active packs")
	check(service.notice.begins_with("English (AI translation) could not be loaded") and service.notice.contains("Keeping English"), "visible fallback notice: " + service.notice)
	var missing := members.duplicate(true); missing.images.path = "absent.json"
	service.request("en", {"bundle_path": bundle(missing)}); await settle(service)
	check(service.images == active_images and service.notice.contains("English artwork"), "missing member file keeps the previous set")
	var miscounted := members.duplicate(true); miscounted.images.coverage.reviewed_images = 44
	service.request("en", {"bundle_path": bundle(miscounted)}); await settle(service)
	check(service.images == active_images and service.notice.contains("coverage"), "coverage that disagrees with the pack is rejected")
	var inconsistent := members.duplicate(true); inconsistent.ui_text.coverage.representable_records = 9
	service.request("en", {"bundle_path": bundle(inconsistent)}); await settle(service)
	check(not service.notice.is_empty() and service.text_layer == active_layer, "representable above approved is rejected")
	# Image members may be v1 or v2 packs (v2 needs the v2 image store); others are refused.
	var v2 := members.duplicate(true); v2.images.schema = "tokimemo.translated-image-pack.v2"; v2.draft_images.schema = "tokimemo.translated-image-pack.v2"
	check(Bundle.read_bundle(bundle(v2)).ok, "v2 image pack members are accepted by the manifest")
	var v3 := members.duplicate(true); v3.images.schema = "tokimemo.translated-image-pack.v3"
	check(not Bundle.read_bundle(bundle(v3)).ok, "unknown image pack schema refused")
	for extra: Dictionary in [{"schema": "tokimemo.english-bundle.v0"}, {"disclosure": ""}, {"members": {}}, {"members": {"voice": members.ui_text}}, {"language": "fr"}]:
		service.request("en", {"bundle_path": bundle(members, extra)}); await settle(service)
		check(service.choice == "en" and service.text_layer == active_layer and not service.notice.is_empty(), "invalid bundle %s keeps the active set" % [extra.keys()])
	service.request("en", {"bundle_path": folder.path_join("nowhere.json")}); await settle(service)
	check(service.notice.contains("missing"), "missing bundle falls back")
	var traversal := members.duplicate(true); traversal.ui_text.path = "../ui-text.json"
	service.request("en", {"bundle_path": bundle(traversal)}); await settle(service)
	check(service.text_layer == active_layer and not service.notice.is_empty(), "member paths cannot leave the bundle directory")

	# An omitted member leaves that part in Japanese; the rest loads.
	var partial := members.duplicate(true); partial.erase("images"); partial.erase("draft_images")
	service.request("en", {"bundle_path": bundle(partial, {"omitted": [{"member": "images", "reason": "no approved images"}]})}); await settle(service)
	check(service.choice == "en" and service.images == null and service.coverage.images_reviewed == 0 and service.omitted.size() == 1, "omitted member stays original")
	var drafts_only := members.duplicate(true); drafts_only.erase("images")
	service.request("en", {"bundle_path": bundle(drafts_only)}); await settle(service)
	check(not service.notice.is_empty(), "drafts without the reviewed pack are rejected")

	# A regenerated bundle with new digests keeps English.
	var regenerated := members.duplicate(true)
	var new_lines := dialogue_pack("A regenerated translation."); new_lines.coverage = {"lines": 2}
	regenerated.dialogue = new_lines
	check(regenerated.dialogue.sha256 != members.dialogue.sha256, "regenerated digest differs")
	service.request("en", {"bundle_path": bundle(regenerated)}); await settle(service)
	check(service.choice == "en" and service.notice.is_empty() and service.dialogue.replacement("a".repeat(64), 0).text == "A regenerated translation.", "regenerated bundle keeps English")
	var prefs := Preferences.new("user://language-bundle-tests/prefs.json")
	DirAccess.remove_absolute(ProjectSettings.globalize_path(prefs.path)); prefs.load_preferences()
	check(prefs.set_value("language", "bundle", "en") and JSON.stringify(prefs.settings.language).find("sha256\":\"" + regenerated.dialogue.sha256) < 0, "the preference stores the word, not the digest")

	# 'ja' clears at once; a superseded request is discarded.
	check(service.request("ja") and service.choice == "ja" and service.images == null and service.text_layer == null and service.dialogue == null and not service.loading, "Japanese clears every pack at once")
	var applied_before := applied.size()
	var notices_before := notices.size()
	# A bundle that opens cleanly, so only the generation check can keep 'ja'.
	check(Bundle.open_english(bundle(regenerated), false).ok, "the superseded bundle itself is valid")
	service.request("en", {"bundle_path": bundle(regenerated)})
	service.request("ja")
	check(service.busy(), "the superseded English task is still outstanding")
	# Drive only the per-frame poll() path until the worker has been collected.
	var stale_frames := 0
	while service.busy() and stale_frames < 600:
		await process_frame
		service.poll()
		stale_frames += 1
	check(not service.busy(), "poll collects the superseded task")
	check(service.choice == "ja" and service.text_layer == null and service.images == null and not service.loading, "a stale English result never overrides a later Japanese choice")
	check(applied.size() == applied_before + 1 and applied.back()[0] == "ja", "only the Japanese choice was applied")
	check(notices.size() == notices_before and service.notice.is_empty(), "the stale result is discarded silently")
	check(not service.request("fr"), "unknown choice rejected")

	# 'custom' opens the user-picked manifests with their stored sha256.
	var custom := {"ui_text_manifest": folder.path_join(members.ui_text.path), "ui_text_sha256": members.ui_text.sha256,
		"dialogue_manifest": folder.path_join(regenerated.dialogue.path), "dialogue_sha256": regenerated.dialogue.sha256}
	service.request("custom", {"language": custom}); await settle(service)
	check(service.choice == "custom" and service.text_layer != null and service.dialogue != null and service.images == null, "custom packs open")
	var custom_layer: RefCounted = service.text_layer
	custom.ui_text_sha256 = "1".repeat(64)
	service.request("custom", {"language": custom}); await settle(service)
	check(service.choice == "custom" and service.text_layer == custom_layer and service.notice.begins_with("Custom packs could not be loaded"), "custom sha mismatch shows a notice")
	service.wait()

	if failures.is_empty():
		print("Language bundle: async atomic en open, coverage from manifests, draft opt-in, sha/missing/coverage/schema fallbacks keep the previous set, omitted members, regenerated digests, ja clear, stale results, custom packs passed")
		quit(0)
	else:
		for failure: String in failures: printerr("FAIL: " + failure)
		quit(1)
