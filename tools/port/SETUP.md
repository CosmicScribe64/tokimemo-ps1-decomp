# Fresh-clone content setup

Start Docker Desktop, then run from the repository root:

```sh
bash tools/port/setup_content.sh '/absolute/path/to/Tokimeki Memorial.7z'
```

This builds container-only Python, media and pinned splat tools. It mounts your archive read-only, extracts the disc tracks into ignored `disc/`, identifies the release, extracts its filesystem, and regenerates assembly evidence from the main executable and overlays. The content exporters require this evidence even though it is not committed. No host Python packages or legacy compiler toolchain are needed.

The intended input is the two-track Japanese PlayStation the Best 7z. Other layouts fail rather than silently selecting a track. Existing `disc/` is reused and checked; an interrupted extraction may require inspecting that directory before retrying. Existing installed bundles are never overwritten. The destination is `.asset-cache/port/installed-fresh/manifest.json`.

**Setup currently produces a partial developer bundle, not a playable release.** Inspect `coverage.unavailable_roles` and the warnings. Spring media, the HUD image manifest, and finite April school-day command-0/7 media regenerate from the original inputs and source proofs. School-day coverage does not yet establish complete school gameplay. HUD session context and holiday scene setup still lack fresh producers; any additional export failure is reported in the generated coverage record. The setup wrapper exits 3 when coverage is partial, preserving the bundle for diagnosis; the lower-level exporter exits 0 after publication even for a partial bundle. No provenance check should be bypassed to make a role appear available.

Build and import the engine separately:

```sh
sh tools/port/run.sh build
docker run --rm -v "$PWD:/work" -w /work/port/godot tokimemo-port godot --headless --editor --import
sh tools/port/run.sh test
```

The engine image targets Linux arm64. Its tests do not certify native macOS packaging or audio devices. The core suite uses synthetic fixtures; a pass does not establish gameplay readiness. Connected-route tests additionally need original audio and complete content. Historical defaults pointing into `.claude/orchestrator/state/` do not survive cloning and are not valid fresh-install prerequisites.

T-39002 tracks remaining clean-install gaps and actual-route verification. Logs and original content must remain ignored; never commit generated disc or assembly data.

## Desktop export

All packaging uses images built from this repository; no maintainer cache or historical image name is needed:

```sh
sh tools/port/run.sh build           # tokimemo-port: Godot 4.7.2 + all export templates
sh tools/port/run.sh build-content   # tokimemo-content: Python/compiler image (no game archive needed)
sh tools/port/run.sh build-gui-test  # tokimemo-port-gui-test: X11/PulseAudio checks for verify_desktop.sh
sh tools/port/build_desktop.sh linux-arm64 player .asset-cache/port/desktop-linux-arm64
sh tools/port/verify_desktop.sh .asset-cache/port/desktop-linux-arm64 .asset-cache/port/desktop-verify .asset-cache/port/installed-fresh
```

`setup_content.sh` also builds `tokimemo-content`. Overrides are `GODOT_PORT_IMAGE`, `TOKIMEMO_CONTENT_IMAGE` and `TOKIMEMO_GUI_TEST_IMAGE`. A missing image or stopped Docker exits with the command that fixes it. Each output directory must be new and below `.asset-cache/port`.

`build_desktop.sh` stages project source only (`prepare_desktop.py` allowlist), never original game data, and writes `source-receipt.json`. The `player` scene keeps the production boot shell (`res://shell.tscn`): an empty install opens first-launch setup; installed content opens the New Game entry. `week` and `new-game` are development entry points.

Every export is made twice from the same staged project and the build fails unless both hash the same (`player-contents.sha256`; for the macOS zip the entries are compared, because the zip stores export-time dates). Staging rejects any `.tscn` node without a stored `unique_id`: the pinned engine assigns random IDs to such nodes at export. Re-save the scene in the pinned Godot editor to add them.

- `linux-arm64`: unsigned release export in `player/` (executable, `.pck`, and the reviewed `liboriginal_audio` library beside the executable). The library and its descriptor are staged only if their SHA-256 values equal the pins in `port/godot/services/first_arrival_bgm_bridge.gd`; any other shared library is rejected. Hashes are in `player.sha256`.
- `linux-x86_64`: refused. Templates exist in the image, but no reviewed x86_64 audio library exists, and the BGM bridge accepts Linux arm64 only.
- `macos`: unsigned universal `player.zip`, code-only. No macOS audio library exists, so the BGM bridge rejects the platform and the opening stops at its BGM gate. This is not a supported player distribution. Building the library needs an Apple SDK in a cross toolchain (osxcross) or an Xcode host build, a pinned descriptor entry, signing, and real-device playback acceptance. None of these exist yet; see `wiki/tickets/T-39015-desktop-packaging.md`.

`verify_desktop.sh` is a thin Docker wrapper around `tools/port/desktop_verify/verify_player.py`, which runs in the `tokimemo-port-gui-test` image (`sh tools/port/run.sh build-gui-test`; the wrapper exits with that command if the image is missing). Every run uses a fresh per-user data directory. With no bundle, the exported Linux player must open first-launch setup, the native extension must load, and the BGM bridge's native-component gate (printed as `NativeAudioGate:` under `TOKIMEMO_ENTRY_DEBUG=1`) must accept the library beside the executable; a copy without the library must report the load error and a rejected gate. With a bundle from `setup_content.sh`, it also publishes the bundle through `InstalledPackStore.publish_stage` (the first-launch publication call), requires the player to open the New Game entry with no failed load gate, then always drives the opening with X11 Start presses under Xvfb/PulseAudio and requires recorded output with no stop. Key timing after the entry loads is blind, so a failure there needs log inspection. None of these checks establish gameplay readiness, hardware audio behavior or audio parity.

To preserve an earlier bundle while rebuilding after a tool update, pass `--output .asset-cache/port/installed-retry` after the archive path. The new destination must not exist and must stay below `.asset-cache/port`; a second checkout is unnecessary. Original disc inputs are reused and verified.

## Actual installed Rei route

`sh tools/port/run_installed_route.sh <new-out-dir> [bundle] [candidate.so] [bridge.gd]` replays the installed opening with physical X11 entry keys and the observer `port/godot/tests/opening/run_installed_rei_banner.gd`. The out dir must not exist. The script copies the bundle (default `.asset-cache/port/installed-runtime-v12`) into `<out>/content-install/.staging/prepared-original` and binds the isolated native audio candidate and its BGM bridge read-only over the tracked files, which stay unchanged. The default candidate must match SHA `d937156a…`; the library and bridge SHAs and the bundle manifest SHA are recorded in `<out>/candidate-hashes.txt`. `REI_TARGET_STAGE` (default 10) selects the stage whose first published frame ends the run; the observer requires every source substep of each stage from 8 up to, but not including, that stage. `TOKIMEMO_ROUTE_DEADLINE` (default 900 s here; the driver's own default stays 180 s, also settable with `--deadline`) bounds the run. Results: `rei-proof.json` on success, `rei-failure.json` with the stop message otherwise, plus one PNG per first frame of each substep.
