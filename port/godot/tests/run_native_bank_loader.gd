extends SceneTree
const Loader = preload("res://services/native_bank_loader.gd")
var checks: int = 0
var failures: int = 0
func check(value: bool,label: String) -> void:
	checks += 1
	if not value: printerr("FAIL "+label); failures += 1
func binding() -> Dictionary:
	# Invented LBAs, never original game data.
	return {"bank":0,"se_header":101,"se_body":102,"music_header":103,"music_body":104,"sequence":105}
func commands(out: Loader.Outcome) -> Array:
	var result: Array = []
	for op in out.operations:
		if op.kind=="audio_command": result.append(op.command)
	return result
func _initialize() -> void:
	var loader := Loader.new()
	check(not loader.load_start(binding(),0).error.is_empty(),"explicit source reset/state required")
	loader.reset()
	check(loader.data.cd_baseline==null and loader.data.bank==null and loader.data.playing==null,"reset does not invent unassigned native state")
	loader.data.bank=0
	check(not loader.load_poll(5,false,false).error.is_empty() and loader.data.cd_baseline==null,"a required unknown baseline is not silently converted to zero")
	var first = loader.load_start(binding(),253)
	check(first.error.is_empty() and first.operations.size()==6,"cold bank0 start emits base plus five reads")
	var roles: Array = []; var lbas: Array = []; var counts: Array = []
	for op in first.operations:
		if op.kind=="cd_read_nowait": roles.append(op.destination); lbas.append(op.lba); counts.append(op.sectors)
	check(roles==["bank0_se_header","bank0_se_body","bank0_sequence","bank0_music_body","bank0_music_header"],"native header/body/sequence request order")
	check(lbas==[101,102,105,104,103] and counts==[3,32,37,80,6],"native request extents with invented bindings")
	check(loader.load_poll(1,false,false).operations.is_empty() and loader.data.transfer_polls==0,"four completions over byte wrap do not start transfer")
	var transfer = loader.load_poll(2,false,false)
	check(commands(transfer)==[0x11] and loader.data.transfer_polls==1 and transfer.status==-1,"fifth completion starts transfer but is not ready")
	check(loader.load_poll(2,true,false).status==-1,"both driver banks must be actually ready")
	var ready = loader.load_poll(2,true,true)
	check(ready.status_known and ready.status==1 and commands(ready)==[0x22,0x21,0x2f],"ready poll preserves source completion command order")
	var started = loader.start_sequence(0x40,0x12345)
	check(commands(started)==[0x22,0x21,0x2f,0x2345] and loader.data.sequence==0x45 and loader.data.playing==1 and not started.status_known,"sequence command width and unknown native valid-header return preserved")
	var before: Dictionary = loader.checkpoint()
	check(loader.start_sequence(0x41,0).operations.is_empty() and loader.checkpoint()==before,"invalid sequence magic produces no invented playback")
	var cached = loader.load_start(binding(),0)
	check(cached.operations.size()==1 and loader.data.cd_baseline==251 and loader.data.skip_se==1 and loader.data.skip_music==1,"cached source headers skip all five reads and baseline wraps as u8")
	check(loader.load_poll(0,false,false).operations.is_empty() and loader.data.transfer_polls==1,"all-cached first poll still preserves native transfer polling phase")
	check(loader.load_poll(0,true,true).status==1,"cached readiness is explicit, not assumed")
	loader.reset(); loader.load_start(binding(),0); loader.load_poll(5,false,false)
	var pending: bool = true
	for i in 191: pending = (loader.load_poll(5,false,false).status==-1) and pending
	check(pending and loader.data.transfer_polls==192,"native transfer timeout remains pending through count192")
	var timeout = loader.load_poll(5,false,false)
	check(loader.data.transfer_polls==193 and timeout.status==1,"native >0xC0 fallback preserved without pretending driver ready")
	loader.data.transfer_polls=255
	check(loader.load_poll(5,false,false).status==-1 and loader.data.transfer_polls==0,"transfer poll counter is native u8")
	loader.reset(); var zero: Dictionary = binding(); zero.se_header=0; zero.se_body=0
	loader.load_start(zero,0)
	check(loader.data.skip_se==1 and loader.data.cd_baseline==254,"cached zero header uses native whole-SE skip path")
	check(commands(loader.load_poll(3,false,false))==[0x14],"music-only transfer selects native command14")
	loader.reset(); zero=binding(); zero.music_header=0; zero.music_body=0; zero.sequence=0
	loader.load_start(zero,0)
	check(commands(loader.load_poll(2,false,false))==[0x13],"SE-only transfer selects native command13")
	before=loader.checkpoint()
	check(not loader.load_poll(0,null,false).error.is_empty() and loader.checkpoint()==before,"missing explicit ready value cannot mutate provider")
	check(not loader.load_start(binding(),false).error.is_empty() and loader.checkpoint()==before,"boolean is not a native CD counter")
	loader.data.playing=1; loader.data.cd_baseline=33; loader.data.bank=0; loader.reset()
	check(loader.data.playing==1 and loader.data.cd_baseline==33 and loader.data.bank==0,"reset preserves native unassigned playing/baseline/bank fields")
	print("PASS: %d native bank-loader control checks"%checks if not failures else "FAIL bank-loader control checks")
	quit(1 if failures else 0)
