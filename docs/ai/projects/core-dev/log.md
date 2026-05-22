# core-dev Log

Compressed 2026-05-22. Earlier session-by-session notes collapsed to the
retrospective and gotchas below; full per-session entries are in git history if
needed.

## Milestone 1 — Define goals (2026-05-21)

User-approved scope: `mx/core` + `mx/ezxml` + `mx/utility` only; `mx/api`,
`mx/impl`, their tests, and the 522 core element unit tests excluded. Test
harness is roundtrip-only with dynamic Catch2 registration via `AutoReg` (one
case per file). Makefile: `core-dev`, `check-core-dev`, `test-core-dev`. CMake
exclusion only (no `#ifdef` guards added). CI out of scope. Docs surface:
`AGENTS.md` + `make help`.

## Milestone 2 — Design (2026-05-21 → 2026-05-22)

Final design landed in `design/design.md` + `design/agents-md-snippet.md`.
Notable decisions made during review:

- One top-level `if(MX_CORE_DEV)...else()...endif()` wrap in `CMakeLists.txt`;
  `cmake_build` macro takes `MX_CORE_DEV` as a parameter.
- "Option B" reconciliation: same `.cpp` files compile into two binaries
  (`mxtest-core-dev` always; `mxtest` only under `MX_BUILD_CORE_TESTS=ON`), so
  core roundtrip runs in `make test-all` too. Dual-Compilation Invariant
  documented in design §2.
- Terminology fixed: "core roundtrip" / `CORE_ROUNDTRIP` / `CORE_RT` vs "api
  import" / `API_IMPORT`. Never say "roundtrip" unqualified. Repo `AGENTS.md`
  carries the terminology section.
- Source directory: `src/private/mxtest/corert/`. Discovery excludes
  directories named `expected`, `testOutput`, `generalxml`, `smufl`.
- Normalization narrowed to canonicalization only (xml decl, doctype, root
  version, decimal zero-stripping, attribute sort). The api import suite's
  defect-workaround block was deliberately dropped — a mismatch here is a
  signal to fix `mx::core`, the input, or codegen.
- Failure handling: full normalization on both sides; first-mismatch FAIL with
  node path; per-file `try`/`catch` so an exception fails one case rather than
  aborting the suite. Text comparison exact; trimming deferred unless a real
  case drives it (Phase C surfaced no whitespace-only mismatches).
- Debug output to `data/testOutput/corert/`, flattened filenames
  (`lysuite_ly01a.xml.expected.xml` / `.actual.xml`), `mkdir -p` at write
  time.

## Milestone 3 — Implement (2026-05-22)

Four phases plus wrap-up; each ended green for what it added.

- **Phase A** — CMake option, Makefile targets, Docker `run-core-dev` stage.
  `mxtest-core-dev` reuses `cpul/main.cpp` (Catch2 main) instead of a
  placeholder `main()` to avoid a link collision. `make test-core-dev` passes
  `--allow-running-no-tests` to stay green when ARGS filters to zero matches.
- **Phase B** — `CoreRoundtripImpl.{h,cpp}` + `CoreRoundtripTest.cpp` with one
  hardcoded case (`lysuite/ly01a_Pitches_Pitches.xml`). Helpers reused from
  `mxtest/import/` (`ChangeValues`, `SortAttributes`); their `.cpp` files
  enumerated explicitly via `SRC_MX_CORE_ROUNDTRIP_HELPERS` so core-dev does
  not pull in `MxFileRepository` / `ImportTestImpl`. Version string hardcoded
  `"3.0"` — see "Known conflicts" below.
- **Phase C** — Dynamic Catch2 registration. Gotchas resolved:
  - `Catch::StringRef` does not own storage; test-name `std::string`s live in
    a function-local-static `std::vector` that is `reserve()`d to final size
    before any `push_back` so addresses are stable.
  - `Catch::AutoReg` is non-copyable and non-movable; owned through
    `std::vector<std::unique_ptr<AutoReg>>`.
  - Custom `ITestInvoker` subclass owns its own `std::string` test name copy.
- **Phase D (docs)** — `## Core Development Mode (Codegen)` section appended
  to `AGENTS.md` directly after `## Quality Gates`; one-line README pointer;
  `make help` block. Working filter form under Catch2 v3 is
  `[core-roundtrip] lysuite/*` (not `lysuite/`); both design surfaces updated
  to match.
- **Phase D (wrap-up)** — Full gate green. `plan.md` / `index.md` / `state.md`
  finalized.

## Final gate (2026-05-22)

| Target              | Result                                      |
|---------------------|---------------------------------------------|
| `make fmt`          | pass                                        |
| `make check`        | pass                                        |
| `make check-core-dev` | pass                                      |
| `make test-all`     | 3039 cases, 3004 pass, 35 fail (expected)   |
| `make test-core-dev` | 361 cases, 326 pass, 35 fail (same set)    |

The 35 core roundtrip failures are the consumer signal for the codegen rewrite
at `docs/ai/projects/gen/` and out of scope for this project. By rough
category from the failure messages: ~14 attribute mismatches, ~5 text
mismatches, a couple of child-count / attribute-count mismatches, one
`fromXDoc` returning false. Files span `foundsuite/`, parts of `lysuite/`
(figured bass, accordion registrations, articulation texts, parenthesized
accidentals), `mjbsuite/krz_v40.xml` (image element attribute set),
`musuite/test_harmony.xml` (harmony kind text), `musuite/testInvalid.xml`
(identification sub-element count).

## Known conflicts retained out of scope

- `DocumentSpec.h` vs `DocumentHeader.h` both declare `MusicXmlVersion`;
  `DocumentSpec.h` vs `Document.h` both declare `DocumentChoice`. Co-including
  is a hard compile error. The core roundtrip suite needs `Document.h`, so it
  hardcodes `"3.0"` to match the api import precedent. Fixing this conflict
  belongs to the codegen rewrite, not this project.
- Dormant `MX_COMPILE_*` macros in `CompileControl.h` were left as-is; design
  §2 marks removal out of scope.
