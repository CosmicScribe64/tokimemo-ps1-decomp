# Fresh-clone content setup

Start Docker Desktop, then run from the repository root:

```sh
bash tools/port/setup_content.sh '/absolute/path/to/Tokimeki Memorial.7z'
```

This builds container-only Python, media and pinned splat tools. It mounts your archive read-only, extracts the disc tracks into ignored `disc/`, identifies the release, extracts its filesystem, and regenerates assembly evidence from the main executable and overlays. The content exporters require this evidence even though it is not committed. No host Python packages or legacy compiler toolchain are needed.

The intended input is the two-track Japanese PlayStation the Best 7z. Other layouts fail rather than silently selecting a track. Existing `disc/` is reused and checked; an interrupted extraction may require inspecting that directory before retrying. Existing installed bundles are never overwritten. The destination is `.asset-cache/port/installed-fresh/manifest.json`.

**Setup currently produces a partial developer bundle, not a playable release.** Inspect `coverage.unavailable_roles` and the warnings. Spring media, the HUD image manifest, and finite April school-day command-0/7 media regenerate from the original inputs and source proofs. School-day coverage does not yet establish complete school gameplay. HUD session context and holiday scene setup still lack fresh producers; any additional export failure is reported in the generated coverage record. The setup wrapper exits 3 when coverage is partial, preserving the bundle for diagnosis; the lower-level exporter exits 0 after publication even for a partial bundle. No provenance check should be bypassed to make a role appear available.

## English bundle

After the installed bundle, setup builds the English bundle in the same container from the committed `translations/en`. It writes the bundle into the new installed profile at `language/en/english_bundle.json` (for example `.asset-cache/port/installed-fresh/language/en`), which is where the Language setting looks for it. Choose another new directory with `--english-output .asset-cache/port/NEW_DIRECTORY`, or skip the bundle with `--no-english`. It never writes into the repository or `res://`, or into an existing bundle.

`english_bundle.json` (schema `tokimemo.english-bundle.v1`, read by `port/godot/services/language_bundle_service.gd`) carries the disclosure and lists each member with its relative path, SHA-256, schema, disclosure and coverage counts:

- `ui_text`: the source-keyed text layer pack (`tools/port/build_text_pack.py`);
- `dialogue`: the prologue dialogue pack (`tokimemo.text-translation-pack.v1`). A line stays Japanese when its source bytes hold a name token or another single-byte control, a protected marker, a character without an original glyph, or text that would not fit the two English rows of the original textbox (`layout_overflow`, listed in `layout_overflow_line_ids`). The `lines` count is the number of lines the player sees in English;
- `images`: the approved artwork pack (`tokimemo.translated-image-pack.v1` while it holds only frame replacements), approved assets only;
- `draft_images` (only with `--with-drafts`): a development pack of the approved assets plus unreviewed drafts (`v2` when it holds page or BS drafts), opened only when the player enables draft art.

The disclosure reads: "English text is AI-translated and not yet human-reviewed; voices remain Japanese." Nothing in the bundle is human approval. A missing input (no extracted files, no installed prologue text) omits that member and records it with a reason in `omitted`; an input that is present but differs from its pin fails the build. Setup then exits 4 and leaves the Japanese content as built. Two builds from the same inputs are byte-identical. To build or re-check a bundle by hand:

```sh
docker run --rm -v "$PWD:/work" -w /work tokimemo-content python3 tools/port/build_english_bundle.py \
  --files disc/files --installed .asset-cache/port/installed-fresh --output .asset-cache/port/english-NEW
docker run --rm -v "$PWD:/work" -w /work tokimemo-content python3 tools/port/build_english_bundle.py \
  --verify --output .asset-cache/port/english-NEW
```

Draft page/bs directories from `tools/translation_filesel_drafts.py pages` and `tools/translation_mdec_drafts.py` can be added to the draft pack with `--with-drafts --draft-source DIR`.

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
