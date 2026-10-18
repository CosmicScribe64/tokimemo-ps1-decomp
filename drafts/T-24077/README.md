# T-24077 ETC pair-table scene checkpoint

Only func_8013CF2C (824 bytes), src/ovl/ETC/8013BEF0.c. Production/helpers/
headers/config remain unchanged. Current body.c is102 resolved rows.

Historical T-17300 plain165 ALREADY selects the correct D_8014DBF8[girl] row.
Only the retained typed archive has malformed whole-array arithmetic; old
typed149 and173-to149 history is context, not an untried overall indexing fix.
Original full VRAM8013D118..8013D12C confirms eight-byte rows and two word loads;
earlier B118 shorthand referred to the ROM offset, not VRAM.

Fresh current full-TU explicit-object default resolved diagnostics:

| Form | Rows | Words |
| --- | ---: | ---: |
| baseline |132|206|
| unsigned-state |122|206|
| direct-pair |122|206|
| natural-default |122|206|
| return-view |126|210|
| shared-value |122|206|
| shared-state |102|206|

Original206 words/frame0x28; best uses206 words/frame0x28. Existing actual
void(s32) helpers8013E1DC and8013F320 are declared only in private probe context.
Parent approves their exact ETC.h declarations if promotion needs them; none
has been added. E1DC's matched K&R definition has s32 arg0, original call at
8013D1E0/1E8 passes only counter in a0. No helper/caller/override was changed.

Baseline uses current GameState.unk_1100 signed word with explicit unsigned
comparison views, actual row pointer/columns and Rec24[64/65].unk_00 terminal
flags. Shared state field unk_110C is s8, but original reads all state values
with LBU. Its exact u8 field-address view reduces132 to122 without header mutation.
Direct row/column call arguments and ordinary top-level default are neutral;
all three122 objects have SHA256
2d18cededf0d73e3dece309659abd1c0139a6aaebc3b5601442619c2df55c038.

Target-only s32 fallthrough is a separate126 control, rejected. ETC caller
src/ovl/ETC/8013FF80.c22 (matched801407F0) uses an implicit no-argument call and
ignores result; target establishes no return. The same-address GYOZI.h symbol
is another overlay, not an ETC declaration. The source invents no return, but
compiler emits extra v0zero instructions, and original return type is unproven.

Matched same-TU BFFC/T-24069 uses a consistent GameState pointer plus one shared
word selector/status local to recover a v1 web. Word join alone is neutral122;
completing that exact source recipe improves102. Its graph purpose is FAKE-
tagged inline, without claiming original declarations. Some status loads now
agree, but girl and case2 status remainv0 versusv1; later scratches rotateby1,
and the terminal80000000 constant materializes early. No next exact source form
is established; no register-only or compiler-impossibility claim.

Replay serially in existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24077/replay.py
python3 drafts/T-24077/replay.py --variant baseline
```

The helper privately replaces the exact assigned fallback, or exact pinned body
after promotion, strips the source banner, matches private forwards to selected
return view, and validates funcdiff --built --no-build --resolve --version default
--unit ETC func_8013CF2C before counting rows. Production-unchanged assertion
remains. All objects, original instruction dumps and binaries stay ignored.

Final parent-directed handoff is unresolved In Review. Bounded read-only Astra
review found no supported new form: shared-word girl/status recipe is already
present, scratch rotation alone identifies no missing operation/lifetime, and
correct terminal stores expose no dependency that delays the early constant.
No edit/build came from that review. Further unsupported probes stopped; no
compiler-impossibility claim. Production remains ASM; parent published the
active source packet at9a522dac3.
