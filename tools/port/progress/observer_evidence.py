"""Narrow, root-pinned observer receipt for the T-29025 first-arrival run.

This is deliberately separate from ``tools.port.progress.admission``. It can
verify one preserved first-scene observation, but it cannot admit gameplay
progress, create a route, or add an image to the reviewed gallery.
"""
from __future__ import annotations

import hashlib
import json
import os
import re
import stat
import argparse
import wave
from pathlib import Path, PurePosixPath


HEX64 = re.compile(r"^[a-f0-9]{64}$")
MAX_STATUS = 128 * 1024
MAX_IMAGE = 8 * 1024 * 1024
MAX_AUDIO = 4 * 1024 * 1024

# Exact root-applied first-arrival packet and production source identity.
BGM_PACKET_MANIFEST_SHA256 = "71f8116dc2b4331db8dc9189f781151cc9e7e47a9ac2a8301260e5a9c36a5473"
PRODUCTION_TREE_SHA256 = "c5e0e476d4ceaa4ac3fbbf15bec004902ed8bb20059bc430e9d646398f2394fa"
FIRST_SCENE_TEST = "port/godot/tests/run_installed_first_scene.gd"
FIRST_SCENE_TEST_SHA256 = "9d912f5954a2fbfbf19b505e71b3e1d8aeeeb0a73b70f37d1843e1c8eaef58cd"
APPLICATION_SHA256 = "3d9e63697f69d74f2356b6755aca4e1b7eaa54505dd0adc1997d48cadcdd80bc"
STATUS_SHA256 = "4908298f8cc928b81c2767315de81c10b8c3a37ee9e9119386b380f7b4da384a"
SCREEN_SHA256 = "52ddbd8818295ef01a4b71c71a74a438cce741c4928b93de093581906d750aaf"
WAV_SHA256 = "edfb5b480bc4a731983b512cd8f21f6359098c1dcc525bb567a7c69924aed602"
DRIVER_LOG_SHA256 = "29fd6f0fbf1fe1674fe6d7b7c085f5071bdad4cffb05b8092af8c7a1a59105f2"
GODOT_LOG_SHA256 = "c2e7da32cd69fa0332680e6faf98eb69dcf0ec59bf17e32be20db0936bc5b74a"
INSTALLED_MANIFEST_SHA256 = "00251010797ae23b607fb17d7b5d133e29cf2b99117db72c962fa1bd594833f7"
PACK_READBACK_SHA256 = "11476517c8c943336aeab30061d0cce25b0f1bea389a30991c961f6b95bdaecf"
PACK_CHECKSUMS_SHA256 = "b38b3bf215ffa9dc9e5044e39fddfd82cef3c05994435ba91f060454966a7503"
PACK_PATCH_SHA256 = "b642f514685953d4eae30ddbff4788810c72ad9bd63adf0675921ccc47c8291f"
PACK_PATCH_RAW_SHA256 = "122503cc33662193e2cccc771244b0bcd1576004fe544b0037bedc939e7e1389"
PACK_REPLAY_COMMAND_SHA256 = "47576bfa6b3ef6f0bbdd1cb41248581488f80af26cb1356ca61aafbcd613a2f6"
PACK_EVIDENCE_SHA256 = "e596b338a7ca2b96532b8b205ef8e17f89a28b79e2b5fcb4e188d2ff7ccac46f"
APPLICATION_PACKET_SHA256 = "71f8116dc2b4331db8dc9189f781151cc9e7e47a9ac2a8301260e5a9c36a5473"
AUDIO_PROOF_SHA256 = "f4017d9bafd2f1c09c8d13d6d20a4646c7b80c0076b38ea978672da44b25c095"


class EvidenceError(ValueError):
    pass


def _require(ok: bool, message: str) -> None:
    if not ok:
        raise EvidenceError(message)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _read(root: Path, relative: str, wanted_sha: str, limit: int) -> bytes:
    p = PurePosixPath(relative)
    _require(not p.is_absolute() and ".." not in p.parts and p.as_posix() == relative,
             "unsafe evidence locator")
    path = root / relative
    _require(not any(part.is_symlink() for part in (path, *path.parents)), "symlink evidence is forbidden")
    try:
        before = path.lstat()
    except OSError as exc:
        raise EvidenceError("required evidence file is unavailable") from exc
    _require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, "evidence file type or size differs")
    fd = os.open(path, os.O_RDONLY | getattr(os, "O_NOFOLLOW", 0))
    signature = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
    with os.fdopen(fd, "rb") as stream:
        _require(signature(os.fstat(stream.fileno())) == signature(before), "evidence changed before read")
        data = stream.read(limit + 1)
        _require(signature(os.fstat(stream.fileno())) == signature(before), "evidence changed during read")
    _require(len(data) == before.st_size and signature(path.lstat()) == signature(before), "evidence changed after read")
    _require(_sha(data) == wanted_sha, "pinned evidence hash differs")
    return data


def _json(data: bytes) -> dict:
    def pairs(items):
        out = {}
        for key, value in items:
            _require(key not in out, "duplicate JSON key")
            out[key] = value
        return out
    def invalid(_value):
        raise EvidenceError("non-finite JSON value")
    result = json.loads(data, object_pairs_hook=pairs, parse_constant=invalid)
    _require(isinstance(result, dict), "expected JSON object")
    return result


def _wav_summary(data: bytes) -> dict:
    import io
    with wave.open(io.BytesIO(data), "rb") as wav:
        _require(wav.getcomptype() == "NONE" and wav.getnchannels() == 2
                 and wav.getsampwidth() == 2 and wav.getframerate() == 44100
                 and wav.getnframes() == 229376, "captured WAV format/extent differs")
        pcm = wav.readframes(wav.getnframes())
    _require(len(pcm) == 917504, "captured PCM extent differs")
    return {"format": "pcm_s16le", "channels": 2, "sample_rate": 44100,
            "frames": 229376, "pcm_bytes": len(pcm), "pcm_sha256": _sha(pcm)}


def verify_observation(source_root: Path, evidence_root: Path) -> dict:
    """Verify the one known run and return a non-admission observer receipt."""
    app = _json(_read(evidence_root, "application.json", APPLICATION_SHA256, MAX_STATUS))
    _require(app.get("packet_manifest_sha256") == APPLICATION_PACKET_SHA256,
             "root application packet binding differs")
    applied = app.get("verified_applied_files")
    _require(isinstance(applied, dict), "root application file map is absent")
    test_record = applied.get(FIRST_SCENE_TEST)
    _require(isinstance(test_record, dict) and test_record.get("after_sha256") == FIRST_SCENE_TEST_SHA256,
             "first-scene producer code hash differs")
    _require(bool(applied), "root-applied source file inventory is empty")

    packet_root = Path(".claude/orchestrator/state/T-29025/first-arrival-bgm-v1/review-packet-bgm-a657500c40e9d414")
    packet = _json(_read(source_root, (packet_root / "manifest.json").as_posix(),
                         BGM_PACKET_MANIFEST_SHA256, 512 * 1024))
    readback = _json(_read(source_root, (packet_root / "readback.json").as_posix(),
                            PACK_READBACK_SHA256, 16 * 1024))
    checksums = _json(_read(source_root, (packet_root / "checksums.json").as_posix(),
                             PACK_CHECKSUMS_SHA256, 64 * 1024))
    _read(source_root, (packet_root / "patch.diff").as_posix(), PACK_PATCH_SHA256, 2 * 1024 * 1024)
    _read(source_root, (packet_root / "patch.raw").as_posix(), PACK_PATCH_RAW_SHA256, 2 * 1024 * 1024)
    _read(source_root, (packet_root / "replay_gui.sh").as_posix(), PACK_REPLAY_COMMAND_SHA256, 16 * 1024)
    _read(source_root, (packet_root / "evidence.json").as_posix(), PACK_EVIDENCE_SHA256, 64 * 1024)
    _require(packet.get("after_production_sha256") == PRODUCTION_TREE_SHA256
             and readback.get("full_production_sha256") == PRODUCTION_TREE_SHA256,
             "root-applied production source identity differs")
    source_files = packet.get("source_files")
    _require(isinstance(source_files, dict) and len(source_files) == len(applied),
             "captured packet source inventory differs from the root application inventory")
    for relative, record in applied.items():
        _require(isinstance(relative, str) and isinstance(record, dict)
                 and isinstance(record.get("after_sha256"), str) and HEX64.fullmatch(record["after_sha256"]),
                 "root-applied source inventory record is malformed")
        packet_record = source_files.get(relative)
        _require(isinstance(packet_record, dict) and packet_record.get("after_sha256") == record["after_sha256"]
                 and checksums.get("after/" + relative) == record["after_sha256"],
                 "captured packet after-image/checksum differs from the root application inventory")

    gui = "gui-v1/"
    status_bytes = _read(evidence_root, gui + "entry-keyboard-status.json", STATUS_SHA256, MAX_STATUS)
    status = _json(status_bytes)
    _require(status.get("actual_desktop_entry") is True and status.get("first_scene_rendered") is True
             and status.get("same_entry_rng_pad") is True and status.get("physical_commit_held") is True
             and status.get("first_scene_native_pixel_or_pcm_parity") is False
             and status.get("native_startup_parity") is False,
             "actual-entry status is incomplete or overclaims native parity")
    _require(status.get("input_dispatch") == "XSendEvent/libX11", "desktop input dispatch method differs")
    _require(type(status.get("checks")) is int and status["checks"] == 20
             and status.get("phase") == "passed" and type(status.get("opening_iterations")) is int,
             "bounded entry check status differs")
    _require("line0 fully revealed" in str(status.get("reached", "")),
             "observer status does not identify the line-zero reveal boundary")
    _require(status.get("manifest_sha256") == INSTALLED_MANIFEST_SHA256,
             "installed original-content manifest differs")
    audio = status.get("audio_capture")
    _require(isinstance(audio, dict) and audio.get("file") == "first-scene-mix.wav"
             and audio.get("format") == 1 and audio.get("pcm_bytes") == 917504
             and audio.get("sample_rate") == 44100 and audio.get("stereo") is True
             and audio.get("hardware_PCM_parity") is False,
             "Master capture provenance or parity boundary differs")
    context = status.get("audio_context")
    _require(isinstance(context, dict) and context.get("bank_ready") is True
             and context.get("planned_start") is True and context.get("proof_sha256") == AUDIO_PROOF_SHA256,
             "first-arrival audio context is not bound to the reviewed source proof")
    bgm = status.get("bgm")
    _require(isinstance(bgm, dict) and bgm.get("active_skips") == 0
             and type(bgm.get("published_tick")) is int and bgm["published_tick"] > 0
             and isinstance(bgm.get("history"), list) and len(bgm["history"]) == 1
             and isinstance(bgm.get("engine"), dict) and bgm["engine"].get("initialized") is True
             and bgm["engine"].get("failed") is False,
             "root BGM runtime status is incomplete")

    png = _read(evidence_root, gui + "first-scene.png", SCREEN_SHA256, MAX_IMAGE)
    _require(png[:8] == b"\x89PNG\r\n\x1a\n" and png[12:16] == b"IHDR"
             and int.from_bytes(png[16:20], "big") == 1024
             and int.from_bytes(png[20:24], "big") == 768,
             "first-scene screenshot format/dimensions differ")
    wav = _read(evidence_root, gui + "first-scene-mix.wav", WAV_SHA256, MAX_AUDIO)
    wav_summary = _wav_summary(wav)
    _read(evidence_root, gui + "driver.log", DRIVER_LOG_SHA256, 4096)
    _read(evidence_root, gui + "godot.log", GODOT_LOG_SHA256, 64 * 1024)
    _read(evidence_root, "gui-v1/content-install/installed-content.json",
          "daf61779fddf0c9ca740819e98cd1f8195255fcc461633a421ae7ee3cb14775b", 4096)
    _read(evidence_root, "gui-v1/content-install/packs/original-ja-ps-the-best-00251010797ae23b/manifest.json",
          INSTALLED_MANIFEST_SHA256, 128 * 1024)

    # Deliberately not a gallery-admission receipt. It carries no route claims.
    return {
        "schema": "tokimemo.first-arrival-observer-evidence.v1",
        "decision": "observer_evidence_only",
        "review_required": True,
        "admission_registry_eligible": False,
        "scope": {
            "actual_desktop_entry": True,
            "input_dispatch": status["input_dispatch"],
            "physical_commit_held": True,
            "same_entry_rng_and_pad_checks": True,
            "entry_line_index": 0,
            "entry_line_fully_revealed": True,
            "first_scene_rendered": True,
            "first_arrival_bgm_capture_present": True,
        },
        "nonclaims": {
            "full_route_acceptance": False,
            "new_game_completion": False,
            "native_pixel_parity": False,
            "native_pcm_parity": False,
            "native_startup_parity": False,
            "video_capture": False,
        },
        "identities": {
            "captured_production_tree_sha256": PRODUCTION_TREE_SHA256,
            "root_applied_source_file_count": len(applied),
            "bgm_packet_manifest_sha256": BGM_PACKET_MANIFEST_SHA256,
            "bgm_packet_checksums_sha256": PACK_CHECKSUMS_SHA256,
            "bgm_packet_patch_sha256": PACK_PATCH_SHA256,
            "bgm_packet_replay_command_sha256": PACK_REPLAY_COMMAND_SHA256,
            "first_scene_test_sha256": FIRST_SCENE_TEST_SHA256,
            "installed_content_manifest_sha256": INSTALLED_MANIFEST_SHA256,
            "audio_context_proof_sha256": AUDIO_PROOF_SHA256,
            "active_bank_role_sha256": _sha(str(context["active_bank_role"]).encode()),
        },
        "captures": {
            "screenshot": {"path": gui + "first-scene.png", "sha256": SCREEN_SHA256,
                           "bytes": len(png), "dimensions": [1024, 768]},
            "audio": {"path": gui + "first-scene-mix.wav", "sha256": WAV_SHA256,
                      "bytes": len(wav), **wav_summary},
            "keyboard_status": {"path": gui + "entry-keyboard-status.json", "sha256": STATUS_SHA256,
                                "checks": status["checks"], "opening_iterations": status["opening_iterations"]},
            "audio_runtime_status": {"bank_ready": context["bank_ready"],
                                     "bank_epoch": context["bank_epoch"],
                                     "planned_bank": context["planned_bank"],
                                     "planned_sequence": context["planned_sequence"],
                                     "published_tick": bgm["published_tick"],
                                     "active_skips": bgm["active_skips"],
                                     "engine_initialized": bgm["engine"]["initialized"],
                                     "engine_failed": bgm["engine"]["failed"]},
            "driver_log_sha256": DRIVER_LOG_SHA256,
            "runtime_log_sha256": GODOT_LOG_SHA256,
        },
        "application_record_sha256": APPLICATION_SHA256,
        "application_binding_sha256": _sha(_read(evidence_root, "application.json", APPLICATION_SHA256, MAX_STATUS)),
    }


def write_observation(source_root: Path, evidence_root: Path, output: Path) -> dict:
    receipt = verify_observation(source_root, evidence_root)
    output = output.absolute()
    project_root = Path(__file__).resolve().parents[3]
    cache_root = (project_root / ".asset-cache").absolute()
    _require(output == cache_root or cache_root in output.parents,
             "observer receipt output must remain under this isolated project's .asset-cache")
    _require(not any(part.is_symlink() for part in (output, *output.parents)), "symlink output path is forbidden")
    _require(not output.exists(), "observer receipt output already exists")
    output.parent.mkdir(parents=True, exist_ok=True)
    temporary = output.with_name(output.name + ".tmp")
    _require(not temporary.exists() and not temporary.is_symlink(), "temporary output already exists")
    data = (json.dumps(receipt, sort_keys=True, indent=2) + "\n").encode()
    fd = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL | getattr(os, "O_NOFOLLOW", 0), 0o600)
    with os.fdopen(fd, "wb") as stream:
        stream.write(data)
        stream.flush()
        os.fsync(stream.fileno())
    os.link(temporary, output, follow_symlinks=False)
    temporary.unlink()
    return receipt


def main() -> int:
    project_root = Path(__file__).resolve().parents[3]
    parser = argparse.ArgumentParser(description="Verify the one pinned first-arrival observer capture; this is not gameplay admission.")
    parser.add_argument("--source-root", type=Path, required=True)
    parser.add_argument("--evidence-root", type=Path, required=True)
    parser.add_argument("--output", type=Path, default=project_root / ".asset-cache/first-arrival-observer-v1/receipt.json")
    args = parser.parse_args()
    output = args.output.absolute()
    expected_prefix = (project_root / ".asset-cache").absolute()
    _require(output == expected_prefix or expected_prefix in output.parents,
             "observer receipt output must remain under this isolated project's .asset-cache")
    receipt = write_observation(args.source_root, args.evidence_root, output)
    print(json.dumps({"decision": receipt["decision"], "receipt": str(output),
                      "receipt_sha256": _sha(output.read_bytes()),
                      "screenshot_sha256": SCREEN_SHA256, "audio_sha256": WAV_SHA256}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
