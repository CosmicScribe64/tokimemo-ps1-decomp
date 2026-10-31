extends RefCounted
## T-39314: PsyQ libetc/libapi calls of the facade (asm/libetc.s, asm/libapi_3.s, asm/libapi_4.s): VSync, the
## interrupt mask the libgpu queue saves, the critical-section syscalls and the BIOS events. Not a C file of the
## game; engine.gd registers it as module "sdk_etc".
##
## Blocking-wait rule (wiki/engine-porting-contract.md): VSync(0) and VSync(n > 0) wait for vertical blanks the
## frame loop produces, VSync(1) reads the hardware horizontal-retrace counter; none is a function of
## `frame_count` and memory, so they refuse naming func_800AD950. VSync(-1) returns the library's vblank counter
## word (RAM 0x800E35E0), which only the VBlank interrupt advances; the port never does, so the value is whatever
## the memory holds (a defined read, not an invented clock).

const Math = preload("res://core/native_math.gd")

const C_FILE: String = "libetc/libapi (asm/libetc.s, asm/libapi_3.s, asm/libapi_4.s)"
const C_FUNCTIONS: Array[String] = ["func_800AD950"]
const C_UNPORTED: Array[String] = []
const C_SIGNATURES: Dictionary = {
	"func_800AD950": {"args": ["s32"], "ret": "s32"},
}

const VSYNC_COUNTER: int = 0x800E35E0 ## D_800E35E0: the counter VSync(-1) returns
const VIDEO_MODE: int = 0x800E35B0 ## D_800E35B0: the video mode word (GetVideoMode, func_800ADBE4: 0 NTSC, 1 PAL)

var engine # GameEngine

## I_MASK (0x1F801074): a hardware register, not RAM. libgpu saves and restores it around queued work through
## func_800AD3F8 (write the new mask, return the old one). Interrupts are not modelled, so the register holds
## what the last write left (see irq_mask_swap for the first read).
var irq_mask: int = -1 ## -1 until the first swap: then the value libgpu itself last saved, else 0

const GPU_SAVED_MASKS: Array[int] = [0x800DD694, 0x800DD698, 0x800DD69C] ## libgpu's copies of I_MASK

## Writes the mask, returns the previous one (func_800AD3F8). The register is hardware, not RAM; the first time it
## is read the model takes the value libgpu saved from it last (the first non-zero of the words at 0x800DD694,
## 0x800DD698, 0x800DD69C), so a saved RAM image carries the register as the library last saw it.
func irq_mask_swap(new_mask: int) -> int:
	if irq_mask < 0:
		irq_mask = 0
		for address: int in GPU_SAVED_MASKS:
			var saved: int = engine.memory.u32(address)
			if saved != 0:
				irq_mask = saved & 0xFFFF
				break
	var old: int = irq_mask
	irq_mask = new_mask & 0xFFFF
	return old

## GetVideoMode (func_800ADBE4): the word at 0x800E35B0.
func video_mode() -> int:
	return engine.memory.u32(VIDEO_MODE)

## VSync(mode) (asm/libetc.s 0x800AD950).
func func_800AD950(mode: int) -> int:
	var m: int = Math.s32(mode)
	if m < 0:
		return Math.s32(engine.memory.u32(VSYNC_COUNTER))
	if m == 1:
		engine.refuse("func_800AD950", "VSync(1) reads the hardware horizontal-retrace counter (not a function of frame_count and memory)")
	else:
		engine.refuse("func_800AD950", "VSync(%d) is a blocking wait for vertical blanks the frame loop produces (blocking-wait rule)" % m)
	return 0
