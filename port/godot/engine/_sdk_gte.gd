extends RefCounted
## T-39314: the PsyQ libgte trigonometry the engine C calls (asm/libgte_8009FA5C.s). Not a C file of the game;
## engine.gd registers it as module "sdk_gte". rsin and rcos read the library's own sine table out of game memory
## (the table is part of the executable's data, loaded into RAM like the rest of the image), so the values are
## the original's by construction. Angles are 12-bit fixed point (4096 = one turn); results are s16 scaled by 4096.
##
## The other libgte routines (the GTE register file: RotTrans, SetRotMatrix, gte_* macros) need a GTE model and are
## not provided; they refuse by name.

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "libgte (asm/libgte_8009FA5C.s)"
const C_FUNCTIONS: Array[String] = ["rsin", "func_800A0140"]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"rsin": {"args": ["s32"], "ret": "s32"},
	"func_800A0140": {"args": ["s32"], "ret": "s32"},
}

## D_800DD6C0 (the first quadrant, 0x401 entries); D_800DC6C0, D_800DCEC0 and D_800DBEC0 are the same table
## read at the negative and wrapped indices the library uses (asm/data/data_800CA380.data.s).
const TABLE: int = 0x800DD6C0
const TABLE_LOW: int = 0x800DC6C0
const TABLE_MID: int = 0x800DCEC0
const TABLE_HIGH: int = 0x800DBEC0

var engine # GameEngine

## func_800A00AC: the sine of 0..0xFFF.
func _sine(angle: int) -> int:
	var m = engine.memory
	if angle < 0x801:
		if angle < 0x401:
			return m.s16(TABLE + angle * 2)
		return m.s16(TABLE + (0x800 - angle) * 2)
	if angle < 0xC01:
		return -m.s16(TABLE_LOW + angle * 2)
	return -m.s16(TABLE + (0x1000 - angle) * 2)

## rsin(a): odd function of the angle.
func rsin(angle: int) -> int:
	var a: int = Math.s32(angle)
	if a < 0:
		return -_sine((-a) & 0xFFF)
	return _sine(a & 0xFFF)

## func_800A0140 = rcos(a): even function of the angle (the library indexes the same table at shifted bases).
func func_800A0140(angle: int) -> int:
	var m = engine.memory
	var a: int = Math.s32(angle)
	if a < 0:
		a = -a
	a &= 0xFFF
	if a < 0x801:
		if a < 0x401:
			return m.s16(TABLE + (0x400 - a) * 2)
		return -m.s16(TABLE_MID + a * 2)
	if a < 0xC01:
		return -m.s16(TABLE + (0xC00 - a) * 2)
	return m.s16(TABLE_HIGH + a * 2)
