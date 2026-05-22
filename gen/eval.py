#!/usr/bin/env python3
"""
Score the diff between generated and original mx/core files.

Usage:
    python3 gen/eval.py [--config path/to/config.yaml] [--verbose]

Run gen/generate.py first, then this script scores the resulting
git diff against src/private/mx/core/.
"""

import importlib
import re
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

import yaml


@dataclass
class DiffLine:
    content: str
    is_added: bool
    file_path: str
    hunk_id: int


@dataclass
class DiffHunk:
    file_path: str
    hunk_id: int
    lines: list  # list of DiffLine
    context: list  # context lines (unchanged) for reference


@dataclass
class ClassifiedLine:
    line: DiffLine
    category: str
    rule_name: str
    penalty: float


def get_diff():
    result = subprocess.run(
        ["git", "diff", "-U3", "--", "src/private/mx/core/"],
        capture_output=True,
        text=True,
    )
    return result.stdout


def parse_diff(diff_text: str) -> list:
    """Parse unified diff into DiffHunks."""
    hunks = []
    current_file = None
    current_hunk_lines = []
    current_context = []
    hunk_id = 0

    for raw_line in diff_text.split("\n"):
        if raw_line.startswith("diff --git"):
            if current_hunk_lines:
                hunks.append(
                    DiffHunk(current_file, hunk_id, current_hunk_lines, current_context)
                )
                current_hunk_lines = []
                current_context = []
            match = re.search(r" b/(.+)$", raw_line)
            if match:
                current_file = match.group(1)
        elif raw_line.startswith("@@"):
            if current_hunk_lines:
                hunks.append(
                    DiffHunk(current_file, hunk_id, current_hunk_lines, current_context)
                )
            hunk_id += 1
            current_hunk_lines = []
            current_context = []
        elif raw_line.startswith("+") and not raw_line.startswith("+++"):
            current_hunk_lines.append(
                DiffLine(raw_line[1:], True, current_file, hunk_id)
            )
        elif raw_line.startswith("-") and not raw_line.startswith("---"):
            current_hunk_lines.append(
                DiffLine(raw_line[1:], False, current_file, hunk_id)
            )
        elif not raw_line.startswith("\\"):
            current_context.append(raw_line)

    if current_hunk_lines:
        hunks.append(
            DiffHunk(current_file, hunk_id, current_hunk_lines, current_context)
        )

    return hunks


def is_whitespace_only_hunk(hunk: DiffHunk) -> bool:
    """Check if a hunk's changes are purely whitespace/formatting."""
    removed = [dl.content for dl in hunk.lines if not dl.is_added]
    added = [dl.content for dl in hunk.lines if dl.is_added]

    removed_stripped = "".join("".join(l.split()) for l in removed)
    added_stripped = "".join("".join(l.split()) for l in added)

    if not removed_stripped and not added_stripped:
        return True

    if removed_stripped == added_stripped:
        return True

    return False


def detect_const_qualification(hunk: DiffHunk) -> list:
    """Detect const bool -> bool changes in a hunk. Returns indices of matched lines."""
    removed = [(i, dl) for i, dl in enumerate(hunk.lines) if not dl.is_added]
    added = [(i, dl) for i, dl in enumerate(hunk.lines) if dl.is_added]

    matched_indices = set()
    for ri, rdl in removed:
        rs = rdl.content.strip()
        if "const bool" not in rs and "const " not in rs:
            continue
        without_const = rs.replace("const ", "", 1)
        for ai, adl in added:
            if ai in matched_indices:
                continue
            if adl.content.strip() == without_const:
                matched_indices.add(ri)
                matched_indices.add(ai)
                break

    return matched_indices


def detect_enum_parse_change(hunk: DiffHunk) -> list:
    """Detect parseXEnum() -> myValue.parse() changes."""
    matched = set()
    for i, dl in enumerate(hunk.lines):
        content = dl.content.strip()
        if not dl.is_added and re.search(r"parse\w+Enum\(", content):
            matched.add(i)
        elif dl.is_added and "myValue.parse(" in content:
            for j, other in enumerate(hunk.lines):
                if not other.is_added and re.search(r"parse\w+Enum\(", other.content):
                    matched.add(i)
                    matched.add(j)
                    break

    return matched


def detect_ctor_default_change(hunk: DiffHunk) -> list:
    """Detect myValue(SomeDefault) -> myValue() changes."""
    matched = set()
    for i, dl in enumerate(hunk.lines):
        content = dl.content.strip()
        if not dl.is_added and re.search(
            r"myValue\([A-Z]\w+::\w+\)", content
        ):
            for j, other in enumerate(hunk.lines):
                if other.is_added and "myValue()" in other.content:
                    matched.add(i)
                    matched.add(j)
                    break
        elif not dl.is_added and re.search(r"myValue\{\d+\}", content):
            for j, other in enumerate(hunk.lines):
                if other.is_added and "myValue()" in other.content:
                    matched.add(i)
                    matched.add(j)
                    break
        elif not dl.is_added and re.search(r"myValue\(\d+\.\d+\)", content):
            for j, other in enumerate(hunk.lines):
                if other.is_added and "myValue()" in other.content:
                    matched.add(i)
                    matched.add(j)
                    break

    return matched


def classify_hunks(hunks: list, config: dict,
                   overwrite_files: set = None) -> list:
    """Classify all diff lines using hunk-level and line-level rules."""
    results = []
    weights = config["weights"]
    defaults = config.get("defaults", {})
    file_cats = config.get("file_categories", {})
    line_rules = config.get("line_rules", {})
    if overwrite_files is None:
        overwrite_files = set()

    for hunk in hunks:
        if hunk.file_path and overwrite_files:
            basename = Path(hunk.file_path).stem if hunk.file_path else ""
            if basename in overwrite_files:
                for dl in hunk.lines:
                    results.append(
                        ClassifiedLine(dl, "EXPECTED", "overwrite element", 0)
                    )
                continue

        file_override = None
        for cat, entries in file_cats.items():
            for entry in entries or []:
                pattern = entry if isinstance(entry, str) else entry.get("pattern", "")
                if re.search(pattern, hunk.file_path or ""):
                    file_override = cat
                    break
            if file_override:
                break

        if file_override:
            for dl in hunk.lines:
                results.append(
                    ClassifiedLine(
                        dl, file_override, "file override", weights.get(file_override, 0)
                    )
                )
            continue

        if is_whitespace_only_hunk(hunk):
            for dl in hunk.lines:
                results.append(
                    ClassifiedLine(dl, "EXPECTED", "whitespace/formatting only", 0)
                )
            continue

        const_indices = detect_const_qualification(hunk)
        enum_indices = detect_enum_parse_change(hunk)
        ctor_indices = detect_ctor_default_change(hunk)

        for i, dl in enumerate(hunk.lines):
            if i in const_indices:
                results.append(
                    ClassifiedLine(
                        dl, "COSMETIC", "const qualification change", weights["COSMETIC"]
                    )
                )
                continue

            if i in enum_indices:
                results.append(
                    ClassifiedLine(
                        dl, "STRUCTURAL", "enum parse pattern change", weights["STRUCTURAL"]
                    )
                )
                continue

            if i in ctor_indices:
                results.append(
                    ClassifiedLine(
                        dl,
                        "STRUCTURAL",
                        "ctor default value missing",
                        weights["STRUCTURAL"],
                    )
                )
                continue

            classified = False
            for cat in ["GOOD", "EXPECTED", "COSMETIC", "STRUCTURAL", "WRONG"]:
                for rule in line_rules.get(cat, []):
                    if re.search(rule["pattern"], dl.content):
                        results.append(
                            ClassifiedLine(
                                dl,
                                cat,
                                rule.get("description", rule["pattern"]),
                                weights.get(cat, 0),
                            )
                        )
                        classified = True
                        break
                if classified:
                    break

            if not classified:
                if dl.content.strip() == "":
                    results.append(
                        ClassifiedLine(dl, "EXPECTED", "blank line", 0)
                    )
                else:
                    ext = (
                        Path(dl.file_path).suffix if dl.file_path else ".cpp"
                    )
                    default_cat = defaults.get(ext, "STRUCTURAL")
                    results.append(
                        ClassifiedLine(
                            dl,
                            default_cat,
                            f"unclassified ({ext})",
                            weights.get(default_cat, 5),
                        )
                    )

    return results


def report(classified: list, config: dict, verbose: bool = False,
           skip_elements: set = None, overwrite_stems: set = None,
           choice_skip_elements: set = None):
    weights = config["weights"]
    if skip_elements is None:
        skip_elements = set()
    if overwrite_stems is None:
        overwrite_stems = set()
    if choice_skip_elements is None:
        choice_skip_elements = set()

    cat_lines = defaultdict(int)
    cat_penalty = defaultdict(float)
    rule_counts = defaultdict(lambda: defaultdict(int))
    file_penalties = defaultdict(float)
    file_cats = defaultdict(lambda: defaultdict(int))
    file_lines = defaultdict(int)

    for cl in classified:
        cat_lines[cl.category] += 1
        cat_penalty[cl.category] += cl.penalty
        rule_counts[cl.category][cl.rule_name] += 1
        if cl.line.file_path:
            file_penalties[cl.line.file_path] += cl.penalty
            file_cats[cl.line.file_path][cl.category] += 1
            file_lines[cl.line.file_path] += 1

    total_penalty = sum(cat_penalty.values())
    total_lines = sum(cat_lines.values())
    skip_penalty = len(skip_elements) * SKIP_PENALTY
    choice_skip_penalty = len(choice_skip_elements) * SKIP_PENALTY

    zero_penalty_lines = cat_lines.get("GOOD", 0) + cat_lines.get("EXPECTED", 0)
    zero_pct = (zero_penalty_lines / total_lines * 100) if total_lines else 0

    print(f"\n{'='*70}")
    print("CODEGEN DIFF SCORE")
    print(f"{'='*70}")
    print(f"Total changed lines:  {total_lines}")
    print(f"Diff penalty:         {total_penalty:.1f}")
    if skip_elements:
        print(f"Skip penalty:         {skip_penalty} ({len(skip_elements)} elements x {SKIP_PENALTY})")
        sorted_skips = sorted(skip_elements)
        for s in sorted_skips:
            print(f"  - {s}")
    if choice_skip_elements:
        print(f"Choice-skip penalty:  {choice_skip_penalty} ({len(choice_skip_elements)} elements x {SKIP_PENALTY})")
        for s in sorted(choice_skip_elements):
            print(f"  - {s}")
    print(f"Total penalty:        {total_penalty + skip_penalty + choice_skip_penalty:.1f}")
    if overwrite_stems:
        print(f"Overwrite files:      {len(overwrite_stems)} (exempt from penalty)")
        for s in sorted(overwrite_stems):
            print(f"  - {s}")
    print(f"Zero-penalty lines:   {zero_penalty_lines} ({zero_pct:.1f}%)")
    print()

    print("Category breakdown:")
    print(f"  {'Category':12s}  {'Lines':>6s}  {'Weight':>6s}  {'Penalty':>10s}")
    print(f"  {'-'*12}  {'-'*6}  {'-'*6}  {'-'*10}")
    for cat in ["GOOD", "EXPECTED", "COSMETIC", "STRUCTURAL", "WRONG"]:
        lines = cat_lines.get(cat, 0)
        weight = weights.get(cat, 0)
        penalty = cat_penalty.get(cat, 0)
        print(f"  {cat:12s}  {lines:6d}  {weight:6.1f}  {penalty:10.1f}")
    print()

    print("Rules breakdown:")
    for cat in ["GOOD", "EXPECTED", "COSMETIC", "STRUCTURAL", "WRONG"]:
        rules = rule_counts.get(cat, {})
        if rules:
            print(f"  {cat}:")
            for rule, count in sorted(rules.items(), key=lambda x: -x[1]):
                print(f"    {count:5d}  {rule}")
    print()

    penalized_files = {
        fp: pen for fp, pen in file_penalties.items() if pen > 0
    }
    print(f"Penalized files: {len(penalized_files)} of {len(file_penalties)}")
    print()
    print("Top 20 penalized files:")
    print(f"  {'Penalty':>8s}  {'File':40s}  Categories")
    print(f"  {'-'*8}  {'-'*40}  {'-'*30}")
    for fp, pen in sorted(penalized_files.items(), key=lambda x: -x[1])[:20]:
        basename = Path(fp).name if fp else "?"
        cats = dict(file_cats[fp])
        cat_parts = []
        for c in ["GOOD", "EXPECTED", "COSMETIC", "STRUCTURAL", "WRONG"]:
            if c in cats:
                cat_parts.append(f"{c[:4]}:{cats[c]}")
        cat_str = " ".join(cat_parts)
        print(f"  {pen:8.1f}  {basename:40s}  {cat_str}")

    if verbose:
        print(f"\n{'='*70}")
        print("VERBOSE: All penalized lines")
        print(f"{'='*70}")
        for cl in classified:
            if cl.penalty > 0:
                prefix = "+" if cl.line.is_added else "-"
                basename = (
                    Path(cl.line.file_path).name if cl.line.file_path else "?"
                )
                content = cl.line.content[:60]
                print(
                    f"  {cl.penalty:5.1f}  {cl.category:12s}  {basename:30s}  "
                    f"{prefix}{content}"
                )


SKIP_PENALTY = 1000


def get_skip_elements():
    """Import SKIP_ELEMENTS from generate.py."""
    gen_dir = Path(__file__).parent
    sys.path.insert(0, str(gen_dir))
    try:
        mod = importlib.import_module("generate")
        return set(mod.SKIP_ELEMENTS)
    finally:
        sys.path.pop(0)


def get_choice_skip_elements():
    """Import CHOICE_SKIP from generate.py."""
    gen_dir = Path(__file__).parent
    sys.path.insert(0, str(gen_dir))
    try:
        mod = importlib.import_module("generate")
        return set(mod.CHOICE_SKIP)
    finally:
        sys.path.pop(0)


def get_overwrite_file_stems():
    """Import OVERWRITE_FILE_STEMS from generate.py.

    These are file stems (e.g. "Direction", "DirectionType") whose
    generated code intentionally replaces the original bespoke
    implementation. Diffs for these files are scored as EXPECTED (zero
    penalty) because the generated output is the new source of truth.
    """
    gen_dir = Path(__file__).parent
    sys.path.insert(0, str(gen_dir))
    try:
        mod = importlib.import_module("generate")
        return set(getattr(mod, "OVERWRITE_FILE_STEMS", set()))
    finally:
        sys.path.pop(0)


def main():
    config_path = "gen/eval_config.yaml"
    verbose = False

    args = sys.argv[1:]
    while args:
        if args[0] == "--config" and len(args) > 1:
            config_path = args[1]
            args = args[2:]
        elif args[0] == "--verbose":
            verbose = True
            args = args[1:]
        else:
            args = args[1:]

    with open(config_path) as f:
        config = yaml.safe_load(f)

    diff_text = get_diff()
    if not diff_text.strip():
        print("No diff found. Run generate.py first.")
        sys.exit(1)

    hunks = parse_diff(diff_text)
    overwrite_stems = get_overwrite_file_stems()
    classified = classify_hunks(hunks, config, overwrite_stems)

    skip_elements = get_skip_elements()
    choice_skip_elements = get_choice_skip_elements()
    report(classified, config, verbose, skip_elements, overwrite_stems,
           choice_skip_elements)


if __name__ == "__main__":
    main()
