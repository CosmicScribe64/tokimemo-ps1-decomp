"""Report suspicious dialogue breaks without rewriting them (T-39537).

Run in Docker. Reports contain record IDs, provenance and measurements, not text.
Retained source/reviewed breaks are separate from automatically wrapped breaks;
all findings need contextual review, particularly short replies and pauses.
"""
import argparse
from collections import Counter
import json
from pathlib import Path

from tools.patch.audit_script_disc import (
    build_containers, load_units, load_jsonl, load_targets, english_payloads, FontMetrics,
)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, default=Path('build/en'))
    parser.add_argument('--out', type=Path, default=Path('build/dialogue-break-review.json'))
    args = parser.parse_args()
    supplemental = 'translations/en/supplemental.jsonl'
    containers = build_containers('disc/files', load_units('.asset-cache/text/units/units.jsonl'),
                                  load_jsonl(supplemental))
    targets = load_targets('translations/en/returned-v3.jsonl', supplemental)
    result = english_payloads(containers, targets, FontMetrics.load(args.build / 'font_metrics.json'))
    records = []
    totals = Counter()
    for ident, note in result.notes.items():
        if 'break_review' not in note:
            continue
        totals['records_checked'] += 1
        if note['break_review']:
            records.append({'id': ident, 'window': note['window'], 'breaks': note['break_review']})
        for item in note['break_review']:
            totals[item['origin']] += 1
            for reason in item['reasons']:
                totals[item['origin'] + ':' + reason] += 1
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps({'totals': dict(totals), 'records': records}, indent=2) + '\n')
    print(json.dumps(dict(totals), indent=2))
    print(args.out)


if __name__ == '__main__':
    main()
