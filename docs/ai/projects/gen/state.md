# gen State

## Milestone

**M1 and M2 complete.** `make test-all`: 0 failed (2678 cases, 9914 assertions).

## What the next session should do

**Milestone 3: increase test coverage.** Per `plan.md`: more MusicXML round-trip files and a
dedicated mx/core-level round-trip harness.

No design yet. Next session should:
1. Review existing test suite to understand coverage gaps.
2. Propose a coverage plan to the user.
3. Wait for user direction before implementing.

## Gotchas

- **`make test-all` must run WITH generated files present.** HEAD's `ArpeggiateAttributes.h` has
  a hand-applied `UpDownNone direction` (MusicXML 4.0 backport) but `NotationsWriter.cpp` assigns
  `UpDown::down` — incompatible. The generator emits `UpDown` (from 3.x XSD), so the build only
  succeeds with generated files present. Workflow: `python3 gen/generate.py && make fmt` THEN
  `make test-all`. Reset AFTER it passes.
- **`git checkout -- src/private/mx/core/` preserves mtimes**, so incremental cmake builds skip
  recompiling unchanged-mtime .cpp files. Stale-build measurements bit us mid-M2. Use
  `make clean && make test-all` for an authoritative count.
- **Use `make test-all`, never `make test`.** Build target for tests is `make core`.
