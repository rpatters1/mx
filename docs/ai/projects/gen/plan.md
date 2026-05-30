# gen Plan

## Milestone 1: revgen — reverse-engineer codegen ✅

Complete 2026-05-21. Generator produces every C++ class in `mx/core` from `docs/musicxml.xsd`.
`SKIP_ELEMENTS` and `CHOICE_SKIP` empty. See `log.md`.

## Milestone 2: fix-gen — fix generator bugs surfaced by failing tests ✅

Complete 2026-05-22. `make test-all` 3028/3028. All fixes landed in the generator; workarounds
reverted. One item deferred to M5/M6: HEAD's hand-applied MusicXML 4.0 `UpDownNone` backport vs.
schema-faithful 3.x regen (TODOs in `mx/impl/NotationsWriter.cpp:398`,
`mx/impl/ArpeggiateFunctions.cpp:35`).

## Milestone 3: fix-core-dev — fix bugs surfaced by new core-dev test mode ✅

Complete 2026-05-22 (5 iterations). 31 corert failures triaged into 10 `{file}.invalid` markers and
4 real fixes. The `src/private/mxtest/corert/` harness is now part of `make test-all`. See `log.md`
M3 for per-iteration breakdown.

## Milestone 4: test fixer (clamping-aware comparison) ✅

Complete 2026-05-25. A new `Fixer` patches the expected tree before comparison via per-file
`<file>.fixup.xml` sidecars. Convention documented in `data/README.md`. Design doc at
`src/private/mxtest/corert/Fixer.h`. Closed at 387/387 `test-core-dev` pass and 3065/3065
`test-all`.

## Milestone 5: increase test coverage ✅

Completed 2026-05-30. Added real-world corpus files. Generated 235 synthetic files
(`data/synthetic/`).

## Milestone 6: better-gen — fix garbage

The generator is `~14k` lines of bad Python. Refactor. Use the corert suite as the correctness
oracle.

Sub-milestones:

## 6A_QUAL_GATES: research code quality analysis tools for python.

We need to find a way to give the coding distinct feedback scoring on these measures of code
quality:

- Cyclomatic Complexity: Measures the number of linearly independent paths through source code
  (e.g., counting if, else, while, and for statements). Lower scores mean the code is easier to test
  and maintain.
- Cognitive Complexity: Evaluates code based on human readability rather than just program paths. It
  accounts for how difficult it is for a developer to mentally parse control flow jumps, nesting,
  and logical operations.Maintainability Index: A calculated score that indicates how easy it is to
  support, alter, or analyze the code. Higher scores indicate better overall maintainability.
- Halstead Complexity: Measures the computational complexity of software by counting the number of
  unique operators and operands. It helps predict defect-proneness and overall program volume
- Depth of Inheritance Tree: Counts the number of classes that inherit from one another back to the
  base class. High values indicate a high risk of breaking changes when modifications are made to
  parent classes.
- Coupling measures how dependent different modules or classes are on one another. Loose coupling is
  highly desired.
- Cohesion measures how closely the tasks performed by a single module or class are related. High
  cohesion means a component has a singular, well-defined purpose.
- Code Duplication: The repetition of identical or very similar blocks of code across the project.
  This violates the DRY (Don't Repeat Yourself) principle and causes major maintenance overheads.
- Standard Linters

## Milestone 7: mxml4-types — generate MusicXML 4.1 types

Replace `docs/musicxml.xsd` with MusicXML 4.1, regenerate, fix all existing tests. Watch for
backported / bolted-on features (SMuFL, `UpDown`, …) that were added with hacks to 3.0/3.1 but are
first-class in 4.0. Be backward-compatible with files mx may have written using those hacks. Restore
the `mx/impl` TODOs left from revgen.

## Milestone 8: Surface MusicXML 4.1 features in mx/api

