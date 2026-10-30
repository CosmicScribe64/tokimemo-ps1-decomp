extends SceneTree
## T-39002 diagnostic: native Shiori race over many Rei-start-to-C1 phases.
## Reports each phase's KON voice count and any native refusal (pump, text).
## Env as run_shiori_native_race.gd, plus SHIORI_PHASES="first,last".
const Race = preload("res://tests/opening/run_shiori_native_race.gd")
const Shiori = preload("res://services/prologue_shiori_bank_resources.gd")
func _initialize() -> void:
	GDExtensionManager.load_extension("res://original_audio.gdextension")
	var race = Race.new()
	race.source = load("res://bgm_source_pack.gd").read_pack(OS.get_environment("ORIGINAL_BGM_PACK"))
	var rei = load("res://services/rei_bank_resources.gd").read_inputs(OS.get_environment("REI_BANK_PACK"))
	race.rei_inputs = [rei.inputs.music_header,rei.inputs.music_body,rei.inputs.sequence]
	race.shiori = Shiori.read_inputs(OS.get_environment("SHIORI_BANK_PACK"))
	var span := OS.get_environment("SHIORI_PHASES").split(",")
	var failed := 0
	for delta: int in range(int(span[0]),int(span[1])+1):
		var engine = race._rei_engine()
		var begun: String = race._begin(engine,delta)
		var rms := {}
		var ran: String = begun if not begun.is_empty() else race._run_to(engine,200,181,rms)
		var keyed := 0
		for row: PackedInt64Array in engine.diagnostics().get("kon_log",[]):
			if int(row[1]) >= 1: keyed += 1
		if not ran.is_empty(): failed += 1
		print("phase %d: voices=%d pump=%d %s" % [delta,keyed,int(engine.diagnostics().get("shiori_pump",-1)),ran])
		engine.dispose()
	print("phases failed: %d" % failed)
	quit(0)
