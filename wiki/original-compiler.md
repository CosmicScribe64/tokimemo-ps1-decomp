---
type: research
updated: 2026-10-10
ticket: T-3100
sources: ["disc/files/CDROM/EXEDIR/O.BIN", "disc/files/SLPM_86.053", "disc/files/CDROM/EXEDIR/*.EXN", "disc/track1.bin", "tools/obin_syms.py", "wiki/raw/original-compiler-sources.md"]
---

# The original game-code compiler (T-3100)

Question: which compiler and version built the game code? We already know it is a MIPS ucode-family compiler (cfe/uopt/ugen/as1, like SGI IDO) that differs from IDO 5.3 in three measured ways ([[matching-notes]], [[toolchain]]). This page holds the file evidence, the ranked hypotheses and the rules for [[tickets/T-1321-register-promotion-build-step]]. URLs for every external claim are in the raw note `wiki/raw/original-compiler-sources.md`. Ticket: [[tickets/T-3100-identify-original-compiler]].

Licensing rule (user): no OS images and no compiler binaries beyond what the project already uses. Everything below comes from public documents, the game's own files and the in-image IDO 5.3/7.1.

## Short answer

The most likely compiler is a MIPS/SGI ucode compiler suite at release **3.18**. SGI shipped that release as the C compiler of IDO 5.2 (IRIX 5.2, March 1994), one minor version before IDO 5.3 (3.19). Here it was configured to emit little-endian ECOFF, and the linker ran on a big-endian host. A Sony NEWS workstation running Sony's early PlayStation toolchain fits all of this, but nothing documents that setup (UNVERIFIED). The release number comes from the version stamps the linker wrote into O.BIN, the developer build's symbol map. Its procedure descriptors show the same +16 frame layout as the retail game, so the same compiler built the 1995-07-25 developer build and the retail build.

## 1. O.BIN header fields (exact values)

`tools/docker.sh python3 tools/obin_syms.py --headers` prints all of them. Format background is in [[obin]].

| structure | field | value | reading |
|---|---|---|---|
| file header | f_magic | 0x0162 | MIPSELMAGIC, little-endian MIPS ECOFF |
| | f_nscns | 4 | .text, .rdata, .data, .comment |
| | f_timdat | 0x30152746 | 1995-07-25 17:17:58 UTC |
| | f_symptr / f_nsyms | 0x5C0 / 0x60 | the symbolic header and its size |
| | f_opthdr | 0x38 | |
| | f_flags | 0x800F | F_RELFLG, F_EXEC, F_LNNO and F_LSYMS (relocations, line numbers and local symbols stripped), plus 0x8000. 0x8000 is F_MIPS_NO_REMOVE ("do not remove nops") in the Tru64 documentation; no pre-OSF definition was found. F_AR32WR (0x0100) is not set, but BFD/GNU ld always sets it for little-endian output, so GNU ld did not write this file. |
| a.out header | magic | 0407 (OMAGIC) | `ld -N` output |
| | **vstamp** | **0x0312 = 3.18** | linker version stamp |
| | tsize / dsize / bsize | 0x3A0 / 0x130 / 0 | |
| | entry, text_start | 0x80132000 | |
| | data_start / bss_start | 0x801323A0 / 0x801324D0 | |
| | gprmask | 0xE1FFC0FE | at, v0-v3, a0-a3, t6, t7, s0-s7, t8, sp, fp, ra. t0-t5, t9 and gp are unused, which fits ucode temporaries that start at $t6. |
| | cprmask | 0, 0, 0, 0 | no FPU |
| | gp_value | 0x8013A4C0 | = bss_start + 0x7FF0, the MIPS ld convention for `_gp`. The code never uses `$gp` (`-G 0`). |
| sections | .text | 0x80132000, size 0x3A0, file offset 0xF0, flags 0x20 | |
| | .rdata | 0x801323A0, size 0x90, offset 0x490, flags 0x100 | |
| | .data | 0x80132430, size 0xA0, offset 0x520, flags 0x40, lnnoptr 0x5C0 (odd: it points at the symbolic header) | |
| | .comment | **header stored big-endian**: size 0x24, scnptr 0x620, flags 0x02100000 (STYP_COMMENT); read little-endian it is garbage (size 0x24000000, flags 0x1002) | every other header field in the file is little-endian. The plain explanation is a linker or strip on a big-endian host that byte-swapped every header it knew and copied this one in host order. No public source documents this quirk (UNVERIFIED). |
| symbolic header (HDRR) | magic | 0x7009 | magicSym |
| | **vstamp** | **0x0312 = 3.18** | written by the linker (`ld -VS` sets both stamps) |
| | non-zero counts | ipdMax 5 at 0x638, issExtMax 0x6F7C at 0x73C, ifdMax 1 at 0x76B8, iextMax 0x862 at 0x7700 | lines, dense numbers, local symbols, optimization symbols, aux entries, local strings and relative file descriptors are all 0 (stripped) |
| file descriptor (FDR 0) | adr / rss | 0x80132000 / -1 (no name) | stripped; no source file name or path survives |
| | cpd | 5 | |
| | bits | 0x204: lang 4 (langMachine), glevel field 2 (= GLEVEL_0), fBigendian 0 | the descriptor a strip or ld synthesizes; it says nothing about the C front end |
| PDR 0-4 | see section 2 | | isym points into the external table (0x854-0x857, 0x861), iline -1, no line numbers |
| external symbols | 2146 | 2132 Global/Abs, 9 Global/Data, 5 Proc/Text; flags 0, ifd 0, index 0xFFFFF | |

The version stamps, decoded:
- ECOFF stamps are major.minor with a decimal minor (Ultrix `stamp.h` MS_STAMP 1, LS_STAMP 31 = "1.31", per the gas source). The in-image compilers fit this: IDO 5.3 as1 writes 0x0313 (3.19) and IDO 7.1 writes 0x070A (7.10), both measured.
- Per the SGI FAQ of June 1994, IRIX 4.0.x shipped IDO 4.1.1 with C 3.10.1 and a 3.12 back end, and IRIX 5.2 shipped IDO 5.2 with C 3.18. So 3.18 sits between 3.12 and IDO 5.3's 3.19.
- The stamp belongs to the tool that wrote the file (ld, and probably strip). The IDO ld checks the stamps of its input objects and warns on a mismatch (`version stamp: %d.%d, does not match`), so the objects were probably also 3.18. That is an inference: the objects themselves are not on the disc.
- IDO 5.3's ld writes only ELF; it contains no COFF strings. So O.BIN was not linked by an IRIX 5.3 ld. Whether IRIX 5.2's ld could still write ECOFF is unknown.

No compiler, assembler or linker identification string exists anywhere on the disc. The scan covered `SLPM_86.053`, the 26 overlays, O.BIN and the whole raw `track1.bin`, looking for `$Header`, `@(#)`, `NEWS`, `/usr/`, `Mips Computer`, `MIPS`, `psylink`, `PSYQ`, `SN Systems`, `ULTRIX`, `IRIX` and `Copyright`. It found only the known PsyQ RCS ids, the SCE copyright and two game source names inside error strings: `p_line.c` and `kanji.c` (main exe). The ISO publisher field is `KONAMI_KCET` (Konami Computer Entertainment Tokyo, founded 1995-04-03), and the data preparer is `K_FUKUHARA`.

## 2. Frame layout in the developer build

The O.BIN procedure descriptors show the +16 layout already in the 1995-07-25 build. Here `save_end` is the end of the register-save block, computed as frame + regoffset + 4.

| procedure | frame | regmask | regoffset | save_end | frame - save_end | retail |
|---|---|---|---|---|---|---|
| olh_main | 0x28 | ra | -0x14 | 0x18 | 0x10, no locals | `func_80132000`: 0x28, ra at 0x14 |
| olh_init | 0xC0 | s0-s7, fp, ra | -0x7C | 0x48 | 0x78; lowest local at 0x74, so 0x48-0x57 is unused | `func_80132114`: 0xC0, same save slots |
| olh | 0x28 | ra | -0x14 | 0x18 | 0x10 | `func_80132294`: 0x28 |
| olh_exit | 0x28 | ra | -0x14 | 0x18 | 0x10 | `func_801323AC`: 0x28 |
| move_menu_title | 0x30 | s0, s1, ra | -0x14 | 0x20 | 0x10, no locals | (no retail counterpart identified) |

In all 5 descriptors the frame is the save block plus 16 plus the locals. IDO 5.3 would give 0x18, 0xB0, 0x18, 0x18 and 0x20. The disassembled O.BIN `.text` also shows the other fingerprints: `$t6`/`$t7`/`$t8` temporaries, `or rd,rs,zero` moves, IDO's `sltiu; beqz; sll` jump-table entry, and `olh_main` dispatching on a global loaded into **`$v1`** where IDO 5.3 and 7.1 load it into `$v0`. That last one is the T-0018 signature, and it was already present in the developer build.

## 3. What distinguishes the candidates (public documentation)

| candidate | what fits | what does not fit or is unknown |
|---|---|---|
| MIPS/SGI release 3.18 (SGI: IDO 5.2, March 1994) | the stamps are exactly 3.18; it is one release before IDO 5.3 (3.19), which explains why IDO 5.3 is the closest available compiler; the date fits a 1995 project | IRIX 5.x links ELF, but O.BIN is ECOFF linked on a big-endian host, so the packaging was not stock IRIX 5.2 unless its ld could still write ECOFF (unknown); SGI's 3.18 probably had kpicopt, which the original lacks (unknown) |
| Sony NEWS-OS cc (MIPS ucode, ECOFF, big-endian NEWS host, `-EL` supported) | early PS1 kits were NEWS-based (MW.2/MW.3); a secondhand summary of a Sony document from August 1995 still names NEWS-OS as an R&D environment; NEWS-OS 4.2.1R ld has `-VS`; a big-endian host explains the unswapped `.comment` header | no source gives any NEWS-OS compiler version; the 4.2.1R man page names `ccom` as the front end, not `cfe` (the IDO 5.x name); whether Sony shipped a 3.18-level compiler is UNVERIFIED |
| Ultrix 4.x cc (MIPS 2.x/3.x, DECstation) | same pass list, `-EL` native | the host is little-endian, so it could not leave a big-endian `.comment` header; no evidence of DEC use in Japanese PS1 development |
| MIPS RISCompiler 2.x / 3.10 (RISC/os) | ucode family | the stamp is 3.18, not 2.x or 3.10 |
| IDO 5.3 with unknown options | closest codegen | the stamp is 3.19 and 3.18 was measured in the file; every cfe, uopt and ugen option was tried (T-0014, T-0017, T-0018, and this ticket) |
| PsyQ / SN gcc | | ruled out by codegen (T-0013); no PS1 decomp, decomp.me preset or splat example uses a ucode compiler for PS1. Evo's Space Adventures, raised as a possible lead, is gcc 2.95.2 + maspsx on PS1 (its N64 version is the IDO one), and CelestialAmber/tokimemo (an abandoned 2024 splat setup for version 1.1) assumes the same gcc template |

Documentation about the three known differences:
- **Frame (+16):** no document mentions a reserved 16-byte block or leaf functions that keep the argument area. UNVERIFIED in every candidate.
- **-O levels:** the NEWS-OS and Ultrix man pages say `-O2` runs the global ucode optimizer and that `-O3` adds ucode linking and "global register allocation". OSF/1 uopt(1) says `-O3` uses interprocedural register allocation, and `-zdbug:6` traces uopt's priority-based-coloring allocator (Chow and Hennessy). No document describes the promotion threshold for globals. The original code is plain `-O2` per-function code (no cross-file inlining or register passing was seen), so `-O3` is not indicated.
- **kpicopt:** not documented anywhere I read. The option appears in the uopt option tables of IDO 5.3/7.1, and its name suggests it came with IRIX 5's PIC code. That would fit a pre-PIC or non-IRIX build of the 3.x suite (inference).

## 4. Experiments with the in-image IDO (this ticket)

Not repeated: everything listed in [[matching-notes]] (frame section, T-0017 and T-0018 sections).

- **New uopt options found in the 5.3 option table and tried:** `-regr N`, `-rege N`, `-nomultibbunroll`, `-unrolllimit`, `-no_r23`, `-pic2`, `-fortran_lang`, `-f77alias`, `-dwopcode`, `-dowhyuncolor`. Only `-regr` changes the code. It sets the size or selection of uopt's caller-saved register pool, and the result is not monotonic: 0 spills, 1-6 give `$t0`, 7-8 give `$v1`, 9 gives `$v0` (the default), 10 and up give `$t0`.
  - `-Wo,-regr,8` turns the `olh_main` selector into `$v1`, as in the original.
  - But it does not promote anything new, and a full rebuild with it regresses 194 of the 1647 matched functions. The comparison was per function, on the objects, with immediates masked.
  - So it is not the original's setting. `-rege` (callee-saved pool) changed nothing in these tests.
  - `-Wo,-zdbug:6`, uopt's register-allocation trace, aborts in the recomp (`wrapper_ecvt`), so uopt's priorities cannot be printed.
- **The T-0018 premise needs correcting.** IDO 5.3 does promote a global scalar in straight-line code. It only needs more references than the original. The table shows straight-line C with an `extern u8 D`. The outcome is the same for an extern, a file-defined and a `static` D.

| C shape (no loop) | IDO 5.3 |
|---|---|
| `if (D==1) { D++; g(); D += D; }` | **promotes**: `lbu v1`, `addiu t6,v1,1`, reload `lbu v1` after the call |
| `switch (D) { case 0: D++; g(); D += D; break; ... }` | **promotes**: `lbu v1; beqz v1; move v0,v1` (the switch temporary copied to `$v0` in the delay slot), reloads into `$v1` |
| `if (D==1) { D++; g(); D++; }` | the value is CSE'd in `$v0` with a reload into `$v0` |
| `if (D==1) { g(); D++; }`, `if (D==1) { g(); D++; g(); D++; }`, `if (D==1) { g(); D += D; }` | no promotion: the compare uses `$t6`, the read-modify-write uses fresh `$t7/$t8...` |
| `switch (D) { case 0: f(); break; ... }` (one reference) | no promotion: selector in `$v0` |
| `switch (D) { case 0: f(); D++; break; ... }` | no promotion: selector in `$v0`, `D++` in fresh temporaries |

The original's version of the last three shapes is the promoted shape. Two examples:
- `func_8005A560`: `lbu v1` before the chain, `lbu v1` again after the call, `addiu t6,v1,1; sb t6`.
- `func_8013A40C` and the O.BIN `olh_main`: `lbu v1; beqz v1; or v0,v1,zero`.

So the original emits exactly IDO's own promoted code, at a lower reference count.

**Corpus counts** (all functions of the main exe and the 26 overlays whose first two instructions load a global into `$v0`/`$v1`; scratch scan, not committed):

| shape of the dispatch on the loaded global | `$v1` | `$v0` |
|---|---|---|
| `switch` compiled as a `beq` chain, main-exe global | 262 (136 with the `or v0,v1,zero` copy) | 25 |
| same, overlay-defined data (addresses >= 0x80132000) | 24 | 45 |
| jump-table switch (`sltiu; ... jr`) | 28 | 138 |
| `if` chain (`bne`) | 11 | 108 |

These counts fit one model:
- A switch on a promoted scalar global gives v1 + copy.
- A switch on something not promoted (for example struct or array data) gives v0, as in IDO.
- An if-chain has no switch temporary, so the promoted global takes v0.
- A jump-table selector is used in only one block, so it stays in v0.

## 5. Rules for the build step (handed to T-1321)

Each rule is grounded in the evidence above and can be tested against [[data/t0018-cases]] and the matched functions.

1. **Frame:** keep `tools/frame_pass.py` unchanged. O.BIN shows that the developer build already had frame = save block + 16 + locals (5 of 5 procedure descriptors), the same layout as retail, so this is a property of the compiler, not of a late build change.
2. **Constants and addresses:** keep `-Wo,-nokpicopt`. Nothing new contradicts it.
3. **Promotion decision (the T-0018 gap).** A global scalar is one accessed directly through `%hi/%lo(sym)` with no index, not volatile, address not taken, and not a struct or array member.
   - The original promotes such a global whenever it has at least one reference whose value is needed in more than one basic block (a switch selector or compare chain), or when it is read again after a call or a store.
   - IDO 5.3 promotes only when the global has more references; in the measured shapes, two reads and two writes around one call were enough, but one compare plus one `D++` was not.
   - Hypothesis to test: the original allocates live ranges with priority >= 0 where IDO requires > 0. Equivalently, the original does not charge the entry load against a global's priority.
4. **Register and shape once promoted:** do not rewrite registers after the fact; IDO already produces the original's shape once it promotes.
   - Rule 3 is therefore the whole change.
   - The priority order of uopt's coloring then gives:
     - switch temporary in `$v0`;
     - the global in `$v1`, with `move v0,v1` kept when the temporary survives;
     - in if-chains, the global in `$v0`;
     - every reload after a call into the same register;
     - read-modify-write through `addiu tN,reg,k; s* tN`.
   - A binasm or post-pass that renames registers would have to guess interference; that fails CODING_STANDARDS 7a.
5. **Where to change it:** the cleanest candidate is a single-comparison patch of uopt's allocation threshold in a copy of the recompiled `uopt`. This is a toolchain change, not a fakematch.
   - Locate the comparison through the uopt reconstructions (n64decomp/ido, LLONSIT/ido-decomp).
   - Do not change `-Wo,-regr`: it moves the selector to `$v1` without promoting, and it regresses 194 matched functions.
6. **Gate:**
   - every `promo` row of [[data/t0018-cases]] must reproduce;
   - all 1647 matched functions must stay identical (per-function object compare, immediates masked);
   - the 27 sha1 checks must pass.
   - The 3 `reverse` rows (`func_80132DC4`, `func_8007B5EC`, `func_8014EDCC`: the original reloads where IDO keeps a register) are not explained by rule 3 and must be checked separately.
7. **Not promoted:** data the original did not promote, which is where IDO already matches. The original keeps it in `$v0` like IDO: 45 of 69 beq-chains on overlay-defined data. A matched example is `func_8013AE80` (GYOZI), which is `switch (D_801474B8)` on a `u8` that the overlay itself defines, compiled by stock IDO.
   - Two explanations fit, and the rule must pick between them before it is implemented:
     - (a) these overlay variables were struct or array members in the original source (the addresses 0x8014749C-0x801474B8 are a run of s32/s16/u8 fields);
     - (b) the original promotes only globals defined outside the compilation unit.
   - Test: compare the matched C of all overlay-data switches and compare chains against the main-exe ones.

## 6. Unknowns and next steps

- The exact product: an SGI IDO 5.2-era suite in an ECOFF cross configuration, or a Sony NEWS-OS release at 3.18? No document links Sony to 3.18.
- Whether 3.18 itself produces the +16 frames and the lower promotion threshold. The decisive test is IDO 5.2, which decomp.me runs under qemu-irix. The licensing rule forbids running it here, so this needs the user's decision ([[tickets/T-0100-older-mips-compiler-emulation]]). The same applies to IDO 4.1 (3.10/3.12). On the four frame canaries of T-0100 and the promo rows, a match would settle hypothesis 1.
- The exact uopt priority formula (loop weighting, entry-load cost). It is not in any public document read; it is in the uopt reconstructions.

## Update: IDO 5.2 and 4.1 tested (T-3110)
[[ido-52-evaluation]]:
- IDO 5.2 (stamp 3.18) is byte-identical to 5.3 on all 2564 matched functions and on 46 blocked cases. It has 5.3's frames, and it does not reproduce the T-0018/T-1321 behaviours.
- IDO 4.1's ugen (3.12) makes exactly the frame-pass layout (0 frame differences in 2564 functions), but its register allocation differs from the original in 80 of them.
- 4.1 objects store the ECOFF header big-endian with f_flags 0x8000 and opthdr 0x38, like O.BIN.

So the original combines a 3.18 (5.x-generation) code generator with the older frame layout; stock IRIX 5.2 is not it. Hypothesis 2 below ("stock IDO 5.2") is now unlikely; hypothesis 1 (a non-IRIX 3.18 build) stays.

## Ranked hypotheses

1. **MIPS/SGI ucode suite release 3.18 (IDO 5.2-generation back end), linked to ECOFF on a big-endian host** (probably Sony's NEWS-based PlayStation toolchain). Confidence about 60%. For the release number alone (3.18), about 80%, on the strength of the linker stamps.
2. The same 3.18 compiler as stock IDO 5.2 on IRIX, with the ECOFF link done by another 3.18 linker. About 20%.
3. An older RISCompiler/NEWS-OS 4.2.1R `ccom`-based compiler with a newer linker. About 15%: the stamps say 3.18.
4. IDO 5.3 with a hidden option or patch. Under 5%: the stamp is lower than 5.3's, and the options are exhausted.

## Update: selector register rule (T-5010)
[[matching-notes]], "Selector register rule (T-5010)": of the two explanations in rule 7 above, (b) fits the data and (a) does not. Variables that only one overlay or main file uses (overlay data, and main-bss runs used by one overlay only, such as SHUGAKU's `D_800CA2CC..D_800CA2EC`) keep `$v0` in 28 of 30 entry switches, shared game state is `$v1` in 258 of 270, and struct or array declarations do not change IDO's result. The reading is that the original did not promote variables defined in the compilation unit. A second, compiler-side rule: a global first touched after a call or branch is not promoted (135 of 155 call-first chains are `$v0`). `tools/cvt_pass.py` models the second rule; the first is written in C as a `FAKE` local copy.
