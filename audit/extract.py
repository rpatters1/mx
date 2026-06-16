"""Extract the MusicXML feature surface of a single file.

The surface is every element (by local name) mapped to the set of attribute
names seen on it anywhere in the document. Attribute names keep their
conventional namespace prefix (``xml:lang``, ``xlink:href``); element names are
namespace-less in MusicXML and pass through bare.
"""

from __future__ import annotations

import xml.etree.ElementTree as ET
from dataclasses import dataclass, field
from pathlib import Path

# Namespaces MusicXML attributes use, mapped back to the prefix the corpus and
# schema spell them with. ElementTree expands a namespaced attribute to Clark
# notation ({uri}local); we restore the prefix so the audit reads like source.
_NS_PREFIX = {
    "http://www.w3.org/XML/1998/namespace": "xml",
    "http://www.w3.org/1999/xlink": "xlink",
}


def _local_name(tag: str) -> str:
    # Strip a Clark-notation namespace if present. MusicXML elements are
    # namespace-less so this is normally a no-op, but it keeps us robust.
    if tag.startswith("{"):
        return tag.rsplit("}", 1)[1]
    return tag


def _attr_name(raw: str) -> str:
    if raw.startswith("{"):
        uri, local = raw[1:].split("}", 1)
        prefix = _NS_PREFIX.get(uri)
        return f"{prefix}:{local}" if prefix else local
    return raw


@dataclass
class FileFeatures:
    """The audited element/attribute surface of one corpus file."""

    audited: str  # data-relative path, forward slashes
    version: str  # declared root @version (e.g. "3.0"), or "" if absent
    root: str  # root element local name
    # element local name -> set of attribute names seen on it
    elements: dict[str, set[str]] = field(default_factory=dict)


def extract_file(path: Path, audited: str) -> FileFeatures:
    """Parse ``path`` and collect its element/attribute surface."""
    root = ET.parse(path).getroot()
    ff = FileFeatures(
        audited=audited,
        version=root.get("version", ""),
        root=_local_name(root.tag),
    )
    for el in root.iter():
        name = _local_name(el.tag)
        attrs = ff.elements.setdefault(name, set())
        for raw in el.attrib:
            attrs.add(_attr_name(raw))
    return ff
