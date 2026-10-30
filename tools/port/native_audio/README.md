# Native original-audio backend

These sources build the Linux arm64 original-audio extension. No original game data is embedded. `PROVENANCE.md` records the historical V2 source/dependency qualification; current source changes and candidate builds require their own review and playback evidence. `GODOT_CPP_LICENSE.md` accompanies the linked bindings. The library in `port/godot/bin/` may lag an unpublished candidate; compare its hash with the accepted build receipt before testing.

Build a candidate from the repository root, using the compiler image created by the content setup workflow:

```sh
bash tools/port/native_audio/build.sh --output .asset-cache/port/native-audio-candidate
```

If that image is absent, build it with `docker build -t tokimemo-content -f tools/port/Dockerfile.content tools/port`. To reuse a compatible existing Linux arm64 image containing Python3.12+ and g++, set `TOKIMEMO_CONTENT_IMAGE`. Everything runs in Docker; no host compiler or Python packages are needed.

The output directory must be new and below `.asset-cache`. The builder downloads godot-cpp and SCons with TLS verification, checks the sizes and SHA-256 values in `dependency-provenance.json`, then copies the current native sources into the output. It records source/dependency identities and compiler version in `build-inputs.json`, logs SCons output to `build.log`, and records the result/library hash in `build-result.json`. Failed output is retained for diagnosis; retry with a different output directory. No original game data is needed to compile.

The library is written under the candidate's `project/bin/`. The command does not replace the runtime library. Independently review the candidate and test actual original-resource playback before publication. This command builds Linux arm64 only; it does not resolve the missing macOS backend or certify hardware audio behavior. `PROVENANCE.md` describes the historical accepted compiler/source checkpoint; a new compiler receipt is separate evidence, not a claim of identical output.

The historical checkpoint qualifies a bounded Linux opening scene. Rei bank loading and fade changes are under review, including actual PCM regressions. macOS builds, native device playback and full-game audio remain unverified. Tests using an isolated candidate must record its library hash; they do not validate the unchanged packaged library.
