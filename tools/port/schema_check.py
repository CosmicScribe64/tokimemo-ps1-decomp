#!/usr/bin/env python3
"""T-39028: validate JSON documents against the port's JSON Schemas.

Uses jsonschema (pinned in tools/port/requirements-test.txt) when it is
installed. Otherwise it falls back to a strict subset validator for the
Draft 2020-12 keywords the port schemas use. The subset fails loudly on any
keyword it does not implement, so a schema it cannot check is an error, never
a pass.

    python3 tools/port/schema_check.py SCHEMA DOCUMENT [DOCUMENT...]
"""
from __future__ import annotations

import json
from pathlib import Path
import re
import sys

TYPES = {"object": dict, "array": list, "string": str, "boolean": bool, "null": type(None)}
ANNOTATIONS = {"$schema", "$id", "$comment", "title", "description", "default", "examples", "$defs", "definitions"}
KEYWORDS = ANNOTATIONS | {"type", "properties", "required", "additionalProperties", "items", "enum", "const",
                          "pattern", "minItems", "maxItems", "minimum", "maximum", "minLength", "maxLength",
                          "uniqueItems", "$ref", "oneOf", "anyOf", "allOf", "minProperties", "patternProperties"}


class SchemaError(ValueError):
    """A document does not match its schema, or the schema uses an unsupported keyword."""


def _type_ok(value, name: str) -> bool:
    if name == "integer":
        return type(value) is int
    if name == "number":
        return type(value) in (int, float)
    return isinstance(value, TYPES[name]) and not (name != "boolean" and type(value) is bool)


def _resolve(root: dict, ref: str) -> dict:
    if not ref.startswith("#/"):
        raise SchemaError("only local $ref is supported: " + ref)
    node = root
    for part in ref[2:].split("/"):
        node = node[part]
    return node


def subset_validate(value, schema, root=None, path="$") -> None:
    root = root if root is not None else schema
    if schema is True:
        return
    if schema is False:
        raise SchemaError(path + ": not allowed")
    unknown = set(schema) - KEYWORDS
    if unknown:
        raise SchemaError(f"{path}: unsupported schema keyword(s) {sorted(unknown)}; install jsonschema")
    if "$ref" in schema:
        subset_validate(value, _resolve(root, schema["$ref"]), root, path)
    if "type" in schema:
        names = schema["type"] if isinstance(schema["type"], list) else [schema["type"]]
        if not any(_type_ok(value, n) for n in names):
            raise SchemaError(f"{path}: expected {names}")
    if "const" in schema and value != schema["const"]:
        raise SchemaError(f"{path}: must equal {schema['const']!r}")
    if "enum" in schema and value not in schema["enum"]:
        raise SchemaError(f"{path}: {value!r} not in {schema['enum']}")
    if isinstance(value, str):
        if "pattern" in schema and not re.search(schema["pattern"], value):
            raise SchemaError(f"{path}: does not match {schema['pattern']}")
        if len(value) < schema.get("minLength", 0) or len(value) > schema.get("maxLength", len(value)):
            raise SchemaError(f"{path}: length out of range")
    if type(value) in (int, float):
        if "minimum" in schema and value < schema["minimum"]:
            raise SchemaError(f"{path}: below minimum")
        if "maximum" in schema and value > schema["maximum"]:
            raise SchemaError(f"{path}: above maximum")
    if isinstance(value, list):
        if len(value) < schema.get("minItems", 0) or len(value) > schema.get("maxItems", len(value)):
            raise SchemaError(f"{path}: item count out of range")
        if schema.get("uniqueItems") and len({json.dumps(v, sort_keys=True) for v in value}) != len(value):
            raise SchemaError(f"{path}: items are not unique")
        if "items" in schema:
            for index, item in enumerate(value):
                subset_validate(item, schema["items"], root, f"{path}[{index}]")
    if isinstance(value, dict):
        for key in schema.get("required", []):
            if key not in value:
                raise SchemaError(f"{path}: missing required {key}")
        if len(value) < schema.get("minProperties", 0):
            raise SchemaError(f"{path}: too few properties")
        properties = schema.get("properties", {})
        patterns = schema.get("patternProperties", {})
        for key, item in value.items():
            matched = False
            if key in properties:
                subset_validate(item, properties[key], root, f"{path}.{key}")
                matched = True
            for pattern, sub in patterns.items():
                if re.search(pattern, key):
                    subset_validate(item, sub, root, f"{path}.{key}")
                    matched = True
            if not matched and "additionalProperties" in schema:
                subset_validate(item, schema["additionalProperties"], root, f"{path}.{key}")
    for keyword in ("allOf", "anyOf", "oneOf"):
        if keyword not in schema:
            continue
        passed = 0
        for sub in schema[keyword]:
            try:
                subset_validate(value, sub, root, path)
                passed += 1
            except SchemaError as error:
                if "unsupported schema keyword" in str(error):
                    raise
        if (keyword == "allOf" and passed != len(schema[keyword])) or (keyword == "anyOf" and not passed) \
                or (keyword == "oneOf" and passed != 1):
            raise SchemaError(f"{path}: {keyword} not satisfied")


def validate(document, schema) -> str:
    """Validate and return the engine name; raise SchemaError on any failure."""
    try:
        import jsonschema  # noqa: PLC0415 - optional, pinned in requirements-test.txt
    except ImportError:
        subset_validate(document, schema)
        return "subset"
    validator = jsonschema.Draft202012Validator(schema)
    errors = sorted(validator.iter_errors(document), key=lambda e: list(e.path))
    if errors:
        raise SchemaError("; ".join(f"{list(e.path)}: {e.message}" for e in errors[:5]))
    return "jsonschema " + jsonschema.__version__


def main(argv: list[str] | None = None) -> int:
    args = sys.argv[1:] if argv is None else argv
    if len(args) < 2:
        print(__doc__.strip().splitlines()[-1].strip(), file=sys.stderr)
        return 2
    schema = json.loads(Path(args[0]).read_text())
    status = 0
    for name in args[1:]:
        try:
            engine = validate(json.loads(Path(name).read_text()), schema)
            print(f"PASS {name} ({engine})")
        except (SchemaError, ValueError, OSError) as error:
            print(f"FAIL {name}: {error}")
            status = 1
    return status


if __name__ == "__main__":
    sys.exit(main())
