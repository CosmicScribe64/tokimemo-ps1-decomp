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
