"""Source-checked translation corrections with unchanged controls (T-39538)."""
import hashlib
import json
from pathlib import Path

from .segments import MARKER


REPAIRS = json.loads((Path(__file__).resolve().parents[3] /
                      'translations/en/reviewed-targets.json').read_text())


def reviewed_target(record_id, source, row, repairs=REPAIRS):
    repair = repairs.get(record_id)
    if repair is None:
        return row
    previous = row.get('target_text', '')
    if hashlib.sha256(source).hexdigest() != repair['source_sha256']:
        raise ValueError('Source changed for reviewed translation: ' + record_id)
    if hashlib.sha256(previous.encode('utf-8')).hexdigest() != repair['previous_target_sha256']:
        raise ValueError('Target changed for reviewed translation: ' + record_id)
    target = repair['target_text']
    if MARKER.findall(previous) != MARKER.findall(target):
        raise ValueError('Control markers changed in reviewed translation: ' + record_id)
    return {**row, 'target_text': target}
