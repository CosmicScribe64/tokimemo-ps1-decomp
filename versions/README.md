# Game versions

Put any dumps of the game here, one folder per release, for example
`versions/SLPM-86053-best/` or `versions/SLPS-00061-v1.0/`. Git ignores everything in
this folder except this file, so disc images never reach the repository.

Any common dump format works:

- BIN + CUE is a raw copy of every disc sector (`.bin`), plus a small text file (`.cue`)
  that lists the tracks. Multi-track discs, such as ones with CD audio, often come as
  several `.bin` files with one `.cue`. This is the most complete format.
- CHD holds the same raw data as BIN + CUE in MAME's compressed format. It converts back to
  BIN + CUE without loss.
- ISO holds only the data track, with 2048-byte sectors. It loses CD audio and the extra
  sector data that XA audio and video streams use, so it is less complete.

The build reads only the executables, so any of these formats works.
To build, put the supported release (PlayStation the Best) in `game/` instead; see `game/README.md`. Supporting several versions is future work. The build currently targets one release,
`SLPM_86.053`, the Japanese "PlayStation the Best" disc, checked by
`config/SLPM_86.053.sha1`.

The releases catalogued so far, their hashes, and how they differ from the target are in
`wiki/versions.md` and `config/versions.txt`; `tools/identify_version.py <image|folder|archive>` tells
you which one a disc is. It reads `.zip`, `.7z`, and `.chd` files directly.
