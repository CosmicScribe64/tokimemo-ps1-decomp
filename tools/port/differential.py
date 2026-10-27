"""Strict differential check for original-executable and actual-port traces.

This comparator emits bounded evidence; hashes establish identity, not that a
producer is truthful. A separate reviewer must inspect raw traces and sources.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

HERE = Path(__file__).resolve().parent
TRACE_TOOL = next((candidate for base in (HERE, *HERE.parents)
                   for candidate in (base / "compare_trace.py", base / "tools" / "port" / "compare_trace.py")
                   if candidate.is_file()), None)
if TRACE_TOOL is None:
    raise RuntimeError("cannot locate sibling tools/port/compare_trace.py")
_spec = importlib.util.spec_from_file_location("tokimemo_compare_trace", TRACE_TOOL)
compare_trace = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(compare_trace)

SHA = re.compile(r"[0-9a-f]{64}\Z")
REQUIRED_STATE_KEYS = {"projected_state", "input", "rng", "services"}
MAX_FILE_BYTES = 512 * 1024 * 1024
MAX_FILES = 10000
MAX_TOTAL_BYTES = 2 * 1024 * 1024 * 1024


class Invalid(ValueError):
    pass


def no_symlink_chain(path: Path):
    path = Path(path).absolute()
    for part in (path, *path.parents):
        if part.is_symlink():
            raise Invalid(f"symbolic-link path component: {part}")
    return path


def digest_file(path: Path) -> dict:
    path = no_symlink_chain(path)
    if not path.is_file():
        raise Invalid(f"missing or symbolic-link input: {path}")
    size = path.stat().st_size
    if size > MAX_FILE_BYTES:
        raise Invalid(f"file too large: {path}")
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return {"sha256": h.hexdigest(), "bytes": size}


def tree_identity(root: Path) -> str:
    root = no_symlink_chain(root)
    if not root.is_dir():
        raise Invalid("port tree root is missing or symbolic")
    rows, total = [], 0
    for path in sorted(root.rglob("*")):
        rel = path.relative_to(root)
        if any(part in (".git", ".godot") for part in rel.parts):
            continue
        if path.is_symlink():
            raise Invalid(f"symbolic link in port tree: {rel}")
        if path.is_dir():
            continue
        item = digest_file(path)
        total += item["bytes"]
        if len(rows) >= MAX_FILES or total > MAX_TOTAL_BYTES:
            raise Invalid("port tree exceeds bounded inventory")
        rows.append({"path": rel.as_posix(), **item})
    if not rows:
        raise Invalid("port tree is empty")
    return canonical_sha(rows)


def canonical_sha(value) -> str:
    payload = json.dumps(value, sort_keys=True, separators=(",", ":"), ensure_ascii=True).encode()
    return hashlib.sha256(payload).hexdigest()


def safe_relative(root: Path, value: str) -> Path:
    if not isinstance(value, str) or not value or "\\" in value or ":" in value:
        raise Invalid("invalid evidence-relative path")
    path = Path(value)
    if path.is_absolute() or any(part in ("", ".", "..") for part in value.split("/")):
        raise Invalid("evidence path traversal is forbidden")
    root = no_symlink_chain(root)
    target = root / path
    if target.is_symlink() or root.resolve() not in target.resolve().parents:
        raise Invalid("evidence path escapes root or is symbolic")
    for parent in target.parents:
        if parent == root.parent:
            break
        if parent.is_symlink():
            raise Invalid("symbolic-link evidence path")
    return target


def strict_json(path: Path):
    def reject(value):
        raise Invalid(f"non-integer JSON number is not permitted: {value}")
    return json.loads(path.read_text(encoding="utf-8"), parse_float=reject,
                      parse_constant=reject, object_pairs_hook=_unique_object)


def write_new_result(path: Path, report: dict):
    path = Path(path).absolute()
    for parent in path.parents:
        if parent.is_symlink():
            raise Invalid("result path contains a symbolic-link parent")
    if path.is_symlink() or path.exists():
        raise Invalid("result output must be a new file; refusing overwrite")
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("x", encoding="utf-8") as stream:
        stream.write(json.dumps(report, sort_keys=True, indent=2) + "\n")


def preflight_new_output(path: Path):
    path = Path(path).absolute()
    for parent in path.parents:
        if parent.is_symlink():
            raise Invalid("result path contains a symbolic-link parent")
    if path.is_symlink() or path.exists():
        raise Invalid("result output must be a new file; refusing overwrite")


def write_artifact_once(path: Path, payload: bytes):
    path = Path(path)
    if path.is_symlink():
        raise Invalid(f"refusing symbolic-link tooling artifact: {path}")
    if path.exists():
        if not path.is_file() or digest_file(path) != {"sha256": hashlib.sha256(payload).hexdigest(), "bytes": len(payload)}:
            raise Invalid(f"existing tooling artifact differs: {path}")
        return
    with path.open("xb") as stream:
        stream.write(payload)


def _unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise Invalid(f"duplicate JSON key: {key}")
        result[key] = value
    return result


def load_projection(path: Path, expected_sha256: str) -> dict:
    identity = digest_file(path)
    if not SHA.fullmatch(expected_sha256 or "") or identity["sha256"] != expected_sha256:
        raise Invalid("projection schema pin mismatch")
    schema = strict_json(path)
    if not isinstance(schema, dict) or schema.get("schema") != "tokimemo.trace-projection.v1":
        raise Invalid("unsupported projection schema")
    required = schema.get("required_state_keys")
    if not isinstance(required, list) or not REQUIRED_STATE_KEYS <= set(required) or len(required) != len(set(required)):
        raise Invalid("projection must require projected_state, input, rng, and services")
    projected_fields = schema.get("required_projected_state_fields")
    if (not isinstance(projected_fields, list) or not projected_fields or
            any(not isinstance(field, str) or not field for field in projected_fields) or
            len(projected_fields) != len(set(projected_fields))):
        raise Invalid("projection must pin nonempty required_projected_state_fields")
    return schema


def load_run_metadata(path: Path, expected_role: str) -> dict:
    record = strict_json(Path(path))
    if not isinstance(record, dict) or record.get("schema") != "tokimemo.differential-run.v1" or record.get("role") != expected_role:
        raise Invalid(f"invalid {expected_role} run metadata")
    return record


def validate_projected_trace(rows: list[dict], required_state_keys: set[str], projected_fields: set[str], label: str):
    for index, row in enumerate(rows):
        state = row["state"]
        missing = required_state_keys - state.keys()
        if missing:
            raise Invalid(f"{label} record {index} missing projected fields: {', '.join(sorted(missing))}")
        projected = state["projected_state"]
        if not isinstance(projected, dict) or not projected:
            raise Invalid(f"{label} record {index} projected_state must be a nonempty object")
        if not projected_fields <= projected.keys():
            raise Invalid(f"{label} record {index} projected_state lacks required fields: " +
                          ", ".join(sorted(projected_fields - projected.keys())))
        buttons = state["input"]
        if not isinstance(buttons, dict) or set(buttons) != {"held", "pressed", "released"}:
            raise Invalid(f"{label} record {index} input must contain held/pressed/released bitmasks")
        if any(type(buttons[key]) is not int or not 0 <= buttons[key] <= 0xFFFFFFFF
               for key in ("held", "pressed", "released")):
            raise Invalid(f"{label} record {index} input masks must be uint32 integers")
        rng = state["rng"]
        if not isinstance(rng, dict) or type(rng.get("state")) is not int or type(rng.get("draw_count")) is not int:
            raise Invalid(f"{label} record {index} rng must contain integer state/draw_count")
        if not isinstance(state["services"], list):
            raise Invalid(f"{label} record {index} services must be an ordered array")


def strict_trace_preflight(path: Path):
    """Reject duplicate JSON keys; then delegate record shape/order to compare_trace."""
    found = False
    with Path(path).open(encoding="utf-8") as stream:
        for number, line in enumerate(stream, 1):
            if not line.strip():
                raise Invalid(f"blank/truncated trace line at {path}:{number}")
            value = json.loads(line, parse_float=lambda v: (_ for _ in ()).throw(Invalid("float trace value")),
                                parse_constant=lambda v: (_ for _ in ()).throw(Invalid("non-finite trace value")),
                                object_pairs_hook=_unique_object)
            if not isinstance(value, dict):
                raise Invalid(f"trace row is not an object at {path}:{number}")
            found = True
    if not found:
        raise Invalid(f"{path}: empty traces cannot establish parity")


def _evidence_identity(root: Path, path: Path) -> dict:
    root = no_symlink_chain(root)
    target = no_symlink_chain(path)
    if target.is_symlink() or root.resolve() not in target.resolve().parents:
        raise Invalid(f"trace is outside evidence root: {path}")
    item = digest_file(target)
    return {"path": target.relative_to(root.absolute()).as_posix(), **item}


def compare_runs(*, evidence_root: Path, native_trace: Path, port_trace: Path,
                 projection_schema: Path, expected_projection_sha256: str,
                 original_executable: Path, port_root: Path,
                 initial_state: Path, input_log: Path,
                 native_metadata: Path, port_metadata: Path,
                 native_producer_id: str, native_producer_script: Path,
                 port_producer_id: str, port_producer_script: Path) -> dict:
    root = no_symlink_chain(evidence_root)
    for path in (native_trace, port_trace, projection_schema, original_executable, port_root,
                 initial_state, input_log, native_metadata, port_metadata,
                 native_producer_script, port_producer_script):
        no_symlink_chain(path)
    if Path(native_trace).absolute() == Path(port_trace).absolute():
        raise Invalid("native and port traces must be distinct files")
    if Path(native_trace).exists() and Path(port_trace).exists() and Path(native_trace).samefile(Path(port_trace)):
        raise Invalid("native and port traces must not alias the same file")
    if not native_producer_id or not port_producer_id or native_producer_id == port_producer_id:
        raise Invalid("native and port producers must have distinct nonempty IDs")
    native_script = digest_file(native_producer_script)
    port_script = digest_file(port_producer_script)
    if Path(native_producer_script).resolve() == Path(port_producer_script).resolve() or native_script["sha256"] == port_script["sha256"]:
        raise Invalid("native and port producer scripts must be distinct")
    projection = load_projection(Path(projection_schema), expected_projection_sha256)
    original_identity = digest_file(Path(original_executable))
    port_tree_sha = tree_identity(Path(port_root))
    initial_sha = digest_file(Path(initial_state))["sha256"]
    input_sha = digest_file(Path(input_log))["sha256"]
    native_meta = load_run_metadata(Path(native_metadata), "native_emulator")
    port_meta = load_run_metadata(Path(port_metadata), "actual_app")
    common_pins = {"projection_schema_sha256": expected_projection_sha256,
                   "initial_state_sha256": initial_sha, "input_log_sha256": input_sha}
    if any(native_meta.get(key) != value or port_meta.get(key) != value for key, value in common_pins.items()):
        raise Invalid("native and port runs do not share projection/initial/input identities")
    if (native_meta.get("original_executable_sha256") != original_identity["sha256"] or
            port_meta.get("port_tree_sha256") != port_tree_sha):
        raise Invalid("run metadata original executable/current port tree mismatch")
    if (native_meta.get("producer_id") != native_producer_id or port_meta.get("producer_id") != port_producer_id or
            native_meta.get("producer_script_sha256") != native_script["sha256"] or
            port_meta.get("producer_script_sha256") != port_script["sha256"]):
        raise Invalid("run metadata producer identity mismatch")
    digest_file(Path(native_trace))
    digest_file(Path(port_trace))
    strict_trace_preflight(Path(native_trace))
    strict_trace_preflight(Path(port_trace))
    native_rows = compare_trace.load_trace(Path(native_trace))
    port_rows = compare_trace.load_trace(Path(port_trace))
    required = set(projection["required_state_keys"])
    projected_fields = set(projection["required_projected_state_fields"])
    validate_projected_trace(native_rows, required, projected_fields, "native")
    validate_projected_trace(port_rows, required, projected_fields, "port")
    divergence = compare_trace.compare(native_rows, port_rows)
    native_identity = _evidence_identity(root, Path(native_trace))
    port_identity = _evidence_identity(root, Path(port_trace))
    native_script_identity = _evidence_identity(root, Path(native_producer_script))
    port_script_identity = _evidence_identity(root, Path(port_producer_script))
    if native_identity["path"] == port_identity["path"]:
        raise Invalid("native and port trace paths must be distinct")
    tooling = root / "tooling"
    if tooling.is_symlink():
        raise Invalid("tooling evidence directory is symbolic")
    tooling.mkdir(parents=True, exist_ok=True)
    comparator_copy = tooling / "differential.py"
    dependency_source = Path(compare_trace.__file__).resolve()
    dependency_copy = tooling / "compare_trace.py"
    write_artifact_once(comparator_copy, Path(__file__).read_bytes())
    write_artifact_once(dependency_copy, dependency_source.read_bytes())
    comparator_identity = digest_file(comparator_copy)
    dependency_identity = digest_file(dependency_copy)
    return {
        "schema": "tokimemo.differential-result.v1",
        "passed": divergence is None,
        "records": len(native_rows) if divergence is None else min(len(native_rows), len(port_rows)),
        "first_divergence": divergence,
        "native_trace": {**native_identity, "origin": "native_emulator", "producer_id": native_producer_id,
                         "producer_script_path": native_script_identity["path"],
                         "producer_script_sha256": native_script["sha256"]},
        "port_trace": {**port_identity, "origin": "actual_app", "producer_id": port_producer_id,
                       "producer_script_path": port_script_identity["path"],
                       "producer_script_sha256": port_script["sha256"]},
        "comparator": {"path": "tooling/differential.py", **comparator_identity,
                       "dependency": {"path": "tooling/compare_trace.py", **dependency_identity}},
        "projection_schema": {"path": _evidence_identity(root, Path(projection_schema))["path"],
                              **digest_file(Path(projection_schema))},
        "initial_state": {"path": _evidence_identity(root, Path(initial_state))["path"],
                          **digest_file(Path(initial_state))},
        "input_log": {"path": _evidence_identity(root, Path(input_log))["path"],
                      **digest_file(Path(input_log))},
        "native_metadata": {"path": _evidence_identity(root, Path(native_metadata))["path"],
                            **digest_file(Path(native_metadata))},
        "port_metadata": {"path": _evidence_identity(root, Path(port_metadata))["path"],
                          **digest_file(Path(port_metadata))},
        "pins": {
            "projection_schema_sha256": expected_projection_sha256,
            "original_executable_sha256": original_identity["sha256"],
            "original_executable_bytes": original_identity["bytes"],
            "port_tree_sha256": port_tree_sha,
            "initial_state_sha256": initial_sha,
            "input_log_sha256": input_sha,
            "native_producer_script_sha256": native_script["sha256"],
            "port_producer_script_sha256": port_script["sha256"],
        },
    }


def verify_report(report: dict, evidence_root: Path) -> None:
    """Re-open both traces and re-run parity; never trust a producer's `passed`."""
    if not isinstance(report, dict) or report.get("schema") != "tokimemo.differential-result.v1":
        raise Invalid("unsupported differential result schema")
    pins = report.get("pins")
    if not isinstance(pins, dict) or not all(SHA.fullmatch(str(pins.get(key, ""))) for key in
             ("projection_schema_sha256", "original_executable_sha256", "port_tree_sha256",
              "initial_state_sha256", "input_log_sha256", "native_producer_script_sha256",
              "port_producer_script_sha256")):
        raise Invalid("differential identity pins are missing or invalid")
    projection = None
    run_metadata = {}
    for key in ("projection_schema", "initial_state", "input_log", "native_metadata", "port_metadata"):
        record = report.get(key)
        if not isinstance(record, dict):
            raise Invalid(f"{key} identity missing")
        path = safe_relative(evidence_root, record.get("path"))
        identity = digest_file(path)
        if identity != {k: record.get(k) for k in ("sha256", "bytes")}:
            raise Invalid(f"{key} identity mismatch")
        if key == "projection_schema":
            projection = load_projection(path, pins["projection_schema_sha256"])
        elif key in ("native_metadata", "port_metadata"):
            role = "native_emulator" if key == "native_metadata" else "actual_app"
            metadata = load_run_metadata(path, role)
            run_metadata[key] = metadata
            if metadata.get("projection_schema_sha256") != pins["projection_schema_sha256"]:
                raise Invalid(f"{key} projection identity mismatch")
            if metadata.get("initial_state_sha256") != pins["initial_state_sha256"] or metadata.get("input_log_sha256") != pins["input_log_sha256"]:
                raise Invalid(f"{key} initial/input identity mismatch")
            if key == "native_metadata" and metadata.get("original_executable_sha256") != pins["original_executable_sha256"]:
                raise Invalid("native original executable identity mismatch")
            if key == "port_metadata" and metadata.get("port_tree_sha256") != pins["port_tree_sha256"]:
                raise Invalid("port current tree identity mismatch")
        elif identity["sha256"] != pins[key + "_sha256"]:
            raise Invalid(f"{key} pin mismatch")
    trace_ids, trace_paths = [], []
    traces = []
    for key, origin in (("native_trace", "native_emulator"), ("port_trace", "actual_app")):
        record = report.get(key)
        if not isinstance(record, dict) or record.get("origin") != origin:
            raise Invalid(f"invalid {key} origin")
        trace_ids.append(record.get("producer_id"))
        path = safe_relative(evidence_root, record.get("path"))
        trace_paths.append(path)
        identity = digest_file(path)
        if identity != {k: record.get(k) for k in ("sha256", "bytes")}:
            raise Invalid(f"{key} identity mismatch")
        strict_trace_preflight(path)
        traces.append(compare_trace.load_trace(path))
    if trace_ids[0] == trace_ids[1]:
        raise Invalid("producer IDs are not distinct")
    if (not trace_ids[0] or not trace_ids[1] or trace_paths[0].resolve() == trace_paths[1].resolve() or
            trace_paths[0].samefile(trace_paths[1])):
        raise Invalid("native and port traces/producers must be distinct")
    for rows, label in ((traces[0], "native"), (traces[1], "port")):
        validate_projected_trace(rows, set(projection["required_state_keys"]),
                                 set(projection["required_projected_state_fields"]), label)
    script_ids = []
    for report_key, pin_key in (("native_trace", "native_producer_script_sha256"),
                                ("port_trace", "port_producer_script_sha256")):
        script_path = safe_relative(evidence_root, report[report_key].get("producer_script_path"))
        script_identity = digest_file(script_path)
        if report[report_key].get("producer_script_sha256") != pins[pin_key] or script_identity["sha256"] != pins[pin_key]:
            raise Invalid("producer script identity mismatch")
        script_ids.append((script_path.resolve(), script_identity["sha256"]))
    if script_ids[0][0] == script_ids[1][0] or script_ids[0][1] == script_ids[1][1]:
        raise Invalid("native and port producer scripts are not distinct")
    for trace_key, metadata_key, script_pin in (
            ("native_trace", "native_metadata", "native_producer_script_sha256"),
            ("port_trace", "port_metadata", "port_producer_script_sha256")):
        metadata = run_metadata[metadata_key]
        if (report[trace_key].get("producer_id") != metadata.get("producer_id") or
                report[trace_key].get("producer_script_sha256") != metadata.get("producer_script_sha256") or
                metadata.get("producer_script_sha256") != pins[script_pin]):
            raise Invalid("trace and producer run metadata identity mismatch")
    if type(pins.get("original_executable_bytes")) is not int or pins["original_executable_bytes"] <= 0:
        raise Invalid("original executable byte size is missing")
    divergence = compare_trace.compare(traces[0], traces[1])
    if divergence is not None or report.get("first_divergence") is not None or report.get("passed") is not True:
        raise Invalid("differential result does not match trace comparison")
    expected_records = len(traces[0])
    if type(report.get("records")) is not int or report["records"] != expected_records or expected_records <= 0:
        raise Invalid("differential record count mismatch")
    comparator = report.get("comparator")
    if not isinstance(comparator, dict):
        raise Invalid("comparator source identity missing")
    comparator_file = safe_relative(evidence_root, comparator.get("path"))
    comparator_identity = digest_file(comparator_file)
    dependency = comparator.get("dependency")
    if not isinstance(dependency, dict):
        raise Invalid("comparator dependency identity missing")
    dependency_file = safe_relative(evidence_root, dependency.get("path"))
    dependency_identity = digest_file(dependency_file)
    if (comparator_identity != {k: comparator.get(k) for k in ("sha256", "bytes")} or
            dependency_identity != {k: dependency.get(k) for k in ("sha256", "bytes")} or
            comparator_identity["sha256"] != digest_file(Path(__file__))["sha256"] or
            dependency_identity["sha256"] != digest_file(Path(compare_trace.__file__))["sha256"]):
        raise Invalid("comparator source identity mismatch")


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    for flag in ("evidence-root", "native-trace", "port-trace", "projection-schema", "expected-projection-sha256",
                 "original-executable", "port-root", "initial-state", "input-log", "native-metadata",
                 "port-metadata", "native-producer-id",
                 "native-producer-script", "port-producer-id", "port-producer-script", "output"):
        parser.add_argument("--" + flag, required=True)
    args = parser.parse_args(argv)
    try:
        preflight_new_output(Path(args.output))
        report = compare_runs(evidence_root=Path(args.evidence_root), native_trace=Path(args.native_trace),
                              port_trace=Path(args.port_trace), projection_schema=Path(args.projection_schema),
                              expected_projection_sha256=args.expected_projection_sha256,
                              original_executable=Path(args.original_executable), port_root=Path(args.port_root),
                              initial_state=Path(args.initial_state), input_log=Path(args.input_log),
                              native_metadata=Path(args.native_metadata), port_metadata=Path(args.port_metadata),
                              native_producer_id=args.native_producer_id,
                              native_producer_script=Path(args.native_producer_script),
                              port_producer_id=args.port_producer_id,
                              port_producer_script=Path(args.port_producer_script))
        write_new_result(Path(args.output), report)
        print(json.dumps(report, sort_keys=True))
        return 0 if report["passed"] else 1
    except (Invalid, OSError, ValueError, TypeError) as error:
        print(f"differential comparison failed: {error}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
