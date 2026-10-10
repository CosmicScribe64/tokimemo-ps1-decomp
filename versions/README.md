# Game versions

Put any dumps of the game here, one folder per release, for example
`versions/SLPM-86053-best/` or `versions/SLPS-00061-v1.0/`. Git ignores everything in
this folder except this file, so disc images never reach the repository.

Any common dump format is fine:

- **BIN + CUE**: a raw copy of every disc sector (`.bin`), plus a small text file (`.cue`)
  that lists the tracks. Multi-track discs, such as ones with CD audio, often come as
  several `.bin` files with one `.cue`. This is the most complete format.
- **CHD**: the same raw data as BIN + CUE, compressed (MAME's format). It converts back to
  BIN + CUE without loss.
- **ISO**: only the data track, with 2048-byte sectors. It loses CD audio and the extra
  sector data that XA audio and video streams use, so it is less complete.

The build reads only the executables, so it doesn't matter which format a version comes
in. Supporting several versions is future work. The build currently targets one release:
`SLPM_86.053`, the Japanese "PlayStation the Best" disc, checked by
`config/SLPM_86.053.sha1`.
