# Opening audio source checkpoint

These are the exact reviewed V2 native sources used by the Linux arm64 opening-audio extension in `port/godot/bin/`. No original game data is embedded. `PROVENANCE.md` records the source, dependency and build qualification; `GODOT_CPP_LICENSE.md` accompanies the linked bindings.

Build in an ignored writable copy, never in this source directory. Restore the pinned godot-cpp and SCons dependencies described by `dependency-provenance.json`, then run the Docker build described in `PROVENANCE.md`. SConstruct expects `deps/godot-cpp` and writes `project/bin/`; copy only the verified resulting library into the runtime.

This checkpoint qualifies the bounded Linux opening scene. Editor import, macOS builds, device playback and full-game audio remain unverified. The separately reviewed warm-bank successor is still isolated and is not included here.
