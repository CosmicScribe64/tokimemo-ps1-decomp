# Tokimeki Memorial decompilation

[![Build](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)

*Tokimeki Memorial: Forever with You* is Konami's high-school dating sim for the PlayStation. This project is a
matching decompilation of the Japanese "PlayStation the Best" release (`SLPM_86.053`).

The boot executable and all 26 overlays (`CDROM/EXEDIR/*.EXN`) already rebuild identically. Most functions are still
assembly that the build pulls in from generated files, so the C is at an early stage. See [ROADMAP.md](ROADMAP.md) for where this project is going.

**This repository contains no game data.** You need your own copy of the disc. The build reads the executables from
it and nothing else is shipped here. The disassembly is not in the repository either. You generate it locally.

This project is independent. It is not affiliated with or endorsed by Konami.

## Building

Everything runs in Docker, so Docker is the only thing you install. The image is linux/amd64 and builds on first
use. On Apple Silicon it runs under emulation, which is slow but works.

1. Put your Redump-style zip of the game in the repository root. The build expects the executable with SHA-1
   `e823bd844a8f8fa4d05483b59c66bc54b8393b26` and a matching set of overlays.
2. Extract the disc into `disc/` (gitignored):
   ```
   tools/docker.sh python3 tools/extract_disc.py "Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best).zip" disc
   ```
3. Configure and build:
   ```
   tools/docker.sh python3 configure.py
   tools/docker.sh ninja
   ```

The build splits the executables into assembly, compiles the C, links, and checks the SHA-1 of the main executable and
of every overlay. A good build prints `OK` 27 times. `ninja overlays` builds only the overlays.

```
tools/docker.sh ninja progress                       # progress table
tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX   # diff one function against the original
tools/docker.sh python3 tools/list_leaves.py         # remaining leaf functions
```

## Toolchain

The original compiler is unknown. The closest match found is SGI IDO 5.3, a MIPS compiler that the PsyQ SDK did not
use, run with `-O2 -G 0` and the optimizer option `-Wo,-no_const_in_reg`. Two build steps adjust its output to the
original's layout. Each applies one rule to every function and has unit tests:

- `tools/frame_pass.py` runs inside every IDO compile. The original stack frames are 16 bytes larger than IDO's,
  and the pass adds those bytes, so the C stays ordinary.
- `tools/cc.py` pads each object's code to 16 bytes, the alignment the original objects have.

Separately, `tools/asm.py` re-encodes Japanese string literals to Shift-JIS before assembling, because splat writes
them as UTF-8 for readability.

SDK library code is assembly for now. If it moves to C, the repository already has a GCC 2.7 plus maspsx path for it.
Details and the evidence behind each choice are in [wiki/toolchain.md](wiki/toolchain.md),
[wiki/compiler-mismatch-research.md](wiki/compiler-mismatch-research.md) and
[wiki/build-system.md](wiki/build-system.md).

## Layout

| Path | What is there |
|---|---|
| `src/main/` | C for the main executable, one file per original object (named by address) |
| `src/ovl/` | C for the 26 overlays |
| `include/` | headers, including per-overlay headers in `include/ovl/` |
| `config/` | splat configs, symbol files, expected SHA-1 files |
| `tools/` | Docker image, build helpers, progress and diff tools |
| `wiki/` | project knowledge base, tickets and the kanban board |
| `asm/`, `build/`, `disc/`, `expected/` | generated or game-derived, gitignored |

The wiki opens in Obsidian. Start at [wiki/index.md](wiki/index.md). File boundaries and the evidence for them are
in [wiki/source-files.md](wiki/source-files.md), overlays in [wiki/overlays.md](wiki/overlays.md).

## Matching Decompilation

A function counts as decompiled only when its compiled code is byte-identical to the original. CI checks that on
every push, along with the SHA-1 of every binary.

Names, types and comments do not change the bytes, so the match rule cannot check them. Treat them as hypotheses.
Many function names come from the symbol map in the game's own `O.BIN` developer build; they are listed in
`config/symbol_addrs_obin.txt`. In a spot check of 24 of them, 12 fit the functions they call, none contradicted the code, and the rest could not be checked.

## Roadmap

[ROADMAP.md](ROADMAP.md) lays out five phases, from a complete matching decompilation to an English-language Godot
port.

## AI assistance

AI coding agents (Claude Code) wrote most of the code and documentation here, directed by the maintainer.

The English translation planned in the roadmap will also be made with AI as a placeholder for human translation. Human translators are
welcome and encouraged to provide their own translations to replace the placeholders.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md). Bug reports and decompiled functions are welcome. Agents and humans follow the
same rules, in [CODING_STANDARDS.md](CODING_STANDARDS.md) and [AGENTS.md](AGENTS.md).

## License

The project's own code, tools and documentation are released under [CC0 1.0 Universal](LICENSE), a public-domain
dedication. That covers our work only. It does not cover the game, its executables, its data or its assets, which
belong to their rights holders, and none of them are included here.
