extends RefCounted
## T-39314: engine/_sdk_gpu.gd on invented memory; every expectation is derived by hand from asm/libgpu.s (the
## derivation is in each check's comment). The captures (config/port/engine-diff/sdk_gpu.json) compare the same
## functions against the real library. Renderer-bound calls are checked for their RAM effects and for the
## refusal when the generic renderer is not available (the core suite container has no GDExtension).

const GameEngine = preload("res://engine/engine.gd")

var failures: Array[String] = []

func _check(condition: bool, message: String) -> void:
	if not condition:
		failures.append(message)

const OT: int = 0x80100000
const PRIM: int = 0x80101000
const GRAPH_TYPE: int = 0x800DD588

func run() -> Array[String]:
	failures.clear()
	_initialisers()
	_ordering_tables()
	_words()
	_clear_tables()
	_queue_and_refusals()
	_libgs()
	return failures

func _initialisers() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# header length (byte 3) and GP0 code (byte 7) per initialiser (asm/libgpu.s 0x8009EFDC..0x8009F190)
	var table: Dictionary = {
		"func_8009EFDC": [4, 0x20], "func_8009EFF0": [7, 0x24], "func_8009F004": [6, 0x30], "func_8009F018": [9, 0x34],
		"func_8009F02C": [5, 0x28], "func_8009F040": [9, 0x2C], "func_8009F054": [8, 0x38], "func_8009F068": [12, 0x3C],
		"func_8009F07C": [3, 0x74], "func_8009F090": [3, 0x7C], "func_8009F0A4": [4, 0x64], "func_8009F0B8": [2, 0x68],
		"func_8009F0CC": [2, 0x70], "func_8009F0E0": [2, 0x78], "func_8009F0F4": [3, 0x60], "func_8009F108": [3, 0x40],
		"func_8009F11C": [4, 0x50],
	}
	for name: String in table:
		m.fill(PRIM, 0x40, 0xAA)
		engine.call_c(name, [PRIM])
		_check(m.u8(PRIM + 3) == table[name][0] and m.u8(PRIM + 7) == table[name][1], "%s writes length %d and code 0x%02X" % [name, table[name][0], table[name][1]])
		_check(m.u8(PRIM + 2) == 0xAA and m.u8(PRIM + 8) == 0xAA, "%s writes nothing else" % name)
	# the polylines also write the 0x55555555 terminator word (offsets 0x14, 0x1C, 0x18, 0x24)
	var lines: Dictionary = {"func_8009F130": [5, 0x48, 0x14], "func_8009F150": [7, 0x58, 0x1C], "func_8009F170": [6, 0x4C, 0x18], "func_8009F190": [9, 0x5C, 0x24]}
	for name: String in lines:
		m.fill(PRIM, 0x40, 0xAA)
		engine.call_c(name, [PRIM])
		_check(m.u8(PRIM + 3) == lines[name][0] and m.u8(PRIM + 7) == lines[name][1] and m.u32(PRIM + lines[name][2]) == 0x55555555, "%s writes the polyline terminator" % name)
	# SetSemiTrans / SetShadeTex touch only bit 1 / bit 0 of the code byte
	m.write_u8(PRIM + 7, 0x2C)
	engine.call_c("SetSemiTrans", [PRIM, 1])
	_check(m.u8(PRIM + 7) == 0x2E, "SetSemiTrans(1) sets bit 1")
	engine.call_c("SetSemiTrans", [PRIM, 0])
	_check(m.u8(PRIM + 7) == 0x2C, "SetSemiTrans(0) clears bit 1")
	engine.call_c("SetShadeTex", [PRIM, 5])
	_check(m.u8(PRIM + 7) == 0x2D, "SetShadeTex(non-zero) sets bit 0")
	engine.call_c("SetShadeTex", [PRIM, 0])
	_check(m.u8(PRIM + 7) == 0x2C, "SetShadeTex(0) clears bit 0")
	engine.shutdown()

func _ordering_tables() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# AddPrim(ot, p): p keeps its length byte and takes ot's old next; ot points at p
	m.write_u32(OT, 0x00FFFFFF)
	m.write_u32(PRIM, 0x09000000)
	engine.call_c("AddPrim", [OT, PRIM])
	_check(m.u32(PRIM) == 0x09FFFFFF and m.u32(OT) == (PRIM & 0xFFFFFF), "AddPrim links the first primitive")
	m.write_u32(PRIM + 0x40, 0x0C123456)
	engine.call_c("AddPrim", [OT, PRIM + 0x40])
	_check(m.u32(PRIM + 0x40) == (0x0C000000 | (PRIM & 0xFFFFFF)) and m.u32(OT) == ((PRIM + 0x40) & 0xFFFFFF), "AddPrim inserts in front and keeps the length byte")
	# AddPrims(ot, first, last): last takes ot's next, ot points at first
	m.write_u32(OT + 4, 0x00ABCDEF)
	m.write_u32(PRIM + 0x80, 0x03000000)
	m.write_u32(PRIM + 0x90, 0x04000000)
	engine.call_c("AddPrims", [OT + 4, PRIM + 0x80, PRIM + 0x90])
	_check(m.u32(PRIM + 0x90) == 0x04ABCDEF and m.u32(OT + 4) == ((PRIM + 0x80) & 0xFFFFFF), "AddPrims links a chain")
	engine.shutdown()

func _words() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# GetTPage(2, 1, 0x2C0, 0x100): GraphType 0 = (tp & 3) << 7 | (abr & 3) << 5 | (y & 0x100) >> 4 | (x & 0x3FF) >> 6 | (y & 0x200) << 2
	#   = 256 | 32 | 16 | 11 | 0 = 315; GraphType 1 and 2 use (tp << 9) | (abr << 7) | (y & 0x300) >> 3 | x >> 6
	#   = 1024 | 128 | 32 | 11 = 1195
	m.write_u8(GRAPH_TYPE, 0)
	_check(engine.call_c("func_8009ECB0", [2, 1, 0x2C0, 0x100]) == 315, "GetTPage, graph type 0")
	_check(engine.call_c("func_8009ECB0", [0, 0, 0x3C0, 0x200]) == (0xF | 0x800), "GetTPage y bit 9 goes to bit 11")
	m.write_u8(GRAPH_TYPE, 1)
	_check(engine.call_c("func_8009ECB0", [2, 1, 0x2C0, 0x100]) == 1195, "GetTPage, graph type 1")
	m.write_u8(GRAPH_TYPE, 2)
	_check(engine.call_c("func_8009ECB0", [2, 1, 0x2C0, 0x100]) == 1195, "GetTPage, graph type 2")
	# GetClut(x, y) = ((x >> 4) & 0x3F) | (y << 6), 16 bits
	_check(engine.call_c("func_8009ED74", [0, 0x1F0]) == 0x7C00, "GetClut(0, 0x1F0)")
	_check(engine.call_c("func_8009ED74", [0x30, 2]) == 0x83, "GetClut(0x30, 2)")
	_check(engine.call_c("func_8009ED74", [0x3F0, 0x1FF]) == (0x3F | (0x1FF << 6)), "GetClut(0x3F0, 0x1FF)")
	# SetDrawMode(p, dfe, dtd, tpage, tw): header length 2; word 1 = 0xE1000000 | dtd 0x200 | (tpage & 0x9FF) | dfe 0x400;
	# word 2 = the window word (0 for a null window)
	m.write_u8(GRAPH_TYPE, 0)
	m.fill(PRIM, 0x20, 0xAA)
	engine.call_c("func_8009D294", [PRIM, 1, 1, 0x13B, 0])
	_check(m.u8(PRIM + 3) == 2 and m.u32(PRIM + 4) == (0xE1000000 | 0x200 | 0x13B | 0x400) and m.u32(PRIM + 8) == 0, "SetDrawMode type 0 with dither and display drawing")
	engine.call_c("func_8009D294", [PRIM, 0, 0, 0xFFFF, 0])
	_check(m.u32(PRIM + 4) == (0xE1000000 | 0x9FF), "SetDrawMode masks the tpage to 0x9FF")
	m.write_u8(GRAPH_TYPE, 1)
	engine.call_c("func_8009D294", [PRIM, 1, 1, 0xFFFF, 0])
	_check(m.u32(PRIM + 4) == (0xE1000800 | 0x27FF | 0x1000), "SetDrawMode graph type 1: dither 0x800, tpage mask 0x27FF, display 0x1000")
	# window RECT x=8 y=16 w=32 h=64: x>>3=1, y>>3=2, ((-w) & 0xFF)>>3 = 28, ((-h) & 0xFF)>>3 = 24
	var rect: int = PRIM + 0x40
	m.write_u8(rect, 8)
	m.write_u8(rect + 2, 16)
	m.write_u16(rect + 4, 32)
	m.write_u16(rect + 6, 64)
	engine.call_c("func_8009D294", [PRIM, 0, 0, 0, rect])
	_check(m.u32(PRIM + 8) == (0xE2000000 | (1 << 10) | (2 << 15) | (24 << 5) | 28), "SetDrawMode texture window word")
	engine.shutdown()

func _clear_tables() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	var terminator: int = 0x800DD644 & 0xFFFFFF
	# ClearOTag(ot, 3): entries 0 and 1 link to the next, entry 2 to the terminator node; returns the last entry
	m.fill(OT, 0x40, 0x77)
	var last: int = engine.call_c("func_8009C9F8", [OT, 3])
	_check(last == OT + 8 and m.u32(OT) == ((OT + 4) & 0xFFFFFF) and m.u32(OT + 4) == ((OT + 8) & 0xFFFFFF) and m.u32(OT + 8) == terminator, "ClearOTag links forward and ends in the terminator node")
	_check(engine.call_c("func_8009C9F8", [OT, 1]) == OT and m.u32(OT) == terminator, "ClearOTag of one entry")
	# ClearOTagR(ot, 4): entry k links to entry k-1 (the OTC DMA writes 0xFFFFFF in entry 0), then entry 0 -> terminator
	m.write_u32(0x800E35E0, 100)
	var table: int = engine.call_c("func_8009CAAC", [OT, 4])
	_check(table == OT and m.u32(OT + 12) == ((OT + 8) & 0xFFFFFF) and m.u32(OT + 8) == ((OT + 4) & 0xFFFFFF) and m.u32(OT + 4) == (OT & 0xFFFFFF) and m.u32(OT) == terminator, "ClearOTagR links backward")
	_check(m.u32(0x800DD6A0) == 100 + 0xF0 and m.u32(0x800DD6A4) == 0, "ClearOTagR refreshes the draw-timeout words (VSync(-1) + 0xF0)")
	engine.shutdown()

func _queue_and_refusals() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# DrawSync(0) with an empty queue: 0, and the deadline words are refreshed (counter 0x800E35E0 + 0xF0)
	m.write_u32(0x800E35E0, 7)
	_check(engine.call_c("func_8009C674", [0]) == 0 and m.u32(0x800DD6A0) == 7 + 0xF0, "DrawSync(0) idle")
	_check(engine.call_c("func_8009C674", [1]) == 0, "DrawSync(1) idle returns the empty queue length")
	_check(not engine.halted(), "DrawSync does not refuse on an idle queue")
	# a job in the queue refuses (the model never creates one)
	m.write_u32(0x800DD68C, 1)
	engine.call_c("func_8009C674", [0])
	_check(engine.halted() and engine.refusal.contains("DrawSync"), "DrawSync refuses with a queued job")
	engine.shutdown()
	engine = GameEngine.new()
	# ResetGraph(0) is the boot-time full reset: refuses naming ResetGraph
	engine.call_c("func_8009C210", [0])
	_check(engine.halted() and engine.refusal.contains("ResetGraph"), "ResetGraph(0) refuses")
	engine.shutdown()
	engine = GameEngine.new()
	if not engine.renderer_ready():
		# no GDExtension in this environment: the renderer-bound calls refuse by their names
		for pair: Array in [["func_8009CB9C", [OT]], ["func_8009C884", [PRIM, PRIM]], ["func_8009C8E0", [PRIM, PRIM]], ["func_8009CC04", [PRIM]], ["func_8009CCF4", [PRIM]]]:
			engine.clear_failure()
			engine.memory.fill(PRIM, 0x100, 0)
			engine.call_c(pair[0], pair[1])
			_check(engine.halted() and engine.refusal.contains("renderer is not available"), "%s refuses without the renderer (got '%s')" % [pair[0], engine.refusal])
	engine.shutdown()

func _libgs() -> void:
	var engine = GameEngine.new()
	var m = engine.memory
	# GsSortClear(r, g, b, ot) (asm/libgs.s 0x8009824C): buffer 1 -> packet 0x800E6100 + 16; colour bytes at +4..+6, x at +8,
	# y at +0xA, +0xE = D_801296A8, +0xC = low half of D_801296AC (flag D_801296A1 clear); then AddPrim(ot->org, packet)
	var org: int = OT + 0x100
	var gs_ot: int = OT + 0x200
	m.write_u32(gs_ot + 0x10, org)
	m.write_u32(org, 0x00FFFFFF)
	m.write_u16(0x80129628, 1)
	m.write_u16(0x80129724 + 2, 0x10)
	m.write_u16(0x80129728 + 2, 0x20)
	m.write_u16(0x801296A8, 0x140)
	m.write_u32(0x801296AC, 0xF0)
	m.write_u8(0x801296A1, 0)
	m.write_u32(0x800E6110, 0x04000000)
	engine.call_c("func_8009824C", [1, 2, 3, gs_ot])
	var packet: int = 0x800E6110
	_check(m.u8(packet + 4) == 1 and m.u8(packet + 5) == 2 and m.u8(packet + 6) == 3 and m.u16(packet + 8) == 0x10 and m.u16(packet + 0xA) == 0x20, "GsSortClear colour and position")
	_check(m.u16(packet + 0xE) == 0x140 and m.u16(packet + 0xC) == 0xF0, "GsSortClear size")
	_check(m.u32(packet) == 0x04FFFFFF and m.u32(org) == (packet & 0xFFFFFF), "GsSortClear links the packet at ot->org")
	# with the flag set the height is 3/2 of the word at D_801296AC
	m.write_u8(0x801296A1, 1)
	engine.call_c("func_8009824C", [1, 2, 3, gs_ot])
	_check(m.u16(packet + 0xC) == 0x168, "GsSortClear with the flag: 3 * 0xF0 / 2")
	# GsDrawOt(ot) calls DrawOTag(ot->org): a recording double on DrawOTag sees the table address at ot + 0x10
	var seen: Array = []
	engine.set_stub("func_8009CB9C", func(args: Array) -> int:
		seen.append(args)
		return 0)
	engine.call_c("func_8009AD00", [gs_ot])
	_check(seen.size() == 1 and seen[0] == [org] and not engine.halted(), "GsDrawOt calls DrawOTag(ot->org) (saw %s)" % str(seen))
	engine.clear_stubs()
	engine.call_c("func_8009AD00", [gs_ot])
	if not engine.renderer_ready():
		_check(engine.halted() and engine.refusal.begins_with("func_8009CB9C -> DrawOTag"), "without the renderer the refusal comes from DrawOTag inside GsDrawOt (got '%s')" % engine.refusal)
	engine.shutdown()
