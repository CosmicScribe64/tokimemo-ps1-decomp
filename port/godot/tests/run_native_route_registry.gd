extends SceneTree
const Registry = preload("res://core/native_route_registry.gd")
const Router = preload("res://core/mode_router.gd")
const Session = preload("res://core/game_session.gd")
const InputFrame = preload("res://core/input_frame.gd")
const Rng = preload("res://core/native_rng.gd")
var checks := 0
var failures := 0
func check(ok: bool, label: String) -> void:
 checks += 1
 if not ok:
  failures += 1
  printerr(label)
func _initialize() -> void:
 var router := Router.new()
 var called := [false]
 var preview := func(_a, _b, _c, _d): called[0] = true
 check(Registry.all_native_ids().size() == 68, "all original native identities catalogued")
 for mode: int in Registry.all_native_ids():
  var record := Registry.describe(mode)
  check(not record.symbol.is_empty(), "native symbol present")
  if record.classification != "implemented_subset":
   check(not router.bind_overlay_mode(mode, preview).is_empty(), "non-production classification cannot bind")
   check(not router.missing_handler(mode).is_empty(), "preview cannot create callback")
 check(Registry.describe(0x45).symbol == "GAME_MODE_TELEPHONE_PROFILE", "phone native identity")
 check(Registry.describe(0x59).symbol == "GAME_MODE_GIRL_ENCOUNTER", "encounter native identity")
 check(Registry.describe(0x63).symbol == "GAME_MODE_ENDING", "ending native identity")
 var copied := Registry.describe(0x45)
 copied.symbol = "mutated"
 check(Registry.describe(0x45).symbol != "mutated", "metadata copy isolated")
 for mode: int in [0x3d, 0x3e, 0x3f, 0x44, 0x45, 0x50, 0x59, 0x5c, 0x63]:
  var session := Session.new()
  session.rng.configure(Rng.OPENBIOS_PROFILE, 7)
  session.state.game_mode = mode
  session.state.frame_count = 123
  var before := session.state_hash()
  var rng_before := session.rng.checkpoint()
  check(not session.router.bind_overlay_mode(mode, preview).is_empty(), "legacy aliases/pending reject registration")
  var result := session.step(InputFrame.new(0, 0x20), [])
  check(not result.committed and result.error.contains("no implemented handler"), "unsupported route stops explicitly")
  check(session.state_hash() == before and session.rng.checkpoint() == rng_before, "missing route preserves state and RNG")
  check(session.clock.logic_iterations == 0 and session.latched_input.tick == -1, "missing route preserves clock/input")
 check(not called[0], "preview never invoked or auto-completed")
 print("registry checks=%d failures=%d" % [checks, failures])
 quit(1 if failures else 0)
