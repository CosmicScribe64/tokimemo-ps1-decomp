# Tokimeki Memorial: PS1 decompilation and English patch

[![Build](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp)

This repository contains a completed, byte-matching C decompilation of *Tokimeki Memorial: Forever with You*
for PlayStation, an English patch covering text and artwork, and a browser patcher. The patch aims to stay
faithful to the original game's tone and visual style. It targets the Japanese
PlayStation the Best release (`SLPM-86053`).

[Patch your disc in your browser](https://cosmicscribe64.github.io/tokimemo-ps1-decomp/), or use the source,
English text and artwork, disc-extraction tools, and Docker build tools to work on the game yourself.

The boot executable and all 26 overlays (`CDROM/EXEDIR/*.EXN`) rebuild byte for byte, every
game function is written in C, and the matched code has readable names and comments. The PsyQ SDK libraries stay in
assembly. [wiki/status.md](wiki/status.md) has the final numbers and how they were verified.

**You need your own copy of the disc.** Original disc images, executables, extracted originals, and generated
disassembly are not included. `translations/` keeps English text records and translated artwork with their review
history ([translations/en/graphics/artwork](translations/en/graphics/artwork/README.md)).

This project is independent. It is not affiliated with or endorsed by Konami.

## English patch

[Patch your disc in your browser](https://cosmicscribe64.github.io/tokimemo-ps1-decomp/).

The patch translates dialogue and menus and redraws Japanese lettering in artwork, including signs and interface
graphics. The aim is for the English to feel at home in the original game, preserving its meaning, character voices
and visual style. The release has passed text audits, artwork review, and scripted emulator checks across
the main screens and selected events. This does not cover every possible route through the game.
[Report a problem](https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues) with a screenshot and where it happened.

Drag your original disc files onto the page, or use the file picker. It accepts BIN/CUE, CHD, combined BIN or IMG,
raw ISO images, and ZIP or 7z archives containing them. For loose tracks, select the CUE and all its BIN files together.
Use the Japanese PlayStation the Best release (`SLPM-86053`). Data-only ISOs lack the audio and movie sectors needed
for a complete disc, so the patcher will ask for a lossless copy.

The patcher unpacks and checks your files on your computer, applies the patch, and verifies the result. Your disc is
never uploaded. Save the patched data track and CUE together with the unchanged audio track. If the audio came from
an archive or combined image, the page provides that download too. Open the CUE in your emulator.

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
