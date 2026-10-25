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
    "message_window_set_title_hidden": {
        "source": "src/main/hud_panels.c",
        "source_sha256": "b942acb5863f26326c044720768384cf23b156f4736c69f1eda0d34831016ae1",
        "assembly": "asm/matchings/main/hud_panels/message_window_set_title_hidden.s",
        "assembly_sha256": "e305c82716c8603c81ead32c7821503dbd92a13a8aabe252b8a0ab10e5e7abe4",
        "renderer_assembly": "asm/matchings/main/hud_panels/message_window_set_title.s",
        "renderer_assembly_sha256": "bf4a0429db4211b64d66b89f560dd0b30aec9eddf089cb551f57a491e4766165",
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
        "source": "src/main/menu_speaker_girl.c",
        "source_sha256": "bac5ba1d7ea8f17e9bf69df803c9c1f1cebbf21fd5c3b3fc1e568bce20ca7185",
        "assembly": "asm/matchings/main/menu_speaker_girl/x_taku_string_set.s",
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
        "crosscheck": ".asset-cache/text/current source custom-consumer crosscheck: x_taku_string_set, 4 sites",
    },
    "rpg_label_create": {
        "unit": "RPG_BAT",
        "source": "src/ovl/rpg_battle/actor_animation_damage_voice.c",
        "source_sha256": "d4ec36d4c72f3b01e76fe29a6b4e1c743e5e9b32c5a89d67f180e24a6f836e39",
        "assembly": "asm/ovl/RPG_BAT/matchings/rpg_battle/actor_animation_damage_voice/rpg_label_create.s",
        "assembly_sha256": "aa29d828dd366d97f5cc9d0b4675ba08f462497a85382ac05e83b7c424adbb5c",
        "binary": "CDROM/EXEDIR/RPG_BAT.EXN",
        "binary_sha256": "59a6d1687e59132017eb7212fd6560b2e96bddad0a19ecac0929517b9ec4944c",
        "window_offset": 0xC934,
        "window_size": 0x48,
        "window_sha256": "143b1668058768a2be44b5ade5250224b16b80c6e1e7f7e8110143084da2b797",
        "entry_pc": 0x8013E934,
        "verified_instruction_words": 18,
        "read_arguments": [{"index": 2, "role": "display", "renderer": "set_kanji_string"}],
        "evidence": "third function argument is forwarded to set_kanji_string argument 3",
        "crosscheck": ".asset-cache/text/current source custom-consumer crosscheck: rpg_label_create, 14 unique same-function label/byte bindings",
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
    "rpg_label_create": {
        "source": SOURCE_PROOFS["rpg_label_create"]["source"],
        "source_sha256": SOURCE_PROOFS["rpg_label_create"]["source_sha256"],
        "assembly": SOURCE_PROOFS["rpg_label_create"]["assembly"],
        "assembly_sha256": SOURCE_PROOFS["rpg_label_create"]["assembly_sha256"],
        "binary": SOURCE_PROOFS["rpg_label_create"]["binary"],
        "binary_sha256": SOURCE_PROOFS["rpg_label_create"]["binary_sha256"],
        "verified_instruction_words": 18,
        "read_arguments": [], "evidence": "custom consumer proof reference; role is registered separately",
    },
    "rpg_message_format_damage": {
        "source": "src/ovl/rpg_battle/hud_messages_and_swimming_effect.c",
        "source_sha256": "68c44fa0ca57953986fd8db30e04ad092ce7ab24317b1a3875a7de3e9ae8b943",
        "assembly": "asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_format_damage.s",
        "assembly_sha256": "8f7a648d6b25ab2adda28048ae8f98bb86adfbb8d3ca03bc0ca761ae7e289999",
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
        "rpg_label_create": {"sites": 14, "arguments": {"2": 14},
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
