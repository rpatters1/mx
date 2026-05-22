# core-dev Design

Permanent in-repo "core development" mode. Lets a codegen agent churn `mx/core` symbols
without keeping `mx/api`, `mx/impl`, or higher-level tests compiling. Activated via
`make core-dev` / `make check-core-dev` / `make test-core-dev`, backed by an `MX_CORE_DEV=ON`
CMake option.

The mode introduces a new test suite, **core roundtrip** (`CORE_ROUNDTRIP`, `CORE_RT`), under
`src/private/mxtest/corert/`. It runs standalone under `make test-core-dev` and inside the
normal `mxtest` binary during `make test-all`. It does not run during `make test` (the fast
path) — gated on `MX_BUILD_CORE_TESTS=ON`, the same flag that gates the 522 per-element core
unit tests.

Distinct from **api import** (`API_IMPORT`), the `src/private/mxtest/import/` suite
that exercises the full `mx::api` stack against pre-generated expected files. "roundtrip"
is never used unqualified; always "core roundtrip" or "api import."

Sections:

1. Build System
2. Compilation Guards
3. Core Roundtrip Test
4. Agent Surface

---

## 1. Build System

### CMake Option

```
option(MX_CORE_DEV "Build only mx/core + ezxml + utility with roundtrip tests" OFF)
```

Default OFF; normal build unaffected.

### Shared Glob

`SRC_MX_CORE_ROUNDTRIP` is a new glob set, defined unconditionally near the
`SRC_MX_TEST_*` globs, targeting `src/private/mxtest/corert/*.{cpp,h}`. Both build modes
reference it (see section 3 for contents).

### Effect of `MX_CORE_DEV=ON` (core-dev branch)

`CMakeLists.txt` enters a distinct branch that builds only two targets:

| Target            | Type           | Sources                                           |
|-------------------|----------------|---------------------------------------------------|
| `mx-core`         | static library | `SRC_MX_CORE` + `SRC_MX_EZXML` + `SRC_MX_UTILITY` |
| `mxtest-core-dev` | executable     | `SRC_MX_CORE_ROUNDTRIP` + `SRC_CPUL`              |

`SRC_MX_API` and `SRC_MX_IMPL` are never passed to any `add_library` / `add_executable` in the
core-dev branch, so they never compile. `MX_BUILD_TESTS`, `MX_BUILD_CORE_TESTS`, and
`MX_BUILD_EXAMPLES` are ignored in core-dev mode — they control only the normal build.

`PathRoot.h` is still generated (it records `MX_REPO_ROOT_PATH`, used by the core roundtrip
test for file discovery).

### Effect of `MX_CORE_DEV=OFF` (normal branch)

The existing build runs unchanged — `mx` library, `mxtest`, examples, install rules —
with one addition: when `MX_BUILD_TESTS=ON` *and* `MX_BUILD_CORE_TESTS=ON`,
`SRC_MX_CORE_ROUNDTRIP` is appended to `mxtest`'s source list, so the core
roundtrip cases run as part of `mxtest` alongside the api import, api, impl, and
per-element core suites.

Gating rationale: `MX_BUILD_CORE_TESTS` gates the 522 per-element core unit tests — OFF
during `make test` / `make check`, ON during `make test-all`. Reusing it keeps `make test`
fast and groups the core roundtrip suite with "anything related to core." No new
test-suite flag is introduced.

### Structure

One top-level `if(MX_CORE_DEV) ... else() ... endif()` in `CMakeLists.txt`. The `else()`
branch holds the existing normal build with the `MX_BUILD_CORE_TESTS`-gated
append described above; the `if()` branch holds only the two core-dev targets. No small
`if(NOT MX_CORE_DEV)` gates sprinkled through the file.

### Build Directory

`build/core-dev/$(BUILD_TYPE)/`, following the `mode_dir` convention. Independent
CMake cache and incremental state, so switching between `dev` and `core-dev` never
reconfigures the other.

### Makefile Targets

| Target           | What it does                                                   |
|------------------|----------------------------------------------------------------|
| `core-dev`       | Configure + build with `MX_CORE_DEV=ON`                        |
| `check-core-dev` | fmt-check + warning-free build in Docker with `MX_CORE_DEV=ON` |
| `test-core-dev`  | Depends on `core-dev`; runs `mxtest-core-dev` (passes `ARGS`)  |

`test-core-dev` always passes `--allow-running-no-tests` to the binary so the
target stays green when `ARGS` filters down to zero cases (and during early
implementation phases before any cases are registered). Real failures still
fail the target — the flag only changes how Catch2 treats the empty-result
case.

`core-dev`:

```makefile
core-dev:
	$(CMAKE) -S . -B $(call mode_dir,core-dev) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DMX_CORE_DEV=ON \
		$(GEN_ARG)
	$(CMAKE) --build $(call mode_dir,core-dev) --parallel $(JOBS) \
		--config $(BUILD_TYPE)
```

The `cmake_build` macro passes the three `MX_BUILD_*` flags positionally. It is
refactored to also accept `MX_CORE_DEV` as a parameter; `core-dev` invokes it with
`MX_CORE_DEV=ON` and the three `MX_BUILD_*` flags OFF.

`check-core-dev`: mirrors `check` — fmt-check plus warning-free build, run under the pinned
Docker toolchain so the gate is deterministic. Configures with `MX_CORE_DEV=ON` (and the
three `MX_BUILD_*` flags OFF) instead of the `MX_BUILD_TESTS=on` set `check` uses.

Docker delegation: `make check` outside the container runs `docker buildx build --target
run`; the `run` stage in `Dockerfile` hardcodes `RUN make check` against the pinned
toolchain. `check-core-dev` follows the same pattern: a new `FROM base AS run-core-dev`
stage parallel to `run`, with `RUN make check-core-dev`. Outside the container,
`make check-core-dev` runs `docker buildx build --target run-core-dev`.

---

## 2. Compilation Guards

No per-file `#ifdef` guards. Exclusion happens at the CMake target level by omitting
api/impl/higher-test glob sets in the core-dev branch (see section 1).

`src/private/mxtest/control/CompileControl.h` is unchanged. Its macros
(`MX_COMPILE_API_TESTS`, etc.) guard code in files the core-dev binary does not compile, so
they are inert there. Removing dormant macros is out of scope.

### Dual-Compilation Invariant

The core roundtrip sources under `mxtest/corert/` compile in two contexts:

- Linked into `mxtest-core-dev` (core-dev branch), against `mx-core` only.
- Linked into `mxtest` (normal branch with `MX_BUILD_CORE_TESTS=ON`), against the full `mx`
  library.

The sources must therefore have no `#ifdef MX_CORE_DEV` branches and must not depend on
anything that exists in only one context. Concretely:

- Includes are limited to `mx/core/*`, `mx/ezxml/*`, `mx/utility/*`, `cpul/catch.h`, and the
  two helpers below.
- The suite reuses `ChangeValues.h` and `SortAttributes.h` from `mxtest/import/`. Both are
  unguarded today; the implementer must confirm this at integration time.

---

## 3. Core Roundtrip Test

### Source Location

New files under `src/private/mxtest/corert/`:

```
CoreRoundtripTest.cpp     Catch2 dynamic registration + test body
CoreRoundtripImpl.h       File discovery, normalization, comparison declarations
CoreRoundtripImpl.cpp     Implementations
```

No dependency on `mxtest/` infrastructure beyond the two unguarded helpers from
`mxtest/import/` (`ChangeValues.h`, `SortAttributes.h`). The suite does not
use `MxFileRepository`, `ImportTestImpl`, or `CompileControl.h`. It includes only
`mx/core/*`, `mx/ezxml/*`, `mx/utility/*`, and `cpul/catch.h`.

The corresponding implementation files (`ChangeValues.cpp`, `SortAttributes.cpp`)
are linked into `mxtest-core-dev` via an explicit `SRC_MX_CORE_ROUNDTRIP_HELPERS`
list in `CMakeLists.txt` (the path is enumerated rather than globbed to pick
exactly these two helpers and not the rest of the api import suite, which would
drag in `MxFileRepository`, `ImportTestImpl`, etc.). In the normal branch the
same `.cpp` files are already linked transitively via `SRC_MX_TEST_IMPORT`.

### File Discovery

Runs once at static-initialization time, before Catch2's runner starts:

1. Scan root: `MX_REPO_ROOT_PATH "/data"` (injected by CMake into `PathRoot.h`).
2. Recurse all subdirectories.
3. Collect files with extension `.xml` or `.musicxml`.
4. Exclude any path containing a directory segment named `expected`, `testOutput`,
   `generalxml`, or `smufl`.

The `expected/` and `testOutput/` exclusions remove generated outputs from the input set.
The `generalxml/` and `smufl/` exclusions remove non-MusicXML fixtures: `generalxml/` holds
only `fake.xml` (a non-MusicXML XML sample) and `smufl/` holds only `glyphnames.json` (not
XML). If MusicXML test inputs are ever added to those directories, the exclusions
must be revisited.

Returns absolute paths. Each test name is the path relative to `data/` (e.g.,
`lysuite/ly01a_Pitches_Pitches.xml`).

### Catch2 Dynamic Registration

Catch2 v3 (amalgamated at `src/private/cpul/catch.h`) exposes `Catch::AutoReg` and
`Catch::ITestInvoker` — the primitives `TEST_CASE` uses internally.

A custom `ITestInvoker` carries the file path; a static initializer constructs one
`Catch::AutoReg` per discovered file, tagged `[core-roundtrip]`. Each case runs
independently: one failure does not block the rest. Agents filter via
`mxtest-core-dev [core-roundtrip] lysuite/`, or `mxtest [core-roundtrip]` inside the
`test-all` binary.

### Per-File Test Flow

The body below runs inside a `try` block; any thrown exception (from `ezxml`,
`fromXDoc`, `toXDoc`, or normalization) is caught and reported via Catch2's `FAIL` with the
file path and `exception::what()`. Catch2 ends the current case on `FAIL` but continues to
the next, so a thrown file never aborts the suite.

```
1. Load input via ezxml                                  → inputXDoc
2. Set input root `version` attribute to the supported
   MusicXML version (see note below)
3. mx::core::Document::fromXDoc(inputXDoc)               → mxDoc
4. mxDoc->toXDoc()                                       → actualXDoc
5. Apply full normalization to actualXDoc

6. Load input from disk again (fresh)                    → expectedXDoc
7. Apply full normalization to expectedXDoc

8. Depth-first compare expectedXDoc vs actualXDoc
   On first mismatch: FAIL with path + detail; return
```

If `fromXDoc` returns false (non-throwing failure), `FAIL` immediately with file name.

**Version string.** Step 2 uses a hardcoded `"3.0"` declared as a local
`constexpr const char *const kMusicXmlVersionString` in `CoreRoundtripImpl.cpp`.

The original intent was to use `mx::core::toString(mx::core::DEFAULT_MUSIC_XML_VERSION)`
as a single source of truth. That is not currently possible: the `mx::core`
namespace declares the version in two conflicting places
(`DocumentSpec.h::DEFAULT_MUSIC_XML_VERSION`, lower-case `threePointZero`, with
stringification; `DocumentHeader.h::kDefaultMusicXmlVersion`, upper-case
`ThreePointZero`, no stringification helper), and including both in one TU is a
hard compile error — the `DocumentChoice` and `MusicXmlVersion` enums are
redeclared with incompatible scopes. The core roundtrip suite needs `Document.h`
for `makeDocument` / `fromXDoc` / `toXDoc`, which transitively conflicts with
`DocumentSpec.h`. The api import suite hardcodes `"3.0"` for the same reason
(see `ImportTestImpl::loadTestFile`); we follow that precedent. The constant
carries a comment pointing at the conflict so a future cleanup — most likely
part of the codegen rewrite that brings MusicXML 4.0 support — can replace it
with the canonical symbol.

**Normalization symmetry.** Steps 5 and 7 apply the *same* normalization pipeline to
both sides, so the comparison is canonical-against-canonical.

### Normalization

Normalization is restricted to representational canonicalization — operations that neutralize
XML's non-semantic degrees of freedom. It excludes the defect-workaround fixups in
`mxtest/import/ExpectedFiles.cpp::generateExpectedFile`, which keep api import green against
known-bad inputs and `mx::core` quirks. Core roundtrip rejects that approach: a
mismatch is a signal to fix the bug (in `mx::core`, the input, or codegen), not to hide it
with normalization. The helpers (`convertValues`, `addChildIfNone`, etc.) live in
`mxtest/import/ChangeValues.h`; the suite imports only the canonicalization helpers below.

Steps, in order:

1. `setXmlDeclaration` — standalone=false, XML 1.0, preserve encoding (default UTF-8).
2. `setDoctype` — based on root element name (`score-partwise` or `score-timewise`).
3. `setRootMusicXmlVersion` — set root `version` from
   `mx::core::toString(mx::core::DEFAULT_MUSIC_XML_VERSION)`.
4. `stripZerosFromDecimalFields` — canonicalize decimal text (`"1.000"` → `"1"`) on fields
   in the decimal-field set. XML compares text strings, not numeric values, so this is
   representational normalization, not a defect workaround.
5. `sortAttributes` — **must be last**. Attribute order is not semantic in XML.

The same pipeline is applied to both `actualXDoc` (step 5 of the per-file flow) and
`expectedXDoc` (step 7), so the comparison sees two canonically-normalized trees.

`SortAttributes.h` from `mxtest/import/` is the source for `sortAttributes`; the four
remaining helpers come from `ChangeValues.h`. Both headers are unguarded (per Section 2's
Dual-Compilation Invariant).

### Depth-First Tree Comparison

At each node-pair: element name, text content (see open question below), attribute count
and each `(name, value)` pair in order, child count. Attributes are already sorted by
normalization step 5, so the comparator walks them in order. Recurse into each child pair in
order. On first mismatch, record and return.

Comparison stops at the first mismatch and calls `FAIL`. Catch2 ends the current case and
proceeds to the next, so one file's mismatch never blocks the suite. Continuing past a
mismatch would emit cascade noise — one structural difference makes every subsequent sibling
comparison a likely mismatch — so only the first signal is useful.

Each call carries a node path like `/score-partwise/part[0]/measure[2]/note[1]/pitch`, which
appears in the failure message. Example:

```
core roundtrip mismatch in lysuite/ly01a_Pitches_Pitches.xml
  at /score-partwise/part[0]/measure[2]/note[1]/pitch/step
  expected element name: 'step'
  actual element name:   'alter'
```

Attribute mismatches use `[@attr]` in the path.

**Open question — text trimming.** Whether text comparison should trim leading/trailing
whitespace or compare exactly is deferred to implementation. Start with exact comparison;
if real round-trip cases mismatch only on whitespace inside text nodes, switch to trimmed
comparison and document the case that drove the decision.

### No Pre-Generated Expected Inputs

The core roundtrip suite does not read `data/expected/`. Expected is computed in-memory from
the input at runtime — no `GenerateExpected` step, no staleness concern. (A key difference
from api import, which compares against pre-generated expected files under
`data/expected/`.)

### Output Files on Failure

On any failure (exception caught, `fromXDoc` returning false, or tree-comparison mismatch),
two files are written to `data/testOutput/corert/` for diffing:

- `<flattened-path>.expected.xml` — `expectedXDoc` after normalization
- `<flattened-path>.actual.xml` — `actualXDoc` after normalization

`<flattened-path>` is the test name (path relative to `data/`) with directory separators
replaced by `_`, e.g., `lysuite_ly01a_Pitches_Pitches.xml.expected.xml`. Each failure
is a single pair of files under `data/testOutput/corert/` with no nested subdirectories.

The `corert/` subdirectory namespaces core roundtrip's debug output away from api import's
output (which writes flat into `data/testOutput/`), so the two suites can run in the same
`make test-all` invocation without filename collisions. The test creates
`data/testOutput/corert/` via `mkdir -p` before writing, so a fresh checkout works.

---

## 4. Agent Surface

Core-dev is an AI/codegen workflow, not a user feature. Substance is documented in
`AGENTS.md` and `make help`. The README gets a short pointer (≤20 words) at implementation
time naming the mode and pointing at `AGENTS.md`; no further README content.

### `make help` Addition

Appended after existing sections, before the Knobs/Layout line:

```
Core development (codegen):
  make core-dev           Build trimmed library (mx/core + ezxml + utility) and
                          mxtest-core-dev. No mx/api or mx/impl compiled.
  make check-core-dev     fmt-check + warning-free build for core-dev (Docker).
  make test-core-dev      Build core-dev then run the core roundtrip suite.
                          Each file under data/ is a separate Catch2 test case.
                          Filter: make test-core-dev ARGS='[core-roundtrip] lysuite/*'
```

The `ARGS` hint is included because agents iterate on subsets. The same `[core-roundtrip]`
tag filters the suite inside `mxtest` during `make test-all`.

### `AGENTS.md` Addition

Verbatim Markdown in `agents-md-snippet.md` (kept separate to avoid nested fenced code
blocks). Introduces a top-level `## Core Development Mode (Codegen)` section: when to use
the mode, in-mode commands, in-mode gate, and full pre-merge gate.

### Build Directories

| Action              | Command              | Build dir used           |
|---------------------|----------------------|--------------------------|
| Enter (build)       | `make core-dev`      | `build/core-dev/<type>/` |
| Enter (test)        | `make test-core-dev` | `build/core-dev/<type>/` |
| Leave (verify full) | `make test-all`      | `build/core/<type>/`     |
| Leave (fast verify) | `make test`          | `build/dev/<type>/`      |

Each mode has its own build directory; switching never reconfigures the other. No persistent
state changes on entering or leaving the mode.
