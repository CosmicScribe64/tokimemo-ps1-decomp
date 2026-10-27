"""Bounded source-predicate implementation-gap planning, not a readiness gate."""
from __future__ import annotations
import argparse
import json
from pathlib import Path
from .readiness import Invalid, require, read_json, file_identity, relative_file, runtime_identity, unique_ids, canonical_sha

IMPLEMENTATION = {'implemented_subset', 'implementation_unreviewed', 'known_unsupported'}
EVIDENCE = {'untested', 'verified', 'verified_failure', 'regression'}

def inventory(repo, path, expected_sha256):
    require(file_identity(path)['sha256'] == expected_sha256, 'preflight inventory differs from externally pinned hash')
    data = read_json(path)
    require(data.get('schema') == 'tokimemo.month-boundary-preflight.v1', 'unsupported preflight schema')
    for relative, pin in data['source_pins'].items():
        require(file_identity(relative_file(repo, relative))['sha256'] == pin['sha256'], 'outdated source: ' + relative)
        text = relative_file(repo, relative).read_text()
        require(all(anchor in text for anchor in pin['anchors']), 'source locator no longer matches: ' + relative)
    policy = read_json(relative_file(repo, data['policy']['path']))
    require(file_identity(relative_file(repo, data['policy']['path']))['sha256'] == data['policy']['sha256'], 'outdated readiness policy')
    require(policy.get('schema') == 'tokimemo.readiness-policy.v1' and data['policy']['coverage_status'] == policy.get('coverage_status'), 'policy identity/status differs')
    policy_checkpoints = {(s['id'], c['id']) for s in policy['scenarios'] for c in s['checkpoints']}
    cases = unique_ids(data['cases'], 'preflight cases')
    for cid, case in cases.items():
        require(case['implementation'] in IMPLEMENTATION and case['evidence']['status'] in EVIDENCE, 'invalid implementation/evidence state: ' + cid)
        require(case['owner'] and case['dependencies'] and case['source_locators'], 'missing owner/dependencies/source locators: ' + cid)
        require(case['native_mode'] == data['registry']['native_mode'] == 0x38 and data['registry']['path'] in data['source_pins'], 'case must retain existing native month mode registry')
        require(tuple(case['policy_checkpoint']) in policy_checkpoints, 'case must reference an existing readiness checkpoint: ' + cid)
        for locator in case['source_locators']:
            require(locator['path'] in data['source_pins'] and locator['symbol'], 'unbound source locator: ' + cid)
        for evidence in case['evidence'].get('references', []):
            require(file_identity(relative_file(repo, evidence['path']))['sha256'] == evidence['sha256'], 'outdated evidence: ' + cid)
        if case['evidence']['status'] in ['verified', 'regression']:
            require(case['evidence'].get('runtime_identity_sha256') and case['evidence'].get('references'), 'verified case lacks exact runtime/review evidence')
            require(isinstance(case['evidence'].get('selection'), dict) and case['evidence']['selection'], 'verified case lacks explicit reviewed recipe selection')
    return data, cases

def selected_ids(operation, year, month, vacation=None, day=None):
    require(type(year) is int and 95 <= year <= 98 and type(month) is int and 1 <= month <= 12, 'explicit source-profile year/month required')
    require((95,4) <= (year,month) <= (98,3), 'date outside original CALROM April95..March98 profile')
    if operation == 'month-start':
        require(day in [None,1], 'month-start recipe requires day1')
        if year == 95 and month == 5: return ['ordinary-may95']
        if month % 3: return ['ordinary-month']
        if month == 9: return ['september96-skip' if year == 96 else 'september-notice']
        return [{3:'quarter-march',6:'quarter-june',12:'quarter-december'}[month]]
    if operation == 'year-roll':
        require(month == 12 and day == 31 and year < 98, 'year-roll recipe is explicit December31 to January1 within supported years')
        return ['calendar-year-roll', 'ordinary-month']
    if operation == 'day-bank':
        require(type(vacation) is int and vacation in range(4), 'day-bank requires explicit current vacation0..3')
        require(type(day) is int and 1 <= day <= 31, 'day-bank requires explicit current day')
        # This selector belongs to hud_day_bgm_load; it does NOT invent that
        # call in the September96 month-start branch that skips it.
        if year == 96 and month == 9 and day == 1: return ['bank-september96-override']
        if vacation: return ['bank-vacation-' + str(vacation)]
        season = 0 if month in [3,4,5] else 1 if month in [6,7,8] else 2 if month in [9,10,11] else 3
        return ['bank-season-' + str(season)]
    raise Invalid('unknown bounded preflight operation')

def report(repo, path, expected_sha256, runtime_root, case_ids, requested_selection=None):
    data, cases = inventory(repo, path, expected_sha256)
    require(case_ids and len(case_ids) == len(set(case_ids)), 'select unique explicit cases')
    current_runtime = runtime_identity(runtime_root)
    results = []
    for cid in case_ids:
        require(cid in cases, 'unknown case: ' + cid)
        case = cases[cid]
        evidence = case['evidence']
        same_runtime = evidence.get('runtime_identity_sha256') == current_runtime['sha256']
        same_selection = isinstance(requested_selection, dict) and bool(requested_selection) and isinstance(evidence.get('selection'), dict) and canonical_sha(requested_selection) == canonical_sha(evidence['selection'])
        state = 'known_unsupported' if case['implementation'] == 'known_unsupported' else (evidence['status'] if evidence['status'] in ['verified','regression'] and same_runtime and same_selection else 'untested')
        results.append({'id': cid, 'status': state, 'implementation': case['implementation'],
            'historical_evidence': evidence['status'], 'evidence_applies_to_selected_runtime': same_runtime,
            'evidence_applies_to_requested_selection': same_selection,
            'historical_scope': evidence.get('scope', 'none'), 'predicate': case['predicate'], 'source_locators': case['source_locators'],
            'owner': case['owner'], 'dependencies': case['dependencies'], 'evidence_references': evidence.get('references', []),
            'policy_checkpoint': case['policy_checkpoint'], 'native_mode': case['native_mode']})
    return {'schema': 'tokimemo.month-boundary-preflight-report.v1', 'scope': 'explicit bounded source cases only; no exhaustive reachability or gameplay certification',
        'inventory_sha256': expected_sha256, 'runtime_identity': current_runtime, 'cases': results,
        'known_expected_stops': [r['id'] for r in results if r['status'] == 'known_unsupported'],
        'targeted_evidence_needed': [r['id'] for r in results if r['status'] == 'untested'],
        'regressions': [r['id'] for r in results if r['status'] == 'regression'],
        'preflight_clear_for_selected_cases': all(r['status'] == 'verified' for r in results),
        'complete_port_ready': False, 'route_reachability_proven': False,
        'readiness_policy_modified': False, 'policy_coverage_status': data['policy']['coverage_status']}

def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo-root', type=Path, required=True)
    parser.add_argument('--inventory', type=Path, required=True)
    parser.add_argument('--expected-inventory-sha256', required=True)
    parser.add_argument('--runtime-root', type=Path, required=True)
    parser.add_argument('--case', action='append', default=[])
    parser.add_argument('--operation', choices=['month-start','day-bank','year-roll'])
    parser.add_argument('--year', type=int);parser.add_argument('--month', type=int);parser.add_argument('--day', type=int);parser.add_argument('--vacation', type=int)
    args = parser.parse_args(argv)
    try:
        selected = args.case or selected_ids(args.operation, args.year, args.month, args.vacation, args.day)
        selection = {'operation':args.operation,'year':args.year,'month':args.month,'day':1 if args.operation == 'month-start' and args.day is None else args.day}
        if args.vacation is not None: selection['vacation'] = args.vacation
        value = report(args.repo_root, args.inventory, args.expected_inventory_sha256, args.runtime_root, selected, selection)
        value['selection'] = selection
        print(json.dumps(value, indent=2))
        return 0 if value['preflight_clear_for_selected_cases'] else 2
    except (Invalid, KeyError, TypeError, ValueError, OSError) as exc:
        print(json.dumps({'schema':'tokimemo.month-boundary-preflight-report.v1','preflight_clear_for_selected_cases':False,'complete_port_ready':False,'error':str(exc)}))
        return 2

if __name__ == '__main__':
    raise SystemExit(main())
