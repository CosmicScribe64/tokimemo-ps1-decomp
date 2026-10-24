---
id: T-27003
title: "Pilot: O.BIN names into the overlays (alias debt)"
status: In Review
assignee: sonnet (p2-pilot)
created: 2026-10-24
updated: 2026-10-24
blocked-by: [T-27002]
links: [[[phase2]], [[obin]], [[tickets/T-0600-apply-obin-renames]], [[tickets/T-27000-phase-2]]]
---

## Goal

Finish T-0600 for the overlays (audit gaps G2 and G3). The O.BIN names reach only the main executable today:
105 renamed main symbols are still used under their placeholder names in 186 overlay files (3,431 uses), and
`include/main_api.h` declares 56 of them under both names, 45 of the pairs with different types. This is the
first unit to run through `tools/rename.py` (T-27002) and proves the tool on real data.

## Acceptance criteria

- [x] The overlay yamls load `config/symbol_addrs_obin.txt` (and the SDK names), so overlay asm and m2c output
      use the O.BIN names; the deliberate two-name address 0x80084C98 stays explained.
- [x] The 105 O.BIN names applied in the 186 overlay files through `rename.py`, and the SDK-named
      placeholders that overlays still use (`strcpy` as `func_800AE0F0`, `rand`, `rsin` and about 25 more,
      about 1,700 uses in `src/`) renamed the same way; no placeholder of a named main symbol left in `src/`
      or `include/` (`rename.py check`). These aliases are not part of the optional SDK names.
- [x] The 56 dual declarations merged; each of the 45 with a type difference resolved by the build
      (`tools/sync_protos.py --fix` overrides where needed); `config/proto_known.txt` alias rows cleared.
- [x] `cd_read_status` moved out of the shared symbol file (EVENT range, G3); `check_symbols.py` rejects a
      main name in EVENT's range in every file that EVENT loads, `config/symbol_addrs_hypotheses.txt` and
      `config/symbol_addrs_obin.txt` included (T-27002 may already do this; check it here).
- [x] Clean build 27/27 sha1, `ninja progress` unchanged.

## Notes

Out of scope: the English comments on O.BIN names (each main module unit writes them for the names it owns at
the C definition; the SDK-side ones have none and are optional) and new names. Audit section 2.3 and risk 1 describe the alias views.

## Comments

- 2026-10-24 p2-pilot (branch `p2-pilot`, worktree `../tokimemo-wt/p2-pilot`, base `w2-good` d59d5d213), In Review.
  Applied, in six commits, each with a clean build (27 of 27 sha1 OK, 6,952 of 6,952 functions, `nice -n 19`, 2 jobs):
  1. `tools/rename.py`: a row whose new name comes from `config/symbol_addrs_obin.txt` or
     `config/symbol_addrs_sdk.txt` skips the style rules (snake_case, libc, leading underscore); an alias row
     merges the two `main_api.h` declarations (keeps the new name's own, takes the override wrapper); new gcc errors
     that name an alias symbol are notes in proof 2. Tests in `tools/test_rename.py` (61).
  2. 104 O.BIN rows (`config/renames/obin.txt`, table `drafts/T-27003/obin.tsv`): 3,481 uses in 187 files.
  3. 24 SDK rows (`config/renames/sdk.txt`, `drafts/T-27003/sdk.tsv`): `strcpy` (603 uses), `rand`, `rsin`, `srand`,
     `abs`, `memcpy`, `printf`, the matrix and primitive helpers and others, 1,690 uses in 216 files.
  4. The 26 overlay yamls and the `gen_overlay_configs.py` template load the O.BIN and SDK files; both leave
     `build/main_names.ld`; `config/proto_known.txt` loses 11 of its 12 alias rows.
  5. `rename.py check` now also refuses a placeholder of a named main symbol in `src/` and `include/` (exempt: an
     `alias-view` line). The tree passes.
  The dual declarations: 64 names of the tables were declared under both names (the audit counted 56; 50 of the 64 with
  different type strings, counted with `sync_protos.Model` on the base tree), and the merge kept the type of the
  name's own declaration. The build found 8 objects that differed (DATE 80132000, ENDING 80132000 and 801396E0,
  MASTER 80138490, NAME_ENT 80144AD0, SHOUGATU 8013E330, TAIIKU 80138950, main 8007C030), and check_headers flagged
  three types against their main-exe definitions. Fixes: `MAIN_API_OVERRIDE_` for `tpage_buf_clear`, `get_now_k` (3
  files), `Vblnk_Timer_Init`, `Rot_2D`, `get_g_zyotai_s`, `search_tpage_multi`; a block-scope
  `extern void set_dec_bri();` in `func_80145AC4` (NAME_ENT needs both views in one file; a function-pointer cast
  turns the `jal` into `jalr`); `get_now_k`, `menu_set` and `search_tpage_multi` take the type of their definition;
  the file-local `get_now_k` declarations went. Every other pair needed nothing: IDO makes the same bytes.
  G3: `cd_read_status` was already in `config/symbol_addrs_main.txt` (T-27002). `check_symbols.py` rejects a main name in
  EVENT's range in every file that several units load, now including the O.BIN and the hypotheses file (new test);
  the real files pass.
  Skipped: the deliberate two-name address 0x80084C98 (`func_80084C98` stays in 14 places, `alias-view`, its
  `proto_known.txt` row stays with the reason); the 105th name of the audit is that address, so 104 O.BIN rows ran.
  No English comments were added: no C definition of an O.BIN name was touched (the definitions are in `src/main`
  and belong to the main module units). The O.BIN names themselves are unchanged, also the non-snake_case ones
  (`LoadSquare`, `Vblnk_Timer_Init`, `_sprite_set_box_shade_tarao`).
  Checks at the end: clean build 27/27, `rename.py check`, `check_headers.py`, `migrate_globals.py --check`,
  `sync_protos.py --check-branch`, full tool suite (1,641 tests OK, 3 skipped), `funcdiff.py --resolve` MATCH for 13
  functions that contain renamed calls or overrides (NAME_ENT:func_80145AC4, TAIIKU:func_8013E1FC and func_8013C4E4
  and func_8013C0E4, DATE:func_801324DC and func_80133EA0, ENDING:func_8013B510 and func_80132F18,
  MASTER:func_80138490, SHOUGATU:func_8013E410, main:check_k_scroll and main:mod_set_gpu, GEKO:func_80135DC0).
  Cost: each overlay split now prints splat's "Unable to determine a segment" list for the O.BIN and SDK names
  (clean-build log 1.2 MB instead of 0.33 MB); harmless. Procedure for later alias merges: [[rename-tool]] "Alias rows".
