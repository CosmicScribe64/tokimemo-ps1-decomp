---
id: T-10200
title: "Orphan jump tables: enable 21 instruction-exact functions"
status: Done
assignee: r12-jtbl
created: 2026-10-11
updated: 2026-10-11
links: ["[[build-system]]", "[[decompile-workflow]]", "[[tickets/T-10040-wave-11-list-4]]"]
---

## Goal

Give orphan jump tables and strings a C object owner through reproducible configuration, then enable the instruction-exact guarded functions.

## Acceptance criteria

- [x] General ownership tooling and synthetic tests.
- [x] Enable all 21 T-10040 exact drafts and audit other O candidates.
- [x] Minimal queue integration and documented recipe.
- [x] Clean Docker build: 27/27 checksums, header/global/prototype checks, progress gains.
- [x] Inline standards review; leave changes uncommitted for orchestrator.

## Notes

Shared tooling12-brief.md applies. r12-fixes owns queue.py; any integration there will be minimal. No compiler instruction rewrite is planned: object boundaries and ordinary compiler-generated switches provide the tables.

## Comments

- 2026-10-11: Claimed; baseline worktree clean. BUNKASAI's 801511E0 object has two consecutive orphan chunks immediately after its existing island.

- 2026-10-11: The merged BUNKASAI table-only island compiled all 21 drafts and resolved sample instruction diffs, but failed SHA1: it removed internal padding and shortened rodata by 16 bytes. Replaced that experiment with three objects at 801511E0, 80154018 and 80158EB0. The first new boundary is only word-aligned; the uniform object_layout.py step trims only compiler-added, zero, symbol-free and relocation-free text tail padding to the configured subsegment extent. No instructions are rewritten. orphan_rodata.py plans ownership conservatively from original references and preserves each rodata chunk. TACO's documented 8013F250 string split is also applied. Synthetic tests and real unit checks are underway.

- 2026-10-11: In Review. All 21 guarded bodies are active; TACO 8013F250 adds the 22nd conversion. Audit of the original orphan maps finds one additional BUNKASAI user, 80158764: its prior C work remained a 102-line real mismatch (T-10040 notes), so only its ownership is repaired. The earlier queue snapshot ran after BUNKASAI config edits and therefore showed only TACO; it was not a full original-map count. No matching C is left guarded for orphan ownership. Code review uses both standards and requested-scope axes inline, per the shared brief. Queue edit is deliberately limited to documentation and one actionable stderr hint; the config migration clears O for claimed ranges, with a regression test for tables and strings.

## Verification and review

- Clean Docker proof: `rm -rf asm build; python3 configure.py; ninja` exits 0, all **27/27 SHA1** lines are OK. `build/headers.ok` and `build/globals.ok` exist. `sync_protos.py --check-branch`: **0 new disagreements, 17 known**, branch check OK.
- `ninja progress`: **5099/6958 functions; 911084/2279368 bytes (40.0%)**. Gain over the 5077-function baseline: **22 functions / 27036 bytes** (21 BUNKASAI / 26556 bytes, TACO / 480 bytes).
- **124 tests pass**: object_layout 10, orphan_rodata 13, queue 49, objects/splitter 17, rodata tools 12, configure toolchains 5, trailing padding 18. Tests use synthetic assembly/config/ELF, with no game data fixtures. New tests cover unsafe tails, symbol/relocation preservation, word-aligned source extents, ownership rejection cases, read-only planning failure and queue unblocking for strings/tables.
- Re-running orphan ownership produces zero planned writes for both units. Splitter dry-run for all five affected objects produces zero source/YAML/prototype changes. `git diff --check` passes.
- Inline **standards review**: ordinary C89 functions remain in address order; generated assembly is untouched; compiler switches and literals own the rodata; the TACO unsigned-byte alias documents its measured load view and its frame pad has a ticketed FAKE explanation. Prior FAKE comments survive the BUNKASAI moves. The new TACO prototype is declared once in its overlay header. No instruction rewrite, per-function compiler switch, raw-source edit, host dependency or copyrighted fixture was introduced. The uniform layout step checks the YAML section extent and rejects nonzero, labelled or relocated tail bytes.
- Inline **scope review**: all 21 requested guards are removed; one additional exact orphan-string user is converted. BUNKASAI 80158764 remains assembly because its recorded C is not exact, but its ownership is fixed. Both required recipe pages, the toolchain notes, index, ticket and board are updated. The shared queue.py edit is only documentation plus an actionable hint; no ranking logic changes. No unresolved review findings. All changes remain uncommitted for the orchestrator.

Enabled BUNKASAI functions: `80154018`, `801541D8`, `801548C8`, `80154D64`,
`80155388`, `80155B1C`, `80156284`, `801569BC`, `8015706C`, `80157860`,
`80157DD4`, `801580EC`, `80158EB0`, `80158FD4`, `80159344`, `80159698`,
`80159AD4`, `80159FBC`, `8015A5DC`, `8015A870`, `8015AB58` (all `func_` names).
The additional conversion is TACO `func_8013F250`.

Private proof logs are in scratchpad/r12-jtbl/: clean-build.log, progress.log,
prototypes.log, islands.log, taco-last.log, and the queue audit outputs.

- 2026-10-11: Final fresh queue audit (`--flag O --all --next 100 --no-groups`) reports no remaining O rows. Review and clean-build gates complete; moved to Done with the matching board card. Ready for orchestrator review/commit.
