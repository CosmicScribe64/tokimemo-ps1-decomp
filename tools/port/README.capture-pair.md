# Exact capture-pair checks (T-39008)

Run in the existing recovery Docker image with Pillow:

```sh
docker run --rm -v "$PWD:/work" -w /work tokimemo-recovery \
  python3 tools/port/compare_capture_pair.py build/emu/pair.json \
  --out build/emu/pair-report.json
```

Output must be new. Exit **0** means equality within the explicitly declared
scope and synchronization evidence; it is not full-game acceptance. Exit **1**
means observed differences, incompatible field domains or unverified
synchronization. Exit **2** means evidence is missing, malformed or changed.

The input JSON has schema `tokimemo.capture-pair.v1` and these required fields:

- `native` and `port`: `record` and `image` objects containing `path` and
  `sha256`. Paths are relative to the pair JSON, or absolute. `selector` is an
  explicit JSON path array (for example `["shots",0]` or `["first_school"]`).
- `port.image_sha_path`: path within the selected receipt to its image hash,
  such as `["native_frame","sha256"]`. `port.clock_paths` maps clock names to
  receipt paths, such as `{"frame_count":["frame_count"]}`.
- `native_map`: a `tokimemo.native-memory-map.v1` object with the source-proven
  fields needed for this comparison. See `native_memory_projection.py` and
  `config/port/name-entry-memory-projection.json`. Required dumps must exist
  in the selected shot; the tool never substitutes a nearby memory sample.
- `mappings`: records with unique `name`, `native_field`, `port_path`, source
  `meaning` and `compatibility` (`equivalent` or `incompatible`). Incompatible
  fields require a `reason` and block acceptance even if their numeric values
  happen to agree. For example, title-inclusive and dialogue-only glyph counts
  are incompatible domains, not comparable totals.
  Equivalent values require exact JSON types: native integer fields require
  integers, rejecting booleans and even integral floats such as `21.0`; native
  hex fields require strings. There is no implicit numeric conversion.
- `checkpoint`: descriptive `name` and `expected` mapping names/values. Every
  expected field must have an equivalent mapping and match both observations.
- `synchronization`: `status` (`verified` or `unverified`) and explanatory
  `basis`. Verified synchronization additionally requires an `evidence` path/hash
  object. A reviewer must establish that evidence's meaning: hashing a claim
  does not prove it. Raw native frame/cycles/phase/lifecycle and selected port
  clocks remain in the report without alignment or time-shifting.
- `scope`: an honest description of the bounded comparison.

Current emulator shots must have `image_status: "ok"`, matching `png` and
`png_sha256`. Legacy shots need an explicit `native.legacy_image_validation`
description; they can produce diagnostics but remain blocked because the
original conversion receipt is unavailable. Pre-restore lifecycle also blocks
verified synchronization.

Use actual renderer images at source resolution. Different dimensions fail;
there is no scaling, cropping, title/checker masking, phase search or tolerance.
Comparison counts exact decoded RGBA differences, including alpha and outer
edges, and lists changed 16×16 tiles. These counts complement the existing
`verify_tile_diff.py` MAE diagnostics; this gate never calls its normalization
function. Viewport captures and montages remain separate presentation artifacts.

Tests use explicitly synthetic fixtures. Identity passes only that fixture;
single edge/alpha changes fail, as do missing fields, changed hashes and unbound
images. Run `python3 -m unittest tools.port.test_compare_capture_pair` in Docker.

## Rei opening stages 8-10 (`rei_stage_capture_pairs.py`)

`tools/port/rei_stage_capture_pairs.py` writes one `tokimemo.capture-pair.v1`
manifest per port observer key `stageS-K` (from
`tests/opening/run_installed_rei_banner.gd`). It needs an emulator run with
`--sample-every 1` and the dumps `800E7374:32 8014D180:12 801220E0:4
80129628:2`, plus shots at the chosen vsyncs. Rules:

- Native iteration F is the first sample whose post-update RAM holds the key.
  The previous sample must be frame_count F-1. Every PC in the window must be
  inside VSync (0x800AD950-0x800ADB30).
- The image is taken from the shot at frame_count F+`--latency`. Every vsync
  in F..F+latency must advance frame_count by exactly 1. Any other window is
  reported `unsynchronized` and gets no manifest.
- If the port directory holds `checkpoint-receipts.json` (receipt v2, written
  by `tests/support/checkpoint_observer.gd`, T-39019), the pairs point at those
  receipts directly. Their PNG hashes, clocks and observer hashes were bound when
  each frame was written. The tool re-checks every binding with
  `tools/port/checkpoint_receipt.py` (`receipt_sha256` recomputed over all
  other fields, PNG file hash, decoded RGBA8 hash) and stops if one is broken,
  and it writes no `port-png-bindings.json`.
- An older receipt has no PNG hashes, so the tool writes
  `port-png-bindings.json`, which copies each record verbatim and labels the
  hash it adds as post-hoc. It also pins the `--observer` script hash, and
  sets `modified_after_receipt` when that file is newer than the receipt.
- If the receipt is a stop record (`message` plus `state.frame_count`), a key
  recorded at the stop frame is reported `port_failure_frame` and gets no
  manifest. Its native window is still reported.
- The prologue_line mapping is `incompatible`, with a reason, only when the
  native line changes inside the window.
- Synchronization stays `unverified`. `sync-evidence.json` records each window,
  the onset latency probe and per-key iteration/frame durations.

Then run `compare_capture_pair.py` on each manifest. The tests use synthetic
data only: `python3 -m unittest tools.port.test_rei_stage_capture_pairs`.

## Colour-domain projection report (`rgb555_diagnostic.py`, T-39020)

`compare_capture_pair.py` stays unchanged and is the only exact RGBA gate.
`rgb555_diagnostic.py` is a diagnostic. It also does the residual bookkeeping
for the differential suite (`run_differential_suite.py`, T-39021). On its own
it is never parity acceptance, and every report carries `"acceptance": false`.

```sh
docker run --rm -v "$PWD:/work:ro" -w /work tokimemo-recovery \
  python3 tools/port/rgb555_diagnostic.py native.png port.png \
  --native-encoding zero_extended --port-encoding source_proven_mixed \
  --basis "T-39008: native low 3 bits zero; port mixes v<<3 and bit replication" \
  --exclude emulator_x0_column --exclude emulator_y0_row
```

The caller declares each side's 8-bit expansion from source:
`zero_extended` (v<<3), `bit_replicated` ((v<<3)|(v>>2)) or
`source_proven_mixed` (either form, per channel). A channel value outside the
declared encoding is not rounded. The report is `rejected` (exit 2) with
per-side counts and the first offending pixels. There is no tolerance, masking
or phase search.

The report (`tokimemo.rgb555-projection-report.v1`) contains:

- the declared encodings, the basis and the sha256 of each input file and of
  its decoded RGBA bytes;
- `exact_rgba8`: the changed-pixel count, bbox and 16×16 tile counts;
- `rgb555_residual`: the count, bbox, tile counts and a delta histogram of the
  pixels whose 5-bit codes differ;
- `domain_exclusions`: one row per named exclusion, with its own pixel,
  exact-change and residual counts. The only names are `emulator_x0_column` and
  `emulator_y0_row` (T-39008 C1). Excluded pixels are counted, never dropped
  silently.

A bbox is inclusive `[x0, y0, x1, y1]`. The report goes to stdout, or with
`--out` to a new file. Tests: `tools/port/run_playtest_unit_tests.sh
tools.port.test_rgb555_diagnostic`.

The port mixes colour expansions. Presenters use v<<3 in calendar, date,
event and ending, and bit replication in `first_arrival_actor_renderer.gd`.
This is recorded as finding T-39020-F1 in the checkpoint catalog. No presenter
changes until the user picks a rule.
## Long-route driver (input-script v2)

One deterministic input script plays the installed port and the original game
from the first command select (golden set A) for up to three years and pairs
their checkpoints by id.

- Strategy to script: `python3 -I tools/port/route_strategy.py
  config/port/route-strategies/three_year_default.json --out
  config/port/input-scripts/three_year_default.json` (add `--continuous` for
  the variant without `save_relaunch` segments, used for goldens that must run
  past them). `tools/emu/route_script.py validate|expand|preflight|simulate|verdict`
  checks a script, unrolls `repeat`, lists the pinned stops (with their
  segments), the unpinned `known_gaps` and `unsampled_ok`, re-runs the shared
  boundary rule on a recorded `route-trace.jsonl`, and recomputes a run's
  pass/fail from its `route.json`.
- Both sides: `sh tools/port/run_route_driver.sh <new-out> <script> --both`
  (from a worktree set `TOKIMEMO_ROOT` to the main checkout). `--port` with
  `ROUTE_NATIVE=<existing native run>` reuses a golden instead of rerunning the
  emulator; `--emu` captures the original only.
- Port side alone: `ROUTE_OBSERVER=long ROUTE_SCRIPT=<script>
  sh tools/port/run_installed_route.sh <new-out> <bundle> <so> <bridge>`. It
  passes only when the script completes or a port `unsupported` row pins the
  stop: its `stop` equals the stop text and its `at_segment` is the segment
  the player was on. The same text at another segment (another week) fails,
  and a new stop fails until the script pins it. The emulator player applies
  the same rule to native rows.
- Missed presses: a press absent from the pad mask the game sampled for its
  iteration stops the run (input slip). The only exception is a pulse press
  after a boundary state listed in the script's `unsampled_ok` (loading states
  where the original was seen to skip the pad read, each with its run and
  frame as evidence). That press is recorded in `route.json` `unsampled`.
- Emulator side alone: `tools/emu/route_driver/run.sh <new-out> <script>
  [vsyncs]` (starts from `newgame-golden-v1/states/setA/stage20-entry.sstate`
  so it observes the switch into the first 0x34/1/0 as the port does).
- Compare: `python3 -I tools/port/compare_route_checkpoints.py <port-out>
  <native-run> --json report.json`. Per checkpoint: state fields through the
  `compare_newgame_golden.py` projection, the first differing field, the
  frame_count offset (native minus port) and its drift since the first pair
  (n/a with a single pair), and exact pixel equality. It also matches the
  port's `unsampled` presses to the native run's by checkpoint interval and
  button, and fails on a port-only one.
- Tests (synthetic, T0): `python3 -I -m unittest discover -s tools/emu -p
  'test_route_script.py'`, `python3 -I -m unittest discover -s tools/port -p
  'test_compare_route_checkpoints.py'` (one class runs the real projection on
  the synthetic `tools/port/fixtures/route_compare_state.json`), and `godot --headless --path
  port/godot --script res://tests/route/run_route_script_player_tests.gd`
  (GDScript player parity with the Python rule).
