# Tokimeki Memorial decompilation

[![Build](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)

This project is a matching decompilation of the Japanese "PlayStation the Best" release (`SLPM_86.053`), and an eventual English-translated Godot port of the game.

The boot executable and all 26 overlays (`CDROM/EXEDIR/*.EXN`) rebuild identically, and every game function is now
written in C. The source naming/readability phase is recorded as complete. Work now focuses on the Godot port,
fresh-install content generation, and actual gameplay verification. See [wiki/status.md](wiki/status.md) for the
current evidence and [ROADMAP.md](ROADMAP.md) for the broader plan.

**You need your own copy of the disc.** Original disc images, executables, extracted originals, and generated
disassembly are not included. Versioned English artwork edits and review history are backed up under
[translations/en/graphics/artwork](translations/en/graphics/artwork/README.md); they are not a complete runtime asset pack.

This project is independent. It is not affiliated with or endorsed by Konami.

## Roadmap

[ROADMAP.md](ROADMAP.md) lays out five phases, from a complete matching decompilation to an English-language Godot
port.

## Building

Everything runs in Docker, so Docker is the only thing you install. The image builds on first use.

1. Put your copy of the game in the `game/` folder (gitignored). BIN+CUE, CHD, ISO, or a `.zip` or `.7z` of those all
   work, in any subfolder. The build expects the PlayStation the Best release, with executable SHA-1
   `e823bd844a8f8fa4d05483b59c66bc54b8393b26` and a matching set of overlays. Other releases are recognised and
   refused with a message.
2. Configure and build. The build finds the disc, checks the release, and unpacks it into `disc/` for you:
   ```
   tools/docker.sh python3 configure.py
   tools/docker.sh ninja
   ```

The build splits the executables into assembly, compiles the C, links, and checks every binary against the original.
A good build prints `OK` once per binary. `tools/docker.sh ninja progress` shows how much is decompiled.

## Toolchain

The game code was built with an early MIPS compiler of the SGI IDO family, not with the GCC most PlayStation games
used. The build uses SGI IDO 5.3 plus a few small build steps that reproduce the original compiler's differences, each
one rule applied to every function. [wiki/toolchain.md](wiki/toolchain.md) has the details and the evidence.

## Documentation

The project's notes, tickets and research live in [`wiki/`](wiki/index.md), which opens in Obsidian. Start at
[wiki/index.md](wiki/index.md); [wiki/decompile-workflow.md](wiki/decompile-workflow.md) explains how to decompile a
function.

## Matching decompilation

A function counts as decompiled only when its compiled code is byte-identical to the original. CI checks that on
every push, along with the SHA-1 of every binary.

Names, types and comments do not change the bytes, so the match rule cannot check them. Treat them as hypotheses.
Many function names come from the symbol map in the game's own `O.BIN` developer build and are not yet confirmed.

## AI assistance

AI coding agents (Claude Code) wrote most of the code and documentation here, directed by the maintainer.

The English translation planned in the roadmap will also be made with AI as a placeholder for human translation. Human translators are
welcome and encouraged to contribute their own translations to replace the placeholders.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md). Bug reports and decompiled functions are welcome. Agents and humans follow the
same rules, in [CODING_STANDARDS.md](CODING_STANDARDS.md) and [AGENTS.md](AGENTS.md).

## License

The project's own code, tools and documentation are released under [CC0 1.0 Universal](LICENSE), a public-domain
dedication. That covers our work only. It does not cover the game, its executables, its data or its assets, which
belong to their rights holders, and none of them are included here.

## Restoring the Godot port after a fresh clone

See [the container-only content setup guide](tools/port/SETUP.md). It accepts the original two-track 7z and regenerates the source evidence required by exporters. The current output is a partial developer bundle; native release and full fresh-install readiness remain under verification.
