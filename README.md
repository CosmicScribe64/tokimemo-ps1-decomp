# Tokimeki Memorial decompilation

[![Build](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)

*Tokimeki Memorial: Forever with You* is Konami's high-school dating sim for the PlayStation. This project is a
matching decompilation of the Japanese "PlayStation the Best" release (`SLPM_86.053`). It is C that compiles back to
the original machine code, byte for byte.

The boot executable and all 26 overlays (`CDROM/EXEDIR/*.EXN`) already rebuild identically. Most functions are still
assembly that the build pulls in from generated files, so the C is at an early stage. See [Status](#status) for numbers
and [ROADMAP.md](ROADMAP.md) for where this is going.

**This repository contains no game data.** You need your own copy of the disc. The build reads the executables from
it and nothing else is shipped here. The disassembly is not in the repository either. You generate it locally.

This project is independent. It is not affiliated with or endorsed by Konami.

## Status

Numbers from `tools/docker.sh ninja progress` on 2026-10-09:

| Part | Functions | Bytes |
|---|---|---|
| Main executable | 75 of 834 | 2,432 of 284,428 |
| Overlays (26) | 76 of 6,128 | 5,700 of 1,994,940 |
| Total | 151 of 6,962 | 8,132 of 2,279,368 (0.4%) |

The PsyQ SDK library code (722 functions, 165,212 bytes) sits in the executable as assembly and is counted
separately. It is not part of the totals above. [decomp.dev](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
tracks the same progress from CI.

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
use. Two extra steps make its output line up with the original:

- **Frame pass.** The original stack frames are 16 bytes larger than IDO's. `tools/frame_pass.py` runs inside every
  IDO compile and adds the extra bytes, so the C stays ordinary. It has unit tests.
- **Shift-JIS strings.** `tools/asm.py` re-encodes Japanese string literals to Shift-JIS escapes before assembling,
  because splat writes them as UTF-8 for readability.

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

## AI assistance

AI coding agents (Claude Code) wrote most of the code and documentation here, directed by the maintainer.

What protects the result is the match rule. A function counts as decompiled only when its compiled code is
byte-identical to the original, and CI checks that on every push, along with the SHA-1 of the whole build.

The match rule says nothing about names, types or comments, because they do not change the bytes. Treat them as
hypotheses. In particular, many function names come from the symbol map in the game's own `O.BIN` developer build and
are listed as hypotheses in `config/symbol_addrs_obin.txt`.

The English translation planned in the roadmap will also be made with AI, and will say so. Human translators are
welcome to take part.

Some decompilation projects ban AI-generated contributions. This one is independent of them and has no connection
to their work.

## Roadmap

[ROADMAP.md](ROADMAP.md) lays out five phases, from a complete matching decompilation to an English-language Godot
port.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md). Bug reports and decompiled functions are welcome. Agents and humans follow the
same rules, in [CODING_STANDARDS.md](CODING_STANDARDS.md) and [AGENTS.md](AGENTS.md).

## License

The project's own code, tools and documentation are released under [CC0 1.0 Universal](LICENSE), a public-domain
dedication. That covers our work only. It does not cover the game, its executables, its data or its assets, which
belong to their rights holders, and none of them are included here.
