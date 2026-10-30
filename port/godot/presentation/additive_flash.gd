extends RefCounted
## prologue_flash_boxes_draw (prologue.c:1257-1270): two flat semi-transparent
## POLY_G4s over x -160..160, y -120..40 at OT 0xB, preceded in draw order by
## dtd_on_tpage(0,0,0xB,1,1): dither on, ABR 1 (B+F). Shared by the stage8
## Rei banner flash and the stage19 flash.
const DITHER := [-4,0,-3,1,2,-2,3,-1,-3,1,-4,0,3,-1,2,-2]

static func paint(target: Image, level: int) -> String:
	if target == null or target.get_size() != Vector2i(320,240): return "additive flash target is unsupported"
	if level < 0 or level > 255: return "additive flash level is outside native byte range"
	for y: int in 160:
		for x: int in 320:
			var foreground := clampi((level+DITHER[(y&3)*4+(x&3)])>>3,0,31)
			var old := target.get_pixel(x,y)
			var red := mini(31,(old.r8>>3)+foreground)
			var green := mini(31,(old.g8>>3)+foreground)
			var blue := mini(31,(old.b8>>3)+foreground)
			target.set_pixel(x,y,Color8((red<<3)|(red>>2),(green<<3)|(green>>2),(blue<<3)|(blue>>2)))
	return ""
