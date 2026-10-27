extends SceneTree
## Reader/launch refusal boundaries. Genuine installed success is exercised by
## the separate connected GUI test, never by inventing ready resources here.
const App = preload("res://app/first_arrival_boot.gd")
const Resources = preload("res://services/first_arrival_launch_resources.gd")
var checks := 0
var failures := 0

func check(value: bool, label: String) -> void:
	checks += 1
	if not value:
		failures += 1
		printerr("FAIL: ",label)

func _initialize() -> void:
	call_deferred("run")

func run() -> void:
	for bad: Variant in [null,{},[],false,1.0,"true"]:
		var profile := {"valid":bad,"profile_root":"/missing","installation_manifest_sha256":"0".repeat(64)}
		var before := profile.duplicate(true)
		check(not Resources.current_profile(profile).error.is_empty() and profile == before,"malformed validity rejects without changing profile")
	for bad: Variant in [null,{},[],false,1.0,"relative/root","/missing/../pack"]:
		var profile := {"valid":true,"profile_root":bad,"installation_manifest_sha256":"0".repeat(64)}
		check(not Resources.current_profile(profile).error.is_empty(),"missing or unsafe installed root rejects")
	for bad: Variant in [null,{},[],false,1.0,"bad",""]:
		var profile := {"valid":true,"profile_root":"/missing","installation_manifest_sha256":bad}
		check(not Resources.current_profile(profile).error.is_empty(),"malformed installation pin rejects")
	for wrong_path: String in ["","save-load","resume","options","return-to-title"]:
		var app := App.new()
		app.initialization_profile = wrong_path
		root.add_child(app)
		await process_frame
		check(app.failure_gate == "fresh_initialization_profile" and app.consumer == null and not app._active,"non-fresh launch cannot construct or advance a New Game Session")
		app.queue_free()
		await process_frame
	print("PASS: %d installed first-arrival refusal checks" % checks if failures == 0 else "FAIL: %d/%d installed first-arrival refusal checks" % [failures,checks])
	quit(0 if failures == 0 else 1)
