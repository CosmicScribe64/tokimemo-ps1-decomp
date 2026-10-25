"""Export conditionally proven sequence/VAB request contexts.

Run in project Docker. Original/source/native facts and the selected SEQ
manifest are validated before output. No playback or renderer claim is made.
"""
import argparse
import json
from pathlib import Path

from tools.assets import manifest
from tools.assets.sequence_bank_provenance import (
    build_graph, sha256, validated_sequence_packages, verify_authority,
)

ROOT = Path(manifest.REPOSITORY).resolve()
DEFAULT_SEQUENCE_MANIFEST = ROOT / (
    ".asset-cache/music/sequence-candidate-v9/exports-v9-json/manifest.json")
DEFAULT_OUTPUT = ROOT / ".asset-cache/music/bank-provenance/exports-v3"


def export(sequence_manifest_path=DEFAULT_SEQUENCE_MANIFEST, output_dir=DEFAULT_OUTPUT):
    """Verify original proof and selected sequence directory, then export graph."""
    authority = verify_authority()
    catalog, packages, catalog_sha = validated_sequence_packages(sequence_manifest_path)
    graph = build_graph(authority, catalog, packages, catalog_sha)
    out = manifest.output_root(output_dir)
    graph_bytes = (json.dumps(graph, ensure_ascii=True, sort_keys=True, indent=2) + "\n").encode()
    graph_record = manifest.write_bytes(out / "conditional-bank-provenance.json", graph_bytes)
    output_manifest = {
        "schema": "tokimemo.sequence-bank-provenance-manifest.v2",
        "sequence_catalog_sha256": catalog_sha,
        "trusted_profile_sha256": graph["source_proof"]["trusted_profile_sha256"],
        "output": graph_record,
        "coverage": graph["coverage"],
        "claim_scope": graph["classification"],
    }
    manifest_record = manifest.write_manifest(out / "manifest.json", output_manifest)
    return {"graph": graph_record, "manifest": manifest_record, "coverage": graph["coverage"]}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sequence-manifest", type=Path, default=DEFAULT_SEQUENCE_MANIFEST)
    parser.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    print(json.dumps(export(args.sequence_manifest, args.output_dir), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
