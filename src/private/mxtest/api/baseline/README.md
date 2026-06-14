# Step-0 baseline capture (mx-impl-port-plan.md §8)

This directory is the record of the Phase-2 pre-port baseline: the old
library's (master `71fc402`) honest corpus-wide api roundtrip, judged with
the NEW normalize-and-compare machinery. The resulting pinned pass-list is
`../roundtrip-baseline.txt` — the zero-regression bar for the Phase-2
corpus api-roundtrip gate.

Contents:

- `CaptureDriver.cpp` — the one-off driver, compiled against OLD master in
  a scratch worktree (it does NOT compile in this tree; this directory is
  excluded from the `MX_API` globs). For each eligible corpus file it ran
  `createFromFile -> getData -> createFromScore -> writeToFile`, recording
  a status row per file. Build/run instructions are in its header comment.
- `JudgeMain.cpp` — the judge (`cmake -DMX_BASELINE_JUDGE=on`, target
  `mxtest-baseline-judge`). Scores each captured output against its input
  with corert's comparator: `normalizeForComparison` both sides, apply the
  input's `.fixup.xml` sidecar to the expected side, strict depth-first
  DOM compare with numeric equivalence.
- `capture-status-71fc402.txt` — the driver's status output (2026-06-12,
  data root = this tree's `data/`): 829 files discovered, 794 OK,
  19 LOADFAIL, 8 GETDATAFAIL, 8 CREATEFAIL. The failures are the old
  3.0-era api meeting 3.1/4.0 features (unknown `listen`/`brass-bend`/...
  elements, numeral harmony without root/function, metronome
  note-relation, non-BPM tempo writes).
- `judge-results-71fc402.txt` — the judge's verdict per file: 1 PASS,
  793 FAIL, 35 SKIP (the capture failures). The FAILs are genuine
  api-subset fidelity losses, verified by hand on samples: `mx::api`
  reorders `<encoding>` children (encoding-date before software), drops
  `<part-group>`, `<credit>`, most of `<defaults>`, `<work>` (writes
  movement-title only), link/bookmark attributes, second `<creator>`
  elements, position attributes on `<fermata>`, etc.

The single passing file is the baseline. This is the honest pre-port truth
of the old library under a strict comparator the old tree never had; the
design doc anticipated exactly this ("files using unsupported features
cannot round-trip with fidelity, by design"). The pass-list grows in
Step 5 via discovery mode as deliberate commits.
