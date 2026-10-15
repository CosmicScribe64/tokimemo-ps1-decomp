---
type: research
updated: 2026-10-15
---

# Broad coverage experiment — T-12230

User asked to broaden matching work for large coverage jumps. Baseline 6d1e9cf2, isolated codex/acceleration-audit worktree. **190 new targets across 18 units / 235,624 list bytes attempted; five literal-verified matches / 5,036 bytes added.** Progress rises from 5,756 to 5,761/6,958 functions, and from 1,306,708 to 1,311,744/2,279,368 game bytes (57.5%, +0.221 percentage points). This is useful progress, but not a large coverage jump.

## Selection and outcome

Excluded TT wholesale, the prior 24-target cohort, cx1–cx10 claimed objects, source differences in other accessible worktrees, and filesystem differences in three stale w17 checkouts with broken Git metadata. Main and all other worktrees remained read-only. These conservative exclusions define the population; this is not a representative sample of all remaining game code.

First cohort: per unit, up to eight largest 600–1,999-byte targets plus three largest 2,000–7,999-byte targets. 101 functions / 150,524 bytes; two direct wins / 2,228 bytes, 92 near, seven draft/compile failures; 459.49 seconds. Second cohort: remaining 89 functions / 85,100 bytes; one direct win / 792 bytes, 86 near, two compile failures; 432.70 seconds. Both used existing bulk_m2c, one default variant, --time 0 --jobs 1 --score-jobs 1 --no-order --no-regdiff. One CPU and 1 GiB per main Docker run; brief read-only diagnostic containers capped at one-quarter CPU. No dependency install or tool implementation changes.

Direct result: **3/190 matches**. A subsequent pass tried up to five existing m2c variants on nine close remaining targets: no new wins, 142.84 seconds; default remained best. These are recorded pipeline durations, not end-to-end throughput: selection, diagnostics, manual probes, verification, documentation and review are additional time. The earlier 16/24 result in two related overlays must not be extrapolated to this population.

| Unit | Attempts | List bytes | Direct wins | Final wins |
|---|---:|---:|---:|---:|
| BUNKAKEN | 9 | 13,176 | 0 | 1 |
| BUNKASAI | 11 | 13,324 | 3 | 4 |
| BUNKA_SD | 1 | 676 | 0 | 0 |
| ETC | 8 | 10,504 | 0 | 0 |
| EVENT | 11 | 16,764 | 0 | 0 |
| GEKO | 1 | 836 | 0 | 0 |
| GYOZI | 5 | 4,668 | 0 | 0 |
| KANGEI | 1 | 1,448 | 0 | 0 |
| MASTER | 6 | 8,932 | 0 | 0 |
| NAME_ENT | 3 | 6,892 | 0 | 0 |
| RPG_BAT | 50 | 64,452 | 0 | 0 |
| SHOUGATU | 1 | 652 | 0 | 0 |
| SHUGAKU | 1 | 624 | 0 | 0 |
| TACO | 43 | 44,100 | 0 | 0 |
| TAIIKU | 10 | 13,072 | 0 | 0 |
| TEL | 3 | 5,976 | 0 | 0 |
| VALEN | 3 | 3,240 | 0 | 0 |
| main | 23 | 26,288 | 0 | 0 |

## Verified changes

| Function | Literal bytes | Method |
|---|---:|---|
| BUNKASAI:func_80146100 | 936 | Default draft, exact rescore |
| BUNKASAI:func_8013B688 | 1,292 | Default draft, exact rescore |
| BUNKASAI:func_8013C0AC | 792 | Default draft, exact rescore |
| BUNKASAI:func_801466FC | 892 | Existing K&R call convention |
| BUNKAKEN:func_80143494 | 1,124 | Existing CharFlags bitfield test |

The two-row BUNKASAI miss was an `andi a0,v0,0xff` where the target passes the full register. Existing main_api override infrastructure permits an old-style declaration of func_80083440; its callee narrows the argument. This follows existing sibling conventions and leaves the canonical shared prototype unchanged. The ambiguous old D_800E68FC access uses the existing halfword GameState.unk_66C[4].unk_00.

BUNKAKEN's 44-row miss used a mask test where the original uses a shift/sign branch. The existing `.unk_0C.f.b2` view fixes it and the subsequent register sequence. A forward declaration matching the already-defined overlay helper is included in the overlay header. No new types, compiler passes, global settings, FAKE tricks, or unverified C bodies were added.

## What remains missing

The broad default pipeline is not enough for this remaining population. Productive follow-up should reconstruct types and calling conventions for coherent families before considering mutation search. This is an evidence-backed prioritization for these targets, not proof of a game-wide bottleneck.

Saved drafts omit declarations temporarily supplied by the bulk scorer. Reusing their bodies alone caused ambiguous field mappings or compilation failures in manual probes; recover those declarations from saved compiler context before comparing scores. Also distinguish a word-sized flag read from a byte-sized flag store: a decompiler cast followed by a word assignment can hide the original bitfield store. Treat this as source reconstruction verified against the target, not a semantics-preserving cleanup of the draft.

Further CharFlags probes reduced BUNKAKEN func_80141654 from 114 to **8** differing rows, but its aggregate-address allocation still differs. Related func_80142A40/func_801422C4 reached **22** rows with the flag read fixed; adding the bit store changed allocation/layout and did not finish them. Local flag-copy probes did not help. These remain assembly; none is counted as matched. GEKO's attempted flag probe still failed compilation, so no conclusion about its bitfield form follows.

RPG_BAT func_801454F0 remains 20 rows away: two counter sequences choose v0 instead of v1. Four increment/local forms, aggregate access, and return-type probes did not improve it. Other close targets include RPG_BAT func_8013AF1C (10), main func_800412E0 (19), RPG_BAT func_801568A4 (22), BUNKA_SD func_80137890 (30), and RPG_BAT func_80152080 (34). These are exact candidate scores, not matches or predictions of completion effort.

Recommended next bounded experiment: finish one shared flag/address-allocation family using full object context, test its source form on held-out siblings, and measure new linked bytes including unsuccessful work. Do not expand blind variant or random-mutation sweeps based on these results. Nine broad draft/compile failures and many distant differences also need proper source/type reconstruction; size selection alone does not solve them.

## Verification and reproduction

Five functions pass funcdiff --resolve literal linked-byte comparison; BUNKAKEN and BUNKASAI SHA1 checks pass after final headers and all accepted edits. check_headers, migrate_globals --check and sync_protos --check-branch pass (0 new disagreements, 15 known). No full-game build was run; no merge or push. Evidence: build/broad/proof.txt, build.log, verify.log, progress.txt, close-diffs.txt, typed-final-diffs.txt, rpg-probe.log, rpg-return.log, local-flag.log. Scripts and rejected drafts remain ignored/local.

Local manifests and initial outcomes: /private/tmp/tokimemo-broad-out/{cohort.json,state.jsonl,run.log} and /private/tmp/tokimemo-broad-rest/{cohort.json,state.jsonl,run.log}. Variant and accepted targeted results: /private/tmp/tokimemo-broad-followup/. The complete initial cohort/outcomes are preserved below so failures are not lost with scratch directories. See [[tickets/T-12230-broad-coverage-sweep]] and [[matching-breakthroughs]].

## Complete target ledger

Exact scores are the initial draft scores; targeted improvements are described above. A dash means no exact score recorded. List-byte sizes are kept separate from the literal proof table.

| Scope | Bytes | Initial result | Exact rows | Final matched |
|---|---:|---|---:|---|
| BUNKAKEN:func_8013B510 | 2164 | fail | — | no |
| BUNKAKEN:func_80141088 | 1484 | near | 374 | no |
| BUNKAKEN:func_80141654 | 1324 | near | 114 | no |
| BUNKAKEN:func_80141B80 | 1860 | near | 180 | no |
| BUNKAKEN:func_801422C4 | 1440 | near | 124 | no |
| BUNKAKEN:func_80142A40 | 1428 | near | 64 | no |
| BUNKAKEN:func_80142FD4 | 1216 | near | 97 | no |
| BUNKAKEN:func_80143494 | 1124 | near | 44 | yes |
| BUNKAKEN:func_801438F8 | 1136 | near | 169 | no |
| BUNKASAI:func_8013B25C | 1068 | near | 183 | no |
| BUNKASAI:func_8013B688 | 1292 | match | 0 | yes |
| BUNKASAI:func_8013BB94 | 1304 | near | 98 | no |
| BUNKASAI:func_8013C0AC | 792 | match | 0 | yes |
| BUNKASAI:func_80142700 | 1176 | fail | — | no |
| BUNKASAI:func_80142BE8 | 1516 | near | 717 | no |
| BUNKASAI:func_801433C0 | 932 | near | 332 | no |
| BUNKASAI:func_80146100 | 936 | match | 0 | yes |
| BUNKASAI:func_801466FC | 892 | near | 2 | yes |
| BUNKASAI:func_80146A78 | 1216 | near | 69 | no |
| BUNKASAI:func_8015AF90 | 2200 | near | 402 | no |
| BUNKA_SD:func_80137890 | 676 | near | 30 | no |
| ETC:func_801328A4 | 920 | near | 313 | no |
| ETC:func_80132C3C | 884 | near | 241 | no |
| ETC:func_80132FB0 | 832 | near | 224 | no |
| ETC:func_80136448 | 1280 | near | 145 | no |
| ETC:func_8013B1F0 | 1948 | near | 188 | no |
| ETC:func_8013FC64 | 796 | near | 224 | no |
| ETC:func_8013FFAC | 2116 | near | 512 | no |
| ETC:func_80142B80 | 1728 | near | 242 | no |
| EVENT:func_800FEAE8 | 736 | near | 135 | no |
| EVENT:func_800FEF74 | 940 | near | — | no |
| EVENT:func_800FF320 | 708 | near | 327 | no |
| EVENT:func_80102E54 | 672 | near | 159 | no |
| EVENT:func_80103774 | 628 | near | 206 | no |
| EVENT:func_80105DF8 | 856 | near | 330 | no |
| EVENT:func_80109404 | 768 | near | 284 | no |
| EVENT:func_8010F74C | 1728 | fail | — | no |
| EVENT:func_8011C750 | 3700 | near | 1285 | no |
| EVENT:func_8011D5C4 | 2036 | near | 700 | no |
| EVENT:func_8011DDB8 | 3992 | near | 1374 | no |
| GEKO:func_80133C50 | 836 | near | 164 | no |
| GYOZI:func_80139E4C | 696 | near | 196 | no |
| GYOZI:func_80144018 | 1204 | near | 461 | no |
| GYOZI:func_801444CC | 752 | near | 124 | no |
| GYOZI:func_80144B70 | 1300 | near | — | no |
| GYOZI:func_80145084 | 716 | near | 116 | no |
| KANGEI:func_80132FF8 | 1448 | near | 441 | no |
| MASTER:func_801391E4 | 1580 | near | 270 | no |
| MASTER:func_80139910 | 952 | near | 197 | no |
| MASTER:func_80139CC8 | 2748 | near | 885 | no |
| MASTER:func_8013A784 | 1388 | near | 268 | no |
| MASTER:func_8013ACF0 | 1308 | near | 269 | no |
| MASTER:func_8013B448 | 956 | near | 331 | no |
| NAME_ENT:func_80142A80 | 932 | near | 30 | no |
| NAME_ENT:func_80142E24 | 1652 | near | 56 | no |
| NAME_ENT:func_80143580 | 4308 | near | 1189 | no |
| RPG_BAT:func_80133108 | 4068 | near | 863 | no |
| RPG_BAT:func_80138324 | 1228 | near | 176 | no |
| RPG_BAT:func_801387F0 | 940 | near | 66 | no |
| RPG_BAT:func_80138B9C | 976 | near | 88 | no |
| RPG_BAT:func_80138F6C | 1268 | near | 128 | no |
| RPG_BAT:func_80139510 | 2424 | near | 281 | no |
| RPG_BAT:func_8013A034 | 704 | near | 138 | no |
| RPG_BAT:func_8013AF1C | 980 | near | 10 | no |
| RPG_BAT:func_8013B5B0 | 864 | near | 109 | no |
| RPG_BAT:func_8013BBF8 | 688 | fail | — | no |
| RPG_BAT:func_8013C890 | 1012 | near | 212 | no |
| RPG_BAT:func_8013CC90 | 928 | near | 161 | no |
| RPG_BAT:func_8013D834 | 1068 | near | 158 | no |
| RPG_BAT:func_8013DC60 | 1692 | near | 319 | no |
| RPG_BAT:func_8013E54C | 628 | near | 1095 | no |
| RPG_BAT:func_8013EA60 | 1100 | near | 144 | no |
| RPG_BAT:func_8013F82C | 1376 | near | 248 | no |
| RPG_BAT:func_80140204 | 2132 | near | — | no |
| RPG_BAT:func_80140A58 | 624 | near | — | no |
| RPG_BAT:func_80141060 | 2564 | near | 1831 | no |
| RPG_BAT:func_801447E0 | 924 | near | 216 | no |
| RPG_BAT:func_80144C84 | 1276 | near | 393 | no |
| RPG_BAT:func_801454F0 | 1996 | near | 20 | no |
| RPG_BAT:func_80145CC0 | 2232 | near | 195 | no |
| RPG_BAT:func_80146578 | 812 | near | 277 | no |
| RPG_BAT:func_80146C30 | 2984 | near | 329 | no |
| RPG_BAT:func_80147930 | 1500 | near | 396 | no |
| RPG_BAT:func_80148714 | 1012 | near | — | no |
| RPG_BAT:func_80148B08 | 1120 | near | — | no |
| RPG_BAT:func_80148F70 | 1644 | near | 521 | no |
| RPG_BAT:func_80149C5C | 1296 | near | 567 | no |
| RPG_BAT:func_8014A3DC | 632 | near | 66 | no |
| RPG_BAT:func_8014A660 | 1316 | near | 103 | no |
| RPG_BAT:func_8014ADD0 | 1212 | near | 284 | no |
| RPG_BAT:func_8014B294 | 1188 | near | 501 | no |
| RPG_BAT:func_8014B79C | 776 | near | 148 | no |
| RPG_BAT:func_8014BB80 | 1124 | near | 214 | no |
| RPG_BAT:func_8014BFE4 | 892 | near | 229 | no |
| RPG_BAT:func_8014D11C | 600 | near | 156 | no |
| RPG_BAT:func_8014F88C | 1328 | near | 145 | no |
| RPG_BAT:func_801505B8 | 1260 | near | 235 | no |
| RPG_BAT:func_80151C10 | 808 | near | 62 | no |
| RPG_BAT:func_80152080 | 772 | near | 34 | no |
| RPG_BAT:func_80154760 | 1656 | near | — | no |
| RPG_BAT:func_80154DD8 | 1204 | near | — | no |
| RPG_BAT:func_8015528C | 1500 | near | 498 | no |
| RPG_BAT:func_80156064 | 828 | near | — | no |
| RPG_BAT:func_801568A4 | 1048 | near | 22 | no |
| RPG_BAT:func_80156D78 | 992 | near | 91 | no |
| RPG_BAT:func_80157158 | 1256 | near | 127 | no |
| SHOUGATU:func_8013BDE0 | 652 | near | 215 | no |
| SHUGAKU:func_80137490 | 624 | near | 98 | no |
| TACO:func_8013240C | 1188 | near | 534 | no |
| TACO:func_801328B0 | 1848 | near | 541 | no |
| TACO:func_80132FE8 | 908 | near | 89 | no |
| TACO:func_8013760C | 640 | near | 114 | no |
| TACO:func_80138D08 | 1040 | near | — | no |
| TACO:func_80139424 | 1676 | near | 606 | no |
| TACO:func_80139BCC | 1004 | near | 332 | no |
| TACO:func_80139FB8 | 1480 | near | 558 | no |
| TACO:func_8013A790 | 1492 | fail | — | no |
| TACO:func_8013AD64 | 632 | near | 194 | no |
| TACO:func_8013B460 | 1328 | near | — | no |
| TACO:func_8013BAC4 | 1840 | near | 817 | no |
| TACO:func_8013C440 | 660 | near | 217 | no |
| TACO:func_8013C6D4 | 1388 | near | 539 | no |
| TACO:func_8013D528 | 784 | near | — | no |
| TACO:func_8013D838 | 832 | near | — | no |
| TACO:func_8013DB80 | 612 | near | 235 | no |
| TACO:func_8013DEE4 | 1336 | near | 431 | no |
| TACO:func_8013E778 | 812 | near | 265 | no |
| TACO:func_8013EAA4 | 1948 | near | 644 | no |
| TACO:func_8013F468 | 880 | near | 264 | no |
| TACO:func_8013FBF0 | 1040 | near | 394 | no |
| TACO:func_80140028 | 828 | near | 296 | no |
| TACO:func_801407D4 | 776 | near | 310 | no |
| TACO:func_80140ADC | 780 | near | 241 | no |
| TACO:func_801413B8 | 816 | near | 254 | no |
| TACO:func_801416E8 | 1072 | near | 384 | no |
| TACO:func_80141B18 | 708 | near | 245 | no |
| TACO:func_80141DE0 | 764 | near | 257 | no |
| TACO:func_801424A8 | 880 | near | — | no |
| TACO:func_801432F0 | 644 | near | 124 | no |
| TACO:func_80143B58 | 804 | near | 123 | no |
| TACO:func_80144094 | 924 | near | 289 | no |
| TACO:func_80144430 | 672 | near | 197 | no |
| TACO:func_801487A4 | 948 | near | — | no |
| TACO:func_80149538 | 692 | near | — | no |
| TACO:func_801497EC | 912 | near | — | no |
| TACO:func_80153B38 | 832 | near | — | no |
| TACO:func_8015443C | 704 | near | 193 | no |
| TACO:func_80154960 | 1556 | fail | — | no |
| TACO:func_801553AC | 1084 | near | 364 | no |
| TACO:func_801563E4 | 1344 | fail | — | no |
| TACO:func_801585D0 | 992 | near | 259 | no |
| TAIIKU:func_80132000 | 2416 | near | 539 | no |
| TAIIKU:func_80140A90 | 2020 | near | 228 | no |
| TAIIKU:func_80141280 | 1764 | near | 446 | no |
| TAIIKU:func_80141CDC | 1120 | near | 98 | no |
| TAIIKU:func_80142400 | 1036 | near | 391 | no |
| TAIIKU:func_8014280C | 1412 | near | 260 | no |
| TAIIKU:func_80142FB8 | 992 | near | 439 | no |
| TAIIKU:func_8014357C | 664 | near | 173 | no |
| TAIIKU:func_80143814 | 692 | near | 211 | no |
| TAIIKU:func_80143F50 | 956 | fail | — | no |
| TEL:func_8013CCD0 | 876 | near | 220 | no |
| TEL:func_8013D03C | 2128 | near | 1050 | no |
| TEL:func_8013D88C | 2972 | fail | — | no |
| VALEN:func_80132834 | 1240 | near | 509 | no |
| VALEN:func_80132D0C | 892 | near | 218 | no |
| VALEN:func_80133088 | 1108 | near | 264 | no |
| main:_sprite_set_light_effect1 | 1124 | near | 403 | no |
| main:func_800412E0 | 676 | near | 19 | no |
| main:func_80041C2C | 796 | near | 449 | no |
| main:func_80042CB0 | 864 | near | 333 | no |
| main:func_800430C0 | 904 | near | 146 | no |
| main:func_80049A40 | 1452 | near | 589 | no |
| main:func_8004A1A0 | 628 | near | 231 | no |
| main:func_8004A414 | 1024 | near | 396 | no |
| main:func_8004A814 | 1028 | near | 436 | no |
| main:func_8004AE54 | 840 | near | 149 | no |
| main:func_80056BA8 | 1296 | near | 287 | no |
| main:func_800578F4 | 1064 | near | 151 | no |
| main:func_80057D28 | 1648 | near | 266 | no |
| main:func_80058398 | 2432 | near | 388 | no |
| main:get_fnt | 1036 | near | 347 | no |
| main:make_color_bar | 1072 | near | 665 | no |
| main:make_color_bar16 | 1540 | near | 840 | no |
| main:mod_set | 780 | near | 254 | no |
| main:mod_set_gpu | 804 | near | 210 | no |
| main:mod_trans_vram | 752 | near | 245 | no |
| main:srn_tpage_show | 3224 | near | 1311 | no |
| main:srn_vram_set | 664 | near | 138 | no |
| main:strCallback | 640 | near | 139 | no |
