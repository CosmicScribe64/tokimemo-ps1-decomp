extends SceneTree
## Actual fresh developer-prepared original pack, not a production raw import.
const Profile = preload("res://services/installed_launch_profile.gd")
const Resources = preload("res://services/first_arrival_launch_resources.gd")
var checks := 0
var failures := 0

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func _initialize() -> void:
	var root := OS.get_environment("FIRST_ARRIVAL_PROFILE_ROOT")
	var manifest: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(root.path_join("manifest.json")))
	var profile := Profile.read_pack(root,manifest)
	check(profile.get("valid",false) and profile.available_capabilities.has(Profile.OPENING_RESOURCES_CAP),"actual350-file original pack validates inventoried opening roles")
	if failures != 0:
		quit(1)
		return
	check(Resources.current_profile(profile).error.is_empty(),"actual installed locators and manifest pins revalidate")
	for field: String in ["schema","version","release_profile"]:
		for value: Variant in [{},[],null,false,"wrong-release"]:
			var bad := manifest.duplicate(true); bad[field] = value
			check(not Profile.read_pack(root,bad).get("valid",false),"malformed or foreign release metadata rejects: "+field)
	for value: Variant in [{},[],null,false,"0".repeat(64)]:
		var bad := manifest.duplicate(true); bad.source.executable_sha256 = value
		check(not Profile.read_pack(root,bad).get("valid",false),"foreign original executable identity rejects")
	var incomplete := manifest.duplicate(true)
	incomplete.roles.erase("prologue_text")
	var incomplete_profile := Profile.read_pack(root,incomplete)
	check(incomplete_profile.get("valid",false) and not incomplete_profile.available_capabilities.has(Profile.OPENING_RESOURCES_CAP),"entry files alone cannot grant opening-resource capability")
	check(not Resources.current_profile(incomplete_profile).error.is_empty(),"locators from a missing-role projection cannot replace actual installation")
	for defect: String in ["stale_installation","foreign_pack","wrong_role","missing_hash","wrong_hash_type","missing_manifest"]:
		var bad := profile.duplicate(true)
		match defect:
			"stale_installation": bad.installation_manifest_sha256 = "0".repeat(64)
			"foreign_pack": bad.paths.prologue_text = "/another-installation/packs/prologue_text"
			"wrong_role": bad.paths.prologue_text = bad.paths.font_root
			"missing_hash": bad.role_manifests.prologue_text.erase("sha256")
			"wrong_hash_type": bad.role_manifests.prologue_text.sha256 = {}
			"missing_manifest": bad.erase("role_manifests")
		var before := bad.duplicate(true)
		var resources := Resources.new()
		check(not resources.prepare(bad).is_empty() and Resources._same(bad,before) and resources.launch_profile.is_empty() and resources.text == null,"invalid installed binding rejects before claiming resources: "+defect)
	var target: String = profile.paths.prologue_text.path_join("prologue.json")
	var original := FileAccess.get_file_as_bytes(target)
	var altered := original.duplicate(); altered[0] ^= 1
	var file := FileAccess.open(target,FileAccess.WRITE); file.store_buffer(altered); file.close()
	var resources := Resources.new()
	check(not resources.prepare(profile).is_empty() and resources.launch_profile.is_empty(),"actual modified inventoried payload refuses preparation")
	file = FileAccess.open(target,FileAccess.WRITE); file.store_buffer(original); file.close()
	check(resources.prepare(profile).is_empty() and resources.text != null and resources.media_store != null and resources.hud != null and resources.audio.prepared_bank_roles.has("prologue_arrival"),"restored authentic resources prepare atomically without future voice PCM")
	check(not resources.prepare(profile).is_empty(),"prepared resource owner cannot be silently rebound")
	print("PASS: %d actual installed first-arrival profile checks"%checks if failures == 0 else "FAIL: %d/%d installed profile checks"%[failures,checks])
	quit(0 if failures == 0 else 1)
