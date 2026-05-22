# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Build a generator that produces every C++ class in `mx/core` from `docs/musicxml.xsd` with no
skipped elements.

**Complete** (2026-05-21, iteration 40). SKIP_ELEMENTS and CHOICE_SKIP both empty.

## Milestone 2: fix-gen — fix generator bugs surfaced as failing tests ✅

**Complete** (2026-05-22). All 2678 tests pass.

The commit "src: issues caused by revgen" contained hand-edits to non-generated code (mx/impl,
mx/core/FromXElement, some tests) that worked around generator bugs. Each was triaged as BUG /
BENIGN / WEIRD, generator bugs were fixed, and the workarounds reverted.

One WEIRD item deferred to M5: original `mx/core` had a hand-applied MusicXML 4.0 `UpDownNone`
backport that a schema-faithful 3.x regen overwrites. The `// TODO: fixme - MusicXML 4.0 ...`
comments in `mx/impl` (NotationsWriter.cpp:398, ArpeggiateFunctions.cpp:35) bookmark this.

## Milestone 3: increase test coverage

Add a lot more MusicXML round-trip input files. Build a dedicated mx/core-level round-trip test
harness (not just api-level freezing tests). No specific design yet.

## Milestone 4: better-gen — fix garbage

The gen program is 12k lines of bad Python. Fix it. Use dedicated mx/core round-trip tests as the
north star for correctness.

## Milestone 5: mxml4-types — generate MusicXML 4.0 types

Replace `docs/musicxml.xsd` with MusicXML 4.0, regenerate, fix all existing tests. Watch for
backported or bolted-on features (SMuFL, UpDown, etc.) that were added with hacks to 3.0/3.1 but
are first-class in 4.0. Be backward-compatible with files `mx` may have written using those hacks.
Restore the `mx/impl` TODOs from revgen.
