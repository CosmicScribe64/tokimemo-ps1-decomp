"""Independently reconstruct and validate a conditional bank graph export."""
import argparse
import json
import re
from pathlib import Path

from tools.assets import manifest
from tools.assets.sequence_bank_provenance import (
    PROFILE_SHA256, build_graph, sha256, validated_sequence_packages, verify_authority,
)

ROOT = Path(manifest.REPOSITORY).resolve()
DEFAULT_SEQUENCE_MANIFEST = ROOT / (
    ".asset-cache/music/sequence-candidate-v9/exports-v9-json/manifest.json")
DEFAULT_OUTPUT = ROOT / ".asset-cache/music/bank-provenance/exports-v3"
SHA = re.compile(r"^[0-9a-f]{64}$")
MANIFEST_KEYS = {"schema", "sequence_catalog_sha256", "trusted_profile_sha256",
                 "output", "coverage", "claim_scope"}


def _validate_output_manifest(value, out_dir, catalog_sha, graph):
    if not isinstance(value, dict) or set(value) != MANIFEST_KEYS:
        raise ValueError("bank provenance output manifest fields are invalid")
    if value["schema"] != "tokimemo.sequence-bank-provenance-manifest.v2":
        raise ValueError("unsupported bank provenance output manifest schema")
    if (not isinstance(value["sequence_catalog_sha256"], str)
            or not re.fullmatch(r"[0-9a-f]{64}", value["sequence_catalog_sha256"])
            or value["sequence_catalog_sha256"] != catalog_sha):
        raise ValueError("manifest sequence catalog pin differs")
    if (not isinstance(value["trusted_profile_sha256"], str)
            or not SHA.fullmatch(value["trusted_profile_sha256"])
            or value["trusted_profile_sha256"] != PROFILE_SHA256):
        raise ValueError("manifest trusted profile pin differs")
    if not isinstance(value["coverage"], dict) or any(
            not isinstance(count, int) or isinstance(count, bool)
            for count in value["coverage"].values()):
        raise ValueError("manifest coverage fields must be integer counts")
    if not isinstance(value["claim_scope"], str):
        raise ValueError("manifest claim scope must be text")
    if value["coverage"] != graph["coverage"] or value["claim_scope"] != graph["classification"]:
        raise ValueError("manifest coverage or claim scope differs")
    descriptor = value["output"]
    if not isinstance(descriptor, dict) or set(descriptor) != {"path", "size", "sha256"}:
        raise ValueError("graph output descriptor fields are invalid")
    expected_rel = (out_dir / "conditional-bank-provenance.json").resolve().relative_to(ROOT).as_posix()
    if descriptor["path"] != expected_rel:
        raise ValueError("graph output path does not match selected output directory")
    if (not isinstance(descriptor["size"], int) or isinstance(descriptor["size"], bool)
            or descriptor["size"] < 0 or not isinstance(descriptor["sha256"], str)
            or not SHA.fullmatch(descriptor["sha256"])):
        raise ValueError("graph output size or hash descriptor is malformed")
    graph_path = manifest.output_root(ROOT / descriptor["path"])
    actual = graph_path.read_bytes()
    expected = (json.dumps(graph, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode()
    if descriptor["size"] != len(actual) or descriptor["sha256"] != sha256(actual):
        raise ValueError("graph output size/hash descriptor mismatch")
    if actual != expected:
        raise ValueError("graph differs from independently rebuilt original provenance")
    return actual


def validate(sequence_manifest_path=DEFAULT_SEQUENCE_MANIFEST, output_dir=DEFAULT_OUTPUT):
    """Validate selected original inputs, the exact selected manifest and output."""
    authority = verify_authority()
    catalog, packages, catalog_sha = validated_sequence_packages(sequence_manifest_path)
    graph = build_graph(authority, catalog, packages, catalog_sha)
    out = manifest.output_root(output_dir)
    manifest_path = manifest.output_root(out / "manifest.json")
    output_manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    actual = _validate_output_manifest(output_manifest, out, catalog_sha, graph)
    return {"status": "valid", "graph_sha256": sha256(actual),
            "coverage": graph["coverage"], "sequence_rows": len(graph["sequences"]),
            "conditional_requests": len(graph["requests"])}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sequence-manifest", type=Path, default=DEFAULT_SEQUENCE_MANIFEST)
    parser.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    print(json.dumps(validate(args.sequence_manifest, args.output_dir), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
