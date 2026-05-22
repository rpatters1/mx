# core-dev Plan

## Milestone 1: Define goals ✓ COMPLETE

See `index.md ## Goal`; Q&A in `log.md`.

## Milestone 2: Design — in progress

Design lives in `design/`. Until approved, no code outside this project directory changes.

Design docs describe current state only (per `/project` skill); decisions and history go in
`log.md`.

Exit: design reviewed with user; open questions resolved.

## Milestone 3: Implement ✓ COMPLETE

Landed the design across four implementation phases plus a wrap-up:

1. Phase A — CMake option + Makefile targets (normal build unchanged when `MX_CORE_DEV=OFF`).
2. Phase B — core roundtrip sources behind one hardcoded `TEST_CASE` under
   `src/private/mxtest/corert/`.
3. Phase C — dynamic Catch2 registration over every discovered file.
4. Phase D — agent surface (`AGENTS.md` section + README pointer + `make help` block).
5. Phase D wrap-up — full pre-merge gate run, docs finalized.

Final gate (2026-05-22, this branch):

- `make fmt`, `make check`, `make check-core-dev`: pass.
- `make test-all`: 3039 cases, 3004 pass, 35 fail (expected — codegen-rewrite signals).
- `make test-core-dev`: 361 cases, 326 pass, 35 fail (same set).

The `TEMP: codegen-rewrite harness` edits were already reverted before Phase A
began (working tree clean as of 2026-05-22), so no revert step was needed during
implementation. CI integration for core-dev (item 5 in the original ordering) is
deferred — the design treats it as optional; no further session is allocated.
The 35 core roundtrip failures are out of scope here and are the consumer signal
for the codegen rewrite tracked in `docs/ai/projects/gen/`.

## Milestone 4: Pass Tests and CI

## Milestone 5: Code Coverage

## Milestone 6: Increase Coverage


