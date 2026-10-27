#!/usr/bin/env python3
"""Prune existing elf_ctrl_pkt.bin files from 12 duplicate copies down to one.

Older builds packed 12 copies of the same control packet (one per
0_0..2_3 core) into each prebuilt/<combo>/elf_ctrl_pkt.bin, back when
every core needed its own copy. Now that all 12 cores read the same
control packet, only the first copy needs to be kept. This script finds
every elf_ctrl_pkt.bin under this prebuilt directory, verifies its size
is exactly 12x the packet_size recorded in the sibling packet_size.json,
and truncates it to just the first copy.
"""
import argparse
import json
from pathlib import Path

EXPECTED_COPIES = 12


def prune_dir(combo_dir: Path, dry_run: bool) -> str:
    bin_path = combo_dir / "elf_ctrl_pkt.bin"
    json_path = combo_dir / "packet_size.json"

    if not bin_path.exists() or not json_path.exists():
        return "skip (missing elf_ctrl_pkt.bin or packet_size.json)"

    with json_path.open("r", encoding="utf-8") as f:
        packet_size = int(json.load(f)["packet_size"])

    file_size = bin_path.stat().st_size

    if file_size == packet_size:
        return "already pruned"

    assert file_size == packet_size * EXPECTED_COPIES, (
        f"{bin_path}: size {file_size} is not packet_size ({packet_size}) "
        f"x {EXPECTED_COPIES} copies (got ratio {file_size / packet_size})"
    )

    if dry_run:
        return f"would prune {file_size} -> {packet_size} bytes"

    with bin_path.open("r+b") as f:
        f.truncate(packet_size)

    return f"pruned {file_size} -> {packet_size} bytes"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--dry-run", action="store_true",
        help="Report what would change without modifying any files.",
    )
    args = parser.parse_args()

    prebuilt_dir = Path(__file__).resolve().parent

    for bin_path in sorted(prebuilt_dir.glob("*/elf_ctrl_pkt.bin")):
        combo_dir = bin_path.parent
        result = prune_dir(combo_dir, args.dry_run)
        print(f"{combo_dir.name}: {result}")


if __name__ == "__main__":
    main()
