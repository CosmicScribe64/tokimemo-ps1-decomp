# T-24007 GEKO scene completion

Source: codex/complete-option after 801292401, 2026-10-17. Bounded continuation of unstarted w4-p35. The old four-row T-13900 scratch under build/scenes13900 was unavailable; its documented source recipe was reconstructed.

## Verified helper

func_80133598 matches 452 linked bytes. Start from T-17700 but use one saved u8 status, the full Rec38 flag word (unk_0C.w & 1, target lw), and direct signed amount update of D_80144C3C. This reproduces the T-13900 four-row form: only the two saved-status stack accesses differ (0x2F versus original 0x2B). A genuine s32 result local before status, assigned from get_g_zyotai_s before the mask, gives the original stack slot without changing instructions. u32 also matches; u8/s16 result changes the initial ranges. Using a quotient/amount local shifts the final amount from v0 to v1. No padding or FAKE needed.

Proof: Docker ninja -j1 build/ovl/GEKO.ok, then funcdiff.py --unit GEKO --resolve --version default --no-build func_80133598: literal linked bytes equal target (452 bytes), overlay checksum OK.

## Guard and unresolved drafts

func_80133024 retains a ticketed guard at 36 rows (T-17700 hand.c was 40). Moving i=1 after the initial flag reset fixes constant scheduling. Frame and table offsets match. Original month/day are a3/a2 and table value v1; draft month is t2, day v1, table value a2. Source width, inline-table, GameState-pointer and initialization-order probes did not close it. Narrow result/local variants gave 40-48, GS pointer 51, inline table 53. No fake or padding added.

func_80132D04: merged split byte and pointer ranges, removed dead sp2C assignments, corrected scaled GameState pointer arithmetic, and read Rec38 flags byte 2 directly (target lbu), not a cast of its halfword. Current body.c is 130 differing rows / 114 aligned edits. Frame now matches 0x38. Original saved status is sp+0x33; compiler uses a different byte slot and early flag/counter ranges, shifting later temporaries. A genuine result local (successful in 33598) worsens this function to 172 rows. Original entry pointers must be reloaded after calls; the current C is still nonmatching and stays archived.

func_80133308: folded old GameState labels using their original declaration views. Current body.c 164 rows (archived list score140 predates current header context). Flag bit-field and GS-local forms remain164; byte-width quotient/remainder locals give163 rows in widths.c. Original quotient is one s2 range across both paths, divisor20 s4 and GameState base s3; current IDO splits the first quotient into a temporary and gives the constants/base different saved registers. The status-byte variant worsens the shape. Not promoted.

func_80132B24: original frame0x50 and table offsets recovered by declaring the packed date word before thresholds then month-days, and removing redundant birth-month/day-difference locals in favor of expressions. Current body.c152 rows, 160 previously. GsWord local copy / bit-field view182; GS-local186. Original girl is v0, current v1; many copy-phase temporaries and load schedules still differ. Original birthday date uses shifted bits, already represented by the existing GsDateBits layout, but copying that aggregate costs a stack slot and worsens the result. Not promoted.

probe.py takes JSON [label, full function body] pairs and scores complete translation units in Docker. Instruction diagnostics are temporary and uncommitted. Only source drafts and this provenance are retained.

All loose experimental candidates, JSON inputs, scripts and instruction diagnostics are preserved under ignored build/T-24007-probes; nothing was deleted.
