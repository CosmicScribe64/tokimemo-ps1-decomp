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
