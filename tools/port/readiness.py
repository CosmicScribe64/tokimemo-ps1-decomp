"""Validate bounded, independently reviewed playthrough evidence; Docker only.

Hashes bind records to files, not to semantic truth. A separate coverage audit
and independent receipt review are mandatory. This tool never declares the
complete port ready. Adapters may reference immutable raw producer outputs.
"""
import argparse
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import re
import sys

MAX_JSON_BYTES = 8 * 1024 * 1024
MAX_FILE_BYTES = 1024 * 1024 * 1024
MAX_TOTAL_BYTES = 4 * 1024 * 1024 * 1024
MAX_FILES = 10000
MAX_RECORDS = 512
OUTCOMES = {"pass", "fail", "skip", "unsupported"}
KINDS = {"input_log", "state_trace", "viewport", "save", "audio", "reference_comparison"}
SHA = re.compile(r"[0-9a-f]{64}\Z")
IDENTIFIER = re.compile(r"[A-Za-z0-9_.-]{1,96}\Z")


class Invalid(ValueError):
    """A deterministic structural or integrity failure."""


def canonical_sha(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":"), ensure_ascii=True).encode()).hexdigest()


def require(condition, message):
    if not condition:
        raise Invalid(message)


def identifier(value):
    require(isinstance(value, str) and IDENTIFIER.fullmatch(value), "invalid identifier")
    return value


def sha(value):
    require(isinstance(value, str) and SHA.fullmatch(value), "invalid SHA256")
    return value


def no_symlink(path):
    path = Path(path).absolute()
    require(".." not in path.parts, "path traversal is forbidden")
    for part in (path, *path.parents):
        require(not part.is_symlink(), "symbolic links are forbidden")
    return path


def relative_file(root, value):
    require(isinstance(value, str) and value and "\\" not in value and ":" not in value, "invalid relative artifact path")
    path = Path(value)
    require(not path.is_absolute() and all(p not in ("", ".", "..") for p in value.split("/")), "path traversal is forbidden")
    target = no_symlink(Path(root) / path)
    require(Path(root).resolve() in target.resolve().parents, "artifact escaped its root")
    return target


def file_identity(path):
    path = no_symlink(path)
    require(path.is_file(), "file is missing")
    size = path.stat().st_size
    require(size <= MAX_FILE_BYTES, "file exceeds bounded size")
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return {"sha256": digest.hexdigest(), "bytes": size}


def _unique_object(pairs):
    result = {}
    for key, value in pairs:
        require(key not in result, "duplicate JSON key")
        result[key] = value
    return result


def read_json(path):
    path = no_symlink(path)
    require(path.is_file() and path.stat().st_size <= MAX_JSON_BYTES, "JSON missing or exceeds bounded size")
    value = json.loads(path.read_text(encoding="utf-8"), object_pairs_hook=_unique_object,
                       parse_constant=lambda _: (_ for _ in ()).throw(Invalid("non-finite JSON number")))
    require(isinstance(value, dict), "JSON record must be an object")
    return value


def runtime_identity(root):
    """Hash the full current tree, including new files; omit engine/Git metadata."""
    root = no_symlink(root)
    require(root.is_dir(), "runtime root is missing")
    inventory, total = [], 0
    for path in sorted(root.rglob("*")):
        relative = path.relative_to(root)
        no_symlink(path)
        if any(part in (".godot", ".git") for part in relative.parts):
            continue
        if path.is_dir():
            continue
        record = file_identity(path)
        total += record["bytes"]
        require(len(inventory) < MAX_FILES and total <= MAX_TOTAL_BYTES, "runtime inventory exceeds bounded size")
        inventory.append({"path": relative.as_posix(), **record})
    require(inventory, "runtime tree is empty")
    return {"sha256": canonical_sha(inventory), "files": len(inventory), "bytes": total}


def rows(value, label):
    require(isinstance(value, list) and len(value) <= MAX_RECORDS, label + " must be a bounded array")
    return value


def unique_ids(records, label):
    result = {}
    for record in rows(records, label):
        require(isinstance(record, dict), label + " contains a non-object")
        key = identifier(record.get("id"))
        require(key not in result, "duplicate " + label + " identifier: " + key)
        result[key] = record
    return result


def utc(value):
    require(isinstance(value, str), "timestamp must be a string")
    result = datetime.fromisoformat(value.replace("Z", "+00:00"))
    require(result.tzinfo is not None and result.utcoffset().total_seconds() == 0, "timestamp must be UTC")
    return result


def raw_failures(record, label, require_audio=False, scenario_id=None, start_kind=None, runtime_root=None, script_sha=None):
    """Fixed v1 actual-app schemas; never interpret arbitrary gameplay booleans."""
    supported = {"tokimemo.actual-app-producer.v1", "tokimemo.actual-app-route.v1", "tokimemo.actual-app-artifacts.v1"}
    schema = record.get("schema")
    require(schema in supported, "unsupported raw producer schema")
    failures = []
    if record.get("start_kind") != start_kind:
        failures.append(label + ": raw producer start kind was relabeled")
    if schema != "tokimemo.actual-app-artifacts.v1" and record.get("scenario") != scenario_id:
        failures.append(label + ": raw producer scenario was relabeled")
    if record.get("synthetic") is True:
        failures.append(label + ": synthetic producer evidence")
    raw_ids = set()
    for check in rows(record.get("checks", []), "raw checks"):
        require(isinstance(check, dict), "raw check must be an object")
        raw_id = check.get("id")
        require(isinstance(raw_id, str) and 0 < len(raw_id) <= 256, "invalid raw check ID")
        if raw_id in raw_ids:
            failures.append(label + ": duplicate raw check ID: " + raw_id)
        raw_ids.add(raw_id)
        outcome = check.get("outcome")
        if outcome not in OUTCOMES or outcome != "pass":
            failures.append(label + ": raw check not pass: " + str(check.get("id", "unknown")))
    if schema == "tokimemo.actual-app-route.v1":
        if record.get("passed") is not True or record.get("test_kind") != "production_input":
            failures.append(label + ": actual-app route did not pass production input")
        for key in ("failures", "unattempted"):
            if record.get(key):
                failures.append(label + ": nonempty producer " + key)
        if require_audio and record.get("audio_verified") is not True:
            failures.append(label + ": required original audio is unverified")
    if schema == "tokimemo.actual-app-artifacts.v1":
        if record.get("test_kind") != "production_input" or record.get("outcome") != "pass" or record.get("runtime_exit_code") != 0 or record.get("import_exit_code") != 0:
            failures.append(label + ": actual-app artifact producer failed")
    if schema == "tokimemo.actual-app-producer.v1":
        require(record.get("observer_sha256") == script_sha, "producer script hash differs from raw producer record")
        inventory = record.get("production_source_files")
        require(isinstance(inventory, dict) and inventory and len(inventory) <= MAX_FILES, "raw runtime subset inventory missing")
        require(canonical_sha(inventory) == record.get("runtime_tree_sha256"), "raw runtime subset digest mismatch")
        for path, expected in inventory.items():
            require(file_identity(relative_file(runtime_root, path))["sha256"] == sha(expected), "raw producer runtime subset is stale: " + path)
        if record.get("harness_gameplay_writes") != [] or record.get("harness_service_completion_injection") is not False:
            failures.append(label + ": gameplay state or service completion was injected")
        actual_viewport = "actual root viewport get_texture().get_image(); no cached presenter image or rescale"
        if record.get("image_producer") != actual_viewport:
            failures.append(label + ": actual viewport producer is unrecognized or synthetic")
        if require_audio:
            audio = record.get("audio", {})
            if not isinstance(audio, dict) or audio.get("backend") == "Dummy" or audio.get("captured") is not True or audio.get("native_audio_verified") is not True:
                failures.append(label + ": required audio lacks original audible capture")
    return sorted(set(failures))


def record_files(directory):
    directory = no_symlink(directory)
    require(directory.is_dir(), "receipt/review directory is missing")
    paths = sorted(directory.glob("*.json"))
    require(len(paths) <= MAX_RECORDS, "too many receipt/review files")
    return [(path, read_json(path)) for path in paths]


def validate(policy_path, expected_policy_sha256, runtime_root, content_root, evidence_root,
             receipt_dir, review_dir, now=None):
    """Return scope acceptance and missing scenarios; never a global ready claim."""
    result = {"schema": "tokimemo.readiness-result.v1", "scope_id": None,
              "independently_reviewed": False, "full_port_complete": False,
              "missing_scenarios": [], "accepted_scenarios": [], "out_of_scope_declarations": [], "errors": []}
    errors = result["errors"]
    try:
        policy = read_json(policy_path)
        # Keep a missing list even when structural/pin validation fails later.
        result["missing_scenarios"] = sorted({r["id"] for r in policy.get("scenarios", []) if isinstance(r, dict) and isinstance(r.get("id"), str)})
        result["scope_id"] = policy.get("scope_id")
        policy_sha = file_identity(policy_path)["sha256"]
        require(policy_sha == sha(expected_policy_sha256), "externally pinned policy SHA256 mismatch")
        require(policy.get("schema") == "tokimemo.readiness-policy.v1", "unknown policy schema")
        identifier(policy.get("scope_id"))
        author = identifier(policy.get("author_id"))
        reviewer_ids = rows(policy.get("reviewer_ids"), "reviewer_ids")
        require(reviewer_ids and len(reviewer_ids) == len(set(reviewer_ids)), "reviewer IDs missing or duplicated")
        for reviewer in reviewer_ids:
            identifier(reviewer)
        require(policy.get("coverage_status") in ("provisional", "audited"), "unknown policy coverage status")
        if policy["coverage_status"] != "audited":
            errors.append("policy coverage remains provisional; independent coverage audit required")
        age = policy.get("max_age_seconds")
        require(type(age) is int and 1 <= age <= 366 * 86400, "invalid policy evidence age bound")
        scenarios = unique_ids(policy.get("scenarios"), "scenario")
        require(scenarios, "policy scenarios are empty")
        for scenario in scenarios.values():
            identifier(scenario.get("start_kind"))
            checkpoints = unique_ids(scenario.get("checkpoints"), "checkpoint")
            require(checkpoints, "scenario checkpoints are empty")
            for checkpoint in checkpoints.values():
                raw_ids = checkpoint.get("raw_check_ids")
                if not raw_ids:
                    errors.append("policy checkpoint raw checks not pinned")
                else:
                    rows(raw_ids, "policy raw check IDs")
                    require(all(isinstance(value, str) and 0 < len(value) <= 256 for value in raw_ids) and len(raw_ids) == len(set(raw_ids)), "invalid or duplicate policy raw check IDs")
                requirements = rows(checkpoint.get("evidence"), "checkpoint evidence")
                require(requirements, "checkpoint evidence requirements are empty")
                kinds = set()
                for requirement in requirements:
                    require(isinstance(requirement, dict) and requirement.get("kind") in KINDS, "unknown policy evidence kind")
                    require(requirement["kind"] not in kinds, "duplicate policy evidence kind")
                    kinds.add(requirement["kind"])
                    origins = rows(requirement.get("allowed_origins"), "allowed origins")
                    require(origins and len(origins) == len(set(origins)) and "synthetic" not in origins, "invalid allowed origins")
                    for origin in origins:
                        identifier(origin)
        content_records = unique_ids(policy.get("content_files"), "content")
        if not content_records:
            errors.append("content inputs not pinned")
        content_inventory = []
        require(len({r.get("path") for r in content_records.values()}) == len(content_records), "duplicate content path alias")
        for key, record in sorted(content_records.items()):
            actual = file_identity(relative_file(content_root, record.get("path")))
            require(actual["sha256"] == sha(record.get("sha256")) and type(record.get("bytes")) is int and actual["bytes"] == record["bytes"], "current content identity mismatch: " + key)
            content_inventory.append({"id": key, "path": record["path"], **actual})
        content_root_path = no_symlink(content_root)
        require(content_root_path.is_dir(), "isolated content root is missing")
        actual_content_paths = set()
        for path in content_root_path.rglob("*"):
            no_symlink(path)
            if path.is_file():
                actual_content_paths.add(path.relative_to(content_root_path).as_posix())
                require(len(actual_content_paths) <= MAX_FILES, "content tree exceeds bounded size")
        require(actual_content_paths == {r["path"] for r in content_inventory}, "policy does not pin the entire isolated content tree")
        content_sha = canonical_sha(content_inventory)
        execution = policy.get("execution")
        if not isinstance(execution, dict):
            errors.append("execution engine/platform/container identities not pinned")
            execution = {}
        else:
            identifier(execution.get("engine_id"))
            identifier(execution.get("platform_id"))
            sha(execution.get("container_image_sha256"))
        runtime = runtime_identity(runtime_root)
        result["runtime_tree_sha256"] = runtime["sha256"]
        result["content_inputs_sha256"] = content_sha
        receipts = record_files(receipt_dir)
        reviews = record_files(review_dir)
        unique_ids([record for _, record in receipts], "receipt")
        unique_ids([record for _, record in reviews], "review")
        for _, review in reviews:
            require(review.get("schema") in ("tokimemo.readiness-policy-review.v1", "tokimemo.readiness-review.v1"), "unknown review schema")
            require(review.get("policy_sha256") == policy_sha, "stale review policy identity")
        policy_audited = False
        for _, review in reviews:
            if review.get("schema") == "tokimemo.readiness-policy-review.v1" and review.get("policy_sha256") == policy_sha:
                require(review.get("reviewer_id") in reviewer_ids and review.get("reviewer_id") != author, "policy self-review or unapproved reviewer")
                require(review.get("decision") == "accepted" and review.get("coverage_audit_accepted") is True and review.get("unresolved_findings") == [], "policy audit is not accepted")
                policy_audited = True
        if not policy_audited:
            errors.append("independent policy coverage audit missing")
        now = now or datetime.now(timezone.utc)
        seen_scenarios, total_artifact_bytes = set(), 0
        for path, receipt in receipts:
            label = path.name
            before_errors = len(errors)
            try:
                require(receipt.get("schema") == "tokimemo.readiness-receipt.v1", "unknown receipt schema")
                scenario_id = receipt.get("scenario_id")
                require(scenario_id in scenarios, "unknown receipt scenario")
                require(scenario_id not in seen_scenarios, "duplicate scenario receipt")
                seen_scenarios.add(scenario_id)
                scenario = scenarios[scenario_id]
                require(receipt.get("start_kind") == scenario["start_kind"], "receipt start kind mismatch")
                producer = identifier(receipt.get("producer_id"))
                require(receipt.get("status") in OUTCOMES, "unknown receipt outcome")
                require(receipt["status"] == "pass", "receipt outcome is not pass")
                require(receipt.get("synthetic") is False, "receipt is synthetic or synthesis flag absent")
                require(receipt.get("policy_sha256") == policy_sha, "receipt policy identity mismatch")
                require(receipt.get("runtime_tree_sha256") == runtime["sha256"], "receipt runtime is stale")
                require(receipt.get("content_inputs_sha256") == content_sha, "receipt content identity is stale")
                run_execution = receipt.get("execution")
                require(isinstance(run_execution, dict) and execution, "receipt execution identity missing or unpinned")
                for key in ("engine_id", "platform_id", "container_image_sha256"):
                    require(run_execution.get(key) == execution.get(key), "receipt execution identity mismatch: " + key)
                started, finished = utc(receipt.get("started_at")), utc(receipt.get("finished_at"))
                require(started <= finished <= now and (now - started).total_seconds() <= age, "receipt evidence is stale or has invalid timestamps")
                for key in ("skipped", "unsupported", "unattempted", "failures"):
                    require(not receipt.get(key), "receipt has " + key)
                checks = unique_ids(receipt.get("checks"), "check")
                require(checks, "receipt contains no checks")
                passed_checkpoints = set()
                for check in checks.values():
                    require(check.get("status") in OUTCOMES and check["status"] == "pass", "check outcome is unknown or not pass: " + check["id"])
                    checkpoint_id = identifier(check.get("checkpoint_id"))
                    passed_checkpoints.add(checkpoint_id)
                checkpoint_map = {row["id"]: row for row in scenario["checkpoints"]}
                require(set(checkpoint_map) <= passed_checkpoints, "required checkpoint omitted")
                evidence = unique_ids(receipt.get("evidence"), "evidence")
                raw_outputs = unique_ids(receipt.get("raw_outputs"), "raw output")
                require(raw_outputs, "immutable raw producer outputs are missing")
                require(run_execution.get("producer_script_artifact_id") in raw_outputs, "hashed producer script artifact is missing")
                script_record = raw_outputs[run_execution["producer_script_artifact_id"]]
                require(script_record.get("record_type") == "producer_script", "producer script must be a typed script artifact")
                require(not set(evidence) & set(raw_outputs), "duplicate artifact IDs across evidence/raw outputs")
                evidence_pairs, seen_paths, raw_roles = set(), set(), {}
                for record in [*evidence.values(), *raw_outputs.values()]:
                    target = relative_file(evidence_root, record.get("path"))
                    require(str(target) not in seen_paths, "artifact path alias/duplicate")
                    seen_paths.add(str(target))
                    actual = file_identity(target)
                    total_artifact_bytes += actual["bytes"]
                    require(total_artifact_bytes <= MAX_TOTAL_BYTES, "artifact set exceeds bounded size")
                    require(actual["sha256"] == sha(record.get("sha256")) and type(record.get("bytes")) is int and actual["bytes"] == record["bytes"], "artifact hash/size mismatch: " + record["id"])
                    if record["id"] in raw_outputs and record.get("record_type") == "producer_json":
                        raw_record = read_json(target)
                        role = raw_record.get("schema")
                        require(role not in raw_roles, "duplicate raw producer schema role")
                        raw_roles[role] = (raw_record, record, target)
                        require_audio = any(r["kind"] == "audio" for c in checkpoint_map.values() for r in c["evidence"])
                        errors.extend(raw_failures(raw_record, label + "/" + record["id"], require_audio, scenario_id, scenario["start_kind"], runtime_root, script_record.get("sha256")))
                        if raw_record.get("schema") == "tokimemo.actual-app-route.v1":
                            declarations = rows(raw_record.get("not_covered", []), "raw outside-scope declarations")
                            for declaration in declarations:
                                require(isinstance(declaration, dict), "invalid outside-scope declaration")
                                require(declaration.get("id") not in checkpoint_map, "required checkpoint declared outside scope")
                                result["out_of_scope_declarations"].append({"scenario_id": scenario_id, **declaration})
                    elif record["id"] in raw_outputs:
                        require(record["id"] == script_record["id"] and record.get("record_type") == "producer_script", "unknown raw artifact type; producer JSON must be declared and parsed")
                    if record["id"] in evidence:
                        require(record.get("synthetic") is False, "synthetic evidence or absent synthesis flag")
                        require(record.get("kind") in KINDS, "unknown evidence kind")
                        checkpoint_id = record.get("checkpoint_id")
                        require(checkpoint_id in checkpoint_map, "evidence checkpoint is not in policy")
                        requirements = {r["kind"]: r for r in checkpoint_map[checkpoint_id]["evidence"]}
                        require(record["kind"] in requirements and record.get("origin") in requirements[record["kind"]]["allowed_origins"], "evidence kind/origin not authorized by policy")
                        evidence_pairs.add((checkpoint_id, record["kind"]))
                        if record["kind"] == "reference_comparison":
                            try:
                                from tools.port import differential
                            except ModuleNotFoundError:
                                import differential
                            report = read_json(target)
                            pins = report.get("pins", {})
                            require(isinstance(pins, dict), "differential identity pins must be an object")
                            approved = policy.get("differential")
                            require(isinstance(approved, dict), "differential original/projection policy pins missing")
                            content_id = approved.get("original_executable_content_id")
                            require(content_id in content_records, "differential original executable is not a pinned content input")
                            original = content_records[content_id]
                            require(pins.get("port_tree_sha256") == runtime["sha256"], "differential port runtime is stale")
                            require(pins.get("original_executable_sha256") == original["sha256"] and pins.get("original_executable_bytes") == original["bytes"], "differential original executable identity mismatch")
                            projections = rows(approved.get("approved_projection_sha256"), "approved projection hashes")
                            require(projections and all(SHA.fullmatch(str(value)) for value in projections) and pins.get("projection_schema_sha256") in projections, "differential projection is not externally approved")
                            scoped = requirements["reference_comparison"]
                            for key in ("initial_state_sha256", "input_log_sha256"):
                                require(pins.get(key) == sha(scoped.get(key)), "differential checkpoint scope mismatch: " + key)
                            expected_producer = identifier(scoped.get("port_producer_id"))
                            require(isinstance(report.get("port_trace"), dict) and report["port_trace"].get("producer_id") == expected_producer, "differential checkpoint port producer mismatch")
                            for field, kind, pin in (("initial_state_evidence_id", "state_trace", "initial_state_sha256"),
                                                     ("input_log_evidence_id", "input_log", "input_log_sha256")):
                                linked = evidence.get(record.get(field))
                                require(isinstance(linked, dict) and linked.get("checkpoint_id") == checkpoint_id and linked.get("kind") == kind and linked.get("origin") == "actual_app" and linked.get("sha256") == pins[pin], "differential checkpoint actual input/state evidence binding mismatch: " + field)
                            differential.verify_report(report, Path(evidence_root))
                required_roles = {"tokimemo.actual-app-producer.v1", "tokimemo.actual-app-route.v1", "tokimemo.actual-app-artifacts.v1"}
                require(set(raw_roles) == required_roles, "complete producer/route/artifacts raw schema trio is required")
                producer_raw, producer_descriptor, producer_path = raw_roles["tokimemo.actual-app-producer.v1"]
                route_raw, route_descriptor, route_path = raw_roles["tokimemo.actual-app-route.v1"]
                artifact_raw = raw_roles["tokimemo.actual-app-artifacts.v1"][0]
                require(route_raw.get("producer_sha256") == producer_descriptor["sha256"], "raw route producer hash binding mismatch")
                run_root = Path(evidence_root)
                if receipt.get("producer_run_root") not in (None, "."):
                    run_root = relative_file(evidence_root, receipt["producer_run_root"])
                    require(run_root.is_dir(), "producer run root is missing")
                for key, descriptor, original_path in (("producer", producer_descriptor, producer_path), ("route", route_descriptor, route_path)):
                    reference = artifact_raw.get(key)
                    require(isinstance(reference, dict) and reference.get("sha256") == descriptor["sha256"], "raw artifact crosshash binding mismatch: " + key)
                    require(relative_file(run_root, reference.get("path")) == original_path, "raw artifact path binding mismatch: " + key)
                require(artifact_raw.get("runtime_tree_sha256") == producer_raw.get("runtime_tree_sha256"), "raw artifact/producer runtime subset identity mismatch")
                for check in checks.values():
                    ids = rows(check.get("evidence_ids"), "check evidence IDs")
                    require(ids and len(ids) == len(set(ids)), "check evidence IDs missing or duplicated")
                    require(all(key in evidence and evidence[key]["checkpoint_id"] == check["checkpoint_id"] for key in ids), "check evidence IDs do not bind its checkpoint")
                checked_raw_pairs = set()
                for check in checks.values():
                    checkpoint_id = check["checkpoint_id"]
                    require(checkpoint_id in checkpoint_map, "check checkpoint is not authorized by policy")
                    raw_check_id = check.get("raw_check_id")
                    require(raw_check_id in checkpoint_map[checkpoint_id].get("raw_check_ids", []), "check is not bound to a policy-required raw check ID")
                    raw_output_id = check.get("raw_output_id")
                    roles = [entry[0] for entry in raw_roles.values() if entry[1]["id"] == raw_output_id]
                    require(len(roles) == 1, "check raw producer output reference is missing")
                    matches = [row for row in roles[0].get("checks", []) if row.get("id") == raw_check_id]
                    require(len(matches) == 1 and matches[0].get("outcome") == "pass", "policy-required raw check is missing, duplicated or not pass")
                    checked_raw_pairs.add((checkpoint_id, raw_check_id))
                for checkpoint_id, checkpoint in checkpoint_map.items():
                    require(checkpoint.get("raw_check_ids") and all((checkpoint_id, raw_id) in checked_raw_pairs for raw_id in checkpoint["raw_check_ids"]), "policy-required raw checkpoint check omitted")
                for checkpoint_id, checkpoint in checkpoint_map.items():
                    for requirement in checkpoint["evidence"]:
                        require((checkpoint_id, requirement["kind"]) in evidence_pairs, "required evidence omitted: " + checkpoint_id + "/" + requirement["kind"])
                receipt_sha = file_identity(path)["sha256"]
                matching = [review for _, review in reviews if review.get("schema") == "tokimemo.readiness-review.v1" and review.get("receipt_sha256") == receipt_sha]
                require(matching, "independent receipt review is required")
                for review in matching:
                    require(review.get("reviewer_id") in reviewer_ids and review.get("reviewer_id") != producer, "self-review or unapproved reviewer")
                    require(review.get("runtime_tree_sha256") == runtime["sha256"], "review runtime binding mismatch")
                    require(review.get("decision") == "accepted" and review.get("semantic_authenticity_accepted") is True and review.get("unresolved_findings") == [], "independent semantic review is not accepted")
                if len(errors) == before_errors:
                    result["accepted_scenarios"].append(scenario_id)
            except (Invalid, ValueError, TypeError, KeyError, OSError, OverflowError, ImportError, RuntimeError) as exc:
                errors.append(label + ": " + str(exc))
        result["missing_scenarios"] = sorted(set(scenarios) - set(result["accepted_scenarios"]))
        if result["missing_scenarios"]:
            errors.append("required scenarios lack accepted evidence")
        result["independently_reviewed"] = not errors and len(result["accepted_scenarios"]) == len(scenarios)
    except (Invalid, ValueError, TypeError, KeyError, OSError, OverflowError, RecursionError) as exc:
        errors.append(str(exc))
    result["accepted_scenarios"].sort()
    result["errors"] = sorted(set(errors))
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("policy", "expected-policy-sha256", "runtime-root", "content-root", "evidence-root", "receipt-dir", "review-dir"):
        parser.add_argument("--" + name, required=True)
    args = parser.parse_args(argv)
    result = validate(args.policy, args.expected_policy_sha256, args.runtime_root, args.content_root,
                      args.evidence_root, args.receipt_dir, args.review_dir)
    print(json.dumps(result, sort_keys=True, indent=2))
    return 0 if result["independently_reviewed"] else 1


if __name__ == "__main__":
    sys.exit(main())
