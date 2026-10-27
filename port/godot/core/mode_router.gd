extends RefCounted
## T29030: registration for the currently implemented source-bound subset.
## The native identity catalog deliberately does not imply a live capability.
const Registry = preload("res://core/native_route_registry.gd")
const NATIVE_HANDLERS: Dictionary = Registry.IMPLEMENTED_HANDLERS
# Compatibility metadata only: no additional overlay callbacks are enabled.
const OVERLAY_HANDLERS: Dictionary = {}
var _handlers: Dictionary = {}

func bind_mode(native_mode: int, handler: Callable) -> String:
	if not Registry.can_bind(native_mode):
		return "native mode 0x%02x has no implemented production route in this scope" % native_mode
	if not handler.is_valid(): return "mode handler is not callable"
	_handlers[native_mode] = handler
	return ""

func bind_overlay_mode(native_mode: int, handler: Callable) -> String:
	# Legacy API cannot turn a preview/pending native identity into a handler.
	return bind_mode(native_mode, handler)

func missing_handler(native_mode: int) -> String:
	if not _handlers.has(native_mode):
		return "unsupported native mode 0x%02x: no implemented handler" % native_mode
	return ""

func handler(native_mode: int) -> Callable:
	return _handlers[native_mode]
