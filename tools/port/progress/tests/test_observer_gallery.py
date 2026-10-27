"""Positive/negative tests for one typed observer screenshot admission."""
from __future__ import annotations

import hashlib
import json
import os
import shutil
import sys
import tempfile
import unittest
import unittest.mock
from pathlib import Path

HERE = Path(__file__).resolve()
PROGRESS = HERE.parents[1]
sys.path.insert(0, str(PROGRESS))
import admission
from publish import publish


class ObserverGalleryTests(unittest.TestCase):
    def setUp(self) -> None:
        self.source = Path(os.environ["TOKIMEMO_SOURCE_ROOT"])
        self.evidence = self.source / ".claude/orchestrator/state/T-29025/root-bgm-integration"
        self.status_rel = "gui-v1/entry-keyboard-status.json"

    def copy_observer_evidence(self, workspace: Path) -> Path:
        evidence_dir = workspace / ".claude/orchestrator/state/T-29025/root-bgm-integration"
        for rel in ["application.json", self.status_rel, "gui-v1/first-scene.png",
                    "gui-v1/first-scene-mix.wav", "gui-v1/driver.log", "gui-v1/godot.log",
                    "gui-v1/content-install/installed-content.json",
                    "gui-v1/content-install/packs/original-ja-ps-the-best-00251010797ae23b/manifest.json"]:
            target = evidence_dir / rel
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(self.evidence / rel, target)
        return evidence_dir

    def test_only_pinned_screenshot_is_published_with_observer_type(self) -> None:
        source = self.evidence / "gui-v1/first-scene.png"
        sha = hashlib.sha256(source.read_bytes()).hexdigest()
        with tempfile.TemporaryDirectory() as tmp:
            media = Path(tmp) / "gallery"
            added = publish([source], media, evidence_root=self.source)
            manifest = json.loads((media / "manifest.json").read_text())
            self.assertEqual(added, 1)
            self.assertEqual(len(manifest["items"]), 1)
            item = manifest["items"][0]
            proof = item["admission"]
            self.assertEqual(item["sha256"], sha)
            self.assertEqual(proof["schema"], "tokimemo.progress-observer-admission.v1")
            self.assertEqual(proof["scope"], "first_arrival_line0_observer_only")
            self.assertFalse(proof["gameplay_route_acceptance"])
            self.assertFalse(proof["new_game_completion"])
            self.assertFalse(proof["native_pixel_parity"])
            self.assertFalse(proof["native_pcm_parity"])
            self.assertFalse(proof["video_capture"])
            self.assertEqual(proof["audio_provenance_only"]["sha256"], admission.observer_evidence.WAV_SHA256)
            self.assertFalse(proof["audio_provenance_only"]["gallery_media"])
            self.assertEqual(list(media.glob("*.png")), [media / item["file"]])
            self.assertEqual(list(media.glob("*.wav")), [])

    def test_wav_unknown_png_and_changed_known_png_are_not_gallery_media(self) -> None:
        gate = admission.Admission(self.source)
        with self.assertRaises(ValueError):
            gate.authorize(admission.observer_evidence.WAV_SHA256, ".wav", 917548)
        with self.assertRaises(ValueError):
            gate.authorize("0" * 64, ".png", 99899)
        changed_hash = hashlib.sha256(b"different PNG bytes").hexdigest()
        with self.assertRaises(ValueError):
            gate.authorize(changed_hash, ".png", 99899)

    def test_tampered_status_or_immutable_source_packet_revokes_observer_capture(self) -> None:
        original = self.evidence / "gui-v1/first-scene.png"
        for mutate_source in [False, True]:
            with self.subTest(mutate_source=mutate_source), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp) / "workspace"
                evidence_dir = self.copy_observer_evidence(root)
                source_root = self.source
                packet_rel = ".claude/orchestrator/state/T-29025/first-arrival-bgm-v1/review-packet-bgm-a657500c40e9d414"
                if mutate_source:
                    source_root = Path(tmp) / "source"
                    for rel in ["manifest.json", "readback.json", "checksums.json", "patch.diff",
                                "patch.raw", "replay_gui.sh", "evidence.json"]:
                        target = source_root / packet_rel / rel
                        target.parent.mkdir(parents=True, exist_ok=True)
                        shutil.copyfile(self.source / packet_rel / rel, target)
                    checksums = source_root / packet_rel / "checksums.json"
                    checksums.write_bytes(checksums.read_bytes() + b" ")
                else:
                    status = evidence_dir / self.status_rel
                    status.write_bytes(status.read_bytes() + b" ")
                with unittest.mock.patch.dict(os.environ, {"TOKIMEMO_SOURCE_ROOT": str(source_root)}):
                    gate = admission.Admission(root)
                    digest = hashlib.sha256(original.read_bytes()).hexdigest()
                    with self.assertRaises(ValueError):
                        gate.authorize(digest, ".png", original.stat().st_size)

    def test_missing_or_symlinked_observer_artifact_is_rejected(self) -> None:
        source_png = self.evidence / "gui-v1/first-scene.png"
        png_digest = hashlib.sha256(source_png.read_bytes()).hexdigest()
        for kind in ["missing", "symlink"]:
            with self.subTest(kind=kind), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp) / "workspace"
                evidence_dir = self.copy_observer_evidence(root)
                image = evidence_dir / "gui-v1/first-scene.png"
                if kind == "missing":
                    image.unlink()
                else:
                    saved = image.with_suffix(".saved")
                    image.rename(saved)
                    image.symlink_to(saved.name)
                gate = admission.Admission(root)
                with self.assertRaises(ValueError):
                    gate.authorize(png_digest, ".png", source_png.stat().st_size)

    def test_registry_cannot_repin_observer_to_an_unknown_image(self) -> None:
        original_registry = admission.REGISTRY
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "reviewed_routes.json"
            policy = json.loads(original_registry.read_text())
            policy["observer_captures"][0]["screenshot_sha256"] = "0" * 64
            path.write_text(json.dumps(policy))
            with unittest.mock.patch.object(admission, "REGISTRY", path):
                with self.assertRaises(ValueError):
                    admission.Admission(self.source)

    def test_old_route_admission_record_remains_disjoint(self) -> None:
        gate = admission.Admission(self.source)
        self.assertTrue(gate.captures)
        self.assertTrue(gate.observer_captures)
        self.assertNotIn(admission.observer_evidence.SCREEN_SHA256, gate.captures)
        self.assertIn(admission.observer_evidence.SCREEN_SHA256, gate.observer_captures)


if __name__ == "__main__":
    unittest.main(verbosity=2)
