extends SceneTree
## Focused source hud_screen_frame_draw(2) transition; not a route test.
const Hud = preload("res://presentation/prologue_hud_source_provider.gd")
const Rng = preload("res://core/native_rng.gd")
var checks := 0
var failures := 0
func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL ",label)
func _initialize() -> void: run.call_deferred()
func state(stage: int) -> Dictionary:
	return {"game_mode":21,"mode_stage":stage,"mode_step":0,"substep":0,"frame_count":173,"player_flags":PackedByteArray([0,0]),"pad_held_triangle":false,
		"opening":{"screen_brightness":128,"prologue_line":0,"actor96_draw_flags":0xa4,
		"arrival_media":{"actor_initialized":true,"upload_batch_key":"actual-unit-source-batch"},
		"prologue_text_work":{"line_id":0,"pos":1,"total":1},
		"prologue_hud_live":{"year":95,"month":4,"day":4,"weekday":1,"day_kind":0,"title_timer":184,
			"checker_scroll_u":31,"checker_scroll_v":255,"checker_scroll_owner":"focused-source-transition",
			"screen_brightness":128,"checker_fade":255,"title_visible":false,"date_visible":true,"message_visible":true,
			"checker_scroll_phase":"before_outer_scroll_update"}}}
func image() -> Image:
	var result := Image.create(320,240,false,Image.FORMAT_RGBA8)
	result.fill(Color8(29,81,153,127))
	return result
func run() -> void:
	var root := OS.get_environment("PROLOGUE_HUD_ROOT")
	var hud := Hud.new()
	check(hud.open_installed(root,"packs/prologue_hud/manifest.json",Hud.INSTALLED_PROFILE_SHA256).is_empty(),"authentic installed HUD opens")
	if failures: quit(1); return
	var rng := Rng.new(); rng.configure(Rng.OPENBIOS_PROFILE,9)
	var first := state(0)
	var transition := state(1)
	var a := {}; var b := {}
	check(hud.advance_outer(first,rng,a,0).is_empty(),"actual stage0 argument0 advances")
	check(hud.advance_outer(transition,rng,b,2).is_empty(),"actual stage1 boundary argument2 advances")
	for key: String in ["checker_scroll_u","checker_scroll_v","checker_fade","title_timer","checker_scroll_phase"]:
		check(first.opening.prologue_hud_live[key] == transition.opening.prologue_hud_live[key],"same sourced update "+key)
	check(first.mode_stage == 0 and transition.mode_stage == 1,"actual source stage is retained")
	check(first.opening.prologue_hud_live.screen_frame_argument == 0 and transition.opening.prologue_hud_live.screen_frame_argument == 2,"source call arguments are retained")
	var full := image(); var masked := image(); var expected := image()
	check(hud.draw_checker(full,first).is_empty(),"full checker draws")
	check(hud.draw_checker(masked,transition).is_empty(),"argument2 checker draws")
	# Independent native enabled[col][row] table, not the optimized predicate.
	var enabled: Array = []
	for col in 10:
		var rows: Array = []
		for row in 8: rows.append(true)
		enabled.append(rows)
	for row in 6:
		for col in 10: enabled[col][row] = false
	enabled[0][6] = true; enabled[9][6] = true
	enabled[0][5] = true; enabled[9][5] = true
	var tiles := 0
	for col in 10:
		for row in 8:
			if not enabled[col][row]: continue
			tiles += 1
			expected.blit_rect(full,Rect2i(col*32,row*32,32,mini(32,240-row*32)),Vector2i(col*32,row*32))
	check(tiles == 22 and masked.get_data() == expected.get_data(),"all pixels match native22 enabled sprites and last-row clip")
	for point: Vector2i in [Vector2i(0,0),Vector2i(319,159),Vector2i(32,160),Vector2i(287,191)]:
		check(masked.get_pixelv(point) == Color8(29,81,153,127),"disabled tile preserves framebuffer "+str(point))
	for defect: String in ["argument","stage","step","substep","visibility","line","text","upload"]:
		var bad := state(1)
		var argument := 2
		match defect:
			"argument": argument = 0
			"stage": bad.mode_stage = 2
			"step": bad.mode_step = 1
			"substep": bad.substep = 1
			"visibility": bad.opening.actor96_draw_flags = 0x24
			"line": bad.opening.prologue_line = 1
			"text": bad.opening.prologue_text_work.pos = 0
			"upload": bad.opening.arrival_media.upload_batch_key = ""
		var before := bad.duplicate(true)
		check(not hud.advance_outer(bad,rng,{},argument).is_empty() and bad == before,"unknown transition rejects unchanged "+defect)
	print("PASS: %d HUD argument2 transition checks"%checks if failures == 0 else "FAIL: %d/%d"%[failures,checks])
	quit(0 if failures == 0 else 1)
