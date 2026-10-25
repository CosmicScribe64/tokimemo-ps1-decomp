extends RefCounted
## Only the first slice's original main-executable modes can be bound here.
## Source: src/main/game_mode.c::game_mode_dispatch (0x34 and 0x40).
## Registering a callback does not establish parity for its implementation.

const NATIVE_HANDLERS: Dictionary = {
	0x34: "src/main/home_screens.c::settings_command_screen_mode_handler",
	0x40: "src/main/school_day.c::week_day",
}

var _handlers: Dictionary = {}

func bind_mode(native_mode: int, handler: Callable) -> String:
	if not NATIVE_HANDLERS.has(native_mode):
		return "mode outside bounded command/school-day slice: 0x%02x" % native_mode
	if not handler.is_valid():
		return "mode handler is not callable"
	_handlers[native_mode] = handler
	return ""

func missing_handler(native_mode: int) -> String:
	if not _handlers.has(native_mode):
		return "unsupported native mode 0x%02x: no implemented handler" % native_mode
	return ""

func handler(native_mode: int) -> Callable:
	return _handlers[native_mode]
