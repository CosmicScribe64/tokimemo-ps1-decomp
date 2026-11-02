"""Explicit repairs of proven malformed source text (T-39511).

Raw sources and exported translation rows remain immutable. Both hashes must
match, so a future source/export/translation edit requires reviewing the repair.
"""
import hashlib
import json
from pathlib import Path

REPAIRS = json.loads((Path(__file__).resolve().parents[3] /
                      'translations/en/anomaly-repairs.json').read_text())


def repair_target(record_id, source, previous_target, repairs=REPAIRS):
    repair = repairs.get(record_id)
    if repair is None:
        return None
    if hashlib.sha256(source).hexdigest() != repair['source_sha256']:
        raise ValueError('Source changed for reviewed anomaly repair: ' + record_id)
    if hashlib.sha256(previous_target.encode('utf-8')).hexdigest() != repair['previous_target_sha256']:
        raise ValueError('Translation changed for reviewed anomaly repair: ' + record_id)
    return repair['target_text']
