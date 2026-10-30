extends SceneTree
## Isolated candidate extension only; no runtime library publication.
var failures := 0
func check(ok: bool, label: String) -> void:
	if not ok: failures += 1; printerr(label)
func _initialize() -> void:
	if not GDExtensionManager.is_extension_loaded("res://original_audio.gdextension"):
		GDExtensionManager.load_extension("res://original_audio.gdextension")
	var engine = ClassDB.instantiate("OriginalAudioEngine")
	if engine == null or not engine.has_method("load_rei_bank"):
		printerr("Test requires isolated Rei candidate extension"); quit(1); return
	var source = load("res://bgm_source_pack.gd").read_pack(OS.get_environment("ORIGINAL_BGM_PACK"))
	if not source.error.is_empty(): printerr(source.error); quit(1); return
	check(engine.initialize_inputs(source.files,93).is_empty(),"Original arrival initialization failed")
	var rei = load("res://services/rei_bank_resources.gd").read_inputs(OS.get_environment("REI_BANK_PACK"))
	if not rei.error.is_empty(): printerr(rei.error); quit(1); return
	var inputs := [rei.inputs.music_header,rei.inputs.music_body,rei.inputs.sequence]
	var bad := inputs.duplicate(true); bad[1][0] ^= 1
	check(not engine.load_rei_bank(bad).is_empty() and not engine.diagnostics().rei_loaded,"Native engine accepted substituted Rei bytes")
	check(engine.load_rei_bank(inputs).is_empty(),"Native Rei alternate load failed")
	check(not engine.diagnostics().stopped and not engine.diagnostics().rei_started,"Alternate load interrupted arrival sequence")
	check(engine.begin_rei_fade().is_empty(),"Source Rei fade start failed")
	for tick in range(10):
		check(engine.tick_rei_fade(false).is_empty(),"Source fade update failed")
		var rendered: Dictionary = engine.render_block()
		check(rendered.error.is_empty(),"Arrival/fade rendering failed: " + str(rendered.error))
	check(engine.diagnostics().stopped and engine.diagnostics().fade_ticks == 10,"Source gain93/step10 fade duration differs")
	check(engine.start_rei(false).is_empty(),"Native original Rei sequence start failed")
	var peak := 0.0
	for frame in range(120):
		var rendered: Dictionary = engine.render_block()
		check(rendered.error.is_empty(),"Original Rei rendering failed: " + str(rendered.error))
		if not rendered.error.is_empty(): break
		for sample: Vector2 in rendered.pcm: peak = maxf(peak,maxf(absf(sample.x),absf(sample.y)))
	check(peak > 0.001 and engine.diagnostics().sequence_gain == 83,"Original Rei sequence produced silence or wrong VAB gain")
	print("Native Rei diagnostics: ",engine.diagnostics().duplicate().merged({"mutated_music_header":"omitted","SPU_registers":"omitted"},true))
	engine.dispose()
	_verify_master_fade(source.files)
	var early = ClassDB.instantiate("OriginalAudioEngine")
	check(early.initialize_inputs(source.files,93).is_empty() and early.load_rei_bank(inputs).is_empty(),"Early Rei setup failed")
	check(early.begin_rei_fade().is_empty() and early.tick_rei_fade(false).is_empty(),"Early Rei fade failed")
	check(early.start_rei(false).is_empty() and not early.diagnostics().fade_active and early.diagnostics().sequence_gain == 83,"New Rei request failed to finish outstanding source fade")
	check(early.render_block().error.is_empty(),"Early Rei sequence render failed")
	early.dispose()
	if failures == 0: print("PASS: native original Rei bank/fade/sequence; PCM parity not claimed")
	quit(1 if failures else 0)

func _verify_master_fade(files: Array) -> void:
	# Regression for the reviewed candidate that reported gain3 after nine
	# ticks yet emitted nine byte-identical blocks to an unfaded engine.
	var baseline = ClassDB.instantiate("OriginalAudioEngine")
	var fading = ClassDB.instantiate("OriginalAudioEngine")
	var xa = ClassDB.instantiate("OriginalAudioEngine")
	for engine in [baseline,fading,xa]: check(engine.initialize_inputs(files,93).is_empty(),"Paired fade initialization failed")
	for frame in range(60):
		var reference: Dictionary = baseline.render_block()
		check(reference.pcm == fading.render_block().pcm and reference.pcm == xa.render_block().pcm,"Identical original engines diverged before fade")
	check(fading.begin_rei_fade().is_empty() and xa.begin_rei_fade().is_empty(),"Paired fade request failed")
	var changed := 0
	for tick in range(9):
		check(fading.tick_rei_fade(false).is_empty() and xa.tick_rei_fade(true).is_empty(),"Paired source fade tick failed")
		var a: Dictionary = baseline.render_block()
		var b: Dictionary = fading.render_block()
		var c: Dictionary = xa.render_block()
		check(a.error.is_empty() and b.error.is_empty() and c.error.is_empty(),"Paired fade render failed")
		if a.pcm != b.pcm: changed += 1
		var base_regs: PackedByteArray = baseline.diagnostics().SPU_registers
		var fade_regs: PackedByteArray = fading.diagnostics().SPU_registers
		var xa_regs: PackedByteArray = xa.diagnostics().SPU_registers
		check(xa_regs.decode_u16(0x180) == base_regs.decode_u16(0x180),"XA-active fade changed global SPU master volume")
		var ratio := float(fade_regs.decode_u16(0x180))/float(base_regs.decode_u16(0x180))
		var max_error := 0.0
		for index in a.pcm.size():
			var expected: Vector2 = a.pcm[index] * ratio
			var actual: Vector2 = b.pcm[index]
			max_error = maxf(max_error,maxf(absf(expected.x-actual.x),absf(expected.y-actual.y)))
		check(max_error <= 2.0/32768.0,"Master fade PCM differs from original register scaling at tick%d: %f" % [tick,max_error])
	check(changed == 9,"Master fade left original PCM unchanged")
	for engine in [baseline,fading,xa]: engine.dispose()
