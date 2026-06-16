"""Discover the corpus files to audit.

The audited set is exactly the set the ``corert`` round-trip suite processes, so
every ``*.features.xml`` corresponds to a file the test harness round-trips. The
rules mirror ``src/private/mxtest/corert/CoreRoundtripImpl.cpp`` (and its Go/C
siblings):

  * under ``data/`` with a ``.xml`` or ``.musicxml`` extension,
  * not inside an excluded directory (``expected``/``testOutput``/``generalxml``/
    ``smufl`` -- generated output and non-score XML),
  * not a ``*.fixup.xml`` sidecar,
  * not a file with an ``*.invalid`` sibling marker,

plus the audit-specific exclusion of the tool's own outputs (``*.features.xml``
sidecars and the ``corpus.xml`` aggregate).
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from . import CORPUS_SUMMARY_NAME, FEATURES_SUFFIX

# Directory names anywhere in a path that disqualify a file. Mirrors
# `excludedSegments` in the C++/Go/C corert discoverers.
EXCLUDED_SEGMENTS = frozenset({"expected", "testOutput", "generalxml", "smufl"})

_XML_EXTENSIONS = (".xml", ".musicxml")

# Files directly under data/synthetic/ are built to exercise the specification
# symbol by symbol; everything else is a real-world ("wild") file.
SYNTHETIC_DIR = "synthetic"


@dataclass(frozen=True)
class CorpusFile:
    """One audited file, with the data-relative path used throughout the audit."""

    path: Path  # absolute path on disk
    rel: str  # path relative to data/, forward slashes (e.g. "recsuite/Saltarello.xml")
    synthetic: bool  # under data/synthetic/

    @property
    def features_path(self) -> Path:
        """The sibling ``*.features.xml`` path: the source extension is replaced
        (``Saltarello.xml`` -> ``Saltarello.features.xml``), matching the
        ``*.fixup.xml`` sidecar convention."""
        return self.path.with_suffix("").with_name(self.path.stem + FEATURES_SUFFIX)


def _is_excluded_dir(rel_parts: tuple[str, ...]) -> bool:
    return any(part in EXCLUDED_SEGMENTS for part in rel_parts)


def _has_xml_extension(p: Path) -> bool:
    return p.suffix.lower() in _XML_EXTENSIONS


def is_audit_artifact(name: str) -> bool:
    """A path the audit tool itself produces (sidecar or aggregate)."""
    return name.endswith(FEATURES_SUFFIX) or name == CORPUS_SUMMARY_NAME


def discover(data_root: Path) -> list[CorpusFile]:
    """Return the audited corpus files, sorted by data-relative path."""
    out: list[CorpusFile] = []
    for p in data_root.rglob("*"):
        if not p.is_file():
            continue
        rel_parts = p.relative_to(data_root).parts
        if _is_excluded_dir(rel_parts):
            continue
        if not _has_xml_extension(p):
            continue
        name = p.name
        if name.endswith(".fixup.xml") or is_audit_artifact(name):
            continue
        if p.with_name(name + ".invalid").exists():
            continue
        out.append(
            CorpusFile(
                path=p,
                rel="/".join(rel_parts),
                synthetic=len(rel_parts) > 1 and rel_parts[0] == SYNTHETIC_DIR,
            )
        )
    out.sort(key=lambda c: c.rel)
    return out
