extends RefCounted
## Explicit direct-cold-route initialization, never an arbitrary runtime default.
## Source flow: prologue-hud-scratch-flow-v1/report.json plus make_color_bar16.
const FLOW_SHA := "9689cdd2f8b8562e37679d2116af6827d3e5e47c7b92bb77e2a38b1a6bfe8ce6"

static func initialize(route: Dictionary) -> Dictionary:
	var keys: Array[String] = ["source_flow_sha256","cold_boot","direct_new_game","options_visited","retained_game","memory_card_load"]
	if route.size() != keys.size() or not route.has_all(keys) or not route.source_flow_sha256 is String or route.source_flow_sha256 != FLOW_SHA:
		return {"error":"cold-route scratch initialization requires the exact source-flow scope"}
	for key: String in ["cold_boot","direct_new_game","options_visited","retained_game","memory_card_load"]:
		if not route[key] is bool: return {"error":"cold-route history requires explicit boolean observations"}
	if not route.cold_boot or not route.direct_new_game or route.options_visited or route.retained_game or route.memory_card_load:
		return {"error":"scratch source flow excludes Options, retained, card-loaded or non-cold routes"}
	var bytes := PackedByteArray()
	bytes.resize(512) # game_state_init(1), then hud_vram_init, clear128 words.
	# File-select calls make_color_bar16(DF7FCF,DF7FCF,16,0). Equal endpoints
	# give15 non-black entries; the source then clears entry0 and all16 STP bits.
	var word := (0xdf>>3) | ((0x7f>>3)<<5) | ((0xcf>>3)<<10)
	for color in range(1,16): bytes.encode_u16(color*2,word)
	var known := PackedByteArray()
	known.resize(512)
	known.fill(255)
	return {"error":"","scratch_words":bytes,"known_mask":known,"source_flow_sha256":FLOW_SHA,
		"scope":"direct cold New Game without Options, retained state or card load; prefix clock parity unmeasured"}
