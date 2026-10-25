"""Source-coordinate AST facts for known text consumers.

The inventory is deliberately limited to preprocessed C calls and declarations.
It records expression/type/selector facts only: it does not resolve runtime
state, infer text from numeric IDs, or emit literal string contents.
"""
from __future__ import annotations

import json
import hashlib
import re
import subprocess
import sys
from pathlib import Path

from .text_renderers import (C_STRING_READ_ARGUMENTS, DIAGNOSTIC_ARGUMENT_INDEX,
                             DISPLAY_ARGUMENT_INDEX, FORMAT_ARGUMENT_INDEX,
                             known_calls, read_argument_specs, string_argument_role)

KNOWN_CALLS = known_calls()


def _unit_candidates(mapping, source: Path) -> set[str]:
    """Accept path-, overlay-, or link-unit-keyed candidate symbol maps."""
    rel = source.as_posix()
    parent = source.parent.as_posix()
    if source.parts[:2] == ("src", "ovl") and len(source.parts) > 2:
        overlay = source.parts[2]
        keys = (rel, parent, overlay, f"CDROM/EXEDIR/{overlay}.EXN")
    elif source.parts[:2] == ("src", "main"):
        keys = (rel, parent, "main", "SLPM_86.053")
    else:
        keys = (rel, parent)
    values = set()
    for key in keys:
        supplied = mapping.get(key, ())
        if isinstance(supplied, dict):
            supplied = supplied.get("symbols", supplied.keys())
        if isinstance(supplied, str):
            supplied = (supplied,)
        for item in supplied:
            values.add(item.get("symbol", item.get("name", item.get("label")))
                       if isinstance(item, dict) else item)
    return {value for value in values if isinstance(value, str)}


def _source_root(repository: Path) -> Path:
    root = repository.resolve()
    if (root / "src").is_dir():
        return root
    snapshots = sorted((root / ".asset-cache" / "text").glob("source-snapshot-*"))
    if snapshots:
        return snapshots[-1].resolve()
    raise ValueError("repository/source snapshot has no src/ tree")


def _source_unit(source: Path) -> str | None:
    source = Path(source)
    if source.parts[:2] == ("src", "main"):
        return "main"
    if source.parts[:2] == ("src", "ovl") and len(source.parts) > 2:
        return source.parts[2]
    return None


def _source_inputs(root: Path) -> list[tuple[str, str]]:
    """Return sorted relative paths and hashes for all C/header source inputs."""
    paths = set((root / "src").rglob("*.c")) | set((root / "include").rglob("*.h"))
    inputs = []
    for path in paths:
        resolved = path.resolve(strict=True)
        if not resolved.is_relative_to(root):
            raise ValueError("source input escapes selected source root")
        relative = path.relative_to(root).as_posix()
        inputs.append((relative, hashlib.sha256(resolved.read_bytes()).hexdigest()))
    return sorted(inputs)


def _fingerprint_inputs(inputs: list[tuple[str, str]]) -> str:
    digest = hashlib.sha256()
    for relative, source_hash in inputs:
        path_bytes = relative.encode("utf-8")
        digest.update(len(path_bytes).to_bytes(4, "big"))
        digest.update(path_bytes)
        digest.update(bytes.fromhex(source_hash))
    return digest.hexdigest()


def source_input_fingerprint(repository) -> str:
    """Hash the sorted relative paths and contents of all C and header inputs.

    The returned digest contains no source text and changes when a tracked input
    is added, removed, renamed, or edited.
    """
    root = _source_root(Path(repository))
    return _fingerprint_inputs(_source_inputs(root))


def _source_type(type_node, typedefs, seen=None):
    from pycparser import c_ast
    seen = set() if seen is None else seen
    if isinstance(type_node, c_ast.Decl):
        type_node = type_node.type
    if isinstance(type_node, c_ast.Typename):
        type_node = type_node.type
    if isinstance(type_node, c_ast.TypeDecl):
        if isinstance(type_node.type, c_ast.IdentifierType):
            names = list(type_node.type.names)
            if len(names) == 1 and names[0] in typedefs and names[0] not in seen:
                return _source_type(typedefs[names[0]], typedefs, seen | {names[0]})
            return {"kind": "scalar", "base_type": " ".join(names)}
        if isinstance(type_node.type, c_ast.Struct):
            members = type_node.type.decls or []
            return {"kind": "struct", "base_type": type_node.type.name,
                    "fields": [{"name": field.name, "type": _brief_type(field.type, typedefs)}
                               for field in members if field.name]}
        if isinstance(type_node.type, c_ast.Enum):
            return {"kind": "enum", "base_type": type_node.type.name}
    if isinstance(type_node, c_ast.PtrDecl):
        target = _source_type(type_node.type, typedefs, seen)
        return {"kind": "pointer", "pointers": 1 + target.get("pointers", 0), "target": target}
    if isinstance(type_node, c_ast.ArrayDecl):
        inner = _source_type(type_node.type, typedefs, seen)
        dimension = _constant(type_node.dim)
        return {"kind": "array", "dimension": dimension, "target": inner}
    if isinstance(type_node, c_ast.FuncDecl):
        return {"kind": "function", "result": _source_type(type_node.type, typedefs, seen)}
    if isinstance(type_node, c_ast.Struct):
        members = type_node.decls or []
        return {"kind": "struct", "base_type": type_node.name,
                "fields": [{"name": field.name, "type": _brief_type(field.type, typedefs)}
                           for field in members if field.name]}
    if isinstance(type_node, c_ast.IdentifierType):
        return {"kind": "scalar", "base_type": " ".join(type_node.names)}
    if type_node is None:
        return {"kind": "unknown"}
    return {"kind": type(type_node).__name__.lower()}


def _constant(node):
    from pycparser import c_ast
    if node is None:
        return None
    if isinstance(node, c_ast.Constant) and node.type in ("int", "char"):
        value = node.value
        try:
            value = value.rstrip("uUlL")
            if value.startswith(("0x", "0X")):
                return int(value, 16)
            if len(value) > 1 and value.startswith("0") and all(c in "01234567" for c in value):
                return int(value, 8)
            return int(value, 10)
        except ValueError:
            return None
    if isinstance(node, c_ast.UnaryOp) and node.op in ("+", "-"):
        value = _constant(node.expr)
        return value if value is None or node.op == "+" else -value
    if isinstance(node, c_ast.BinaryOp) and node.op in ("+", "-"):
        left, right = _constant(node.left), _constant(node.right)
        if left is not None and right is not None:
            return left + right if node.op == "+" else left - right
    return None


def _known_nonnull(node, declarations, typedefs):
    """Return True/False only for direct, source-visible nullness facts."""
    from pycparser import c_ast
    constant = _constant(node)
    if constant is not None:
        return constant != 0
    if isinstance(node, c_ast.Constant) and node.type == "string":
        return True
    if isinstance(node, c_ast.UnaryOp) and node.op == "&":
        return True
    if _expression_type(node, declarations, typedefs).get("kind") == "array":
        return True
    return None


def _read_condition_facts(conditions, args, declarations, typedefs):
    """Evaluate simple callee read guards without assuming runtime state."""
    facts = []
    for condition in conditions:
        index = condition["argument_index"]
        expression = args[index] if index < len(args) else None
        if expression is None:
            status = "missing_argument"
            observed = None
        elif condition["operator"] == "nonnull":
            observed = _known_nonnull(expression, declarations, typedefs)
            status = "proven_true" if observed is True else "proven_false" if observed is False else "unknown"
        else:
            observed = _constant(expression)
            if observed is None:
                status = "unknown"
            elif condition["operator"] == "==":
                status = "proven_true" if observed == condition["value"] else "proven_false"
            elif condition["operator"] == "!=":
                status = "proven_true" if observed != condition["value"] else "proven_false"
            else:
                status = "unsupported_operator"
        facts.append({**condition, "observed": observed, "status": status})
    aggregate = ("proven_false" if any(fact["status"] == "proven_false" for fact in facts) else
                 "proven_true" if facts and all(fact["status"] == "proven_true" for fact in facts) else
                 "conditional" if facts else "unconditional")
    return facts, aggregate


def _brief_type(type_node, typedefs):
    """Describe a struct member without recursively expanding nested structs."""
    from pycparser import c_ast
    if isinstance(type_node, c_ast.Decl):
        type_node = type_node.type
    if isinstance(type_node, c_ast.TypeDecl):
        if isinstance(type_node.type, c_ast.IdentifierType):
            names = list(type_node.type.names)
            if len(names) == 1 and names[0] in typedefs:
                return _brief_type(typedefs[names[0]], typedefs)
            return {"kind": "scalar", "base_type": " ".join(names)}
        if isinstance(type_node.type, c_ast.Struct):
            return {"kind": "struct", "base_type": type_node.type.name}
    if isinstance(type_node, c_ast.PtrDecl):
        return {"kind": "pointer", "target": _brief_type(type_node.type, typedefs)}
    if isinstance(type_node, c_ast.ArrayDecl):
        return {"kind": "array", "dimension": _constant(type_node.dim),
                "target": _brief_type(type_node.type, typedefs)}
    if isinstance(type_node, c_ast.Struct):
        return {"kind": "struct", "base_type": type_node.name}
    return {"kind": type(type_node).__name__.lower()}


def _redacted_expr(node, generator):
    from pycparser import c_ast
    text = generator.visit(node)
    # Also mask strings emitted through concatenated expressions or macros.
    text = re.sub(r'"(?:\\.|[^"\\])*"', '"<string-literal>"', text)
    return re.sub(r"'(?:\\.|[^'\\])*'", "'<character-literal>'", text)


def _ids(node):
    from pycparser import c_ast
    found = set()
    class V(c_ast.NodeVisitor):
        def visit_ID(self, item):
            found.add(item.name)
        def visit_StructRef(self, item):
            self.visit(item.name)
    V().visit(node)
    return found


def _selector_facts(expression, candidates):
    from pycparser import c_ast
    facts = []
    generator = _GENERATOR()
    def walk(node, use="argument_value"):
        if isinstance(node, c_ast.ArrayRef):
            selectors = []
            base = node
            while isinstance(base, c_ast.ArrayRef):
                selectors.append(base.subscript)
                base = base.name
            selectors.reverse()
            relevant = sorted(_ids(base) & candidates)
            if relevant:
                facts.append({"base_symbols": relevant, "expression_use": use,
                              "selector_order": "root_to_leaf",
                              "selectors": [{"index": index, "constant": _constant(selector),
                                              "expression": _redacted_expr(selector, generator),
                                              "dynamic": _constant(selector) is None,
                                              "candidate_symbols": sorted(_ids(selector) & candidates)}
                                             for index, selector in enumerate(selectors)],
                              "dereference_count": len(selectors),
                              "dereference_kind": "array_or_pointer_subscript"})
            # Candidate identifiers inside selector expressions are numeric
            # index inputs, not string-address roots; retain their nested accesses.
            for selector in selectors:
                walk(selector, "selector_expression")
            if not isinstance(base, (c_ast.ID, c_ast.StructRef)):
                walk(base, use)
            elif isinstance(base, c_ast.StructRef):
                walk(base.name, use)
            return
        if isinstance(node, c_ast.StructRef):
            walk(node.name, use)
            return
        for _, child in node.children():
            walk(child, use)
    walk(expression)
    return facts


def _flow_analysis(expression, candidates):
    """Separate text-address value flow from selectors and unresolved expressions."""
    from pycparser import c_ast
    occurrence_roles = {}
    flow = set()
    mentioned = set()
    value_roles = {"value", "pointer_base", "address_operand", "member_base"}
    def visit(node, role="value"):
        if isinstance(node, c_ast.ID):
            if node.name in candidates:
                occurrence_roles[id(node)] = role
                mentioned.add(node.name)
                if role in value_roles:
                    flow.add(node.name)
            return
        if isinstance(node, c_ast.ArrayRef):
            visit(node.name, "pointer_base" if role == "value" else role)
            visit(node.subscript, "index_operand")
            return
        if isinstance(node, c_ast.StructRef):
            visit(node.name, "member_base" if role == "value" else role)
            return
        if isinstance(node, c_ast.TernaryOp):
            visit(node.cond, "condition_operand")
            visit(node.iftrue, role)
            visit(node.iffalse, role)
            return
        if isinstance(node, c_ast.UnaryOp):
            child_role = (("address_operand" if role == "value" else role) if node.op == "&" else
                          ("pointer_base" if role == "value" else role) if node.op == "*" else
                          "unresolved_unary_operand")
            visit(node.expr, child_role)
            return
        if isinstance(node, c_ast.BinaryOp):
            visit(node.left, "unresolved_binary_operand")
            visit(node.right, "unresolved_binary_operand")
            return
        if isinstance(node, c_ast.FuncCall):
            if node.args:
                for arg in node.args.exprs:
                    visit(arg, "unresolved_call_argument")
            return
        if isinstance(node, c_ast.ExprList):
            for index, item in enumerate(node.exprs):
                visit(item, role if index == len(node.exprs) - 1 else "unresolved_comma_operand")
            return
        if isinstance(node, c_ast.Assignment):
            visit(node.lvalue, "unresolved_assignment_lvalue")
            visit(node.rvalue, role)
            return
        for _, child in node.children():
            visit(child, role)
    visit(expression)
    return {"value_flow_symbols": sorted(flow),
            "unresolved_symbols": sorted(mentioned - flow),
            "occurrence_roles": occurrence_roles}


def _expression_type(node, declarations, typedefs):
    """Infer only straightforward C expression types needed for pointer-flow gates."""
    from pycparser import c_ast
    unknown = {"kind": "unknown"}
    if isinstance(node, c_ast.ID):
        declaration = declarations.get(node.name)
        return declaration["type"] if declaration else unknown
    if isinstance(node, c_ast.ArrayRef):
        base = _expression_type(node.name, declarations, typedefs)
        if base.get("kind") in ("array", "pointer"):
            return base.get("target", unknown)
        return unknown
    if isinstance(node, c_ast.StructRef):
        base = _expression_type(node.name, declarations, typedefs)
        if node.type == "->" and base.get("kind") == "pointer":
            base = base.get("target", unknown)
        if base.get("kind") == "struct":
            field_name = getattr(node.field, "name", None)
            for field in base.get("fields", []):
                if field["name"] == field_name:
                    return field["type"]
        return unknown
    if isinstance(node, c_ast.UnaryOp):
        child = _expression_type(node.expr, declarations, typedefs)
        if node.op == "&":
            return {"kind": "pointer", "pointers": 1, "target": child}
        if node.op == "*" and child.get("kind") in ("pointer", "array"):
            return child.get("target", unknown)
        return unknown
    if isinstance(node, c_ast.Cast):
        return _source_type(node.to_type, typedefs)
    if isinstance(node, c_ast.TernaryOp):
        yes = _expression_type(node.iftrue, declarations, typedefs)
        no = _expression_type(node.iffalse, declarations, typedefs)
        return yes if yes == no else unknown
    return unknown


def _string_pointer(type_info):
    if type_info.get("kind") == "array":
        type_info = {"kind": "pointer", "target": type_info.get("target", {"kind": "unknown"})}
    if type_info.get("kind") != "pointer":
        return False
    target = type_info.get("target", {})
    if target.get("kind") != "scalar":
        return False
    names = set(target.get("base_type", "").split())
    return bool(names & {"char", "u8", "s8", "signed_char", "unsigned_char"})


def _argument_type(expression, expression_type, declarations, typedefs):
    """Apply C array-to-pointer conversion at a function-call argument."""
    from pycparser import c_ast
    if isinstance(expression, c_ast.Cast):
        target_type = _source_type(expression.to_type, typedefs)
        source_type = _argument_type(expression.expr,
                                     _expression_type(expression.expr, declarations, typedefs),
                                     declarations, typedefs)
        if _integer_32_scalar(target_type) and _string_pointer(source_type):
            return {"kind": "pointer_compatible_scalar_argument", "bit_width": 32,
                    "bit_preserving": True, "target_type": target_type,
                    "source_pointer_type": source_type,
                    "source_type_is_not_claimed_as_c_pointer": True}
        return {"kind": "cast_argument", "target_type": target_type,
                "source_type": source_type, "pointer_compatibility": "unresolved"}
    if expression_type.get("kind") == "array":
        return {"kind": "pointer", "pointers": 1,
                "target": expression_type.get("target", {"kind": "unknown"})}
    return expression_type


def _GENERATOR():
    from pycparser import c_generator
    return c_generator.CGenerator()


def _scalar_width(type_info):
    if type_info.get("kind") == "pointer":
        return 4
    if type_info.get("kind") != "scalar":
        return None
    words = type_info.get("base_type", "").split()
    names = set(words)
    if "double" in names or words.count("long") >= 2:
        return 8
    if "float" in names:
        return 4
    if names & {"u8", "s8", "char", "unsigned_char", "signed_char"}:
        return 1
    if names & {"u16", "s16", "short", "unsigned_short", "signed_short"}:
        return 2
    if names & {"u32", "s32", "int", "unsigned", "long", "unsigned_long", "signed"}:
        return 4
    return None


def _integer_32_scalar(type_info):
    """Recognize only known 32-bit integer scalars for pointer casts."""
    if type_info.get("kind") != "scalar" or _scalar_width(type_info) != 4:
        return False
    names = set(type_info.get("base_type", "").split())
    return bool(names & {"u32", "s32", "int", "unsigned", "signed", "long"})


def _symbol_occurrences(expression, candidates, declarations, occurrence_roles,
                        source_pointer_symbols):
    from pycparser import c_ast
    occurrences = []
    def walk(node, parents=()):
        if isinstance(node, c_ast.ID) and node.name in candidates:
            decl = declarations.get(node.name)
            type_info = decl["type"] if decl else {"kind": "unknown"}
            kind = type_info.get("kind")
            derefs = sum(isinstance(parent, c_ast.ArrayRef) and child_name == "name"
                         or isinstance(parent, c_ast.StructRef) and parent.type == "->" and child_name == "name"
                         or isinstance(parent, c_ast.UnaryOp) and parent.op == "*" and child_name == "expr"
                         for parent, child_name in parents)
            address_of = False
            for parent, child_name in parents:
                if isinstance(parent, c_ast.UnaryOp) and parent.op == "&" and child_name == "expr":
                    address_of = True
            value_semantics = ("array_decay_address_no_global_load" if kind == "array" and derefs == 0 else
                               "pointer_or_scalar_value_load" if kind in ("pointer", "scalar") and derefs == 0 else
                               "address_of_expression" if address_of else "indexed_or_dereferenced_value")
            width = _scalar_width(type_info)
            local_address_only = address_of and derefs == 0
            occurrences.append({"symbol": node.name, "global_type": type_info,
                                "global_declaration": decl,
                                "occurrence_role": occurrence_roles.get(id(node), "unresolved"),
                                "pointer_value_flows_to_argument": node.name in source_pointer_symbols and
                                    occurrence_roles.get(id(node)) in
                                    ("value", "pointer_base", "address_operand", "member_base"),
                                "address_of": address_of, "dereference_count": derefs,
                                "global_value_load_bytes": 0 if kind == "array" or local_address_only else width,
                                "global_value_load_words": (0 if kind == "array" or local_address_only else
                                                            1 if width == 4 else None),
                                "value_semantics": value_semantics})
        for child_name, child in node.children():
            if isinstance(node, c_ast.StructRef) and child_name == "field":
                continue
            walk(child, parents + ((node, child_name),))
    walk(expression)
    return occurrences


def _declarations(ast, candidates):
    from pycparser import c_ast
    typedefs, declarations = {}, {}
    for item in ast.ext:
        if isinstance(item, c_ast.Typedef):
            typedefs[item.name] = item.type
    for item in ast.ext:
        if isinstance(item, c_ast.Decl) and item.name in candidates:
            info = {"name": item.name, "type": _source_type(item.type, typedefs),
                    "storage": list(item.storage or []),
                    "coord": str(item.coord) if item.coord else None,
                    "is_definition": item.init is not None}
            old = declarations.get(item.name)
            if old is None or info["is_definition"] or not old["is_definition"]:
                declarations[item.name] = info
    return declarations, typedefs


def _process_file(root: Path, source: Path, symbols: set[str]) -> dict:
    from pycparser import c_ast, c_parser
    rel = source.as_posix()
    command = ["gcc", "-E", "-std=gnu89", "-DM2CTX", "-Iinclude", rel]
    try:
        preprocessed = subprocess.run(command, cwd=root, check=True,
                                      stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                      text=True).stdout
    except (OSError, subprocess.CalledProcessError) as exc:
        diagnostic = getattr(exc, "stderr", b"")
        if isinstance(diagnostic, str):
            diagnostic = diagnostic.encode("utf-8")
        return {"path": rel, "status": "preprocess_error",
                "diagnostic_sha256": hashlib.sha256(diagnostic).hexdigest()}
    try:
        ast = c_parser.CParser().parse(preprocessed, filename=rel)
    except Exception as exc:
        return {"path": rel, "status": "parse_error", "error_type": type(exc).__name__,
                "coordinate": str(getattr(exc, "coord", ""))}
    declarations, typedefs = _declarations(ast, symbols)
    unit = _source_unit(rel)
    calls = []
    current_function = {"name": None}
    current_locals = {"names": set()}
    generator = _GENERATOR()
    candidate_hits = 0
    class V(c_ast.NodeVisitor):
        def visit_FuncDef(self, node):
            previous = current_function["name"]
            previous_locals = current_locals["names"]
            current_function["name"] = node.decl.name
            local_names = set()
            args = getattr(node.decl.type, "args", None)
            for param in (args.params if args else []):
                if isinstance(param, c_ast.Decl) and param.name:
                    local_names.add(param.name)
            for param in (node.param_decls or []):
                if param.name:
                    local_names.add(param.name)
            class Locals(c_ast.NodeVisitor):
                def visit_Decl(self, declaration):
                    if declaration.name and "extern" not in (declaration.storage or []):
                        local_names.add(declaration.name)
                    self.generic_visit(declaration)
                def visit_Struct(self, _node):
                    return
            Locals().visit(node.body)
            current_locals["names"] = local_names
            self.visit(node.body)
            current_function["name"] = previous
            current_locals["names"] = previous_locals
        def visit_FuncCall(self, node):
            name = node.name.name if isinstance(node.name, c_ast.ID) else None
            if name in known_calls(unit) and name not in current_locals["names"]:
                args = node.args.exprs if node.args else []
                specs = {spec["index"]: spec for spec in read_argument_specs(name, unit)}
                for index, expression in enumerate(args):
                    spec = specs.get(index)
                    if spec is None:
                        continue
                    role = spec["role"]
                    read_conditions, condition_status = _read_condition_facts(
                        spec["conditions"], args, declarations, typedefs)
                    if condition_status == "proven_false":
                        continue
                    ids = _ids(expression)
                    candidates = (ids & symbols) - current_locals["names"]
                    flow = _flow_analysis(expression, candidates)
                    expression_type = _argument_type(
                        expression, _expression_type(expression, declarations, typedefs),
                        declarations, typedefs)
                    pointer_argument = (_string_pointer(expression_type) or
                                        expression_type.get("kind") == "pointer_compatible_scalar_argument")
                    source_pointer_symbols = ({
                        symbol for symbol in flow["value_flow_symbols"]
                        if declarations.get(symbol, {}).get("type", {}).get("kind")
                        in ("pointer", "array", "struct")
                    } if pointer_argument else set())
                    if pointer_argument:
                        matches = sorted(source_pointer_symbols)
                        unresolved = sorted(set(flow["unresolved_symbols"]) |
                                            (set(flow["value_flow_symbols"]) - source_pointer_symbols))
                    else:
                        matches = []
                        unresolved = sorted(candidates)
                    if not matches and not unresolved:
                        continue
                    coord = node.coord
                    if coord and coord.file:
                        coord_path = Path(coord.file)
                        if coord_path.is_absolute():
                            try:
                                coord_name = coord_path.resolve().relative_to(root).as_posix()
                            except ValueError:
                                continue
                        else:
                            coord_name = coord_path.as_posix()
                        if coord_name != rel:
                            continue
                    nonlocal_candidate[0] += len(matches)
                    calls.append({"source_path": rel, "line": coord.line if coord else None,
                                  "column": coord.column if coord else None,
                                  "source_function": current_function["name"], "callee": name,
                                  "source_unit": unit, "argument_index": index, "argument_role": role,
                                  "renderer": spec["renderer"],
                                  "renderer_status": spec.get("renderer_status", "source_verified_call"),
                                  "callee_read_conditions": read_conditions,
                                  "callee_read_condition_status": condition_status,
                                  "argument_expression": _redacted_expr(expression, generator),
                                  "candidate_symbols": matches,
                                  "unresolved_candidate_symbols": unresolved,
                                  "argument_expression_type": expression_type,
                                  "symbol_facts": _symbol_occurrences(expression, candidates, declarations,
                                                                      flow["occurrence_roles"],
                                                                      source_pointer_symbols),
                                  "selectors": _selector_facts(expression, candidates),
                                  "value_flow_status": "source_pointer_candidate" if matches else
                                                      "unresolved_candidate_expression"})
            self.generic_visit(node)
    nonlocal_candidate = [0]
    V().visit(ast)
    return {"path": rel, "status": "parsed", "candidate_symbol_count": len(symbols),
            "candidate_occurrence_count": nonlocal_candidate[0], "calls": calls,
            "candidate_declarations": [declarations[name] for name in sorted(symbols)
                                       if name in declarations]}


def inventory_text_consumers(repository, candidate_symbols_by_unit, files=None) -> dict:
    """Inventory known C text-call AST arguments using source/global type facts.

    ``candidate_symbols_by_unit`` maps source paths, overlay IDs (for example
    ``OLH``), or EXN link-unit names to symbol names or symbol records. ``files``
    optionally limits processing to source paths relative to the selected root.
    Candidate text is never decoded; C string literal expressions are redacted.
    """
    root = _source_root(Path(repository))
    inputs_before = _source_inputs(root)
    inputs_before_hash = _fingerprint_inputs(inputs_before)
    input_hash_by_path = dict(inputs_before)
    import pycparser
    try:
        gcc_version = subprocess.run(["gcc", "--version"], cwd=root, check=True,
                                     stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
                                     text=True).stdout.splitlines()[0]
    except (OSError, subprocess.CalledProcessError, IndexError):
        gcc_version = "unavailable"
    if files is None:
        sources = sorted(set((root / "src/main").rglob("*.c")) |
                         set((root / "src/ovl").rglob("*.c")))
    else:
        sources = [root / Path(path) for path in files]
    results = []
    for source in sources:
        if not source.is_file() or not source.resolve().is_relative_to(root):
            results.append({"path": source.as_posix(), "status": "missing_or_outside_root"})
            continue
        rel = source.relative_to(root)
        symbols = _unit_candidates(candidate_symbols_by_unit, rel)
        result = _process_file(root, rel, symbols)
        result["source_sha256"] = hashlib.sha256(source.read_bytes()).hexdigest()
        results.append(result)
    scan_status = "stable"
    try:
        inputs_after = _source_inputs(root)
        inputs_after_hash = _fingerprint_inputs(inputs_after)
        stable = (inputs_before_hash == inputs_after_hash and
                  all(item.get("source_sha256") == input_hash_by_path.get(item.get("path"))
                      for item in results if item.get("source_sha256")))
        if not stable:
            scan_status = "source_inputs_changed_during_scan"
    except (OSError, ValueError):
        inputs_after_hash = None
        stable = False
        scan_status = "source_inputs_unreadable_during_scan"
    return {"schema": "tokimemo.text.ast-consumers.v1",
            "source_root": root.name, "python_ast_parser": "pycparser",
            "python_version": sys.version.split()[0], "pycparser_version": pycparser.__version__,
            "preprocessor": gcc_version,
            "source_inputs_sha256": inputs_before_hash,
            "source_inputs_sha256_after": inputs_after_hash,
            "stable_source_inputs": stable,
            "source_input_scan_status": scan_status,
            "candidate_symbol_count": sum(item.get("candidate_symbol_count", 0) for item in results),
            "consumer_call_count": sum(len(item.get("calls", [])) for item in results),
            "files": results,
            "limits": ["source_dataflow_only_no_runtime_execution", "unknown_indirect_calls_not_resolved",
                       "string_literal_contents_redacted", "candidate_symbol_map_scope_is_caller_supplied",
                       "block_local_shadowing_is_conservatively_excluded_for_the_whole_function"]}
