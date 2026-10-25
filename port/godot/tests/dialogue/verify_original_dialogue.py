"""Verify the T-29011 adapter against one original input using canonical readers."""
import argparse
import copy
import json
from pathlib import Path

MODULE = Path(__file__).resolve().parents[4] / "tools" / "port" / "dialogue_content.py"
import importlib.util

SPEC = importlib.util.spec_from_file_location("dialogue_content", MODULE)
dialogue_content = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(dialogue_content)

SOURCE_PATH = "CDROM/SERIFU/SERIFU.BIN"
SECTION_ID = "table-0"


def verify(catalog_path: Path, input_path: Path) -> dict:
    catalog = json.loads(catalog_path.read_text(encoding="utf-8"))
    return verify_catalog(catalog, input_path)


def verify_catalog(catalog: dict, input_path: Path) -> dict:
    fresh_script = dialogue_content.verify_against_original(
        catalog, SOURCE_PATH, SECTION_ID, input_path)
    fresh_catalog = dict(catalog)
    fresh_files = [dict(item) if isinstance(item, dict) else item
                   for item in catalog.get("files", [])]
    selected = [index for index, item in enumerate(fresh_files)
                if isinstance(item, dict) and item.get("path") == SOURCE_PATH]
    if len(selected) != 1:
        raise dialogue_content.DialogueContentError("catalog source entry is not unique")
    fresh_files[selected[0]]["script"] = fresh_script
    fresh_catalog["files"] = fresh_files
    component = dialogue_content.project_catalog(fresh_catalog, SOURCE_PATH, SECTION_ID)
    rows = [row for group in component["groups"] for row in group["records"]]
    command_counts = {}
    for row in rows:
        command_counts[row["command"]] = command_counts.get(row["command"], 0) + 1
    return {"component": component, "groups": len(component["groups"]),
            "rows": len(rows), "command_counts": command_counts,
            "voice_rows": sum("voice" in row for row in rows),
            "unknown_tokens": sum(row.get("text", {}).get("unknown_token_count", 0)
                                  for row in rows)}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--input", type=Path, required=True)
    args = parser.parse_args()
    result = verify(args.catalog, args.input)
    component = result.pop("component")
    original_catalog = json.loads(args.catalog.read_text(encoding="utf-8"))
    mutated_rows = copy.deepcopy(original_catalog)
    source_entry = next(item for item in mutated_rows["files"] if item.get("path") == SOURCE_PATH)
    source_entry["script"]["records"][0]["command"]["value"] = 3
    rebuilt = verify_catalog(mutated_rows, args.input)["component"]
    if rebuilt["content_sha256"] != component["content_sha256"]:
        raise SystemExit("cached catalog mutation changed fresh original-derived output")
    mutated_identity = copy.deepcopy(original_catalog)
    source_entry = next(item for item in mutated_identity["files"] if item.get("path") == SOURCE_PATH)
    source_entry["script"]["source"]["sha256"] = "a" * 64
    try:
        verify_catalog(mutated_identity, args.input)
    except dialogue_content.DialogueContentError:
        pass
    else:
        raise SystemExit("mutated catalog source identity was accepted")
    print("original SERIFU reader verification passed:",
          "groups=%d rows=%d voice_rows=%d unknown_tokens=%d" %
          (result["groups"], result["rows"], result["voice_rows"], result["unknown_tokens"]),
          "commands=%s" % json.dumps(result["command_counts"], sort_keys=True),
          "catalog-row-mutation=ignored", "catalog-source-mutation=rejected")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
