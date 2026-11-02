"""Verify English literal generation against saved build sources (T-39510).

Run from the repository root in Docker. Regenerates sources in memory from the
current translation records and compares every byte with both the live link tree
and the frozen release tree. Reports contain only counts, paths, IDs and hashes.
This proves source-generation provenance, not compiled call-site binding or
runtime coverage; use the executable and script-disc audits for those layers.
"""
import argparse
import hashlib
import json
from pathlib import Path

from tools.patch.build_english import literal_payloads, unloaded_sources
from tools.patch.text import c_literals
from tools.patch.text.containers import build_containers, load_units
from tools.patch.text.layout import english_payloads
from tools.patch.text.translations import load_jsonl, load_targets
from tools.patch.text.wrap import FontMetrics


def audit_sources(repository, sites, payloads, skipped, snapshots):
    """Compare regenerated sources without writing or trusting old audit reports."""
    by_id = {site.id: site for site in sites}
    if len(by_id) != len(sites):
        raise ValueError('Duplicate source literal IDs')
    missing = set(payloads) - set(by_id)
    if missing:
        raise ValueError('Translation has no source site: ' + sorted(missing)[0])
    excluded = {rid for rid in payloads if by_id[rid].path in skipped}
    active = {rid: value for rid, value in payloads.items() if rid not in excluded}
    if not active or not snapshots:
        raise ValueError('Source audit requires active literals and build snapshots')
    expected = c_literals.rewrite_sources(repository, sites, active)
    if not expected:
        raise ValueError('Source audit generated no changed files')
    hashes = {}
    for path, source in sorted(expected.items()):
        relative = Path(path)
        if relative.is_absolute() or '..' in relative.parts:
            raise ValueError('Source path escapes snapshot: ' + path)
        encoded = source.encode('utf-8')
        for snapshot in snapshots:
            actual = Path(snapshot) / relative
            if not actual.is_file():
                raise ValueError('Missing generated source: ' + str(actual))
            if actual.read_bytes() != encoded:
                raise ValueError('Generated source differs: ' + str(actual))
        hashes[path] = hashlib.sha256(encoded).hexdigest()
    return {
        'scope': 'exact regenerated English source bytes; no compiled binding claim',
        'active_literals': len(active),
        'excluded_literal_ids': sorted(excluded),
        'generated_files': len(hashes),
        'snapshot_count': len(snapshots),
        'files': hashes,
    }


def audit(build, link):
    units = load_units('.asset-cache/text/units/units.jsonl')
    supplemental = 'translations/en/supplemental.jsonl'
    containers = build_containers('disc/files', units, load_jsonl(supplemental))
    targets = load_targets('translations/en/returned-v3.jsonl', supplemental)
    text = english_payloads(containers, targets, FontMetrics.load(build / 'font_metrics.json'))
    return audit_sources(Path('.'), c_literals.locate_sites('.', units),
                         literal_payloads(containers, text), unloaded_sources(),
                         [link, build])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--link', type=Path, default=Path('build/en'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.build, args.link)
    args.out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(f"Verified {result['active_literals']} literals in "
          f"{result['generated_files']} source files across "
          f"{result['snapshot_count']} snapshots")
