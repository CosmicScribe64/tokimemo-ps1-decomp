"""Shared serialization of reader-validated finite route media components."""
import hashlib
from tools.assets import image_packs_sd, image_mdec


def export_sd_component(data, exe, stage, entry, asset_id, family, profile_index, native_route, copy_png):
    raw = stage / "reader" / asset_id
    exported = image_packs_sd.export_sd_ui(data, exe, raw, asset_id, source_entry=entry)
    if exported.get("status") != "exported":
        raise ValueError("existing SD reader did not export selected source")
    metadata = exported["metadata"]
    caller = metadata.get("caller_profile", {})
    if (caller.get("index") != profile_index or caller.get("family") != family
            or caller.get("source_lba") != entry["location"]["lba"]):
        raise ValueError("SD caller profile/family/LBA differs from selected source")
    frames = []
    for frame in exported["images"]:
        frames.append({"frame_index": frame["frame_index"], "id": f"{asset_id}:frame:{frame['frame_index']}",
            "png": copy_png(raw/frame["png"], stage/"assets"/frame["png"]),
            "stp_png": copy_png(raw/frame["stp_png"], stage/"assets"/frame["stp_png"]),
            "bounds": [frame["bounds"][0], frame["bounds"][1], frame["width"], frame["height"]],
            "origin": frame["origin"], "parts": frame["parts"], "source_palette_words": frame["source_palette_words"]})
    if not frames:
        raise ValueError("selected SD has no physical frames")
    sequences = metadata.get("source_sequence_prefix", [])
    if not sequences or sequences[0].get("sequence_index") != 0:
        raise ValueError("selected sequence zero is not in the validated pointer prefix")
    result = {"kind": "sd_animation", "id": asset_id,
        "source": {"path": entry["path"], "lba": entry["location"]["lba"], "size": len(data),
                   "sha256": hashlib.sha256(data).hexdigest(), "exe_sha256": hashlib.sha256(exe).hexdigest()},
        "profile_index": profile_index, "sequence_index": 0, "native_route": native_route,
        "frames": frames, "sequences": sequences,
        "palette": {"source_words": exported["images"][0]["source_palette_words"],
                    "mode": metadata["actor_runtime_palette_mode"], "stp_policy": metadata["stp_policy"]},
        "reader": "tools/assets/image_packs_sd.py:export_sd_ui",
        "capability": "validated_static_sd_frames_and_sequence_prefix"}
    return result, caller


def export_background_component(data, stage, entry, asset_id, context, native_route, copy_png):
    raw = stage / "reader" / asset_id
    result = image_mdec.export_mdec(data, raw, asset_id, source_path=entry["path"], context=context)
    image = result["images"][0]
    files = {name: copy_png(raw/name, stage/"assets"/name) for name in result["files"]}
    return {"kind": "mdec_background", "id": asset_id,
        "source": {"path": entry["path"], "lba": entry["location"]["lba"], "size": len(data), "sha256": hashlib.sha256(data).hexdigest()},
        "active_size": [320, 88], "padded_size": [320, 96], "active_crop": [0, 0, 320, 88],
        "native_route": native_route,
        "image": files[image["png"]], "stp_mask": files[image["stp_png"]],
        "padded_image": files[result["metadata"]["padded_decode"]["png"]],
        "padded_stp_mask": files[result["metadata"]["padded_decode"]["stp_png"]],
        "geometry_context": context, "reader": "tools/assets/image_mdec.py:export_mdec",
        "capability": "source_profiled_static_background_preview"}
