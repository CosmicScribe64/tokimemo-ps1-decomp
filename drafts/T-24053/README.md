# T-24053 short-function count pass

Two real default C wins,232 bytes, pinned149ad0cfc and independently published by root at d40a1ece7. EVENT func_8010B614100 bytes uses the previously unintegrated T-20620 recipe: initial selector store and later selector read both use byte lvalues. Its canonical EventStateView flag-member byte view was freshly26 rows; the retained opaque stride and byte scheduling are explicitly FAKE marked in production. EVENT SHA1 ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e. BUNKA_SD func_801355AC132 bytes uses T-22021's byte promotion and `*= 0` zero-retention recipe, also FAKE marked; BUNKA_SD SHA1 173ac942ad4efbbbb176f8522fc7d273540169b0. Both have no target INCLUDE_ASM, pass literal `--version default --resolve --no-build` proofs and interface guards. Root independently passed all27 hashes. No header/config/toolchain changes.

SHUGAKU func_8013A1D4 (160 bytes) remains plain ASM. Read the family/T-18000 source and original access/control flow before the brief pass. The old generated source reports38 rows. Removing explicit `&0xFF` from a byte assignment produces10; a meaningful s32 selected-girl local before the u8 callback result gives8 rows/8 aligned and the original result home at sp+0x2B. The entire frame and later call/branch sequence now match. selected.c is that source-only body, with no FAKE workaround. It is not a matching claim or the standalone TU.

Remaining selector region: original loads the girl in v1, compares v1 and moves it into a0 in the get_g_zyotai_h call's delay slot. The selected source loads the value directly into a0 using v0 as the address base; the comparison uses a0 and the delay slot is nop. No per-function flags, forced registers or volatile workaround were used.

Bounded source forms and diff/aligned scores:

| Form | Result |
|---|---:|
| Existing generated control, unused spill local removed |38/38 both|
| Natural byte result (remove redundant explicit mask) |10/10|
| Meaningful word girl before byte result |8/8|
| Redundant byte-selector view |8/8|
| Implicit-int return liveness, byte girl |10/10|
| Implicit-int return liveness, word girl |10/10|
| GameState pointer selector |16/16|
| Unsigned word girl |8/8|
| Direct selector and full-word call result before narrowing |8/8|

The redundant byte-view probe is FAKE marked and rejected; int-return probes did not help. Older drafts and all loose source/input/diagnostic files remain preserved in ignored build/T-24053-probes. No target instruction listings or binaries are committed. The initial scratch aggregate-field spelling failed compilation because Rec34Flags is a struct; the corrected byte-member test then measured26. A harmless unrelated prefix declaration in the initial two SHUGAKU controls was removed for all subsequent forms.

Reproduce selected source inside Docker:

```
tools/docker.sh python3 drafts/T-24053/reproduce.py
tools/docker.sh python3 drafts/T-24053/probe.py func_8013A1D4 build/T-24053-probes/reproduce.json drafts/T-24053/prefix.h SHUGAKU
```

The harness compiles a temporary full unit with guarded C enabled, compares resolved instructions and writes traces only to ignored build storage. It exits nonzero on any candidate failure. Its score is not a default linked actual C proof. The final SHUGAKU archive does not change production C or headers; accepted matches and hashes are not redundantly rebuilt for archive-only work.
