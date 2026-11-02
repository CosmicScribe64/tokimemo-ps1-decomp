# Tokimeki Memorial decompilation

[![Build](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)

This project is a matching decompilation of the Japanese "PlayStation the Best" release (`SLPM_86.053`).

The decompilation is complete. The boot executable and all 26 overlays (`CDROM/EXEDIR/*.EXN`) rebuild byte for byte, every
game function is written in C, and the matched code has readable names and comments. The PsyQ SDK libraries stay in
assembly. [wiki/status.md](wiki/status.md) has the final numbers and how they were verified.

**You need your own copy of the disc.** Original disc images, executables, extracted originals, and generated
disassembly are not included. `translations/` keeps English text records and translated artwork with their review
history ([translations/en/graphics/artwork](translations/en/graphics/artwork/README.md)). An English patch is available
as a review build.

This project is independent. It is not affiliated with or endorsed by Konami.

## English patch

[Patch your disc in your browser](https://cosmicscribe64.github.io/tokimemo-ps1-decomp/).

The patch translates dialogue, menus and selected artwork. Text and visual checks are still in progress, so expect
some rough edges. [Report a problem](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues) with a screenshot
and where it happened.

Use the two original BIN tracks from the Japanese PlayStation the Best release (`SLPM-86053`). The patcher checks
your files, applies the patch on your computer and verifies the result. Your disc is never uploaded. It downloads
a patched data track and a CUE file; keep those beside your unchanged audio track and open the CUE in your emulator.

For the source build, see [the English build notes](wiki/english-build.md). The [patcher source](patcher/) and
[packaging tool](tools/patch/package_patcher.py) are included here.

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

A function counts as decompiled only when its compiled code is byte-identical to the original. CI checks the SHA-1 of
every binary on every push.

Names, types and comments do not change the bytes, so the match rule cannot check them. Treat them as hypotheses.
Many function names come from the symbol map in the game's own `O.BIN` developer build, and some are unconfirmed.

## AI assistance

AI coding agents (Claude Code and Codex) wrote most of the code and documentation here, directed by the maintainer. The English
records in `translations/` were also produced with AI.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md). Bug reports and corrections are welcome. Agents and humans follow the
same rules, in [CODING_STANDARDS.md](CODING_STANDARDS.md) and [AGENTS.md](AGENTS.md).

## License

The project's own code, tools and documentation are released under [CC0 1.0 Universal](LICENSE), a public-domain
dedication. That covers our work only. It does not cover the game, its executables, its data or its assets, which
belong to their rights holders, and none of them are included here.
