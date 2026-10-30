#!/usr/bin/env python3
"""T-39008: build exact capture-pair manifests for the opening Rei stages 8-10.

Inputs are an actual PCSX-Redux run (result.json + memory-samples.jsonl with
per-vsync dumps) and an actual port observer receipt (rei-failure.json with its
stageN-K PNGs). Nothing is resized, masked, shifted or invented:

* A port key ``stageS-K`` is the first published port frame whose post-update
  state has mode_stage S and substep K (tests/opening/run_installed_rei_banner.gd).
* The native iteration F is the first sampled vsync whose post-update RAM has
  game_mode 0x15, mode_stage S, mode_step 0 and substep K, with the previous
  sample at frame_count F-1 and both PCs inside VSync (func_800AD950 and its
  wait helper func_800ADA94, asm/libetc.s): the iteration is observed at its own
  vsync and is not a torn mid-iteration read.
* The native image of iteration F is taken at the vsync where frame_count is
  F+LATENCY, only when F..F+LATENCY are consecutive single-vsync iterations
  (each sample one vsync apart, frame_count +1, PC in VSync). LATENCY is a
  reviewed declaration (see --latency) checked against discrete onset events
  recorded by latency_probe; the comparator does not prove it. Windows that fail are reported unsynchronized
  and get no pair manifest.

The port observer did not bind PNG hashes into its receipt, so this tool writes
a separate binding file that copies each observed record verbatim, pins the
receipt hash and records the PNG hash computed here, labelled as a post-hoc
binding. The observer script that decodes the key (--observer) is hashed into
the binding too, with a flag when the file was modified after the receipt
(then the hash may not be the revision that produced the receipt). A key
recorded at the frame where a stopping receipt (``message`` + ``state``) failed
is rejected: that frame is the failure frame, not a completed checkpoint.
Synchronization is always emitted as ``unverified`` pending review.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path

VSYNC_LO, VSYNC_HI = 0x800AD950, 0x800ADB30
GAME = 0x800E7374
CHECKPOINTS = [(8, k) for k in range(8)] + [(9, k) for k in range(12)] + [(10, k) for k in range(9)]

NATIVE_MAP = {
    "schema": "tokimemo.native-memory-map.v1",
    "scope": "Opening mode 0x15 Rei stages 8-10; RAM sampled at the shot vsync (not the iteration that generated the image).",
    "fields": [
        {"name": "mode", "address": "0x800E7388", "type": "u8", "length": 1, "source": "include/game/game_state.h GameState+0x1108"},
        {"name": "stage", "address": "0x800E7389", "type": "u8", "length": 1, "source": "include/game/game_state.h GameState+0x1109"},
        {"name": "step", "address": "0x800E738A", "type": "u8", "length": 1, "source": "include/game/game_state.h GameState+0x110A"},
        {"name": "substep", "address": "0x800E738D", "type": "u8", "length": 1, "source": "include/game/game_state.h GameState+0x110D"},
        {"name": "native_frame_count", "address": "0x800E7378", "type": "u32", "length": 4, "source": "include/game/game_state.h GameState+0x10F8"},
        {"name": "prologue_line", "address": "0x8014D180", "type": "u8", "length": 1, "source": "config/symbols_NAME_ENT.txt prologue_line"},
        {"name": "prologue_scene_counter", "address": "0x8014D184", "type": "s32", "length": 4, "source": "config/symbols_NAME_ENT.txt prologue_scene_counter"},
        {"name": "D_801220E0", "address": "0x801220E0", "type": "u8", "length": 1, "source": "src/ovl/name_entry/prologue.c prologue_rei_banner_run"},
        {"name": "D_801220E1", "address": "0x801220E1", "type": "u8", "length": 1, "source": "src/ovl/name_entry/prologue.c prologue_rei_banner_run"},
        {"name": "D_801220E2", "address": "0x801220E2", "type": "u8", "length": 1, "source": "src/ovl/name_entry/prologue.c prologue_rei_banner_run"},
        {"name": "gs_buffer_index", "address": "0x80129628", "type": "s16", "length": 2, "source": "asm/libgs.s func_8009AC20 D_80129628"},
    ],
}


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def state(sample):
    dumps = {d["addr"]: bytes.fromhex(d["data"]) for d in sample["dumps"]}
    g = dumps[GAME]
    line = dumps[0x8014D180]
    bri = dumps[0x801220E0]
    return {"vsync": sample["frame"], "frame_count": int.from_bytes(g[4:8], "little"), "mode": g[0x14], "stage": g[0x15],
            "step": g[0x16], "substep": g[0x19], "prologue_line": line[0],
            "prologue_scene_counter": int.from_bytes(line[4:8], "little", signed=True), "D_801220E0": bri[0],
            "gs_buffer_index": int.from_bytes(dumps[0x80129628], "little", signed=True), "pc": hex(sample["pc"]),
            "pc_in_vsync": VSYNC_LO <= sample["pc"] < VSYNC_HI, "cycles": sample["cycles"],
            "state_load_phase": sample.get("state_load_phase")}


TEXT_REGION = (24, 166, 296, 230)   # message window interior, probe only (never a comparison mask)
SCENE_REGION = (0, 0, 320, 160)     # area above the HUD frame, probe only


def latency_probe(rows, result, native_dir, max_latency=4):
    """Discrete onset events with a known first executing iteration E:
    * line start: prologue_line_start (k_sub_reset) runs in the iteration whose post-update prologue_line
      first holds the new line (src/ovl/name_entry/prologue.c:84-104, stage cases calling it) -> text window clears;
    * stage8 white boxes: case 1 of prologue_rei_banner_run (prologue.c:1336-1346) first runs in the iteration
      after the one whose post-update substep becomes 1 -> scene area changes from flash to white boxes.
    For L=0..max_latency-1 the changed-pixel count between the shots at frame_count E+L-1 and E+L is recorded
    inside the probe region; the onset latency is the L with the largest change. Only consecutive single-vsync
    rows with PC in VSync are used. A low-level gray-ramp model was rejected as unreliable (it mixes the
    background brightness byte with the additive flash)."""
    from PIL import Image
    by_fc = {}
    for i, r in enumerate(rows):
        by_fc.setdefault(r["frame_count"], i)
    shots = {s["frame"]: s for s in result["shots"] if s.get("image_status") == "ok" and isinstance(s.get("frame"), int)}
    cache = {}

    def image(vsync):
        if vsync not in cache:
            with Image.open(native_dir / shots[vsync]["png"]) as im:
                cache[vsync] = im.convert("RGBA")
        return cache[vsync]

    def changed(v0, v1, box):
        a, b = image(v0).crop(box).tobytes(), image(v1).crop(box).tobytes()
        return sum(1 for i in range(0, len(a), 4) if a[i:i + 4] != b[i:i + 4])

    events = []
    for i in range(1, len(rows)):
        prev, cur = rows[i - 1], rows[i]
        if cur["frame_count"] != prev["frame_count"] + 1:
            continue
        if cur["prologue_line"] != prev["prologue_line"]:
            events.append(("line_start %d" % cur["prologue_line"], cur["frame_count"], TEXT_REGION))
        if (prev["stage"], prev["substep"], cur["stage"], cur["substep"]) == (8, 0, 8, 1):
            events.append(("stage8 white boxes first drawn", cur["frame_count"] + 1, SCENE_REGION))
    probes = []
    for name, start, box in events:
        idx = by_fc.get(start - 1)
        if idx is None:
            continue
        seq = rows[idx: idx + max_latency + 1]
        if (len(seq) != max_latency + 1 or any(not r["pc_in_vsync"] for r in seq)
                or any(b["frame_count"] != a["frame_count"] + 1 or b["vsync"] != a["vsync"] + 1 for a, b in zip(seq, seq[1:]))
                or any(r["vsync"] not in shots for r in seq)):
            probes.append({"event": name, "iteration": start, "status": "not_measurable"})
            continue
        # seq[k] is the shot at frame_count E-1+k, so a change between seq[k-1] and seq[k] first shows at E+(k-1)
        counts = {str(k - 1): changed(seq[k - 1]["vsync"], seq[k]["vsync"], box) for k in range(1, max_latency + 1)}
        best = max(counts, key=lambda k: counts[k])
        probes.append({"event": name, "iteration": start, "region": list(box),
                       "changed_pixels_first_visible_at_E_plus": counts, "onset_latency": int(best)})
    measured = sorted({p["onset_latency"] for p in probes if "onset_latency" in p})
    return {"method": latency_probe.__doc__, "events": probes, "observed_latencies": measured}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--native-dir", type=Path, required=True, help="emulator run directory (result.json, memory-samples.jsonl, shots)")
    parser.add_argument("--port-dir", type=Path, required=True, help="port observer directory (rei-failure.json, stageN-K.png)")
    parser.add_argument("--observer", type=Path, required=True,
                        help="observer script whose 'stage%%d-%%d' key format is decoded (tests/opening/run_installed_rei_banner.gd)")
    parser.add_argument("--out", type=Path, required=True, help="new output directory")
    parser.add_argument("--latency", type=int, required=True, help="declared vsync display latency (reviewed evidence)")
    parser.add_argument("--latency-basis", required=True)
    args = parser.parse_args(argv)
    args.out.mkdir(parents=False, exist_ok=False)
    result_path, samples_path = args.native_dir / "result.json", args.native_dir / "memory-samples.jsonl"
    result = json.loads(result_path.read_bytes())
    if sha(samples_path) != result["memory_samples"]["sha256"]:
        raise SystemExit("memory samples hash differs from result.json")
    rows = [state(json.loads(line)) for line in samples_path.read_bytes().splitlines()]
    rows = [r for r in rows if r["state_load_phase"] == "restored"]
    shot_index = {s["frame"]: i for i, s in enumerate(result["shots"]) if isinstance(s.get("frame"), int)}
    receipt_path = args.port_dir / "rei-failure.json"
    receipt = json.loads(receipt_path.read_bytes())
    rel = lambda p: os.path.relpath(p, args.out)
    observer = {"path": rel(args.observer), "sha256": sha(args.observer),
                "modified_after_receipt": args.observer.stat().st_mtime > receipt_path.stat().st_mtime,
                "note": "hash of the observer file at pairing time; when modified_after_receipt is true it may differ from the revision that wrote the receipt"}
    # A receipt with a message is a stop record; its state.frame_count is the frame at which the observer stopped.
    stop_frame = receipt.get("state", {}).get("frame_count") if "message" in receipt else None
    bindings = {"schema": "tokimemo.t39008.port-png-binding.v1",
                "binding_origin": "post-hoc: PNG hashes computed by tools/port/rei_stage_capture_pairs.py; the port observer did not hash its PNGs",
                "receipt": {"path": rel(receipt_path), "sha256": sha(receipt_path), "message": receipt.get("message"),
                            "stop_frame": stop_frame},
                "observer": observer, "observed": {}}
    windows, pairs = {}, []
    for stage, sub in CHECKPOINTS:
        key = "stage%d-%d" % (stage, sub)
        port_png = args.port_dir / (key + ".png")
        observed = receipt["observed"].get(key)
        if observed is None or not port_png.is_file():
            windows[key] = {"status": "port_checkpoint_missing"}
            continue
        bindings["observed"][key] = {"record": observed, "png": {"path": rel(port_png), "sha256": sha(port_png)},
                                     "key_stage": stage, "key_substep": sub,
                                     "key_origin": "decoded from observer key 'stage%d-%d' % [mode_stage, substep]; observer file pinned in 'observer'"}
        port_failure = (None if stop_frame is None or observed["frame"] != stop_frame
                        else "port observer stopped at this frame: %s" % receipt["message"])
        idx = next((i for i, r in enumerate(rows) if (r["mode"], r["stage"], r["step"], r["substep"]) == (0x15, stage, 0, sub)), None)
        if idx is None:
            windows[key] = {"status": "native_key_never_observed_at_vsync",
                            "reason": "iteration holding this key was not sampled at its own vsync (multi-vsync iteration)"}
            if port_failure:
                windows[key]["port_failure"] = port_failure
            continue
        if idx == 0 or idx + args.latency >= len(rows):
            windows[key] = {"status": "unsynchronized", "native_iteration_frame_count": rows[idx]["frame_count"],
                            "problems": ["first iteration of key not sampled at its own vsync (no previous sample)" if idx == 0
                                         else "run ends inside the latency window"]}
            if port_failure:
                windows[key]["port_failure"] = port_failure
            continue
        window = rows[idx - 1: idx + args.latency + 1]
        f = rows[idx]["frame_count"]
        problems = []
        if rows[idx - 1]["frame_count"] != f - 1:
            problems.append("first iteration of key not sampled at its own vsync (previous frame_count %d)" % rows[idx - 1]["frame_count"])
        for j, r in enumerate(window):
            if not r["pc_in_vsync"]:
                problems.append("vsync %d pc %s outside VSync" % (r["vsync"], r["pc"]))
            if j and (r["vsync"] != window[j - 1]["vsync"] + 1 or r["frame_count"] != window[j - 1]["frame_count"] + 1):
                problems.append("vsync %d frame_count %d breaks one-iteration-per-vsync" % (r["vsync"], r["frame_count"]))
        if len({(r["mode"], r["stage"], r["step"]) for r in window[1:]}) != 1:
            problems.append("mode/stage/step changes inside the latency window")
        line_constant = len({r["prologue_line"] for r in window[1:]}) == 1
        shot_row = window[-1]
        entry = {"native_iteration_frame_count": f, "first_vsync": rows[idx]["vsync"],
                 "image_vsync": shot_row["vsync"], "image_vsync_frame_count": shot_row["frame_count"], "window": window,
                 "port_frame": observed["frame"], "port_counter": observed["counter"]}
        if shot_row["frame_count"] != f + args.latency:
            problems.append("image vsync frame_count %d != F+latency" % shot_row["frame_count"])
        if shot_row["vsync"] not in shot_index:
            problems.append("no shot captured at vsync %d" % shot_row["vsync"])
        entry["status"] = "window_ok" if not problems else "unsynchronized"
        entry["problems"] = problems
        windows[key] = entry
        if port_failure:
            # native window is still reported for timing diagnostics; the port image is the failure frame, so no pair
            entry["window_status"], entry["status"], entry["port_failure"] = entry["status"], "port_failure_frame", port_failure
            continue
        if problems:
            continue
        shot = result["shots"][shot_index[shot_row["vsync"]]]
        line_mapping = {"name": "prologue_line", "native_field": "prologue_line", "port_path": ["record", "opening", "prologue_line"],
                        "meaning": "prologue_line (0x8014D180)", "compatibility": "equivalent" if line_constant else "incompatible"}
        if not line_constant:
            line_mapping["reason"] = "native prologue_line changes inside the latency window, so the shot-time RAM value is not iteration F's"
        native_png = args.native_dir / shot["png"]
        pair = {
            "schema": "tokimemo.capture-pair.v1",
            "native": {"record": {"path": rel(result_path), "sha256": sha(result_path)},
                       "selector": ["shots", shot_index[shot_row["vsync"]]],
                       "image": {"path": rel(native_png), "sha256": shot["png_sha256"]}},
            "port": {"record": {"path": "port-png-bindings.json", "sha256": None},
                     "selector": ["observed", key],
                     "image": {"path": rel(port_png), "sha256": sha(port_png)},
                     "image_sha_path": ["png", "sha256"],
                     "clock_paths": {"port_frame": ["record", "frame"], "port_counter": ["record", "counter"]}},
            "native_map": NATIVE_MAP,
            "mappings": [
                {"name": "stage", "native_field": "stage", "port_path": ["key_stage"],
                 "meaning": "mode_stage (GameState+0x1109); port value decoded from the observer key; native value constant over the F..F+latency window (sync evidence)",
                 "compatibility": "equivalent"},
                line_mapping,
                {"name": "substep", "native_field": "substep", "port_path": ["key_substep"],
                 "meaning": "substep (GameState+0x110D); port value decoded from the observer key", "compatibility": "incompatible",
                 "reason": "native RAM is sampled at iteration F+latency; iteration F's substep is the checkpoint key and is recorded in the sync evidence window"},
                {"name": "prologue_scene_counter", "native_field": "prologue_scene_counter", "port_path": ["record", "counter"],
                 "meaning": "prologue_scene_counter (0x8014D184)", "compatibility": "incompatible",
                 "reason": "native RAM is sampled at iteration F+latency, the port value is iteration F; the per-iteration native value is in the sync evidence window"},
                {"name": "D_801220E0", "native_field": "D_801220E0", "port_path": ["record", "opening", "D_801220E0"],
                 "meaning": "flash/background brightness byte", "compatibility": "incompatible",
                 "reason": "native RAM is sampled at iteration F+latency, the port value is iteration F"},
            ],
            "checkpoint": {"name": key + " first post-update iteration (native F=%d, image at vsync %d)" % (f, shot_row["vsync"]),
                           "expected": {"stage": stage}},
            "synchronization": {"status": "unverified", "basis": "native iteration F observed at its own vsync; F..F+%d single-vsync iterations in VSync wait; declared display latency %d vsyncs: %s. Pending independent review." % (args.latency, args.latency, args.latency_basis),
                                "evidence": {"path": "sync-evidence.json", "sha256": None}},
            "scope": "single Rei opening checkpoint %s; exact RGBA at 320x240; native PCSX-Redux 8288e927+3a24bbcc with OpenBIOS, not hardware" % key,
        }
        pairs.append((key, pair))
    # Separate timing diagnostic: iterations (native frame_count) and port frames spent per key. Raw clocks only.
    timing = []
    for (s0, k0), (s1, k1) in zip(CHECKPOINTS, CHECKPOINTS[1:]):
        a, b = "stage%d-%d" % (s0, k0), "stage%d-%d" % (s1, k1)
        first = lambda key: (None if any("first iteration" in p for p in windows.get(key, {}).get("problems", []))
                             else windows.get(key, {}).get("native_iteration_frame_count"))
        na, nb = first(a), first(b)
        pa = receipt["observed"].get(a, {}).get("frame")
        pb = receipt["observed"].get(b, {}).get("frame")
        timing.append({"key": a, "native_iterations": nb - na if na is not None and nb is not None else None,
                       "port_frames": pb - pa if pa is not None and pb is not None else None})
    bind_path = args.out / "port-png-bindings.json"
    with bind_path.open("x") as fh:
        json.dump(bindings, fh, indent=1, sort_keys=True)
    evidence = {"schema": "tokimemo.t39008.rei-sync-evidence.v1", "latency_vsyncs": args.latency, "latency_basis": args.latency_basis,
                "native_result": {"path": rel(result_path), "sha256": sha(result_path)},
                "native_samples": {"path": rel(samples_path), "sha256": sha(samples_path)},
                "vsync_pc_range": [hex(VSYNC_LO), hex(VSYNC_HI)], "latency_probe": latency_probe(rows, result, args.native_dir),
                "checkpoints": windows,
                "key_durations": {"note": "native first-iteration frame_count deltas versus port frame deltas; unsampled native keys yield null",
                                  "rows": timing}}
    ev_path = args.out / "sync-evidence.json"
    with ev_path.open("x") as fh:
        json.dump(evidence, fh, indent=1, sort_keys=True)
    for key, pair in pairs:
        pair["port"]["record"]["sha256"] = sha(bind_path)
        pair["synchronization"]["evidence"]["sha256"] = sha(ev_path)
        with (args.out / ("pair-%s.json" % key)).open("x") as fh:
            json.dump(pair, fh, indent=1, sort_keys=True)
    print(json.dumps({k: v["status"] for k, v in windows.items()}, indent=1))


if __name__ == "__main__":
    main()
