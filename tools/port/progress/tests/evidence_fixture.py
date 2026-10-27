"""Invented receipts rooted only in a disposable trusted test bundle."""
import hashlib
import json
from pathlib import Path


def sha(data):
    return hashlib.sha256(data).hexdigest()


def store(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, sort_keys=True))
    return sha(path.read_bytes())


def approve(bundle: Path, evidence: Path, images: list[bytes]):
    """A test operator grants trust in an isolated copy, never in live code."""
    run = evidence / 'runs/fixture'
    producer = {'schema': 'tokimemo.actual-app-producer.v1', 'revision': 'invented',
                'runtime_tree_sha256': '0' * 64, 'scenario': 'invented-unit-case',
                'start_kind': 'captured-boundary', 'harness_gameplay_writes': [],
                'harness_service_completion_injection': False, 'production_script_changes': [],
                'environment': {'TOKIMEMO_PLAYABLE_SMOKE': '0'}}
    producer_hash = store(run / 'producer.json', producer)
    captures, entries, files = [], [], {}
    for index, data in enumerate(images):
        image = f'images/test-{index}.png'
        path = run / 'proof' / image
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        receipt = {'id': f'test-{index}', 'image': image, 'png_sha256': sha(data),
                   'producer_sha256': producer_hash, 'dimensions': [1, 1],
                   'display': 'X11', 'image_producer': 'app.get_viewport().get_texture().get_image()',
                   'resized': False, 'saved': True}
        receipt_hash = store(Path(str(path) + '.json'), receipt)
        captures.append(receipt)
        entries.append({'capture_id': receipt['id'], 'image': image, 'sha256': sha(data),
                        'bytes': len(data), 'receipt_sha256': receipt_hash})
        files['proof/' + image] = {'sha256': sha(data), 'size': len(data)}
        files['proof/' + image + '.json'] = {'sha256': receipt_hash, 'size': Path(str(path) + '.json').stat().st_size}
    route = {'schema': 'tokimemo.actual-app-route.v1', 'test_kind': 'production_input',
             'revision': 'invented', 'scenario': 'invented-unit-case', 'start_kind': 'captured-boundary',
             'producer_sha256': producer_hash, 'passed': True, 'failures': [],
             'checks': [{'id': 'invented-check', 'ok': True, 'outcome': 'pass'}],
             'completed_steps': 1, 'total_steps': 1, 'unattempted': [],
             'harness_gameplay_writes': [], 'host_input_api': 'Input.parse_input_event + Input.flush_buffered_events',
             'inputs': [{'key': 'enter', 'pressed': True}], 'captures': captures}
    route_hash = store(run / 'proof/route.json', route)
    artifact = {'schema': 'tokimemo.actual-app-artifacts.v1', 'outcome': 'pass',
                'test_kind': 'production_input', 'start_kind': 'captured-boundary',
                'revision': 'invented', 'runtime_tree_sha256': '0' * 64,
                'producer': {'path': 'producer.json', 'sha256': producer_hash},
                'route': {'path': 'proof/route.json', 'sha256': route_hash},
                'runtime_exit_code': 0, 'import_exit_code': 0,
                'checks': [{'id': 'invented-finalizer', 'outcome': 'pass'}], 'artifacts': files}
    artifact_hash = store(run / 'proof/artifacts.json', artifact)
    review_hash = store(evidence / 'reviews/root.json', {'scope': 'invented tests only', 'route_sha256': route_hash})
    policy = {'schema': 'tokimemo.progress-admission.v1', 'purpose': 'actual_gameplay_progress',
              'denied': [], 'routes': [{'id': 'invented-unit-case', 'directory': 'runs/fixture',
              'producer_sha256': producer_hash, 'route_sha256': route_hash,
              'artifacts_sha256': artifact_hash,
              'reviews': [{'path': 'reviews/root.json', 'sha256': review_hash}], 'captures': entries}]}
    store(bundle / 'reviewed_routes.json', policy)
    return policy


def repin(bundle: Path, evidence: Path, *, bind_links=True):
    """Test operator coherently pins changed invented documents, never live trust."""
    policy_path = bundle / 'reviewed_routes.json'
    policy = json.loads(policy_path.read_text())
    entry = policy['routes'][0]
    run = evidence / entry['directory']
    producer_hash = sha((run / 'producer.json').read_bytes())
    route_path = run / 'proof/route.json'
    route = json.loads(route_path.read_text())
    artifact_path = run / 'proof/artifacts.json'
    artifact = json.loads(artifact_path.read_text())
    if bind_links:
        route['producer_sha256'] = producer_hash
    for capture in entry['captures']:
        path = run / 'proof' / capture['image']
        sidecar = Path(str(path) + '.json')
        if bind_links:
            receipt = next(c for c in route['captures'] if c['id'] == capture['capture_id'])
            receipt['producer_sha256'] = producer_hash
            store(sidecar, receipt)
        capture['receipt_sha256'] = sha(sidecar.read_bytes())
        artifact['artifacts']['proof/' + capture['image']] = {'sha256': sha(path.read_bytes()), 'size': path.stat().st_size}
        artifact['artifacts']['proof/' + capture['image'] + '.json'] = {'sha256': sha(sidecar.read_bytes()), 'size': sidecar.stat().st_size}
    entry['producer_sha256'] = producer_hash
    entry['route_sha256'] = store(route_path, route)
    artifact['producer']['sha256'] = producer_hash
    artifact['route']['sha256'] = entry['route_sha256']
    entry['artifacts_sha256'] = store(artifact_path, artifact)
    store(policy_path, policy)
