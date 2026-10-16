---
id: T-16550
title: "Types-first pilot: TAIIKU"
status: Done
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[speedup-research-2026-10-16]], [[data-types]], [[taiiku-family-results]], [[tickets/T-16200-matching-wave-2]]
---

## Goal

Recover the TAIIKU overlay's shared data types (structs, arrays, bit-field views, callback tables) in `include/ovl/TAIIKU.h` before function matching, then redraft every unmatched TAIIKU function with the new headers and compare with the census scores. Pilot of the types-first idea from [[speedup-research-2026-10-16]]: in BUNKAKEN/BUNKASAI fresh m2c drafts matched 67% of the time once shared types were in headers, against 1.6% elsewhere (T-12700).

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit TAIIKU`, sibling clusters, census drafts; family page [[taiiku-family-results]].
- [x] Recovered types declared in `include/ovl/TAIIKU.h`, globals given aggregate types; every matched function still matches (TAIIKU sha1 OK).
- [x] Every unmatched TAIIKU function redrafted; the 58 with a compiling draft are under `drafts/T-16550/TAIIKU/<func>/` (body.c, context.c, result.json), scored full translation unit, compared with the census.
- [x] Score-0 drafts moved into `src/` and committed as matches (none scored 0; `func_80141CDC` matched from an 18-row draft with one hand edit).
- [x] Four file-disjoint work packages T-16551..T-16554 (pt-taiiku-p1..p4) defined.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.

## Notes

Worktree `tokimemo-wt/pt-taiiku`, branch `pt-taiiku` from `w2-good`. Ownership: C files under `src/ovl/TAIIKU/` and `include/ovl/TAIIKU.h`; additions only in `include/game.h` and `include/main_api.h`.

## Results

Types (all matched code rewritten, TAIIKU sha1 OK after each family):
- game A `TaiRunnersA D_80149208` (`player`, `cpu[3]` of 0x24 `TaiRunnerA`) replaces `TaiikuSlots`/`TaiSlot24`;
- game B `TaiRunnerB D_8014A13C[4]` replaces `u8[0x90]` and 8 scalars (a struct breaks `func_80143D24`);
- game C player `D_8014A3E4` + `TaiikuController D_8014A400[3]` (two objects; a `[4]` array breaks three functions) and `TaiikuSlot1C D_8014A454[4]`, 9 scalars folded;
- 28 stack-copied constant tables declared as one-array structs.

Redraft (`drafts/T-16550/redraft.py`, variants types / fold / fold-mainonly / fold+fixes / guarded, every distinct body rescored with the final header): 64 functions, median census score 241.5 (234.5 with the guarded drafts), median of the 58 new drafts 183, median best starting point 203; 27 better than census-or-guarded (21,632 bytes), 10 worse, 21 equal, 6 without a compiling new draft; no zero. 25 of the 27 gains come from folding splat labels into `base + offset` before m2c; the TAIIKU types beat a main-only fold in 13 functions. `draft_fixes.py` changed no score. Match: `func_80141CDC` (1,120 bytes).

Packages (lists in the orchestrator scratchpad): pt-taiiku-p1 T-16551 `80133C80.c` 8 functions 7,012 B; pt-taiiku-p2 T-16552 `80138950.c` 9 / 6,196 B; pt-taiiku-p3 T-16553 `801452D0.c` 9 / 5,412 B; pt-taiiku-p4 T-16554 `80140810.c` `80132000.c` `80142240.c` 7 / 6,884 B.

## Comments

- 2026-10-16: In Review. Details, idioms and limits in [[taiiku-family-results]]; driver and drafts in `drafts/T-16550/`.
- 2026-10-16: Code review against `CODING_STANDARDS.md` (all of `git diff w2-good...pt-taiiku`): changed C only under `src/ovl/TAIIKU/` and `include/ovl/TAIIKU.h`; `drafts/T-16550/` is a new folder (no existing archive touched, no asm, binaries or game data); no inline asm or `.word`; the only matched C added is `func_80141CDC` (plain C); the remaining `NON_MATCHING` blocks (`func_801346FC`, `func_801368B8`, `func_80136BB4`, `func_8014666C`) keep their ticket and row counts; `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` OK, TAIIKU sha1 OK. Findings and fixes (commit "review fixes"):
  - `func_80143D24` and `func_8014430C` kept FAKE comments about a byte-record view that the typed `D_8014A13C` no longer is: removed, with a plain note on why the array is an array.
  - Four functions still walked typed records with byte offsets and no FAKE mark. Field forms were tried: `func_80147210`, `func_80148194` and `func_80148228` match as fields and were converted; `func_8013732C`, `func_801448D4` and `func_80144C70` change the code as fields, so they carry a FAKE comment; `func_80138680` (`(&D_80149208.player)[arg0]`) got one too.
  - The `D_80149208` layout comment in `TAIIKU.h` sat above `TaiRunnerB`: moved above `TaiRunnersA`. Wiki notes updated.
  - Cosmetic, left as is: `TaiShorts12` holds nine halfwords (0x12 bytes) and `TaiShorts12x` twelve (0x18).
  Matched in this ticket: 1 function, `func_80141CDC` (1,120 bytes). Status Done.
