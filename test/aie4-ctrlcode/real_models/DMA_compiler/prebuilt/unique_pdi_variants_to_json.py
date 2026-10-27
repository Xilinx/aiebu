"""
Regenerate unique_pdi_variants.json from unique_pdi_variants.py (source of truth).

Usage:
    python unique_pdi_variants_to_json.py
"""
import ast
import json
import os
import re

PREBUILT = os.path.dirname(os.path.abspath(__file__))
PY_PATH = os.path.join(PREBUILT, "unique_pdi_variants.py")
JSON_PATH = os.path.join(PREBUILT, "unique_pdi_variants.json")


def load_py():
    with open(PY_PATH) as f:
        content = f.read()
    match = re.search(r"pdi_variants\s*=\s*(\{.*)", content, re.DOTALL)
    if not match:
        raise SystemExit(f"ERROR: could not find 'pdi_variants = {{...}}' in {PY_PATH}")
    return ast.literal_eval(match.group(1).rstrip())


def main():
    pdi_variants = load_py()
    with open(JSON_PATH, "w") as f:
        json.dump(pdi_variants, f, indent=2)
        f.write("\n")
    print(f"Wrote {len(pdi_variants)} PDI combinations to {JSON_PATH}")


if __name__ == "__main__":
    main()
