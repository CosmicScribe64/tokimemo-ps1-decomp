# T-24059 short copy-chain sibling matches

Two of two matched,328 actual default C bytes: GYOZI func_80137F48 (164) and SHOUGATU func_80136948 (164). Only src/ovl/GYOZI/801372B0.c and src/ovl/SHOUGATU/80135B90.c change. No headers/config/compiler changes. Both target INCLUDE_ASM fallbacks are removed.

Foundation: the matched DATE func_80149F48 T-17205 closer, also transferred to SHUGAKU func_80133038 in T-24017. Read T-9560/T-10020 and the prior copy-chain notes first; the historical statement/permuter grids were not repeated. T-17600's GYOZI body and the existing SHOUGATU guarded body establish the copy pairs.

Each handler first saves three live asset pointers, then assigns the selected girl to its existing s16 scene-selector global. It calculates `(s16)(selector * 3) + month - 0x60` before replacing the three live asset pointers. It finally calls the same state-advance helper as before. GYOZI uses its existing aggregate girl field D_800F53A0.unk_F2F; SHOUGATU uses the canonical GameState member. Keeping the selector calculation between the copy groups and narrowing the product as in the matched sibling preserves the expected v0 value and skipped temporary after sll. The source comment explains that observed behavior. No unused locals, forced registers, dead statements or new FAKE forms were added; the matched sibling's plain s16 calculation is reused.

Only three coherent forms per target were measured: current direct expression40 raw/34 aligned; matched narrowed-global form0/0; narrowed local form40/34. Both fresh zeroes were promoted immediately, built with one compiler and checked from default linked bytes. All source/probe files and instruction diagnostics remain preserved in ignored build/T-24059-probes; no listings/binaries are committed. Production source is the authoritative selected body; the old scratch insertion harness is intentionally not published after removing the target INCLUDE_ASM sites.

Proofs:

- `tools/docker.sh ninja -j1 build/ovl/GYOZI.ok build/ovl/SHOUGATU.ok`: both checksums pass.
- `tools/docker.sh python3 tools/funcdiff.py func_80137F48 --unit GYOZI --resolve --version default --no-build`: MATCH164 bytes, actual unguarded C.
- Same command for func_80136948, unit SHOUGATU: MATCH164 bytes, actual unguarded C.
- GYOZI SHA1 `43f92523116e7b6cd772d4397712a52eeda89703`.
- SHOUGATU SHA1 `d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9`.
- check_headers and migrate_globals --check pass; sync_protos --check-branch passes0 new/15 known disagreements.
- git diff --check passes; independent standards review remains pending, ticket In Review.
