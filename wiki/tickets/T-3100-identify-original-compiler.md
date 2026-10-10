---
id: T-3100
title: Identify the original game-code compiler
status: Done
assignee: agent (o-compiler)
created: 2026-10-09
updated: 2026-10-09
links: ["[[original-compiler]]", "[[toolchain]]", "[[obin]]", "[[compiler-mismatch-research]]", "[[tickets/T-0014-find-exact-ucode-compiler]]", "[[tickets/T-0100-older-mips-compiler-emulation]]", "[[tickets/T-1321-register-promotion-build-step]]"]
---

## Goal

Identify, as precisely as the evidence allows, which MIPS ucode-family compiler and version built the game code, and turn the evidence into concrete, testable rules for the uniform build step of [[tickets/T-1321-register-promotion-build-step]].

## Acceptance criteria

- [x] O.BIN ECOFF headers parsed field by field (file header, a.out header, sections, HDRR, FDR, PDR, symbols); vstamps decoded and matched against published tables; PDR frames compared with the retail exe.
- [x] SLPM_86.053, the overlays and O.BIN scanned for compiler/assembler/linker identification strings.
- [x] Public documentation of the candidate compilers' codegen differences collected with URLs (frame layout, -O levels, global register allocation).
- [x] Experiments with in-image IDO 5.3/7.1 only (no new binaries, no OS images); results recorded.
- [x] [[original-compiler]] written: evidence, ranked hypotheses with confidence, citations, rules for the build step.
- [x] Key findings appended to T-1321; [[toolchain]] and [[index]] updated.

## Notes

Licensing rule (user): no proprietary OS images (NEWS-OS, Ultrix, IRIX) and no compiler binaries the project does not already use. Public pages, man pages, manuals, papers and source code only; IDO versions already distributed by decompals/ido-static-recomp may be used.

## Comments
- 2026-10-09: done. Result in [[original-compiler]]; sources in `wiki/raw/original-compiler-sources.md`.
  - O.BIN: a.out vstamp and HDRR vstamp are both 0x0312 (3.18). In-image IDO 5.3 writes 3.19 and 7.1 writes 7.10. The SGI FAQ (June 1994) maps C 3.18 to IDO 5.2. The `.comment` section header is stored big-endian in a little-endian file (big-endian link host), f_flags is 0x800F, there is no F_AR32WR (so not GNU ld), and gp_value is bss_start + 0x7FF0. All 5 procedure descriptors show the +16 frame layout already in the 1995-07-25 developer build.
  - Disc-wide scan: no toolchain identification strings. The publisher field is `KONAMI_KCET`.
  - Experiments: `-Wo,-regr,N` moves selectors to `$v1` but regresses 194 of 1647 matched functions, so it was rejected. IDO 5.3 does promote globals in straight-line code, at a higher reference count than the original, which gives the T-1321 rules (section 5).
  - The coordinator's leads Evo's Space Adventures and CelestialAmber/tokimemo are both gcc setups and not precedents.
  - Tool: `tools/obin_syms.py --headers` (tests in `tools/test_obin_tools.py`, 10 OK).
  - Verification: clean rebuild 27 of 27 sha1 OK, and 1647 of 1647 matched functions identical after the `-regr` experiment was reverted.
  - Licensing: only the in-image IDO 5.3/7.1 were run; nothing was downloaded. The fetch tool cached two unreadable PDFs outside the repository by itself.
- 2026-10-09: inline review against CODING_STANDARDS.md (Standards and Spec), since 687608e.
  - Standards:
    - The script change is Python 3, run in Docker, documented in the module docstring, and fails non-zero on bad input (`headers` raises ValueError, caught in `main`). The tests use synthetic ECOFF only (no game data).
    - No game data is staged; the wiki quotes header values, two short source-name strings and short asm excerpts, which section 10 allows.
    - No `src/` or build change. The `-regr` experiment edited `tools/cc.py` only temporarily; it was reverted and rebuilt.
    - The commit is scoped to T-3100.
  - Spec: all brief items are covered:
    - every header field with its exact value;
    - vstamps decoded and matched to a published table;
    - FDR, PDR and symbols checked; PDRs compared with retail;
    - string scan;
    - documentation of candidates and options;
    - IDO experiments;
    - check of ido-static-recomp releases;
    - wiki page with confidence levels and URLs;
    - rules appended to T-1321;
    - toolchain and index updated;
    - the coordinator's extra leads.
  - Findings fixed during review:
    - corpus counts in the page corrected (24 and 136);
    - rule 7 qualified after the matched `func_8013AE80` counterexample;
    - [[obin]] corrected: entry and data_start were wrong, and it attributed the link to the PsyQ linker.
  - No open findings.
