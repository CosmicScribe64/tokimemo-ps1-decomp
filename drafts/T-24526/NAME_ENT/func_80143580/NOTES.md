# T-24526 introduction semantic-region checkpoint

Not matched. Production source/header/config/compiler remain unchanged; no new C coverage or literal proof. Parent assigned NAME_ENT80142A80.c. History T-20008/T-24515 and existing matched same-unit helpers read first. All probes are complete full TUs with other guards left default, using existing Docker image/compiler. No broad declaration/scheduling grid.

Canonical T-20008 reproduces1078 words/835 raw rows against1077 target. Register-masked shape alignment has79 structural hunks (many moved instructions),313 register pairs and24 immediate-pair differences. This is not an all-register gap. Frame88/table64 differ from target90/6C. Raw equal-length funcdiff uses positional comparison, so an equal-length variant's1674 rows must not be compared directly with835 aligned rows.

## Preserved source repairs

1. Step5 old_x is s32. Target loads signed halfword X, computes X-1, stores the low half, then passes the complete computed word. Old s16 local spuriously narrowed it before the call; this repair removes two sign-extension instructions.1076 words/835 raw rows alone.
2. Step13 palette copy uses exact endpoint index!=128. Target latch is bne against end; old<128 adds sltu. With word X:1075 words/820 rows.
3. Three-record setup names depth=0x1FF-index++ before the X/Y stores, as target computes it. This meaningful local naturally restores frame90/table6C without padding. Preserved body:1075 words/864 rows. Pointer-field stores/constants still differ; no claim of a regional literal match.

These changes address measured type/control/frame gaps using existing SpriteSlot24, Rec24 and GameState types. Inherited unsigned flag-store and redundant nested-fill FAKE controls remain marked T-20008; their original source is still unresolved. No new unused padding was added.

## Other bounded controls

Step7 old-byte fade is compared after its increment. Explicit capture/increment/test eliminates the extra Boolean xori and saves one instruction, but sharing the existing fade local changes color webs. Alone1077 words/1674 positional rows; with word X1075/872; with word X+palette endpoint1074/857. Isolated actor_fade local plus all preserved repairs1074/883; it does not repair allocation. Kept private, not substituted into this checkpoint.

The selector entry is lbu v0 followed by move t0,v0 instead of target lbu t0. Full-TU trace maps index to t0 priority14 and a separate cvt(GameState.unk110D) to v0 priority3. This suggests separate webs but is not sufficient causal proof: unsigned state/signed counter split1075/956, table home68, copy remains a1->a0. Rejected, no further width grid. Matched42E24 uses direct GameState state updates, so one clean switch/direct increments control removed manual snapshots/redundant writebacks:1070/1003 and loses observed target stores. Rejected, no follow-up global-state grid.

Private lineage is build/T-24515/func_80143580/t24526-{read-control,explicit-oldfade,word-oldx,fade-word-oldx,copy-end,fade-copy-end,depth-first,isolated-actor-fade,state-counter-split,direct-state}. Each has full source/body/object and private comparison. depth-first/reg.txt is the current trace; no old context trace substituted. No raw instruction dump belongs in this archive.

## Remaining regions

- Entry: selector conversion/copy; frame/table now recovered.
- Steps0/2/4/6: old color byte fields, conditions and widths are correct, but base/color registers and constant reuse differ. Target keeps0x01000000 in ra and0x80000000 in t1; current typed source rematerializes several ranges.
- Step5: true X width repaired; selector writeback and call scheduling still differ.
- Step6 actor setup: early depth/frame repaired; induction/register allocation and store ordering remain different.
- Step7: explicit source form removes Boolean inversion, yet shared color ranges shift.
- Steps13/16 nested fills: correct ranges/counts, still inherited redundant-loop controls; target address copy uses addu where current source uses move.
- Step13 palette copy: latch repaired; pointer/source temporary allocation remains.
- Remaining steps: resource IDs, call arguments, post-update thresholds,128-word copy and packed widths audited; no new semantic mismatch found. Many branch displacement/immediate differences follow instruction placement, not wrong condition constants.

Full-TU replay, from repository root inside Docker:

```
python3 drafts/T-24526/NAME_ENT/func_80143580/replay.py
```

Expected checkpoint1075 words/864 resolved rows; output stays ignored under build/T-24526. Canonical helper declarations are scratch context only. Future promotion still requires own declarations/callers/default literal proof and affected-unit checksum; root performs all27 integration checks centrally.
