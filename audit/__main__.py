"""mx feature-audit entry point.

Usage:
  python3 -m audit files [--force]   write a *.features.xml sidecar next to each
                                     corpus file (skips existing unless --force)
  python3 -m audit corpus            (re)build data/corpus.xml from the corpus
  python3 -m audit all [--force]     run `files` then `corpus`

See audit/README.md. The audited set mirrors the corert round-trip suite.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from . import CORPUS_SUMMARY_NAME, featuresfile, summary
from .discover import discover
from .extract import extract_file

DATA_ROOT = Path(__file__).resolve().parent.parent / "data"


def _cmd_files(force: bool) -> int:
    files = discover(DATA_ROOT)
    written = skipped = 0
    for cf in files:
        out = cf.features_path
        if out.exists() and not force:
            skipped += 1
            continue
        ff = extract_file(cf.path, cf.rel)
        featuresfile.write(ff, out)
        written += 1
    print(f"features: {written} written, {skipped} skipped (existing), {len(files)} total")
    return 0


def _cmd_corpus() -> int:
    out = DATA_ROOT / CORPUS_SUMMARY_NAME
    out.write_text(summary.build(DATA_ROOT), encoding="utf-8")
    print(f"corpus: wrote {out.relative_to(DATA_ROOT.parent)}")
    return 0


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog="python3 -m audit")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_files = sub.add_parser("files", help="write per-file *.features.xml sidecars")
    p_files.add_argument(
        "--force", action="store_true", help="overwrite existing sidecars"
    )
    sub.add_parser("corpus", help="rebuild data/corpus.xml")
    p_all = sub.add_parser("all", help="run files then corpus")
    p_all.add_argument("--force", action="store_true", help="overwrite existing sidecars")

    args = parser.parse_args(argv)
    if args.cmd == "files":
        return _cmd_files(args.force)
    if args.cmd == "corpus":
        return _cmd_corpus()
    if args.cmd == "all":
        rc = _cmd_files(args.force)
        return rc or _cmd_corpus()
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
