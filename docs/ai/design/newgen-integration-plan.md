# newgen Integration Plan

Status: APPROVED. This is the binding decision record for integrating the newgen work
(the generator pipeline + the generated `mx::core`) with the old codebase on `master`.
It was produced by studying both trees and interviewing the owner; the decisions below
are settled. Execute Phase 1 from this document.

## 1. Situation

Three commits define the problem:

| Ref | What it is |
|---|---|
| `71fc402bd1d89c` | `master` HEAD. Master has NOT moved since the newgen branch-off; this commit IS the merge-base. |
| `b01288cd01629c` | "initial commit" of the newgen effort: deleted 2,073 files (old `mx::core`, `mx::api`, `mx::impl`, ezxml, old tests, xcode, docs, …) and hollowed out README/AGENTS/CMake/Dockerfile/Makefile/CI so codegen work could proceed without distracting context. These deletions must NOT appear in the integrated history. |
| `ba4916f` (tip of `claude/newgen-branch-checkout-b8i3jg`) | The completed newgen work: the language-agnostic generator (`gen/`), the generated C++ `mx::core` (valid-by-construction, pugixml, all six gates green per `docs/ai/design/mx-core-plan.md`), Go/C/JSON-Schema targets, the corert/validate/probe/unit gates, mx-sdk toolchain. |

The newgen branch is a strict descendant of master, but merging it as-is would carry
`b01288c`'s devastating deletions onto master. This plan replaces that merge with a
**rebuilt branch**: new branch `newgen-integration` created at `71fc402`, followed by a
logical, curated sequence of fresh commits whose trees are assembled from the two
sources (`git checkout <source> -- <paths>`). The only deletions in the new history are
the *intentional* ones (§3). No merge to master is performed; the deliverable is a
branch that is ready for a PR.

## 2. The product stack and the phases

```
mx/api   -> preserved; changes must be minimally invasive (Phase 2 test-harness fallout only)
mx/impl  -> preserved but excluded from build; Phase 2 reimplements it against the new mx::core
mx/core  -> the old hand-written core is DELETED and replaced by the generated core
```

- **Phase 1 (this plan): combine the branches.** New branch off master containing the
  newgen work plus everything preserved from the old codebase. `mx::api`/`mx::impl` are
  restored in-tree but excluded from the build behind `option(MX_API ... OFF)`. It is
  accepted that they do not compile yet. All newgen gates are green at the tip; the
  branch is PR-ready.
- **Phase 2 (follow-up, outline in §8): the `mx::impl` port.** Reimplement impl against
  the new core's shape, rewrite the api seam (`DocumentManager`) on pugixml/`mx::core::parse`,
  bring the preserved `mxtest/api` + `mxtest/impl` suites back to green, flip `MX_API`
  default ON. Opens with its own design doc.
- **Phase 3 (theoretical): distribution.** Reinstate iOS/macOS (Xcode project,
  xcframework) with fresh eyes, and consider distribution channels / package managers.
  Nothing in Phase 1 or 2 blocks on it.

## 3. Disposition manifest

Every path `b01288c` touched, with its settled fate. "old" = take from `71fc402`,
"new" = take from `ba4916f`.

### Deleted intentionally (the only deletions allowed vs master)

- `src/private/mx/core/**` (old hand-written core, 1,247 files) — replaced by the
  generated core. Its 522-file test suite `src/private/mxtest/core/**` (old) dies with
  it (tightly coupled to the old core API, and it took forever to compile).
- `src/private/mx/ezxml/**` (35 files) — retired; the new core consumes pugixml
  directly. Outside the old core, ezxml is referenced only by `DocumentManager.cpp` and
  old test-harness files, all of which are excluded from the build until Phase 2
  rewrites those seams (§8). Restoring code that references a deleted library is
  harmless because it is not compiled.
- `src/private/mxtest/corert/**` (old, 5 files) — superseded by the newgen pugixml port
  at the same paths.
- `src/private/mxtest/import/{ImportTest*,ExpectedFiles*,ChangeValues*,SortAttributes*}`
  remain *restored-but-excluded* (see below), but old `DecimalFields.h` loses to the
  newgen pugixml version at the same path.
- `gen/parse.py`, `gen/ids.py`, `gen/generate.py` (the old generator prototype) —
  replaced by the `gen/xsd|ir|plates|press` pipeline.
- `docs/ai/projects/**` — deleted (do not resurrect; restoring it would also duplicate
  the 154 synthetic corpus files that were *moved* to `data/synthetic/`).
- `.idea/**` — deleted.
- `xcode/**` (incl. `build_xcframework.sh`) — deleted; deferred to Phase 3.
- `CHANGELOG.md` — stays out; will be dealt with at release time.

### Restored from `71fc402` (excluded from the build until Phase 2 where noted)

- `src/include/mx/api/**` (55 public DTO headers; zero core/ezxml references).
- `src/private/mx/api/**` (13 files; only 4 touch core/ezxml — `DocumentManager.cpp`
  is the real seam).
- `src/private/mx/impl/**` (77 files; welded to the old core API; Phase 2's subject).
- `src/private/mx/utility/**` (9 headers; used by impl).
- `src/private/mx/examples/**` (3 files; api-only consumers). [excluded]
- `src/private/mxtest/api/**` (43 files) — the Phase-2 acceptance suite. [excluded]
- `src/private/mxtest/impl/**` (8 files) — impl's unit spec; Phase 2 ports or retires
  per-file. [excluded]
- `src/private/mxtest/import/` old harness files (ImportTest/ExpectedFiles/
  ChangeValues/SortAttributes) — Phase-2 reference for the api-level corpus roundtrip;
  to be rewritten on pugixml. [excluded; coexists with newgen's `Normalize.{h,cpp}` and
  `DecimalFields.h` in the same directory]
- `src/private/mxtest/file/**` (10) + `src/private/mxtest/control/**` (1) — harness
  infra the api tests depend on. [excluded]
- `gen/quality.py`, `gen/.pylintrc` — the generator quality gates, restored and adapted
  (§5.3).

### Taken from `ba4916f` (the newgen tree, wholesale)

- `gen/**` (pipeline, targets, tests, committed Go/C/schema generated output).
- `src/private/mx/core/**` (hand-written runtime) + `src/private/mx/core/generated/**`
  (the generated model, 910 files) — the directory IS the generated/hand-written boundary.
- `src/private/pugixml/**` (vendored 1.15).
- `src/private/mxtest/{core,corert,import,probe,validate}/**` (the new gates; new
  `mxtest/core` is the new core's unit suite, unrelated to the deleted old one).
- `src/private/cpul/**`, `data/**` (incl. `data/synthetic/` moves and sidecars),
  `docs/musicxml-*.xsd` and `docs/sounds-*.xml` (the renames stand),
  `docs/ai/design/**`, `.gitignore` (union with old entries if any matter).

### Merged / rewritten by hand

- `README.md` — start from `git show 71fc402:README.md` and make the **smallest
  possible diff**: preserve the original at every sentence and word where preservation
  is possible. Update only what is factually wrong now: C++17 → C++20, supported
  MusicXML version (new core generates from 4.0), the old build-mode table where
  targets no longer exist, and an honest status note that `mx::api` is present but
  excluded from the build pending the Phase-2 port. Add a brief generator section.
- `AGENTS.md` — **rewrite the original** (`git show 71fc402:AGENTS.md`), folding in as
  much of the newgen AGENTS.md information as possible. The newgen version is heavily
  biased toward codegen, but that is not what the project is solely about: the project
  is a MusicXML C++ library (`mx::api` is the product surface) that now contains a
  generator. Remove the "Ignore git history prior to b01288" instruction — history is
  meaningful again. Document the phase state and the `MX_API` flag.
- `CMakeLists.txt`, `Makefile`, `Dockerfile`, `.github/workflows/ci.yaml` — §5.

## 4. Branch construction

1. `git branch newgen-integration 71fc402` (the name is not load-bearing).
2. Build each commit in §6 by checking out path sets from the two sources and applying
   the hand-merged files. Author fresh commit messages; do not cherry-pick the old
   history.
3. **Verification invariant (run before declaring done):**
   `git diff --name-status 71fc402 newgen-integration` — every `D` line must appear in
   §3's intentional-deletion list. Anything else deleted is a bug.
4. A second useful invariant: `git diff ba4916f newgen-integration` should show only
   restorations (api/impl/utility/tests/examples/public headers), the hand-merged
   build/docs files, and the gen-quality/lint restoration.
5. Push with `git push -u origin newgen-integration`. Do NOT merge to master; do not
   open a PR unless asked.

**Commit-size rule:** large blasts of generated or vendored code are isolated in their
own commits so that every other commit is reviewable in a differ / on GitHub without
crashing it. Blast commits contain nothing a human needs to read.

**Green bar:** the TIP must be green (all gates in §7). Intermediate commits in the
sequence are allowed to be transiently unbuildable; do not introduce artificial CMake
churn to make each slice build independently.

## 5. Build system and CI specifics

### 5.1 CMake

Base: the newgen `CMakeLists.txt` (C++20, pugixml, `mx_core`, gated test binaries,
`MX_CORE_DEV` section). Add:

- `option(MX_API "build the mx::api/mx::impl product library and its tests (Phase 2)" OFF)`
  wiring the restored sources (api, impl, utility, examples, mxtest api/impl/file/
  control/old-import) into the `mx` static library + `mxtest` binary, with a comment
  marking it Phase-2. Default OFF; it is expected NOT to compile until the port. The
  point is that the port has a build home from day one.
- Restore `option(MX_COVERAGE ...)` (`--coverage` flags, non-MSVC guard) from the old
  CMakeLists, applied to the new-core targets.
- The whole project is C++20. No dual-standard build.

### 5.2 Makefile

Base: the newgen Makefile. Changes:

- Lean on mx-sdk: add Docker-gated wrappers for the C++ core gates
  (`core-dev`, `test-core-dev`, `test-cpp-unit`, `validate-cpp`, `probe-cpp`) following
  the existing `gen`/`test-go` pattern, so CI and the documented workflow run everything
  through the pinned toolchain. Native in-container/dev-machine invocation keeps working
  (the `MX_RUNNING_IN_DOCKER` switch already exists).
- Restore `coverage-core-dev` (old mechanism, new subject: `--coverage` build of
  `mx_core` + corert + unit suites, gcovr table) and `check-core-dev` if applicable.
- Restore `gen-quality` and `gen-lint` targets from the old Makefile (§5.3).
- Do NOT restore `lib`/`dev`/`core`/`test`/`test-all`/`examples-run`/`xcode-*` in
  Phase 1 — no advertised target may be broken. They return in Phase 2 (Phase 3 for
  xcode) when they can actually build.
- `core-dev`/`test-core-dev` keep their newgen meaning (the new core). This reuses the
  old names for the same intent: codegen iteration on the core.

### 5.3 Generator quality gates

Restore `gen/quality.py` + `gen/.pylintrc` from `71fc402`, adapted: point the analyzed
file set (`GEN_PY`) at the new `gen/` tree, then **re-baseline the floors** — run once,
record the current composite as `GEN_QUALITY_FLOOR` and the current pylint rating as
`GEN_LINT_FLOOR` (floor = current score; ratchet later). Restore the pinned analyzer
venv to the Dockerfile (radon, astroid, pylint, cognitive_complexity — same pins as old
master unless they fail to install, in which case re-pin and say so in the commit).

### 5.4 Dockerfile (mx-sdk)

Base: the newgen Dockerfile (Ubuntu 24.04, GCC 14, Go, libxml2-dev, clang-format-18,
ccache). Add back from old master: `gcovr`, the quality venv (§5.3). Add
`libxml2-utils` (xmllint, needed in-container by `validate-cpp` once it is
Docker-gated).

### 5.5 CI (`.github/workflows/ci.yaml`)

Rebuilt from scratch (the newgen stub is stale — it still references ezxml). All linux
jobs run through mx-sdk. Reuse the old workflow's mechanics for ccache/build-volume
caching and PR comments.

| Job | Contents |
|---|---|
| `linux-gen` | fmt-check; `make test-gen`; `plates --check` (all targets); regen-then-`git diff --exit-code` (committed generated output must match the generator); `gen-quality` + `gen-lint`, with the quality-report PR comment from the old workflow |
| `linux-core` | `test-core-dev` (corert **829/0/0 pinned**), `test-cpp-unit`, `validate-cpp`, `probe-cpp` |
| `linux-targets` | `test-go`, `test-c` |
| `coverage` | `coverage-core-dev` over the new core, gcovr table, HTML artifact, PR comment — same mechanism as old master |
| `macos` | native (no Docker): build `mx_core` + run unit and corert suites with AppleClang. The new core has only ever been compiled with GCC 14; AppleClang fallout is found here and **fixed in Phase 1** (it is our code) |

## 6. Commit sequence

Content assembled per §3/§4; messages are fresh; blast commits marked **[blast]**.

1. **`gen:` replace the generator prototype with the pipeline.** Delete
   `parse.py`/`ids.py`/`generate.py`; land `gen/` pipeline + target configs/templates +
   gen tests; restore+adapt `quality.py`/`.pylintrc` (floors re-baselined); corpus moves
   (`data/`, incl. `data/synthetic/`), XSD/sounds renames in `docs/`;
   `docs/ai/design/` docs. Excludes committed generated output.
2. **`gen:` generated target output [blast].** `gen/test/go/mx/`, `gen/test/go/vendor/`,
   `gen/test/c/mx/`, `gen/schema/out/`.
3. **`core:` vendor pugixml 1.15 [blast].** `src/private/pugixml/`.
4. **`core:` replace mx::core.** Delete old `src/private/mx/core/**` and
   `src/private/mx/ezxml/**`; land the hand-written runtime (`src/private/mx/core/*`)
   and `src/private/mxtest/{core,corert,import,probe,validate}` (new gates), `cpul`.
   (Old `mxtest/core` + old corert deleted here.)
5. **`core:` the generated model [blast].** `src/private/mx/core/generated/` (910 files).
6. **`api:` preserve mx::api/mx::impl behind MX_API.** Restore public headers, api,
   impl, utility, examples, mxtest api/impl/file/control/old-import per §3; CMake
   `MX_API=OFF` wiring.
7. **`build:` merged CMake/Makefile/Dockerfile.** §5.1–5.4 (whatever of CMake/Makefile
   wasn't already needed by commits 4–6 to keep the tip story coherent; transient
   unbuildability of intermediate commits is acceptable).
8. **`ci:` rebuilt workflow.** §5.5.
9. **`docs:` README minimal-diff revision; AGENTS.md rewritten from the original;**
   this plan rides along if not already present.

## 7. Phase-1 acceptance gates (the tip must pass ALL)

1. `git diff --name-status 71fc402 <tip>`: every deletion is in §3's intentional list.
2. `make test-core-dev` — corert 829/0/0 (pinned-counts case enforces it).
3. `make test-cpp-unit`, `make validate-cpp`, `make probe-cpp` — green.
4. `make test-gen`; `plates --check` for all four targets; regen all targets then
   `git diff --exit-code` (zero drift).
5. `make gen-quality`, `make gen-lint` — at or above the re-baselined floors.
6. `make test-go`, `make test-c` — 777/0/52 each.
7. `make check` (fmt) — clean.
8. CI workflow is syntactically valid and its jobs map 1:1 onto the targets above.
9. README/AGENTS.md done per §3 ("merged / rewritten by hand").
10. Branch pushed (`git push -u origin newgen-integration`). NO merge to master; NO PR
    unless the owner asks.

## 8. Phase 2 outline (the `mx::impl` port) — scope only, opens with its own design doc

- **Goal:** `mx::api` works again, implemented via `mx::impl` against the new
  `mx::core`; api changes minimally invasive (test-harness fallout only).
- **Acceptance:** preserved `mxtest/api` + `mxtest/impl` suites ALL GREEN; `MX_API`
  default ON; `lib`/`dev`/`test`/`examples-run` Makefile targets restored; coverage job
  naturally grows to cover api/impl.
- **Known seams (inventoried during planning):**
  - `src/private/mx/api/DocumentManager.cpp` — the api↔core boundary; rewritten from
    ezxml `XDoc` to `mx::core::parse(const pugi::xml_document&) -> Result<Document>`
    and the serializer.
  - ezxml-referencing files needing rewrite: `DocumentManager.cpp`,
    `mxtest/api/{ApiTester,PitchDataTest,SlurTest}.cpp`, and the old `mxtest/import`
    harness (rebuild the api-level corpus roundtrip on pugixml; prefer newgen's
    `Normalize.cpp`; old `SortAttributes`/`ChangeValues` likely retire).
  - Semantic fallout to adjudicate per-test: the new core clamps values on
    construction, dispatches choices by variant Kind, enforces schema order
    structurally, and returns `Result` from bounded appends — api tests that relied on
    old-core leniency or error behavior will need deliberate decisions.
  - `mxtest/impl` (8 files) assert old-core types directly: port or retire per-file.
- **Out of scope for Phase 2:** xcode/xcframework/iOS/macOS distribution (Phase 3).

## 9. Phase 3 (theoretical)

Reinstate iOS/macOS support with fresh eyes (the old checked-in Xcode project may not
have been working correctly anyway) and consider distribution channels and package
managers. Nothing earlier depends on it; nothing here constrains it.
