# AGENTS.md Snippet — Core Development Mode

Verbatim Markdown to append to the repo-root `AGENTS.md` at implementation time. Stored
separately because nested code fences break Markdown rendering in the main design doc.

Insert as a new top-level section after `## Quality Gates`.

---

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
