---
type: research
updated: 2026-10-09
ticket: T-0015
---

# Compiler mismatch research (T-0015)

Question: every non-leaf frame in the original is 16 bytes larger than IDO 5.3/7.1 produce (see [[matching-notes]], "Frame size", and [[toolchain]]). What compiler built the game, and how do other matching decomps treat a systematic compiler difference? URLs for every claim are in the raw note `raw/compiler-mismatch-research-sources.md`; this page is the synthesis. Not legal advice.

## 1. What compilers early Japanese PS1 developers used

- Official path: Sony/SN Systems PsyQ, gcc based (gcc 2.5.7 to 2.95 across SDK versions). Konami titles identified by decomps all use it: Vandal Hearts gcc 2.6.3 `-O1` (2.5.7 for audio), Castlevania SotN gcc 2.6.x, Silent Hill gcc 2.7.2-cdk / 2.8.1 `-O2`, Metal Gear Solid built with the original PsyQ binaries. Our game's code (IDO-like cfe/uopt/ugen output, `-O2`, no `$gp`) does not look like any of them, so it is an outlier even among Konami games.
- Before the PC kit, Sony supplied NEWS workstations (MIPS R3000 NEWS-OS 4.x RISC) as PS1 dev machines; Sony dropped the plan at CES 1994 in favour of SN Systems' PC tools. The NEWS-OS 4.2.1R `cc` is documented as "the RISC NEWS ucode C compiler" (MIPS RISCompiler family, same passes as IDO: uopt, ugen, as0, as1, `-EL`, `-mips1`, `-O2`). So a Japanese developer with a NEWS could cross-compile little-endian R3000 code with a ucode compiler. No source says any shipped PS1 game did. Hypothesis only.
- Ultrix 4.x `cc` is the same ucode family and also supports `-EL`/`-mips1`/`-O2`.
- No PS1 game built with IDO or another ucode compiler was found (Evo's Space Adventures: IDO on N64, gcc on PS1). Absence of evidence only; PS1 decomp coverage is thin and Suikoden has no decomp I could find.
- Metrowerks CodeWarrior also targeted PS1 (not ucode family; unverified, source page failed to load).

## 2. Which compilers reserve 16 extra bytes

Nothing found. gcc 2.8.1's mips frame code allocates 4 words only for outgoing arguments of non-leaf functions (no extra block, no leaf area), consistent with gcc being ruled out. No documentation compares the frame layout of IDO 4.x/5.0/5.1, Ultrix cc 2.x/3.x or NEWS-OS cc with IDO 5.3. Whether any of them gives "+16 between saves and locals in non-leaf, +16 below saves in leaves" is untested; the T-0014 observation (leaves get an argument area as if ugen lacked the leaf optimization) fits an older ugen but remains a guess.

## 3. How other decomps handle systematic differences

- maspsx rewrites gcc's assembly so GNU `as` reproduces PsyQ's ASPSX output, with a per-ASPSX-version behaviour table. It exists because original tools are DOS/Windows binaries and decomp tooling wants ELF; it is described as reproducing the original toolchain, not as a hack. Used by SotN, Silent Hill, ESA, Croc, Soul Reaver and others.
- asm-processor post-processes IDO objects to splice assembly; ido-static-recomp replaced qemu-irix for running IDO natively; old-gcc patches gcc source to make `mips-sony-psx` targets. All three are accepted infrastructure.
- Match vocabulary in docs (zeldaret, papermario): `NON_MATCHING` (equivalent, bytes differ) and `NON_EQUIVALENT`. Reviewers require equivalent behaviour. Plausible developer-style tricks (temps, extra stack variable) are fine. I found no written definition of "fakematch" in zeldaret/decompals/decomp.me docs; the only one is this project's own `CODING_STANDARDS.md` section 7: right bytes, C the author would not plausibly write.
- My reading (inference, not guidance): a pass is toolchain emulation if it models one deterministic, documented behaviour of the original compiler, runs on every function with no per-function switches, and leaves the C source ordinary. Per-function dummy variables or asm that exist only to hit bytes are fakematches. A global frame rewrite fits the first category, but only if it reproduces both leaf and non-leaf layouts from one rule.

## 4. Licensing, as projects describe it

- ido-static-recomp: repo has no license file; the IDO 5.3 directory carries the SGI Freeware Legal Notice (1995) copied from an archived SGI page, 7.1 carries none; README is silent. Provenance of the binaries unverified.
- qemu-irix: QEMU patch; user supplies an IRIX root filesystem (README).
- PsyQ: FoxdieTeam/psyq_sdk hosts PsyQ 4.3-4.5 and ASPSX publicly with no license; other projects tell users to supply ASPSX/PsyQ themselves. Sony's position not found.
- Ultrix, NEWS-OS, IRIX images: no project statement found; terms unverified. Obtaining them is a user decision ([[tickets/T-0100-older-mips-compiler-emulation]]).

## Ranked recommendation

1. Build a maspsx-style, documented emulation stage in `tools/cc.py` that reproduces the frame rule for every function, not just non-leaf (rule: non-leaf = extra 16 between saves and locals; leaf with frame = extra 16 below saves; frameless leaves unchanged). Best insertion point is the ucode stream between `uopt` and `ugen` (the T-0014 `DEF Mmt` experiment), plus whatever ugen-side rule gives the leaf layout, or as a fallback an `as1`-asm rewrite of `.frame`/`.mask`/sp offsets (the 16 bytes are never accessed). Gate: every already-matched function plus the four named in T-0100 must still match, one rule, no function lists. Document it in [[toolchain]] as emulation of an unidentified older ucode compiler, and keep it switchable so a real compiler can replace it. This is the cheapest path to matching non-leaf functions and has direct precedent (maspsx, asm-processor, old-gcc patches). Risk: reviewers could still call it a fake if the leaf rule needs special-casing; record that in the ticket.
2. In parallel, time-box the older-compiler test ([[tickets/T-0100-older-mips-compiler-emulation]]): Ultrix 4.x cc (little-endian R3000 target, gxemul-class emulation) or NEWS-OS cc. Needs the user's decision on obtaining OS images and their licensing. If one produces +16, it replaces rank 1 and settles the question; if not, it still narrows the family.
3. Until either works: keep decompiling frameless leaves only, and park framed functions as `INCLUDE_ASM`.

Open: does any IDO before 5.3 (4.x, 5.0, 5.1) already do this? A comparison from someone with older IDO binaries would be the cheapest evidence.
