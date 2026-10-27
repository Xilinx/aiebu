"""
Verify that unique_pdi_variants.json matches:
  1. unique_pdi_variants.py  (source of truth)
  2. kernel.md files in each fp16_pdi_with_pdi_combination_* folder

Note: folders without a JSON/py entry (e.g. pdi_combination_39..42) are
reported as informational warnings — they are newer PDI builds not yet
registered in unique_pdi_variants.py and are intentionally left for a
follow-up update to that file.
"""
import ast
import json
import os
import re
import sys

PREBUILT = os.path.dirname(os.path.abspath(__file__))
JSON_PATH = os.path.join(PREBUILT, "unique_pdi_variants.json")
PY_PATH   = os.path.join(PREBUILT, "unique_pdi_variants.py")


def load_json():
    with open(JSON_PATH) as f:
        return json.load(f)


def load_py():
    with open(PY_PATH) as f:
        content = f.read()
    match = re.search(r"pdi_variants\s*=\s*(\{.*)", content, re.DOTALL)
    if not match:
        sys.exit(f"ERROR: could not find 'pdi_variants = {{...}}' in {PY_PATH}")
    return ast.literal_eval(match.group(1).rstrip())


def load_kernel_md(combo):
    folder = os.path.join(PREBUILT, f"fp16_pdi_with_{combo}")
    md_path = os.path.join(folder, "kernel.md")
    if not os.path.exists(folder):
        return None, "folder missing"
    if not os.path.exists(md_path):
        return None, "kernel.md missing"
    with open(md_path) as f:
        content = f.read()
    section = re.search(r"## Kernels Included\s*```\s*(.*?)\s*```", content, re.DOTALL)
    if not section:
        return None, "no 'Kernels Included' section"
    kernels = {}
    for line in section.group(1).strip().splitlines():
        line = line.strip()
        if not line:
            continue
        name, _, kid = line.partition(":")
        kernels[name.strip()] = int(kid.strip())
    return kernels, None


def diff_kernels(a, b, a_label, b_label):
    issues = []
    only_a  = {k: v for k, v in a.items() if k not in b}
    only_b  = {k: v for k, v in b.items() if k not in a}
    id_diff = {k: (a[k], b[k]) for k in a if k in b and a[k] != b[k]}
    if only_a:
        issues.append(f"  Only in {a_label}: {only_a}")
    if only_b:
        issues.append(f"  Only in {b_label}: {only_b}")
    if id_diff:
        issues.append(f"  ID mismatch ({a_label} id, {b_label} id): {id_diff}")
    return issues


def main():
    json_data = load_json()
    py_data   = load_py()

    all_combos = sorted(
        set(list(json_data.keys()) + list(py_data.keys())),
        key=lambda s: int(s.split("_")[-1])
    )

    # ── 1. JSON vs .py ────────────────────────────────────────────────
    print("=" * 60)
    print("CHECK 1: JSON vs .py")
    print("=" * 60)
    py_errors = []
    for combo in all_combos:
        if combo not in json_data:
            py_errors.append(f"[ONLY IN PY]   {combo}")
            continue
        if combo not in py_data:
            py_errors.append(f"[ONLY IN JSON] {combo}")
            continue
        issues = diff_kernels(
            json_data[combo].get("combined_kernel_names", {}),
            py_data[combo].get("combined_kernel_names", {}),
            "json", "py"
        )
        json_incs = sorted(json_data[combo].get("combined_kernel_includes", []))
        py_incs   = sorted(py_data[combo].get("combined_kernel_includes", []))
        if json_incs != py_incs:
            only_json = [x for x in json_incs if x not in py_incs]
            only_py   = [x for x in py_incs   if x not in json_incs]
            if only_json:
                issues.append(f"  includes only in json: {only_json}")
            if only_py:
                issues.append(f"  includes only in py:   {only_py}")
        if issues:
            py_errors.append(f"\n[MISMATCH] {combo}")
            py_errors.extend(issues)

    if py_errors:
        for e in py_errors:
            print(e)
    else:
        print(f"OK — all {len(json_data)} combinations match the .py")

    # ── 2. JSON vs kernel.md ──────────────────────────────────────────
    print()
    print("=" * 60)
    print("CHECK 2: JSON vs kernel.md files")
    print("=" * 60)

    md_errors = []
    # Check every combo in the JSON
    for combo in sorted(json_data.keys(), key=lambda s: int(s.split("_")[-1])):
        md_kernels, err = load_kernel_md(combo)
        if err:
            md_errors.append(f"[{err.upper()}] {combo}")
            continue
        issues = diff_kernels(
            json_data[combo].get("combined_kernel_names", {}),
            md_kernels,
            "json", "kernel.md"
        )
        if issues:
            md_errors.append(f"\n[MISMATCH] {combo}")
            md_errors.extend(issues)

    # Warn about folders on disk not yet registered in JSON/py (not an error —
    # these are newer PDI builds pending addition to unique_pdi_variants.py).
    for name in sorted(os.listdir(PREBUILT)):
        if name.startswith("fp16_pdi_with_pdi_combination_"):
            combo = name[len("fp16_pdi_with_"):]
            if combo not in json_data:
                print(f"[WARN] folder on disk not in JSON (pending registration): {combo}")

    if md_errors:
        for e in md_errors:
            print(e)
    else:
        print(f"OK — all combinations match their kernel.md files")

    # ── Summary ───────────────────────────────────────────────────────
    print()
    print("=" * 60)
    print("SUMMARY")
    print("=" * 60)
    print(f"JSON entries   : {len(json_data)}")
    print(f"PY entries     : {len(py_data)}")
    print(f"JSON vs PY     : {'PASS' if not py_errors else 'FAIL'}")
    print(f"JSON vs md     : {'PASS' if not md_errors else 'FAIL'}")

    if py_errors or md_errors:
        sys.exit(1)


if __name__ == "__main__":
    main()
