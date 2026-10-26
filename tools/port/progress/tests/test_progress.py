"""Synthetic safety and HTTP behavior tests for the local progress viewer."""
import base64
import json
import os
import shutil
import struct
import sys
import subprocess
import tempfile
import threading
import unittest
import zlib
from http.server import ThreadingHTTPServer
from pathlib import Path
from urllib.error import HTTPError
from urllib.request import Request, urlopen

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import publish
import server
import watcher

PNG_FIXTURE = base64.b64decode(
    "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mP8/x8AAwMCAO+/pE8AAAAASUVORK5CYII="
)


class ProgressTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name) / "media"
        self.root.mkdir()
        self.fixture = Path(self.temp.name) / "actual capture.png"
        self.fixture.write_bytes(PNG_FIXTURE)

    def tearDown(self):
        self.temp.cleanup()

    def test_publish_manifest_and_duplicate_no_overwrite(self):
        self.assertEqual(publish.publish([self.fixture], self.root), 1)
        data = json.loads((self.root / "manifest.json").read_text())
        self.assertEqual(len(data["items"]), 1)
        entry = data["items"][0]
        self.assertEqual(entry["bytes"], len(PNG_FIXTURE))
        self.assertEqual(publish.publish([self.fixture], self.root), 0)
        dest = self.root / entry["file"]
        dest.write_bytes(b"changed bytes")
        with self.assertRaises(ValueError):
            publish.publish([self.fixture], self.root)

    def test_reject_symlink_unsupported_type_and_oversize(self):
        link = Path(self.temp.name) / "linked.png"
        link.symlink_to(self.fixture)
        with self.assertRaises(ValueError):
            publish.inspect_capture(link)
        bad = Path(self.temp.name) / "bad.png"
        bad.write_bytes(b"not png bytes")
        with self.assertRaises(ValueError):
            publish.inspect_capture(bad)
        tiny_mp4 = Path(self.temp.name) / "tiny.mp4"
        tiny_mp4.write_bytes(b"\x00\x00\x00\x18ftypisom0000")
        with self.assertRaises(ValueError):
            publish.inspect_capture(tiny_mp4)
        large = Path(self.temp.name) / "large.png"
        large.write_bytes(b"x" * (publish.MAX_IMAGE + 1))
        with self.assertRaises(ValueError):
            publish.inspect_capture(large)

    def test_manifest_refresh_range_mime_and_traversal(self):
        publish.publish([self.fixture], self.root)
        clip = Path(self.temp.name) / "synthetic-test-only.mp4"
        clip.write_bytes(b"\x00\x00\x00\x18ftypisom0000\x00\x00\x00\x08moov\x00\x00\x10\x00mdat" + (b"\x00" * 4096))
        publish.publish([clip], self.root)
        server.ViewerHandler.media_root = self.root
        httpd = ThreadingHTTPServer(("127.0.0.1", 0), server.ViewerHandler)
        thread = threading.Thread(target=httpd.serve_forever, daemon=True)
        thread.start()
        base = f"http://127.0.0.1:{httpd.server_port}"
        try:
            with urlopen(base + "/") as response:
                self.assertEqual(response.status, 200)
                self.assertIn(b"Port media viewer", response.read())
            with urlopen(base + "/api/manifest.json") as response:
                first_manifest = json.loads(response.read())
            self.assertEqual(len(first_manifest["items"]), 2)
            image_item = next(item for item in first_manifest["items"] if item["kind"] == "image")
            name = image_item["file"]
            request = Request(base + "/media/" + name, headers={"Range": "bytes=0-7"})
            with urlopen(request) as response:
                self.assertEqual(response.status, 206)
                self.assertEqual(response.headers["Content-Type"], "image/png")
                self.assertEqual(response.headers["Content-Range"], f"bytes 0-7/{len(PNG_FIXTURE)}")
                self.assertEqual(response.read(), PNG_FIXTURE[:8])
            video_item = next(item for item in first_manifest["items"] if item["kind"] == "video")
            video_request = Request(base + "/media/" + video_item["file"], headers={"Range": "bytes=4-7"})
            with urlopen(video_request) as response:
                self.assertEqual(response.status, 206)
                self.assertEqual(response.headers["Content-Type"], "video/mp4")
                self.assertEqual(response.read(), b"ftyp")
            with self.assertRaises(HTTPError) as caught:
                urlopen(base + "/media/%2e%2e%2fmanifest.json")
            self.assertEqual(caught.exception.code, 404)
            with self.assertRaises(HTTPError) as caught:
                urlopen(base + "/../../etc/passwd")
            self.assertEqual(caught.exception.code, 404)
            second = Path(self.temp.name) / "second.png"
            second.write_bytes(PNG_FIXTURE[:-1] + bytes([PNG_FIXTURE[-1] ^ 1]))
            publish.publish([second], self.root)
            with urlopen(base + "/api/manifest.json") as response:
                updated = json.loads(response.read())
            self.assertEqual(len(updated["items"]), 3)
        finally:
            httpd.shutdown()
            httpd.server_close()
            thread.join(timeout=2)

    def test_server_ignores_symlink_manifest_entries(self):
        digest = "a" * 64
        name = digest[:16] + ".png"
        (self.root / name).symlink_to(self.fixture)
        (self.root / "manifest.json").write_text(json.dumps({"version": 1, "items": [{
            "id": digest[:16], "file": name, "sha256": digest, "bytes": len(PNG_FIXTURE), "created": "x"
        }]}))
        self.assertEqual(server.safe_entries(self.root), [])

    def test_same_size_media_tamper_is_not_served(self):
        publish.publish([self.fixture], self.root)
        self.assertEqual(len(server.safe_entries(self.root)), 1)
        entry = json.loads((self.root / "manifest.json").read_text())["items"][0]
        media_path = self.root / entry["file"]
        original_stat = media_path.stat()
        tampered = bytearray(media_path.read_bytes())
        tampered[-1] ^= 1
        media_path.write_bytes(tampered)
        self.assertEqual(media_path.stat().st_size, original_stat.st_size)
        self.assertEqual(server.safe_entries(self.root), [])

    def test_progress_schema_bounds_and_numeric_whitelist(self):
        progress_path = Path(self.temp.name) / "progress.json"
        progress_path.write_text(json.dumps({
            "schema": "tokimemo.port-progress.v1", "overall_percent": 5,
            "first_slice_percent": 60, "estimated": True, "internal_note": "ignored"
        }))
        self.assertEqual(server.safe_progress(progress_path), {
            "overall_percent": 5, "first_slice_percent": 60, "estimated": True
        })
        for bad in (True, -1, 101, 5.5, "5"):
            with self.subTest(overall=bad):
                progress_path.write_text(json.dumps({
                    "schema": "tokimemo.port-progress.v1", "overall_percent": bad,
                    "first_slice_percent": 60, "estimated": True
                }))
                self.assertEqual(server.safe_progress(progress_path), server.DEFAULT_PROGRESS)
        progress_path.write_text(json.dumps({
            "schema": "wrong", "overall_percent": 5, "first_slice_percent": 60, "estimated": True
        }))
        self.assertEqual(server.safe_progress(progress_path), server.DEFAULT_PROGRESS)

    def test_ssr_percent_and_metrics_only_refresh(self):
        publish.publish([self.fixture], self.root)
        progress_path = Path(self.temp.name) / "progress.json"
        server.PROGRESS_PATH = progress_path
        progress_path.write_text(json.dumps({
            "schema": "tokimemo.port-progress.v1", "overall_percent": 5,
            "first_slice_percent": 60, "estimated": True
        }))
        server.ViewerHandler.media_root = self.root
        httpd = ThreadingHTTPServer(("127.0.0.1", 0), server.ViewerHandler)
        thread = threading.Thread(target=httpd.serve_forever, daemon=True)
        thread.start()
        base = f"http://127.0.0.1:{httpd.server_port}"
        try:
            with urlopen(base + "/") as response:
                html = response.read().decode()
            self.assertIn("Japanese port progress: 5 percent", html)
            self.assertIn("<strong id=\"progress-value\">~5%</strong>", html)
            self.assertIn("<img src=\"/media/", html)
            self.assertNotIn("INITIAL_PROGRESS", html)
            with urlopen(base + "/api/manifest.json") as response:
                first = json.loads(response.read())
            progress_path.write_text(json.dumps({
                "schema": "tokimemo.port-progress.v1", "overall_percent": 6,
                "first_slice_percent": 60, "estimated": True
            }))
            with urlopen(base + "/api/manifest.json") as response:
                second = json.loads(response.read())
            self.assertEqual(first["items"], second["items"])
            self.assertEqual(first["progress"]["overall_percent"], 5)
            self.assertEqual(second["progress"]["overall_percent"], 6)
            app = (Path(server.__file__).resolve().parent / "static/app.js").read_text()
            self.assertLess(app.index("updateProgress(manifest.progress)"), app.index("if (!changed) return;"))
        finally:
            httpd.shutdown()
            httpd.server_close()
            thread.join(timeout=2)

    def test_wrappers_reject_symlinked_media_roots_before_docker(self):
        source_dir = Path(__file__).resolve().parents[1]
        for symlink_component in (".asset-cache", ".asset-cache/port-progress"):
            with self.subTest(component=symlink_component), tempfile.TemporaryDirectory() as temp:
                repo = Path(temp) / "repo"
                script_dir = repo / "tools/port/progress"
                script_dir.mkdir(parents=True)
                for name in ("run.sh", "publish.sh", "cache_root.sh"):
                    shutil.copy2(source_dir / name, script_dir / name)
                outside = Path(temp) / "outside"
                outside.mkdir()
                cache_dir = repo / ".asset-cache"
                if symlink_component == ".asset-cache":
                    cache_dir.symlink_to(outside, target_is_directory=True)
                else:
                    cache_dir.mkdir()
                    (cache_dir / "port-progress").symlink_to(outside, target_is_directory=True)

                fake_bin = Path(temp) / "bin"
                fake_bin.mkdir()
                docker_marker = Path(temp) / "docker-was-called"
                docker_stub = fake_bin / "docker"
                docker_stub.write_text(f"#!/bin/sh\nprintf called > '{docker_marker}'\nexit 99\n")
                docker_stub.chmod(0o755)
                env = dict(os.environ, PATH=str(fake_bin) + os.pathsep + os.environ["PATH"])

                serve = subprocess.run([str(script_dir / "run.sh"), "serve"], env=env, capture_output=True)
                self.assertNotEqual(serve.returncode, 0)
                self.assertFalse(docker_marker.exists())

                capture = Path(temp) / "capture.png"
                capture.write_bytes(PNG_FIXTURE)
                publish_result = subprocess.run([str(script_dir / "publish.sh"), str(capture)], env=env, capture_output=True)
                self.assertNotEqual(publish_result.returncode, 0)
                self.assertFalse(docker_marker.exists())

    def test_watch_wrapper_rejects_symlinked_capture_root_before_docker(self):
        source_dir = Path(__file__).resolve().parents[1]
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp) / "repo"
            script_dir = repo / "tools/port/progress"
            script_dir.mkdir(parents=True)
            for name in ("run.sh", "publish.sh", "cache_root.sh"):
                shutil.copy2(source_dir / name, script_dir / name)
            cache_dir = repo / ".asset-cache"
            cache_dir.mkdir()
            outside = Path(temp) / "outside"
            outside.mkdir()
            (cache_dir / "port-preview-output").symlink_to(outside, target_is_directory=True)
            fake_bin = Path(temp) / "bin"
            fake_bin.mkdir()
            docker_marker = Path(temp) / "docker-was-called"
            docker_stub = fake_bin / "docker"
            docker_stub.write_text(f"#!/bin/sh\nprintf called > '{docker_marker}'\nexit 99\n")
            docker_stub.chmod(0o755)
            env = dict(os.environ, PATH=str(fake_bin) + os.pathsep + os.environ["PATH"])
            result = subprocess.run([str(script_dir / "run.sh"), "watch"], env=env, capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertFalse(docker_marker.exists())

    def test_watcher_waits_for_stability_dedupes_and_tracks_replacement(self):
        roots = [Path(self.temp.name) / name for name in watcher.WATCH_FOLDER_NAMES]
        for root in roots:
            root.mkdir()
        source = roots[0] / "render-smoke.png"
        source.write_bytes(PNG_FIXTURE)
        second_source = roots[1] / "same-capture.png"
        second_source.write_bytes(PNG_FIXTURE)
        capture_watcher = watcher.CaptureWatcher(roots, self.root)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertEqual(capture_watcher.poll_once(), 1)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertEqual(len(json.loads((self.root / "manifest.json").read_text())["items"]), 1)

        replacement = bytearray(PNG_FIXTURE)
        replacement[-5] ^= 1
        source.write_bytes(replacement)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertEqual(capture_watcher.poll_once(), 1)
        self.assertEqual(len(json.loads((self.root / "manifest.json").read_text())["items"]), 2)

    def test_watcher_ignores_symlinks_and_waits_for_completed_capture(self):
        roots = [Path(self.temp.name) / name for name in watcher.WATCH_FOLDER_NAMES]
        for root in roots:
            root.mkdir()
        (roots[0] / "symlink.png").symlink_to(self.fixture)
        partial = roots[1] / "partial.png"
        partial.write_bytes(PNG_FIXTURE[:8])
        capture_watcher = watcher.CaptureWatcher(roots, self.root)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertFalse((self.root / "manifest.json").exists())
        partial.write_bytes(PNG_FIXTURE)
        self.assertEqual(capture_watcher.poll_once(), 0)
        self.assertEqual(capture_watcher.poll_once(), 1)
        self.assertEqual(len(json.loads((self.root / "manifest.json").read_text())["items"]), 1)

    def test_concurrent_publishers_preserve_both_manifest_entries(self):
        first = Path(self.temp.name) / "first.png"
        second = Path(self.temp.name) / "second.png"
        first.write_bytes(PNG_FIXTURE)
        changed = bytearray(PNG_FIXTURE)
        changed[-5] ^= 1
        second.write_bytes(changed)
        command = [sys.executable, str(Path(publish.__file__)), "--media-root", str(self.root)]
        processes = [subprocess.Popen(command + [str(path)], stdout=subprocess.PIPE, stderr=subprocess.PIPE) for path in (first, second)]
        for process in processes:
            stdout, stderr = process.communicate(timeout=10)
            self.assertEqual(process.returncode, 0, (stdout, stderr))
        manifest = json.loads((self.root / "manifest.json").read_text())
        self.assertEqual(len(manifest["items"]), 2)

    @staticmethod
    def _encode_rgb_png(width: int, height: int, row_fn) -> bytes:
        def chunk(ctype: bytes, data: bytes) -> bytes:
            crc = zlib.crc32(ctype + data) & 0xFFFFFFFF
            return struct.pack(">I", len(data)) + ctype + data + struct.pack(">I", crc)

        raw = bytearray()
        for y in range(height):
            raw.append(0)
            raw.extend(row_fn(y, width))
        return (
            b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(bytes(raw)))
            + chunk(b"IEND", b"")
        )

    def test_reject_black_letterboxed_and_fallback_320x240_png(self):
        black_png = Path(self.temp.name) / "black_frame.png"
        black_png.write_bytes(self._encode_rgb_png(320, 240, lambda _y, w: bytes(w * 3)))
        with self.assertRaisesRegex(ValueError, "mostly-black"):
            publish.inspect_capture(black_png)

        letterbox_png = Path(self.temp.name) / "letterbox_frame.png"
        letterbox_png.write_bytes(
            self._encode_rgb_png(
                320,
                240,
                lambda y, w: bytes(w * 3)
                if y < 16
                else b"".join(
                    bytes([((x + y) & 0x7F) + 64, ((x * 3 + y) & 0x7F) + 64, ((x + y * 5) & 0x7F) + 80])
                    for x in range(w)
                ),
            )
        )
        with self.assertRaisesRegex(ValueError, "black-letterboxed"):
            publish.inspect_capture(letterbox_png)

        fallback_png = Path(self.temp.name) / "fallback_frame.png"
        fallback_png.write_bytes(
            self._encode_rgb_png(320, 240, lambda _y, w: bytes([18, 22, 28]) * w)
        )
        with self.assertRaisesRegex(ValueError, "unrendered fallback"):
            publish.inspect_capture(fallback_png)

        shell_clear_png = Path(self.temp.name) / "shell_clear_frame.png"
        shell_clear_png.write_bytes(
            self._encode_rgb_png(640, 480, lambda _y, w: bytes([9, 14, 23]) * w)
        )
        with self.assertRaisesRegex(ValueError, "unrendered fallback"):
            publish.inspect_capture(shell_clear_png)

        valid_png = Path(self.temp.name) / "valid_frame.png"
        valid_png.write_bytes(
            self._encode_rgb_png(
                320,
                240,
                lambda y, w: b"".join(
                    bytes([((x * 5 + y * 3) & 0x7F) + 48, ((x * 7 + y * 11) & 0x7F) + 48, ((x * 13 + y * 17) & 0x7F) + 48])
                    for x in range(w)
                ),
            )
        )
        suffix, size, _ = publish.inspect_capture(valid_png)
        self.assertEqual(suffix, ".png")
        self.assertGreater(size, 100)


if __name__ == "__main__":
    unittest.main()
