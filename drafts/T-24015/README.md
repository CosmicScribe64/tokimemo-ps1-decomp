# T-24015 MAIN menu bounded retry

Source-only forms for src/main/8005A0B0.c from T-17000, T-16000 and T-16510, repaired against current shared types.

Zero of five matched; no production C or header changed, zero verified bytes added. All five remain ASM. Current resolved rows/aligned: CA94 57/57, CC40 118/114, B43C 112/112, A410 41/41, F730 113/111.

A410 is the strongest new lead: signed coordinate halfwords plus a same-array 64-byte end bound remove IDO alignment peeling and recover frame/spill offsets. End-label address sharing, counter allocation and scheduling remain. The form visits sixteen coordinate pairs, two per iteration; an unrelated end-label bound scores 187 rows. This is a concrete retry lead, not proof of a match or compiler limitation.

CA94 repairs the lossy old GameState fold to unk_1118 and uses byte strides through Work800E7D10; selected-index spill remains four bytes high and narrowing/copy allocation differs. CC40 repairs GameState fields, byte strides and unsigned timer comparisons. Its D_800B39C4 table appears to hold two words per index, still an unresolved scratch scalar extern. Its s32 fallthrough definition preserves the existing implicit caller view; a void signature needs a deliberate canonical prototype decision before promotion.

B43C uses Rec38 unk_0C.f.b2 instead of a word mask; original frame and input-nibble allocation differ. F730 follows matched F26C/FDEC siblings with five-row halfword arrays. D_800E67C4 maps to GameState unk_53C[8]. Frame matches but all four array homes are eight bytes high. Coordinate variable names are inherited from T-16510; original menu_set order and values are retained. No FAKE padding was added to selected forms; a rejected padding experiment is preserved only in the ignored probe archive.

Baseline MAIN SHA-1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 matches config/SLPM_86.053.sha1. Ninja -j1 build/SLPM_86.053.ok was current. Headers/globals pass; prototype branch check reports zero new disagreements, fifteen known. Literal no-build win proof is not applicable because no function was promoted.

probe.py scores temporary full units using the default recipe; run in Docker only. Candidate failures retain diagnostics and cause nonzero exit. All loose sources, scripts, inputs and diagnostics were preserved intact in ignored build/T-24015-probes/. No target instruction listings are committed.

CA94 retains inherited dead `sp30`/`sp28` copies only as allocation experiments; both declarations are explicitly marked FAKE. Removing them scored 61 rows. This draft is not production code.
