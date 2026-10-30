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

The development desktop exporter uses the same `tokimemo-port` engine and `tokimemo-content` setup images. Overrides are `GODOT_PORT_IMAGE` and `TOKIMEMO_CONTENT_IMAGE`. macOS export is not yet a supported player distribution: native audio-library packaging and actual platform acceptance remain open in `wiki/port-adversarial-review.md`.

To preserve an earlier bundle while rebuilding after a tool update, pass `--output .asset-cache/port/installed-retry` after the archive path. The new destination must not exist and must stay below `.asset-cache/port`; a second checkout is unnecessary. Original disc inputs are reused and verified.
