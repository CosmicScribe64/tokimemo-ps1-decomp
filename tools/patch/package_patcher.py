"""Package the static browser patcher with a verified build (T-39512). Run in Docker.

Only page sources, the xdelta and a hash manifest enter the output. Disc data never does.
"""
import argparse
import hashlib
import json
import shutil
from pathlib import Path

from tools.patch.make_patch import SOURCE_SHA1, check_source, sha1_file

ROOT = Path(__file__).resolve().parents[2]


def package(build: Path, original: Path, output: Path):
    report = json.loads((build / "report.json").read_text())
    track = build / "track1.bin"
    audio = original.with_name("track2.bin")
    patch = build / "tokimemo-en.xdelta"
    check_source(str(original), SOURCE_SHA1)
    target_sha1 = sha1_file(track)
    if target_sha1 != report["disc"]["track1_sha1"]:
        raise ValueError("Built track no longer matches its verification report")
    if not patch.is_file() or patch.stat().st_size != report["disc"]["xdelta_bytes"]:
        raise ValueError("Build patch is missing or has changed size")
    if output.exists() and any(output.iterdir()):
        raise ValueError("Use an empty output directory to avoid stale release files")
    output.mkdir(parents=True, exist_ok=True)
    for name in ("index.html", "style.css", "app.js", "worker.js"):
        shutil.copyfile(ROOT / "patcher" / name, output / name)
    shutil.copytree(ROOT / "patcher/vendor", output / "vendor")
    (output / "assets").mkdir()
    shutil.copyfile(patch, output / "assets/tokimemo-en.xdelta")
    manifest = {
        "format": 1, "label": "Review build · " + target_sha1[:12],
        "source": {"sha1": SOURCE_SHA1, "size": original.stat().st_size},
        "audio": {"sha1": sha1_file(audio), "size": audio.stat().st_size},
        "target": {"sha1": target_sha1, "size": track.stat().st_size},
        "patch": {"file": "assets/tokimemo-en.xdelta", "size": patch.stat().st_size,
                  "sha256": hashlib.sha256(patch.read_bytes()).hexdigest()},
    }
    (output / "release.json").write_text(json.dumps(manifest, indent=2) + "\n")
    (output / ".nojekyll").touch()
    return manifest


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, default=Path("build/en/disc"))
    parser.add_argument("--original", type=Path, default=Path("disc/orig/track1.bin"))
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(package(args.build, args.original, args.out), indent=2))
