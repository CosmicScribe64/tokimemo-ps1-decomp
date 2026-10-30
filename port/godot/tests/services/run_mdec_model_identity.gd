extends SceneTree
## Original generated packs; store integrity only, not route/pixel parity.
const Identity = preload("res://services/mdec_model_identity.gd")
const School = preload("res://services/school_day_media_store.gd")
const Names = preload("res://services/name_entry_image_store.gd")
const Spring = preload("res://services/spring_school_day_content_store.gd")
const Clubs = preload("res://services/club_command_content_store.gd")
const Weekday = preload("res://services/weekday_56_content_store.gd")
const Hud = preload("res://presentation/command_hud_presenter.gd")
const Seasonal = preload("res://services/seasonal_command_content_store.gd")
var errors: Array[String] = []
func check(ok: bool, label: String) -> void:
	if not ok: errors.append(label)
func _initialize() -> void:
	var base := OS.get_environment("MDEC_BUNDLE")
	var school := School.new()
	check(school.open_pack(OS.get_environment("MDEC_SCHOOL")), "whole original school pack: "+school.error_message())
	for command: int in [0,7]:
		check(not school.get_background("command_%d_background"%command).is_empty(), "background loaded")
		check(not school.get_frame("command_%d_sd"%command,0).is_empty(), "original SD frame loaded")
		check(not school.get_sequence("command_%d_sd"%command).is_empty(), "original SD sequence loaded")
	check(not school.open_pack(OS.get_environment("MDEC_BAD_SCHOOL")), "changed SD bytes rejected on reopen")
	check(school.get_frame("command_0_sd",0).is_empty(), "failed reopen clears loaded SD frame")
	check(school.get_background().is_empty(), "failed reopen clears loaded background")
	var names := Names.new()
	var path := base.path_join("packs/name_entry_media/manifest.json")
	check(names.open_pack(path,FileAccess.get_sha256(path)),"NAME current model: "+names.error)
	var metadata: Dictionary = names.manifest.base_background.decode.duplicate(true)
	check(Identity.valid(metadata),"valid numeric identity")
	for mutation: int in range(5):
		var bad := metadata.duplicate(true)
		match mutation:
			0: bad.decoder_model.implementation_sha256["image_mdec_model.py"] = "0".repeat(64)
			1: bad.decoder_model.id = "legacy-preview"
			2: bad.decoder_model.hardware_pixel_parity_verified = 0
			3: bad.erase("decoder_model")
			4: bad.decoder_model.game_runlevel_transport_independently_verified = true
		check(not Identity.valid(bad),"invalid model rejected %d"%mutation)
	var stale := OS.get_environment("MDEC_STALE_NAME")
	check(not Names.new().open_pack(stale,FileAccess.get_sha256(stale)),"authentic stale NAME model rejected")
	var spring := Spring.new(); check(spring.open_pack(base.path_join("packs/spring_media")),"spring current pack: "+spring.error_message())
	var clubs := Clubs.new(); check(clubs.open_pack(base.path_join("packs/club_media")),"club current pack: "+clubs.error_message())
	var weekday := Weekday.new(); check(weekday.open_pack(base.path_join("packs/weekday56_media")),"weekday current pack: "+weekday.error_message())
	var installed: Dictionary = JSON.parse_string(FileAccess.get_file_as_string(base.path_join("manifest.json")))
	var hud := Hud.new()
	var hud_path := base.path_join(installed.roles.hud_manifest)
	check(hud.open_manifest(hud_path,FileAccess.get_sha256(hud_path),base.path_join(installed.roles.font_root)),"HUD current original-room pack")
	var seasonal := Seasonal.new()
	check(seasonal.open_pack(OS.get_environment("MDEC_SEASONAL"),Seasonal.MANIFEST_SHA),"seasonal current model pack")
	print("MDEC authenticated store checks: ",errors)
	quit(0 if errors.is_empty() else 1)
