# core-dev State

Milestones 1–3 COMPLETE as of 2026-05-22. Milestones 4–6 (pass tests + CI,
code coverage, increase coverage) remain — see `plan.md`.

`core-dev` mode is live: `make core-dev` / `make check-core-dev` /
`make test-core-dev` work; the core roundtrip suite also runs inside
`make test-all` (gated on `MX_BUILD_CORE_TESTS=ON`). The consumer is the
codegen rewrite at `docs/ai/projects/gen/`.

Final gate numbers and the 35-failure breakdown are in `log.md`.

## If a future session reopens this project

- Read `index.md`, `plan.md`, then `design/design.md` (design describes
  current state; history is in `log.md`).
- Likely reopen triggers: starting Milestone 4+; or an item the design
  marked out of scope becoming in scope (CI for core-dev, removing dormant
  `MX_COMPILE_*` macros, the `DocumentSpec.h` / `DocumentHeader.h`
  version-string conflict).
- The 35 expected core roundtrip failures are not a regression of this
  project — investigate via `docs/ai/projects/gen/`.
