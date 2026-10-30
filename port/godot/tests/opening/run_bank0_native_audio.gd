extends SceneTree
## native-bank0: native candidate only. From the Rei start through the Shiori
## race, stage16 audio_bank_start(0,1) (B1 fade of Shiori, PRO_VOL sequence 1),
## stage20/21 D1 (the second ignored), the stage21 COM_MIX/SPRING landing over
## the live bank0 score/header, the +131 0x11 transfer (slot0, slot1 one pump
## apart) and the spring audio_bank_start(0,0). Every pump R+4759..R+7255 is
## compared with M3 (.asset-cache/emu/finish-landing-m3 r1, R = C1 call frame):
## MVOL, sequence gain and key-on sample addresses.
## Env: ORIGINAL_BGM_PACK, REI_BANK_PACK, SHIORI_BANK_PACK, FINISH_BANK_PACK, BANK0_M3.
const Shiori = preload("res://services/prologue_shiori_bank_resources.gd")
const Bank0 = preload("res://services/prologue_bank0_audio.gd")
const FinishBank = preload("res://services/prologue_finish_bank_store.gd")
## compare_bank0.py --emit of M3 r1 (frames 42b4e3ed..., events d7d41d20...).
const EXPECTATIONS_SHA := "3f61a6ba10cbf120312b7bdfc4fbb4da206193d2835a6ba16696a1e3485e5e58"
## Pumps after R (M3 r1 events): Rei start 257 -> C1 2206, stage16 6964, stage20 D1 9315, stage21 9316.
const PREFIX := 1949
const SHIORI_POLL := 181
const STAGE16 := 4758
const STAGE20 := 7109
const STAGE21 := 7110
var failures := 0
var checks := 0
var source: Dictionary
var rei_inputs: Array
var shiori: Dictionary
var finish: Array
var expect: Dictionary

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL: " + label)

func _initialize() -> void:
	if not GDExtensionManager.is_extension_loaded("res://original_audio.gdextension"):
		GDExtensionManager.load_extension("res://original_audio.gdextension")
	var probe = ClassDB.instantiate("OriginalAudioEngine")
	if probe == null or not probe.has_method("begin_bank0_load"):
		printerr("Test requires the isolated native-bank0 candidate extension"); quit(1); return
	source = load("res://bgm_source_pack.gd").read_pack(OS.get_environment("ORIGINAL_BGM_PACK"))
	if not source.error.is_empty(): printerr(source.error); quit(1); return
	var rei = load("res://services/rei_bank_resources.gd").read_inputs(OS.get_environment("REI_BANK_PACK"))
	if not rei.error.is_empty(): printerr(rei.error); quit(1); return
	rei_inputs = [rei.inputs.music_header,rei.inputs.music_body,rei.inputs.sequence]
	shiori = Shiori.read_inputs(OS.get_environment("SHIORI_BANK_PACK"))
	if not shiori.error.is_empty(): printerr(shiori.error); quit(1); return
	var store := FinishBank.new()
	var opened := store.open_pack(OS.get_environment("FINISH_BANK_PACK"))
	var reads := Bank0.native_inputs(store)
	if not opened.is_empty() or not reads.error.is_empty(): printerr(opened + reads.error); quit(1); return
	finish = reads.inputs
	var path := OS.get_environment("BANK0_M3").path_join("native-expectations.json")
	check(FileAccess.get_sha256(path) == EXPECTATIONS_SHA,"M3 expectations differ from the pinned table")
	expect = JSON.parse_string(FileAccess.get_file_as_string(path))
	_main_route()
	_negatives()
	print("bank0 native: %d checks, %d failures" % [checks,failures])
	if failures == 0: print("PASS: native bank0 route reproduces M3 gain/MVOL/key-ons R+4759..R+7255; hardware PCM parity not claimed")
	quit(1 if failures else 0)

func _render(engine, count: int) -> String:
	for _i in range(count):
		var block: Dictionary = engine.render_block()
		if not block.error.is_empty(): return block.error
	return ""

## Engine at the stage16 call frame (STAGE16 pumps after the C1 call).
func _stage16_engine():
	var engine = ClassDB.instantiate("OriginalAudioEngine")
	if not engine.initialize_inputs(source.files,93).is_empty() or not engine.load_rei_bank(rei_inputs).is_empty() or not engine.begin_rei_fade().is_empty(): return null
	for _tick in range(10):
		if not engine.tick_rei_fade(false).is_empty() or not engine.render_block().error.is_empty(): return null
	if not engine.start_rei(false).is_empty() or not _render(engine,PREFIX).is_empty(): return null
	if not engine.begin_fade(0xc0,1,true).is_empty() or not engine.begin_shiori_load(Shiori.native_inputs(shiori.inputs),Shiori.flat_schedule()).is_empty(): return null
	if not _render(engine,SHIORI_POLL).is_empty() or not engine.finish_shiori().is_empty() or not _render(engine,STAGE16-SHIORI_POLL).is_empty(): return null
	return engine

static func _rms(pcm: PackedVector2Array) -> float:
	var sum := 0.0
	for sample: Vector2 in pcm: sum += sample.x*sample.x + sample.y*sample.y
	return sqrt(sum/1470.0)

func _main_route() -> void:
	var engine = _stage16_engine()
	check(engine != null,"route to stage16 failed")
	if engine == null: return
	var r_abs: int = int(engine.bank0_diagnostics().pumps) - STAGE16
	var started: Dictionary = engine.bank0_start(1,Bank0.STAGE16_TICK_SKIP)
	check(started.error.is_empty() and started.status == 1,"stage16 bank0 start refused: " + str(started))
	check(_render(engine,STAGE20-STAGE16).is_empty(),"stage16..20 run failed")
	var d16: Dictionary = engine.bank0_diagnostics()
	check(not d16.stopped and d16.bgm_sequence == 1 and d16.sequence_gain == 93 and d16.pack_word0 == 0x40,"PRO_VOL sequence 1 not playing at gain 93: " + str(d16.sequence_gain))
	for command: int in [5,0xd1]: check(engine.submit_command(command).is_empty(),"stage20 command refused")
	check(_render(engine,1).is_empty(),"stage20->21 pump failed")
	for command: int in [5,0xd1]: check(engine.submit_command(command).is_empty(),"stage21 command refused")
	check(engine.begin_bank0_load(finish,Bank0.flat_schedule()).is_empty(),"stage21 bank0 load refused")
	var rms := {}
	for pump in range(1,Bank0.READ_DONE_FRAMES[4]+Bank0.TRANSFER_READY_FRAMES+20):
		var block: Dictionary = engine.render_block()
		if not block.error.is_empty(): check(false,"bank0 pump +%d failed: %s" % [pump,block.error]); return
		rms[pump] = _rms(block.pcm)
		var landed: Array = engine.bank0_diagnostics().bank0_landed
		var expected := [0,0,0,0,0]
		for row: Array in Bank0.LANDING_SCHEDULE: if row[0] <= pump: expected[row[1]] = row[2]
		if landed != expected: check(false,"landed sectors at +%d are %s, measured %s" % [pump,landed,expected]); return
		if pump == Bank0.READ_DONE_FRAMES[4]: check(engine.submit_command(0x11).is_empty(),"0x11 refused at +131")
		if pump == Bank0.READ_DONE_FRAMES[4]+1: check(not engine.bank0_diagnostics().transfer_done,"slot1 ready one pump early")
		if pump == Bank0.READ_DONE_FRAMES[4]+Bank0.TRANSFER_READY_FRAMES:
			check(engine.bank0_diagnostics().transfer_done,"bank0 transfer not done at the reload poll")
			var spring: Dictionary = engine.bank0_start(0,Bank0.SPRING_TICK_SKIP)
			check(spring.error.is_empty() and spring.status == 1,"spring start refused: " + str(spring))
	var d: Dictionary = engine.bank0_diagnostics()
	check(d.bank0_loaded and d.transfer_done and not d.stopped and d.bgm_sequence == 0 and d.sequence_gain == 94 and not d.system_flag,"spring sequence 0 not playing at gain 94: " + str(d.sequence_gain))
	# Source event pumps after R: B1 4759, B1 finish + PRO_VOL start 4769, D1 7110,
	# ignored D1 7111, dispose 0/1 + fade finish + slot0 7242, slot1 7243, spring 7244.
	var events := []
	for row: PackedInt64Array in d.bank0_log:
		if int(row[1]) in [2,3,4,6,7,8] and int(row[0]) - r_abs >= STAGE16: events.append([int(row[0]) - r_abs,int(row[1]),int(row[2])])
	check(events == [[4759,2,0x100],[4769,3,-9],[4769,4,1],[7110,2,0x80],[7111,8,0x80],[7242,6,0],[7242,6,1],[7242,3,27],[7242,7,0],[7243,7,1],[7244,4,0]],"bank0 event pumps differ: " + str(events))
	# MVOL 0 from the 0x11 fade finish until the spring start: exact silence.
	var spring_peak := 0.0
	for pump in range(135,Bank0.READ_DONE_FRAMES[4]+Bank0.TRANSFER_READY_FRAMES+20): spring_peak = maxf(spring_peak,rms[pump])
	check(rms[133] == 0.0 and rms[134] == 0.0 and spring_peak > 0.0,"PCM around the transfer differs: %s %s peak %s" % [rms[133],rms[134],spring_peak])
	_compare(d,r_abs)

func _compare(d: Dictionary, r_abs: int) -> void:
	var log := {}
	var flat: PackedInt64Array = d.bank0_pump_log
	for i in range(0,flat.size(),4): log[int(flat[i]) - r_abs] = [int(flat[i+1]),int(flat[i+2])]
	var kon := {}
	var flat_kon: PackedInt64Array = d.bank0_kon_log
	for i in range(0,flat_kon.size(),4): kon.get_or_add(int(flat_kon[i]) - r_abs,[]).append(int(flat_kon[i+2]))
	var gain_mismatch := []
	var kon_mismatch := []
	var kons := 0
	for key: String in expect.pumps:
		var pump := int(key)
		var row: Array = expect.pumps[key]
		if not expect.skip_gain.has(float(pump)) and log.get(pump) != [int(row[0]),int(row[1])]: gain_mismatch.append(pump)
		var ours: Array = kon.get(pump,[]); ours.sort()
		var theirs: Array = row[2].map(func(x): return int(x))
		kons += theirs.size()
		if ours != theirs: kon_mismatch.append(pump)
	check(expect.pumps.size() == 2497 and kons == 1257,"M3 table coverage differs: %d pumps, %d key-ons" % [expect.pumps.size(),kons])
	check(gain_mismatch.is_empty(),"MVOL/gain differs from M3 at pumps " + str(gain_mismatch.slice(0,20)))
	check(kon_mismatch.is_empty(),"key-on samples differ from M3 at pumps " + str(kon_mismatch.slice(0,20)))

func _negatives() -> void:
	var fresh = ClassDB.instantiate("OriginalAudioEngine")
	check(fresh.initialize_inputs(source.files,93).is_empty(),"fresh engine refused")
	check(fresh.submit_command(0x12) == "audio command outside the native bank0 owner set","bank1 transfer 0x12 accepted")
	check(fresh.bank0_start(1,2).error == "bank0 start arguments outside the source/measured range","unmeasured tick skip accepted")
	var tampered: Array = finish.duplicate(); var seq: PackedByteArray = tampered[2].duplicate(); seq[0x3900] ^= 1; tampered[2] = seq
	check(fresh.begin_bank0_load(tampered,Bank0.flat_schedule()) == "bank0 original input identity differs","tampered SPRING.SEQ read accepted")
	check(fresh.begin_bank0_load(finish,Bank0.flat_schedule()) == "bank0 load is modelled only after the Shiori start","bank0 load before the Shiori start accepted")
	# Post-Shiori engines: a truncated schedule, and 0x11 one pump before the
	# music header completes (the landing at +131 precedes that pump's commands).
	var early = _stage16_engine()
	check(early != null,"negative route failed")
	if early == null: return
	var cut := Bank0.flat_schedule().slice(0,-3)
	var other = _stage16_engine()
	check(other.begin_bank0_load(finish,cut) == "bank0 landing schedule is incomplete","incomplete landing schedule accepted")
	check(early.begin_bank0_load(finish,Bank0.flat_schedule()).is_empty() and _render(early,Bank0.READ_DONE_FRAMES[4]-2).is_empty(),"early route failed")
	check(early.submit_command(0x11).is_empty() and _render(early,1) == "0x11 transfer before the five bank0 reads landed","0x11 before the D3D5 landing accepted")
	# The measured stage16 tick skip matters: without it the key-ons lead M3 by one pump.
	var unskipped = _stage16_engine()
	var r_abs: int = int(unskipped.bank0_diagnostics().pumps) - STAGE16
	check(unskipped.bank0_start(1,0).status == 1 and _render(unskipped,60).is_empty(),"unskipped route failed")
	var kon := {}
	var flat: PackedInt64Array = unskipped.bank0_diagnostics().bank0_kon_log
	for i in range(0,flat.size(),4): kon.get_or_add(int(flat[i]) - r_abs,[]).append(int(flat[i+2]))
	var differs := false
	for pump in range(4759,4800):
		var ours: Array = kon.get(pump,[]); ours.sort()
		if ours != expect.pumps[str(pump)][2].map(func(x): return int(x)): differs = true
	check(differs,"stage16 key-ons match M3 without the measured tick skip")
