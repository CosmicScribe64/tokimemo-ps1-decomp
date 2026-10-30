extends SceneTree
## T-39002: native candidate only. Reproduces the stage10/substep8 Shiori bank
## race on the original libsnd code: C1 master fade + three sector-granular CD
## landings over the live Rei score/header, then the stage11/substep2 poll
## (0x15 dispose, slot2 transfer, Shiori start). Compared against the M2
## emulator measurement (.asset-cache/emu/shiori-landing-m2, r1 and r3-start33).
## Env: ORIGINAL_BGM_PACK, REI_BANK_PACK, SHIORI_BANK_PACK, SHIORI_M2.
const Shiori = preload("res://services/prologue_shiori_bank_resources.gd")
## Pumps between Rei's audio_bank_start(1,0) call frame and the stage10/8 call
## frame, from r1/events.jsonl (257 -> 2206) and r3-start33 (4462 -> 6408).
const PREFIX_DELTA := 1949
const START33_DELTA := 1946
var failures := 0
var checks := 0
var source: Dictionary
var rei_inputs: Array
var shiori: Dictionary

func check(ok: bool, label: String) -> void:
	checks += 1
	if not ok: failures += 1; printerr("FAIL: " + label)

func _initialize() -> void:
	if not GDExtensionManager.is_extension_loaded("res://original_audio.gdextension"):
		GDExtensionManager.load_extension("res://original_audio.gdextension")
	var probe = ClassDB.instantiate("OriginalAudioEngine")
	if probe == null or not probe.has_method("begin_shiori_load"):
		printerr("Test requires the isolated Shiori candidate extension"); quit(1); return
	source = load("res://bgm_source_pack.gd").read_pack(OS.get_environment("ORIGINAL_BGM_PACK"))
	if not source.error.is_empty(): printerr(source.error); quit(1); return
	var rei = load("res://services/rei_bank_resources.gd").read_inputs(OS.get_environment("REI_BANK_PACK"))
	if not rei.error.is_empty(): printerr(rei.error); quit(1); return
	rei_inputs = [rei.inputs.music_header,rei.inputs.music_body,rei.inputs.sequence]
	shiori = Shiori.read_inputs(OS.get_environment("SHIORI_BANK_PACK"))
	if not shiori.error.is_empty(): printerr(shiori.error); quit(1); return
	var timeline := _timeline(OS.get_environment("SHIORI_M2").path_join("r1/timeline.txt"))
	check(not timeline.is_empty(),"M2 r1 timeline is unreadable")
	_negatives()
	_prefix_phase(timeline)
	_start33_phase()
	_sweep_phase()
	print("Shiori native race: %d checks, %d failures" % [checks,failures])
	if failures == 0: print("PASS: native Shiori race reproduces M2 landing/fade/KON/dispose; hardware PCM parity not claimed")
	quit(1 if failures else 0)

## Engine at Rei start: arrival init, Rei load, B1 fade to finish, start_rei.
func _rei_engine():
	var engine = ClassDB.instantiate("OriginalAudioEngine")
	if not engine.initialize_inputs(source.files,93).is_empty() or not engine.load_rei_bank(rei_inputs).is_empty(): return null
	if not engine.begin_rei_fade().is_empty(): return null
	for _tick in range(10):
		if not engine.tick_rei_fade(false).is_empty() or not engine.render_block().error.is_empty(): return null
	if not engine.start_rei(false).is_empty(): return null
	return engine

func _begin(engine, warmup: int) -> String:
	for _pump in range(warmup):
		var block: Dictionary = engine.render_block()
		if not block.error.is_empty(): return block.error
	# Source order in the stage10/8 frame: audio_command(5), (0xC1), load_start.
	var fade: String = engine.begin_fade(0xc0,1,true)
	if not fade.is_empty(): return fade
	return engine.begin_shiori_load(Shiori.native_inputs(shiori.inputs),Shiori.flat_schedule())

func _run_to(engine, last: int, poll_pump: int, rms: Dictionary) -> String:
	while int(engine.diagnostics().shiori_pump) < last:
		var block: Dictionary = engine.render_block()
		if not block.error.is_empty(): return block.error
		var pump := int(engine.diagnostics().shiori_pump)
		var sum := 0.0
		for sample: Vector2 in block.pcm: sum += sample.x*sample.x + sample.y*sample.y
		rms[pump] = sqrt(sum/1470.0)
		var expected: Array = Shiori.landed_at(pump)
		if engine.diagnostics().shiori_landed != expected: return "landed sectors at +%d are %s, measured %s" % [pump,engine.diagnostics().shiori_landed,expected]
		if pump == poll_pump:
			var finish: String = engine.finish_shiori()
			if not finish.is_empty(): return finish
	return ""

func _prefix_phase(timeline: Dictionary) -> void:
	var engine = _rei_engine()
	check(engine != null,"Rei start setup failed")
	if engine == null: return
	var begun := _begin(engine,PREFIX_DELTA)
	check(begun.is_empty(),"Shiori begin failed: " + begun)
	if not begun.is_empty(): return
	var rms := {}
	var ran := _run_to(engine,Shiori.MEASURED.dispose+20,Shiori.MEASURED.poll,rms)
	check(ran.is_empty(),"Shiori race run failed: " + ran)
	if not ran.is_empty(): return
	var d: Dictionary = engine.diagnostics()
	check(int(d.fade_finish_pump) == 112,"C1 fade finish pump %d, measured +112" % d.fade_finish_pump)
	check(int(d.shiori_finish_pump) == 182,"dispose/transfer pump %d, measured +182" % d.shiori_finish_pump)
	check(d.shiori_started and int(d.sequence_gain) == 101 and not d.stopped,"Shiori sequence not started at VAB gain 101: " + str(d.sequence_gain))
	# bgm/MVOL trajectory: pump_log rows are [pump, MVOL at mix, gain at mix],
	# the same instant as the timeline row (state before that pump's update).
	var log := {}
	for row: PackedInt64Array in d.pump_log: log[int(row[0])] = row
	for pair: Array in [[3,82,0x3efd],[29,62,0x2f5e],[75,28,0x152a],[77,26,0x13a7],[111,1,0x81],[112,0,0],[183,101,0x3fff]]:
		var row: Variant = log.get(pair[0])
		check(row != null and int(row[2]) == pair[1] and int(row[1]) == pair[2],"+%d gain/MVOL %s, measured %d/%x" % [pair[0],row,pair[1],pair[2]])
	var trajectory_mismatch := []
	for pump: int in range(1,184):
		if timeline.has(pump) and log.has(pump) and (int(log[pump][2]) != timeline[pump].gain or int(log[pump][1]) != timeline[pump].mvol): trajectory_mismatch.append(pump)
	check(trajectory_mismatch.is_empty(),"gain/MVOL differs from r1/timeline.txt at pumps " + str(trajectory_mismatch))
	# KON log versus the emulator, per pump, as SPU sample start addresses.
	# The timeline lists a key-on in the row after the pump whose SPU flush
	# wrote it (M1 spu-writes.jsonl tags the same KON one frame earlier), so
	# emulator row n+1 is native pump n. Voice channel numbers are not
	# compared: the SDK voice allocator starts from different prior state
	# (documented deviation); the sample, pump and voice count are compared.
	var ours := {}
	var channels := {}
	for row: PackedInt64Array in d.kon_log:
		if int(row[1]) >= 1 and int(row[1]) <= 182:
			ours.get_or_add(int(row[1]),[]).append("%x" % row[3])
			channels.get_or_add(int(row[1]),[]).append(int(row[2]))
	var theirs := {}
	for pump: int in timeline:
		if pump >= 2 and pump <= 183 and not timeline[pump].kon.is_empty():
			theirs[pump-1] = timeline[pump].kon.map(func(pair: String) -> String: return pair.get_slice(":",1))
	var voices := 0
	for pump: int in ours: ours[pump].sort(); voices += ours[pump].size()
	for pump: int in theirs: theirs[pump].sort()
	check(voices == 36 and ours.size() == theirs.size(),"keyed %d voices in %d pumps, measured 36 in %d" % [voices,ours.size(),theirs.size()])
	check(ours == theirs,"KON log differs from r1/timeline.txt:\n ours=%s\n emu=%s" % [ours,theirs])
	var first: Array = ours.keys(); first.sort()
	check(not first.is_empty() and ours[first[0]] == ["4fa80"] and int(log[first[0]][1]) == 0x3d7a,"first KON is not sample 0x4FA80 at MVOL 0x3D7A")
	var late := []
	for pump: int in ours: if pump > Shiori.MEASURED.cbd4_complete: late.append(pump)
	check(late.is_empty(),"KON after +91: " + str(late))
	for row: PackedInt64Array in d.kon_log:
		if int(row[1]) >= 1 and int(row[1]) <= 182: check(int(row[3]) >= 0x39010 and int(row[3]) < 0x39010+139680,"key-on outside Rei SPU data: %x" % row[3])
	var tails := []
	for pump: int in range(150,182): if float(rms.get(pump,1.0)) >= 0.001: tails.append(pump)
	check(tails.is_empty(),"output not silent (<0.001 RMS) at +150..+181: " + str(tails))
	var loud := 0.0
	for pump: int in range(183,203): loud = maxf(loud,float(rms.get(pump,0.0)))
	# Optional audibility comparison: windowed RMS against the M2 recording
	# scaled by MVOL (summary.json r1 rms_windows "est"), within +-0.01.
	var measured := _measured_windows(OS.get_environment("SHIORI_M2").path_join("summary.json"))
	var window_errors := []
	for start: int in measured:
		var sum := 0.0
		for pump: int in range(start,start+15): sum += pow(float(rms.get(pump,0.0)),2)
		if absf(sqrt(sum/15.0)-float(measured[start])) > 0.01: window_errors.append([start,sqrt(sum/15.0),measured[start]])
	check(measured.size() >= 12 and window_errors.is_empty(),"windowed RMS differs from M2 r1 RMS x MVOL by >0.01: " + str(window_errors))
	print("prefix phase: voices=%d pumps=%s channels=%s windows=%s shiori_peak_after_start=%.4f" % [voices,first,channels,_windows(rms),loud])
	engine.dispose()

func _start33_phase() -> void:
	var engine = _rei_engine()
	if engine == null: check(false,"Rei start setup failed (start33)"); return
	var begun := _begin(engine,START33_DELTA)
	check(begun.is_empty(),"start33 begin failed: " + begun)
	var rms := {}
	var ran := _run_to(engine,Shiori.MEASURED.dispose+2,Shiori.MEASURED.poll,rms)
	check(ran.is_empty(),"start33 race run failed: " + ran)
	var keyed := 0
	for row: PackedInt64Array in engine.diagnostics().kon_log:
		if int(row[1]) >= 4 and int(row[1]) <= 182: keyed += 1
	check(keyed == 0,"START-33 phase keyed %d voices; M2 r3-start33 stalls with 0 KON" % keyed)
	check(int(engine.diagnostics().shiori_finish_pump) == 182,"start33 dispose pump differs")
	engine.dispose()

## Known explicit boundary: at some Rei-to-C1 phases (e.g. 1305, near the
## port route's 1289) the sequencer, reading Shiori bytes as Rei score, makes
## libsnd write a voice volume with bit15 (SPU volume sweep mode). The native
## mixer has no sweep generator, so it must refuse rather than mis-mix.
func _sweep_phase() -> void:
	var engine = _rei_engine()
	if engine == null: check(false,"Rei start setup failed (sweep)"); return
	check(_begin(engine,1305).is_empty(),"sweep-phase begin failed")
	var rms := {}
	check(_run_to(engine,200,181,rms) == "volume sweep unsupported","phase 1305 did not stop at the explicit volume-sweep boundary")
	engine.dispose()

func _negatives() -> void:
	var engine = _rei_engine()
	if engine == null: check(false,"Rei start setup failed (negatives)"); return
	var inputs: Array = Shiori.native_inputs(shiori.inputs)
	check(not engine.begin_shiori_load(inputs,Shiori.flat_schedule()).is_empty(),"load accepted without the C1 fade")
	engine.dispose()
	engine = _rei_engine()
	check(engine.begin_fade(0xc0,2,true) != "" ,"fade accepted a rate/decrement outside audio_bgm_fade_start")
	engine.dispose()
	engine = _rei_engine()
	check(engine.begin_fade(0xc0,1,true).is_empty(),"C1 fade start failed")
	var bad := inputs.duplicate(true); bad[2][0x18] ^= 1
	check(engine.begin_shiori_load(bad,Shiori.flat_schedule()) == "Shiori original input identity differs","substituted CBD4 accepted")
	engine.dispose()
	for shape: Array in [[[3,0,1]],[[3,1,1],[4,0,37]],[[0,0,37],[31,1,80],[77,2,6]],[[3,0,37],[3,1,80],[77,2,6]],[[3,0,20],[2,0,37],[31,1,80],[77,2,6]]]:
		engine = _rei_engine()
		engine.begin_fade(0xc0,1,true)
		var flat := PackedInt32Array()
		for row: Array in shape: flat.append_array(PackedInt32Array(row))
		check(not engine.begin_shiori_load(inputs,flat).is_empty(),"malformed schedule accepted: " + str(shape))
		engine.dispose()
	engine = _rei_engine()
	check(_begin(engine,10).is_empty(),"negative begin failed")
	var rms := {}
	check(_run_to(engine,50,-1,rms).is_empty(),"negative run failed")
	check(engine.finish_shiori() == "Shiori CD reads are incomplete at the bank poll","poll before +91 was not rejected")
	engine.dispose()
	engine = _rei_engine()
	check(_begin(engine,10).is_empty(),"negative begin failed")
	check(_run_to(engine,100,-1,rms).is_empty(),"negative run failed")
	check(engine.finish_shiori() == "Shiori dispose before the C1 fade finished is outside the measured path","poll before fade finish was not rejected")
	check(engine.tick_rei_fade(false) == "engine is disposed or failed","engine stayed usable after a refused poll")
	engine.dispose()

func _measured_windows(path: String) -> Dictionary:
	var parsed: Variant = JSON.parse_string(FileAccess.get_file_as_string(path))
	var out := {}
	if not parsed is Array or parsed.is_empty(): return out
	for text: String in parsed[0].get("rms_windows",[]):
		var start := int(text.get_slice("..",0))
		if start >= 0 and start <= 165: out[start] = float(text.get_slice("est=",1))
	return out

func _windows(rms: Dictionary) -> String:
	var out := []
	for start: int in range(0,180,15):
		var sum := 0.0
		for pump: int in range(start,start+15): sum += pow(float(rms.get(pump,0.0)),2)
		out.append("%+d:%.4f" % [start,sqrt(sum/15.0)])
	return " ".join(out)

func _timeline(path: String) -> Dictionary:
	var rows := {}
	var file := FileAccess.open(path,FileAccess.READ)
	if file == null: return rows
	while not file.eof_reached():
		var line := file.get_line()
		if line.begins_with("#") or line.strip_edges().is_empty(): continue
		var t := line.split(" ",false)
		if t.size() < 11: continue
		var kon := []
		if t[9] != "-": for pair: String in t[10].split(","): kon.append("%d:%x" % [int(pair.get_slice(":",0)),pair.get_slice(":",1).hex_to_int()])
		rows[int(t[1])] = {"gain":int(t[6]),"mvol":t[7].hex_to_int(),"kon":kon}
	return rows
