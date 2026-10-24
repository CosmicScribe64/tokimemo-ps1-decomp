"""Deterministic engine-independent media manifests and safe local output paths.

T-28002/T-28003. Run asset tools in Docker. Real exports belong beneath
the repository's ignored .asset-cache directory. No timestamp enters a manifest.
"""
import hashlib
import json
from pathlib import Path

SCHEMA = "tokimemo.media.v1"
REPOSITORY = Path(__file__).resolve().parents[2]
CACHE = REPOSITORY / ".asset-cache"


def digest(data):
    return hashlib.sha256(data).hexdigest()


def asset_id(path, content_hash):
    """Identify an original path/content pair consistently across input wrappers."""
    return "asset-" + digest((path + "\0" + content_hash).encode("utf-8"))


def output_root(path):
    """Require a real directory beneath the local cache, rejecting symlink escapes."""
    requested = Path(path)
    if not requested.is_absolute():
        requested = REPOSITORY / requested
    cache = CACHE.resolve()
    # Resolving an existing .asset-cache symlink could redirect to original data.
    if cache != CACHE:
        raise ValueError(".asset-cache must not be a symlink")
    resolved = requested.resolve()
    if resolved == cache or cache not in resolved.parents:
        raise ValueError("output must be below .asset-cache/")
    return resolved


def write_bytes(path, data):
    """Create or verify an output; never replace different existing bytes."""
    path = Path(path)
    checked = output_root(path)
    checked.parent.mkdir(parents=True, exist_ok=True)
    try:
        with checked.open("xb") as stream:
            stream.write(data)
    except FileExistsError:
        if not checked.is_file() or checked.read_bytes() != data:
            raise ValueError("output already exists with different bytes: %s" % checked)
    return {"path": str(checked.relative_to(REPOSITORY)), "size": len(data), "sha256": digest(data)}


def write_manifest(path, value):
    return write_bytes(path, (json.dumps(value, ensure_ascii=True, sort_keys=True,
                                        indent=2) + "\n").encode("utf-8"))


def summarize(records):
    totals = {}
    formats = {}
    for record in records:
        status = record["status"]
        totals[status] = totals.get(status, 0) + 1
        name = record.get("format", "unknown")
        formats[name] = formats.get(name, 0) + 1
    return {"entries": len(records), "status": totals, "formats": formats}
