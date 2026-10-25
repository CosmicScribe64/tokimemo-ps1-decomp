"""Verify source-pinned custom text-renderer argument-role evidence.

The verifier reads only source, assembly, hashes, and bounded original code
windows. It emits no source literals or game text.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


SOURCE_PROOFS = {
    "func_80066104": {
        "source": "src/main/80062CD0.c",
        "source_sha256": "4ee59d2d558566456345d0779c199b58dd9ab05754a4cdc7d431f6990a1088bd",
        "assembly": "asm/matchings/main/80062CD0/func_80066104.s",
        "assembly_sha256": "475a44e691f65419b95148dd9b83a70c1c040374c2030f09f22735a9474bc59b",
        "renderer_assembly": "asm/matchings/main/80062CD0/message_window_set_title.s",
        "renderer_assembly_sha256": "c6a469d1da3ec55ca8b05b82ec8a8a34f0bcb90d022bc819b8fd69eb51e3e5f3",
        "renderer_instruction_words": 130,
        "binary": "SLPM_86.053",
        "binary_sha256": "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
        "window_offset": 0x25904,
        "window_size": 0x28,
        "window_sha256": "d30f07357fb11d75f6e369571c9dfbb38d27579d166b5130dd49efa0b19c60d7",
        "verified_instruction_words": 10,
        "read_arguments": [{"index": 0, "role": "display", "renderer": "set_kanji_string"}],
        "evidence": "wrapper forwards incoming a0 to message_window_set_title; callee sends that pointer to set_kanji_string and strlen",
    },
    "x_taku_string_set": {
        "source": "src/main/8004F870.c",
        "source_sha256": "10fba9db2073f4e8d156b366255ca9a3e9f528af3d9b4c15795bad274e1061d2",
        "assembly": "asm/matchings/main/8004F870/x_taku_string_set.s",
        "assembly_sha256": "41ef1329383e8eb6c13c10d09933c6a72bcfd81ed3a9c5568c9b5c7181c55c9a",
        "binary": "SLPM_86.053",
        "binary_sha256": "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
        "window_offset": 0x10578,
        "window_size": 0x84,
        "window_sha256": "950dd4184b0b6119a6632ccae252f479b8962a2591dc6e2ea76787620e2b49c5",
        "entry_pc": 0x80050D78,
        "verified_instruction_words": 33,
        "read_arguments": [
            {"index": 0, "role": "display", "renderer": "set_kanji_string"},
            {"index": 1, "role": "display", "renderer": "set_kanji_string"},
            {"index": 2, "role": "display", "renderer": "set_kanji_string", "conditions": [
                {"argument_index": 3, "operator": "==", "value": 3},
                {"argument_index": 2, "operator": "!=", "value": 0},
            ]},
        ],
        "evidence": "first two label arguments are read unconditionally; third label is read only under the two encoded guards",
        "crosscheck": ".asset-cache/text/custom-consumer-source-evidence.json: x_taku_string_set, 4 sites",
    },
    "func_8013E934": {
        "unit": "RPG_BAT",
        "source": "src/ovl/RPG_BAT/8013E4D0.c",
        "source_sha256": "adbf70d8ae8df0aef1ff8964c681a8d29b828185764d0faf22b3ae8c08bd858b",
        "assembly": "asm/ovl/RPG_BAT/matchings/RPG_BAT/8013E4D0/func_8013E934.s",
        "assembly_sha256": "230cf417eda00fa6f49a8466b397b9eaaf5ca3a7c8be89319a75d87ef43c377a",
        "binary": "CDROM/EXEDIR/RPG_BAT.EXN",
        "binary_sha256": "59a6d1687e59132017eb7212fd6560b2e96bddad0a19ecac0929517b9ec4944c",
        "window_offset": 0xC934,
        "window_size": 0x48,
        "window_sha256": "143b1668058768a2be44b5ade5250224b16b80c6e1e7f7e8110143084da2b797",
        "entry_pc": 0x8013E934,
        "verified_instruction_words": 18,
        "read_arguments": [{"index": 2, "role": "display", "renderer": "set_kanji_string"}],
        "evidence": "third function argument is forwarded to set_kanji_string argument 3",
        "crosscheck": ".asset-cache/text/custom-consumer-source-evidence.json: func_8013E934, 14 unique same-function label/byte bindings",
    },
}

# Sol's custom-consumer evidence pins three functions whose original code
# coverage totals 204 instruction words. The third is crosschecked here only;
# no argument role is inferred for it by this module.
CUSTOM_REFERENCE_PROOFS = {
    "x_taku_string_set": {
        "source": SOURCE_PROOFS["x_taku_string_set"]["source"],
        "source_sha256": SOURCE_PROOFS["x_taku_string_set"]["source_sha256"],
        "assembly": SOURCE_PROOFS["x_taku_string_set"]["assembly"],
        "assembly_sha256": SOURCE_PROOFS["x_taku_string_set"]["assembly_sha256"],
        "binary": SOURCE_PROOFS["x_taku_string_set"]["binary"],
        "binary_sha256": SOURCE_PROOFS["x_taku_string_set"]["binary_sha256"],
        "verified_instruction_words": 33,
        "read_arguments": [], "evidence": "custom consumer proof reference; not registered as a role",
    },
    "func_8013E934": {
        "source": SOURCE_PROOFS["func_8013E934"]["source"],
        "source_sha256": SOURCE_PROOFS["func_8013E934"]["source_sha256"],
        "assembly": SOURCE_PROOFS["func_8013E934"]["assembly"],
        "assembly_sha256": SOURCE_PROOFS["func_8013E934"]["assembly_sha256"],
        "binary": SOURCE_PROOFS["func_8013E934"]["binary"],
        "binary_sha256": SOURCE_PROOFS["func_8013E934"]["binary_sha256"],
        "verified_instruction_words": 18,
        "read_arguments": [], "evidence": "custom consumer proof reference; role is registered separately",
    },
    "func_8014D11C": {
        "source": "src/ovl/RPG_BAT/8014ADD0.c",
        "source_sha256": "fa42924b2dbb8eeb6762ca4e19f33f5f304eeb75c0904a34324e2fae1825a7a6",
        "assembly": "asm/ovl/RPG_BAT/matchings/RPG_BAT/8014ADD0/func_8014D11C.s",
        "assembly_sha256": "3e2d9e3f68bb4a208b5bcde445b500ecf795a0837660849583fd7635aa0595ea",
        "binary": "CDROM/EXEDIR/RPG_BAT.EXN",
        "binary_sha256": "59a6d1687e59132017eb7212fd6560b2e96bddad0a19ecac0929517b9ec4944c",
        "verified_instruction_words": 153,
        "read_arguments": [], "evidence": "custom consumer proof reference; not registered as a role",
    },
}

INSTRUCTION_COMMENT = re.compile(r"^\s*/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s*\*/")


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _verify_assembly_words(assembly_path: Path, binary_path: Path,
                           expected_count: int | None = None) -> dict:
    words = []
    for line_number, line in enumerate(assembly_path.read_text(encoding="utf-8").splitlines(), 1):
        match = INSTRUCTION_COMMENT.match(line)
        if match:
            words.append((line_number, int(match.group(1), 16), bytes.fromhex(match.group(2))))
    mismatches = []
    with binary_path.open("rb") as stream:
        for line_number, offset, expected in words:
            stream.seek(offset)
            actual = stream.read(4)
            if actual != expected:
                mismatches.append({"assembly_line": line_number, "file_offset": offset})
    count_ok = expected_count is None or len(words) == expected_count
    return {"instruction_words": len(words), "expected_instruction_words": expected_count,
            "count_ok": count_ok, "mismatch_count": len(mismatches),
            "mismatches": mismatches, "verified": count_ok and not mismatches}


def _check_proofs(source_root: Path, original_root: Path, proofs: dict) -> dict:
    source_root = source_root.resolve()
    original_root = original_root.resolve()
    results = {}
    for name, proof in proofs.items():
        checks = []
        for path_key, hash_key in (("source", "source_sha256"), ("assembly", "assembly_sha256"),
                                   ("renderer_assembly", "renderer_assembly_sha256")):
            if path_key not in proof:
                continue
            path = (source_root / proof[path_key]).resolve()
            expected = proof[hash_key]
            actual = _sha256(path) if path.is_file() and path.is_relative_to(source_root) else None
            checks.append({"kind": path_key, "path": proof[path_key], "expected_sha256": expected,
                           "actual_sha256": actual, "ok": actual == expected})
        binary = (original_root / proof["binary"]).resolve()
        actual_binary_hash = (_sha256(binary) if binary.is_file() and
                              binary.is_relative_to(original_root) else None)
        checks.append({"kind": "binary", "path": proof["binary"],
                       "expected_sha256": proof["binary_sha256"],
                       "actual_sha256": actual_binary_hash,
                       "ok": actual_binary_hash == proof["binary_sha256"]})
        instruction_checks = []
        if actual_binary_hash == proof["binary_sha256"]:
            for assembly_key, count_key in (("assembly", "verified_instruction_words"),
                                            ("renderer_assembly", "renderer_instruction_words")):
                if assembly_key not in proof:
                    continue
                assembly_path = (source_root / proof[assembly_key]).resolve()
                if assembly_path.is_file() and assembly_path.is_relative_to(source_root):
                    instruction_checks.append({"kind": assembly_key,
                                               **_verify_assembly_words(
                                                   assembly_path, binary,
                                                   proof.get(count_key))})
        window = None
        if actual_binary_hash == proof["binary_sha256"] and "window_offset" in proof:
            with binary.open("rb") as stream:
                stream.seek(proof["window_offset"])
                window_hash = hashlib.sha256(stream.read(proof["window_size"])).hexdigest()
            window = {"offset": proof["window_offset"], "size": proof["window_size"],
                      "expected_sha256": proof["window_sha256"], "actual_sha256": window_hash,
                      "ok": window_hash == proof["window_sha256"]}
        results[name] = {"unit": proof.get("unit", "main"),
                         "read_arguments": proof["read_arguments"],
                         "evidence": proof["evidence"],
                         "checks": checks, "instruction_checks": instruction_checks,
                         "original_code_window": window,
                         "verified": all(item["ok"] for item in checks) and
                                     (window["ok"] if window is not None else "window_offset" not in proof)
                                     and bool(instruction_checks) and
                                     all(item["verified"] for item in instruction_checks)}
    return {"schema": "tokimemo.text.renderer-source-proof.v1",
            "source_root": source_root.name, "original_root": original_root.name,
            "proofs": results, "verified": bool(results) and all(item["verified"] for item in results.values()),
            "limits": ["source_and_original_bytes_pinned", "no_runtime_execution_claim",
                       "text_payloads_not_included"]}


def _check_custom_consumer_evidence(evidence_path: Path, proofs: dict) -> dict:
    if not evidence_path.is_file():
        return {"path": evidence_path.as_posix(), "status": "missing", "verified": False}
    evidence = json.loads(evidence_path.read_text(encoding="utf-8"))
    source_proofs = {item["function"]: item for item in evidence.get("function_proofs", [])}
    expected_summary = {
        "x_taku_string_set": {"sites": 4, "arguments": {"0": 2, "1": 2},
                              "unique_same_function_label_and_exact_bytes": 4},
        "func_8013E934": {"sites": 14, "arguments": {"2": 14},
                          "unique_same_function_label_and_exact_bytes": 14},
    }
    checks = []
    for name, expected in expected_summary.items():
        pinned = proofs[name]
        fact = source_proofs.get(name, {})
        summary = evidence.get("site_summary", {}).get(name, {})
        expected_fields = {
            "source": pinned["source"], "source_sha256": pinned["source_sha256"],
            "assembly": pinned["assembly"], "assembly_sha256": pinned["assembly_sha256"],
            "binary_path": pinned["binary"], "binary_sha256": pinned["binary_sha256"],
            "entry_pc": pinned["entry_pc"],
            "verified_instruction_words": pinned["verified_instruction_words"],
        }
        proof_ok = all(fact.get(key) == value for key, value in expected_fields.items())
        summary_ok = (summary.get("sites") == expected["sites"] and
                      summary.get("arguments") == expected["arguments"] and
                      summary.get("binding_statuses", {}).get(
                          "unique_same_function_label_and_exact_bytes") ==
                      expected["unique_same_function_label_and_exact_bytes"])
        checks.append({"function": name, "source_proof_matches": proof_ok,
                       "numeric_site_summary_matches": summary_ok,
                       "expected_sites": expected["sites"],
                       "actual_sites": summary.get("sites"),
                       "expected_unique_exact_bindings": expected[
                           "unique_same_function_label_and_exact_bytes"],
                       "actual_unique_exact_bindings": summary.get("binding_statuses", {}).get(
                           "unique_same_function_label_and_exact_bytes")})
    return {"path": evidence_path.as_posix(), "sha256": _sha256(evidence_path),
            "checks": checks, "verified": all(item["source_proof_matches"] and
                                                item["numeric_site_summary_matches"]
                                                for item in checks)}


def verify_renderer_source_proofs(source_root, original_root, evidence_path=None) -> dict:
    """Verify the built-in source, assembly, original-file, and code-window pins."""
    report = _check_proofs(Path(source_root), Path(original_root), SOURCE_PROOFS)
    reference_report = _check_proofs(Path(source_root), Path(original_root), CUSTOM_REFERENCE_PROOFS)
    report["custom_reference_instruction_words"] = sum(
        checks[0]["instruction_words"] for value in reference_report["proofs"].values()
        if (checks := value["instruction_checks"]))
    report["custom_reference_words_verified"] = all(
        bool(value["instruction_checks"]) and value["instruction_checks"][0]["verified"]
        for value in reference_report["proofs"].values())
    report["custom_reference_proofs"] = reference_report["proofs"]
    report["verified"] = report["verified"] and reference_report["verified"]
    if evidence_path is not None:
        report["custom_consumer_evidence_crosscheck"] = _check_custom_consumer_evidence(
            Path(evidence_path), SOURCE_PROOFS)
    return report


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-root", required=True, help="Frozen source snapshot root")
    parser.add_argument("--original-root", required=True, help="Extracted original disc/files root")
    parser.add_argument("--output", required=True, help="Metadata-only JSON report path")
    parser.add_argument("--evidence", help="Optional secondary custom-consumer evidence JSON")
    args = parser.parse_args(argv)
    report = verify_renderer_source_proofs(args.source_root, args.original_root, args.evidence)
    output = Path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2, ensure_ascii=True) + "\n", encoding="utf-8")
    print("verified" if report["verified"] else "verification failed")
    return 0 if report["verified"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
