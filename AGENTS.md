# mx

@./README.md

## Project Index

Keep this section as a Markdown table. When updating entries, maintain the table format.

| Path                            | Description                                                                          |
|---------------------------------|--------------------------------------------------------------------------------------|
| `src/`                          | Main C++ source tree (public API, core elements, implementation, tests)              |
| `src/include/mx/api/`           | Public API headers: `*Data` structs and `DocumentManager` entry point                |
| `src/private/mx/api/`           | Implementation backing the public API headers                                        |
| `src/private/mx/core/`          | Strongly-typed MusicXML element classes (mostly code-generated)                      |
| `src/private/mx/core/elements/` | Generated element classes, one pair per MusicXML element (1182 files)                |
| `src/private/mx/impl/`          | Conversion layer mapping the core DOM to the public API                              |
| `src/private/mx/ezxml/`         | Embedded lightweight XML reader/writer used by the core                              |
| `src/private/mx/utility/`       | Shared helpers (string, parsing, file system utilities)                              |
| `src/private/mxtest/`           | Test suite (api, core, file, api import, impl, control, core roundtrip)              |
| `src/private/cpul/`             | Catch-based unit-test harness and test runner main                                   |
| `gen/version-a/`                | Historical Ruby/shell scripts from the original brute-force code gen                 |
| `gen/version-b/`                | Active Rust code generator for reproducing MusicXML 4.0 element classes              |
| `data/`                         | MusicXML test files and expected-output suites for api import / core roundtrip tests |
| `docs/musicxml.xsd`             | MusicXML specification (reference)                                                   |
| `docs/ai/project/`              | AI-assisted project planning and codegen design documents                            |
| `Makefile`                      | Primary build-and-test entry point (wraps CMake; `make help` lists targets)          |
| `CMakeLists.txt`                | CMake build configuration                                                            |

## Code Generation - Historical Context

`src/private/mx/core/` and `src/private/mx/core/elements/` was originally "hand-generated" by human
brute-force using Ruby scripts which can still be found in `./gen/version-a`. This was never a
one-shot solution to generating the code from the XSD spec. Rather, it was an iterative process,
solving problems encountered one-at-a-time until the XSD spec was entirely covered. As such, it is
not viable for re-use at this time, but can be used to understand the historical nature of how the
types were first generated.

At some point, I tried to create a Rust based codegenerator in order to be able to regenerate
`mx/core`. However, this devolved and failed. It is kept in `gen/version-b` for historical
curiosity, but it never worked.

### The Problem

We are stuck somewhere around MusicXML 3.1 (or maybe 3.0) because we cannot reliably re-generate the
types from a newer version of the specification. MusicxML 4.0 has been out for a long time, and we
want to support it. But we need to write new code-gen tooling to reproduce the emission of the core
types and then expose the new features in `src/include/mx/api/`.

Furthermore, many decisions in the original gen process were taken by hand. Using a hand-rolled
type, for example, instead of what would have strictly implemented the spec. There are human choices
throughout `mx/core` that will need to be preserved with future code generating efforts.

Code generation was never, and should not in the future, be designed to generate any valid XSD
specification. Rather, the goal of code generation is bespoke, to produce what is needed for the
`mx` library from the MusicXML specification.

## Terminology: Roundtrip Suites

Two distinct test suites exercise MusicXML round-tripping. Always use the qualified name; never
say "roundtrip" unqualified.

- **api import** (`API_IMPORT`): the existing suite under `src/private/mxtest/import/`. Imports
  a file through `mx::api::DocumentManager`, which exercises `mx/core` + `mx/impl` + `mx/api`,
  then compares against a pre-generated expected XML file under `data/expected/`.
- **core roundtrip** (`CORE_ROUNDTRIP`, `CORE_RT`): the suite under
  `src/private/mxtest/roundtrip/`. Round-trips a file through `mx::core::Document` only
  (`fromXDoc` → `toXDoc`), no api/impl involvement, comparing against the normalized input
  computed in-memory.

## Quality Gates

Always run `make fmt` after modifying code under `src/`.

To see whether your code changes are sound, follow that with:
- for changes in `src/private/mx/core/*`: `make test-all` (very slow, can be more than 10 minutes)
- for changes not in `src/private/mx/core/*`: `make test` (faster, can take a couple minues)

Check for warnings with: `make check`.

CI will run all of these plus the `xcode` targets.

## Core Development Mode (Codegen)

Use core-dev when modifying files under `src/private/mx/core/` and you do not need `mx/api`
or `mx/impl` to compile. Trimmed build for codegen iteration: only `mx/core` + `mx/ezxml` +
`mx/utility` compile.

### In-mode iteration

```
make core-dev          # configure + build (fast: skips api/impl)
make test-core-dev     # run all core roundtrip cases
make test-core-dev ARGS='[core-roundtrip] lysuite/*'   # subset
```

In-mode gate: `make fmt && make check-core-dev && make test-core-dev`. `fmt` and
`check-core-dev` run in Docker; `test-core-dev` runs natively.

### Full pre-merge gate

Before merging core changes, run the full gate. Changes under `mx/core/` require
`test-all`, which exercises per-element core unit tests, api import, and core
roundtrip together:

```
make fmt && make check && make test-all
```

### What core-dev tests

Backed by the **core roundtrip** suite (`CORE_ROUNDTRIP`, `CORE_RT`) under
`src/private/mxtest/corert/`. Each `*.xml` / `*.musicxml` file under `data/` (excluding
`data/expected/`, `data/testOutput/`, `data/generalxml/`, `data/smufl/`) is one Catch2
test case round-tripping through `mx::core::Document` (`fromXDoc` → `toXDoc`) against a
normalized input computed in-memory.

The same suite runs inside the normal `mxtest` binary during `make test-all`, gated on
`MX_BUILD_CORE_TESTS=ON`. Distinct from the **api import** (`API_IMPORT`) suite in
`src/private/mxtest/import/`, which exercises the full `mx::api` stack against pre-generated
expected files under `data/expected/`.

For design details see `docs/ai/projects/core-dev/design/design.md`.

## Current Project

We are working on reverse engineering a new codegen system to regenerate mx/core for MusicXML 4.0.
See the project directory `./docs/ai/project/gen`.
