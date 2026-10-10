---
type: concept
updated: 2026-10-09
sources: ["config/versions.txt", "tools/identify_version.py", "versions/README.md"]
---

# Game versions

Ticket [[tickets/T-3200-catalog-game-versions]]. Nine archives the user collected in `versions/` hold **five distinct discs**. The facts (IDs, sizes, SHA-1, header fields, every overlay SHA-1) are in `config/versions.txt`; `tools/identify_version.py <image|folder>` reports which version a disc is. Related: [[disc-layout]], [[executable]], [[overlays]], [[obin]].

## Archives and duplicates
Hashes are over the extracted track data, not the archive files. Track 2 (CD-DA, 45151344 bytes) is byte-identical in all nine (`6cdd8500...`).

| archive | format | track 1 sha1 (first 10) | disc |
|---|---|---|---|
| Rev 1 .zip | zip, 2 bins + cue | 7b2f72d131 | Rev 1 |
| Rev 1 .7z | 7z, same | 7b2f72d131 | Rev 1 (duplicate) |
| v1.1 .7z | 7z, same | 7b2f72d131 | Rev 1 (duplicate; "v1.1" is Rev 1, see below) |
| Rev 2 .zip / .7z | zip / 7z | f877c53d42 | Rev 2 (duplicate pair) |
| Rev 4 .zip / .7z | zip / 7z | 0ec8e3b093 | Rev 4 (duplicate pair) |
| Shokai Genteiban (Rev 1) .zip | zip | 899cc5c27b | Shokai Genteiban |
| PlayStation the Best .7z | 7z | 00b595095e | Best |

All have the layout BIN MODE2/2352 track 1 + CD-DA track 2 (cue: `TRACK 01 MODE2/2352`, `TRACK 02 AUDIO` with `INDEX 00 00:00:00`, `INDEX 01 00:02:00`). Track 1 is 702104928 bytes (298514 sectors) for every Rev disc and 702072000 bytes (298500 sectors) for Best. No CHD or ISO appeared. The 7z archives of Rev 1/2/4/Best contain a `CDRomance.url` next to the folder; ignore it.

Shokai Genteiban differs from Rev 1 in exactly one byte of user data: the primary volume descriptor's volume set ID (`SLPS00064VER110` vs `SLPS00065VER110`, i.e. product codes SLPS-00064 limited edition and SLPS-00065 regular), plus that sector's checksum. All 1372 files, the exe and all overlays are identical. Rev 1 and Shokai are the same game code.

Duplicates to remove (the orchestrator trashes them; nothing was deleted here): keep the `.zip` of Rev 1, Rev 2 and Rev 4 (`tools/extract_disc.py` reads zips directly, no 7z tool needed), redundant are Rev 1 .7z, v1.1 .7z, Rev 2 .7z and Rev 4 .7z. The two copies in each pair hold identical discs; the 7z is about 30 MB smaller, so keep it instead if space matters more than convenience. Best exists only as 7z and Shokai only as zip: keep both.

## Catalog
| | Rev 1 / Shokai | Rev 2 | Rev 4 | PlayStation the Best |
|---|---|---|---|---|
| volume ID | TOKIMEKIMEMORIAL | same | same | VX009J2 |
| volume set ID | SLPS00065VER110 (Shokai: SLPS00064VER110) | SLPS00065VER125 | SLPS00065VER143 | TOKIMEKIMEMORIALBEST |
| PVD creation date | 1995-08-31 | 1995-11-17 | 1996-03-31 | 1997-12-25 |
| publisher / preparer | KONAMI / K.FUKUHARA | same | same | KONAMI_KCET / K_FUKUHARA |
| SYSTEM.CNF BOOT | `cdrom:PSX.EXE;1` | same | same | `cdrom:SLPM_86.053;1` |
| exe size | 690176 | 690176 | 667648 | 667648 |
| exe sha1 | 2d8d4748... | 6c3eafe5... | 60305396... | e823bd84... (ours) |
| entry / gp | 800422A0 / 800F0D90 | 800420D0 / 800F0A60 | 800420D0 / 800EB540 | 800420D0 / 800EB670 |
| text addr / size | 80041000 / 0xA8000 | same | 0xA2800 | 0xA2800 |
| rodata start / size (header 0x20) | 800B5A60 / 0x3CB0 | 800B57C0 / 0x3C30 | 800AF1F0 / 0x3F00 | 800AF340 / 0x3EE0 |
| files on disc | 1372 | 1372 | 1372 | 1372 |

SYSTEM.CNF otherwise reads `TCB = 8`, `EVENT = 32`, `STACK = 801FFF00` in all. The disc itself carries no product code outside the volume set ID (SLPS-00065 and so on come from there). Full SHA-1 of the exes and of every overlay (26 `.EXN` + `O.BIN` per version) are in `config/versions.txt`.

File-list differences: the name sets are identical in all five except `PSX.EXE` (Rev disks) vs `SLPM_86.053` (Best). Nothing extra (no `.MAP/.SYM/.OBJ/.LIB`, no ECOFF/ELF headers outside O.BIN). Files with different content against Best: Rev 4 31, Rev 2 32, Rev 1 and Shokai 56 (of 1372; this includes the exe name pair, SYSTEM.CNF and 24 overlays; the rest is data: TAIIKU, SERIFU, BUNKA, EN_GAME, FUKUTEST, one BS and VAB_SEQ file). LBAs move for 380 files because the exe size and data differ.

## Overlays and O.BIN
- `EVENT.EXN`, `GYOZI.EXN` and `O.BIN` are **byte-identical in all five versions**. The other 24 overlays differ in every version, even Rev 4 against Best.
- `O.BIN` (see [[obin]], link timestamp 1995-07-25) is the same file everywhere: no version has more symbols, local symbols, line numbers or types. The developer build predates Rev 1 (1995-08-31). The dumped sizes of its code symbols fit Rev 4/Best a little better than Rev 1/2 (ratio 0.47 vs 0.41 in an order-aligned size comparison), so the O.BIN names do not map better onto an older version.
- No debug leftovers elsewhere: the only source-name strings in any exe are PsyQ library RCS ids and `BG_ERROR:p_line.c` / `<kanji.c>error(No-sjis)` (same in all versions, already known); no overlay contains a `.c`/`.h`/`.obj`/path string; exe header padding and the zero fill after code are all zero in every version (no stale build-machine memory).

## Lineage and release order
Evidence: PVD creation date, volume set version number, and the SDK library RCS ids in the exes.

1. **Rev 1 (Ver 1.10), 1995-08-31** and the Shokai Genteiban (same code, SLPS-00064). "v1.1" is this disc: Ver 1.10.
2. **Rev 2 (Ver 1.25), 1995-11-17.**
3. (Rev 3 is not in the set.)
4. **Rev 4 (Ver 1.43), 1996-03-31.**
5. **PlayStation the Best (SLPM-86053), 1997-12-25.** Newest. Exe renamed, same size as Rev 4.

SDK evidence: Rev 1 and Rev 2 carry `sys.c 1.97 (1995/07/21)`, `intr.c 1.68 (1995/07/11)` and a `vsync.c 1.49` object; Rev 4 and Best carry `sys.c 1.107 (1995/10/18)`, `intr.c 1.71 (1995/08/29)`, the DiskError format `%02x:%02x`, and no `vsync.c` id. So the SDK libraries were swapped between Rev 2 and Rev 4 (`ut_f.c 1.2` and the CD/MDEC strings are the same throughout), and Rev 4 to Best kept them. Rev 4 and Best also share the overlay content sizes (end of code identical in the first six overlays checked), Rev 1/2 do not. Best is a rebuilt and slightly edited Rev 4, not a repack. Directory record dates: Best overlays 1997-10-24 (EVENT and GYOZI 1996-03-28), Rev 2/4 1995-09-06, Rev 1 1995-08-28.

Our target: `SLPM_86.053` (sha1 `e823bd84...`, `config/SLPM_86.053.sha1`) is **PlayStation the Best**, the newest version.

## Code differences against ours (Best)
Method: text split into functions (end at `jr $ra` with no pending forward branch), each hashed with relocations masked (`jal` targets, `lui`/low halves of address pairs), then the function sequences aligned with difflib. Counts are upper bounds: data tables inside overlays are counted as functions, and the masking misses some address forms. "changed" = present in both but different after masking.

| version | exe functions (ours 1491) | exe identical after masking | exe changed | added / removed | overlays identical | overlay functions changed (24 differing, 4931 total) |
|---|---|---|---|---|---|---|
| Rev 4 | 1490 | 1444 | 46 | 0 / 1 | 2 of 26 (EVENT, GYOZI) | 311 (247 without OLH) |
| Rev 2 | 1555 | 1000 | 475 | 31 / 16 | 2 of 26 | 334 (270 without OLH) |
| Rev 1 / Shokai | 1555 | 949 | 521 | 36 / 21 | 2 of 26 | 639 (575 without OLH) |

- **Rev 4 vs Best, exe:** 46 changed functions, all in the game code region (0x80041000-0x80086810); about 23 of them differ in one masked word (a constant), the largest are `0x800455c4` (691 words, 236 differ), `0x80076000` (298, 260), `0x80068be4` (183, 147) and `0x8005b43c` (214, 135). SDK libraries are identical after masking. Data addresses shift (rodata starts at 0x800AF1F0 vs 0x800AF340), so every `lui/addiu` pair into data differs raw; only masked comparison helps.
- **Rev 4 vs Best, overlays:** all 24 differ; per overlay the changed functions are 2 (VALEN) to 27 (TAIIKU); OLH reports 64 of 76 in every version (OLH is much smaller in the O.BIN skeleton and is probably reworked; the segmenter may also misjudge it). Code sizes match Rev 4 to Best.
- Rev 2 to Best: the SDK libraries differ (older `sys.c`/`intr.c`, extra `vsync.c`), which probably accounts for much of the 475 changed and 47 added/removed functions (not separated into game and SDK code). Rev 1 adds about 50 more changed functions than Rev 2 (88 changed functions between Rev 1 and Rev 2 in the exe).
- Pairwise exe changed functions: Rev 1 to Rev 2 88, Rev 2 to Rev 4 514 (library swap), Rev 4 to Best 46.

## Recommendation
Keep `SLPM_86.053` (Best) as the target. It is the final, newest build, it shares the SDK libraries with Rev 4, and the progress already made (overlays, O.BIN names, SDK splits) is tied to it. Switching to an older version would mean redoing the work against a different SDK vintage for nothing.

Multi-version support later, by effort:
- **Rev 4:** cheap. Same exe size, same SDK; about 46 exe functions plus roughly 250 overlay functions differ, mostly small edits; data layout shifts, so symbols need per-version addresses (rodata 0x800AF1F0). A per-version symbol/diff overlay on the Best sources would work.
- **Rev 2 and Rev 1/Shokai:** expensive. Older SDK libs (older `sys.c`/`intr.c`, extra `vsync.c`), 475-521 exe functions changed, larger exe, different gp; treat as separate targets that reuse the game logic by hand.
- The overlays of all versions are not byte-compatible with ours (24 of 26 differ), so every version needs its own overlay hashes; `config/versions.txt` holds them.

`tools/identify_version.py` is the basis for "bring your own disc": it takes a bin/iso/cue or a folder, identifies the exe and counts matching overlays. Rev 1 and Shokai identify together (same exe and overlays). Unknown exes exit with status 1.
