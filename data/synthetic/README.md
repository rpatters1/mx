# Synthetic

These are MusicXML files that were created by AI-driven programs in order to reach elements and
attributes that are undiscoverable in the wild. That is, after thoroughly searching the Internet for
actual MusicXML files, no examples of certain MusicXML element and attribute use could be found. In
some cases these may effectively be dead code in the spec that nobody has implemented, in other cases
I just couldn't find any examples.

The files in this directory are the MusicXML 3.0 batch, which lives under `data/` so the core
roundtrip (`corert`) suite picks them up. Batches that exercise features newer than 3.0 are staged
outside `data/` under `docs/ai/projects/gen/synthetic-files/`, since `mx` does not yet target those
versions.

## File naming

`<feature>.<version>.xml`, lower-kebab-case, where `<feature>` is the primary element (or
`element.attribute`) the file targets and `<version>` is the MusicXML version it is valid against -
for example `harmon-mute.3.0.xml`, `cancel.location.3.0.xml`, `key-accidental.smufl.3.1.xml`. Names
stay under 50 characters; a numeric suffix disambiguates when needed.

## Methodology

How these files were produced, end to end, so the process can be repeated for a future version:

1. **Define the target symbol set.** Treat each MusicXML XSD (3.0, 3.1, 4.0) as a universe of
   "symbols": every element name, plus every `element.attribute` pair the element's type allows.
   Resolve named complex types, attribute groups, and simpleContent/complexContent extension bases so
   inherited attributes are counted.
2. **Measure existing coverage.** Extract the same symbol set from the committed corpus (everything
   `corert` reads under `data/`, plus any real-world files gathered in the gen project). Symbols that
   are in the schema universe but absent from the corpus are the gaps to fill.
3. **Split the gaps by version.** Diff the per-version universes so each gap is attributed to the
   version that introduced it: a 3.0 baseline, a 3.1 increment, and a 4.0 increment. This keeps
   newer-version files out of `data/` until `mx` supports those versions.
4. **Generate one small file per gap.** For each missing element, synthesize a minimal
   `score-partwise` document that places the element in a schema-valid context (correct ancestors and
   any required siblings/children) carrying the element's full set of valid attributes. One file per
   element keeps a test failure pointing at a single element's serialization or deserialization.
   Files omit `<!DOCTYPE>` (the corpus covers DOCTYPE handling elsewhere) and aim for plausible music
   semantics on a best-effort basis - the goal is schema validity and symbol coverage, not musical
   correctness.
5. **Repair to validity.** Sample attribute values from each attribute's type (enumerations,
   patterns, atomic types). Where a guessed value is rejected, read the validator's diagnostic and
   try another; drop only attributes the schema genuinely does not allow on that element. SMuFL
   glyph-name attributes use real canonical names from the W3C SMuFL specification (for example
   `noteheadBlack`, `accidentalSharp`, `coda`, `segno`, `pictGong`), never invented values.
6. **Validate every file with a trusted XSD validator.** A file is kept only if `xmllint` accepts it.
   The MusicXML schemas import the W3C `xml.xsd` and `xlink.xsd`; those are vendored next to the
   schemas in `docs/`, with the schemas' `<xs:import schemaLocation>` repointed to the local copies,
   so validation runs fully offline (`--nonet`).
7. **Confirm coverage against the real symbol set.** Re-extract symbols from the generated files and
   confirm each gap is closed. The naive extractor over-collects some attributes (it attributes
   shared formatting and placement groups to simple-text types that do not actually carry them), so
   the validator is the arbiter: a symbol counts as real only if a document carrying it validates. A
   few symbols nested inside model-group choices (for example inside `<key>`) are not reached by the
   automated generator and were hand-authored, then validated the same way.

To revalidate a batch, point `xmllint` at the matching schema, for example:

```
xmllint --noout --nonet --schema docs/musicxml.xsd data/synthetic/*.xml
```

The generation and analysis tooling is throw-away and lives in `docs/ai/projects/gen/coverage/`;
`docs/ai/projects/gen/synthetic-plan.md` records the full plan and results.
