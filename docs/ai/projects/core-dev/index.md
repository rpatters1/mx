---
created: 2026-05-21
status: complete
prs: https://github.com/webern/mx/pull/153
---

# core-dev

## Goal

A permanent in-repo "core development" mode that lets a codegen agent churn
`mx/core` symbols without keeping `mx/api`, `mx/impl`, or higher-level tests
compiling.

Activated via `make core-dev` / `make check-core-dev` / `make test-core-dev`,
backed by an `MX_CORE_DEV=ON` CMake option. CMake builds only `mx/core` +
`mx/ezxml` + `mx/utility` plus a roundtrip test binary; no per-file `#ifdef`
guards.

The test suite is one roundtrip harness: each `*.xml` / `*.musicxml` file under
`data/` (excluding `expected/`, `testOutput/`, `generalxml/`, `smufl/`) is a
Catch2 test case asserting that `Document::fromXDoc -> toXDoc` matches the
normalized input.

CI does not run core-dev. Surfaced in `AGENTS.md` and `make help`. The consumer
is the codegen rewrite tracked in `docs/ai/projects/gen/`; core-dev is its
harness.

## Index

- `plan.md` — milestones
- `state.md` — last/next session
- `log.md` — append-only session log (compressed 2026-05-22)
- `.prompt` — user-owned scratchpad (agents do not read)
- `design/design.md` — full design
- `design/agents-md-snippet.md` — Markdown appended to `AGENTS.md` at
  implementation time

## Notes for Agents

- Milestones 1–3 COMPLETE as of 2026-05-22. Milestones 4–6 remain (see
  `plan.md`).
