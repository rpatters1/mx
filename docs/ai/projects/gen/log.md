# gen Log

Append chronologically, oldest on top.

## M1: revgen (2026-05-18 — 2026-05-21, 40 iterations)

Reverse-engineered the codegen. Iteratively shrank `SKIP_ELEMENTS` and `CHOICE_SKIP` to empty.
Closed 2026-05-21 with the generator producing every C++ class in `mx/core`. Tests failing — the
commit `d4f25ee6` "src: issues caused by revgen" hand-edits non-generated consumers to keep
the build working; those hand-edits were the input to M2.

## M2: fix-gen (2026-05-21 — 2026-05-22)

Triaged `d4f25ee6` into 6 root-cause issues (A–F), then triaged 62 (later 129 after a clean
build) `make test-all` failures into clusters. Fixed iteratively, ending at 0 failures.

### Triage of `d4f25ee6` hand-edits

- **A — UpDownNone collapsed to UpDown** (WEIRD / Low): Hand-applied MusicXML 4.0 backport
  in original `mx/core` overwritten by schema-faithful 3.x regen. Deferred to M5; TODO
  comments left in `mx/impl`.
- **B — `hasLong` escaped to `hasLong_`** (BUG / Low): Keyword-escape over-applied to the
  has-prefix flag. Fix: `has_flag_name` strips trailing underscore added by `camel()` for
  keywords.
- **C — MetronomeTuplet group flattened** (BUG / High): Regen flattened
  `TimeModificationNormalTypeNormalDot` into `MetronomeTuplet`, making `<normal-type>`
  unconditional. Fix: added `EXTENSION_OPTIONAL_GROUP_RENAME` (per-extending-type override) +
  `SUPPRESS_GROUP_SUFFIX` + `WRAPPER_AS_ELEMENT_SYNTH_GROUPS` so the wrapper survives as a
  sub-element rather than being inlined.
- **D — `setPerMinuteOtBeatUnitChoice` typo** (BENIGN): Historical typo in original; accepted
  small test edit, no generator change.
- **E/F — Missing `importGroup` overloads for traditional-/non-traditional-key** (BUG /
  Medium): Fix path option 1: emit `Key::importTraditionalKey` / `Key::importNonTraditionalKey`
  as private member functions via new `TREE_ELEMENT_CONFIG["key"] = {"parent_imports_choice_groups": True}`.
  Reverted hand-edits in `FromXElement.{h,cpp}`.

### Failing-test clusters

Original triage scheme R1–R7 was rewritten mid-M2 as the situation evolved. Final shape:

- **R1 — direction-family parents emit `MX_FROM_XELEMENT_UNUSED`** (19 tests): split into
  R1a (ArrowGroup needs real `fromXElementImpl` body — added "arrow" to
  `GROUPS_WITH_REAL_FROM_X_ELEMENT`) and R1b (Direction parent bespoke — added
  `_emit_direction_family` handler driven by `model.complex_types["direction-type"].content_tree.branches`).
- **R5 — required-set seeding** (10 tests including HarpPedals SIGSEGV): added structural
  rule in `generate_element_cpp` for `min_occurs>=1, max_occurs!=1, not is_group` children:
  ctor pre-seeds default, `hasContents()` returns true, `remove*` gates on `size>1`,
  `clear*Set` re-seeds.
- **R3 — group-class `streamContents` inter-child `endl`** (~10 tests): rewrote
  `generate_group_cpp` separator logic — required-after-required: unconditional endl before;
  required-after-optional: optional's block emits; index-0 optional: endl after.
- **R2 — choice-class spurious leading `endl`** (~7 tests): gated on
  `config.get("skip_parent")` in `generate_choice_class_cpp` so only `direction-type`-style
  wrapper choices keep the leading endl.
- **R4 — attribute default initializers** (bulk fix, ~22 tests): seeded 17 entries in
  `ATTR_DEFAULT_OVERRIDE` (lang="it" defaults, lineEnd=down, number="1", etc.). Continued
  in i5 with `CHILD_INIT_VALUE_OVERRIDE` for `Scaling`/`StaffDetails`.
- **hasContents direction cluster** (6 tests): Rest/Unpitched gated optional groups on
  `myHas`; MeasureLayout/NoteheadText/Slash/BeatRepeat needed
  `ELEMENT_HAS_CONTENTS_ALWAYS_TRUE` / `CHILD_MIN_OCCURS_OVERRIDE` / notehead-text
  `seed_choice_set`.
- **group/tree-group isFirst separator** (i4/i6, ~30 tests): `generate_group_cpp` and
  `generate_tree_group_cpp` were emitting unconditional `endl` before each set-based child
  even when all preceding optional singletons were absent. Fixed with `isFirst` flag
  pattern.
- **MeasureLayout isOneLineOnly**: explicit child-presence check instead of `hasContents()`
  for elements with `ELEMENT_HAS_CONTENTS_ALWAYS_TRUE`.

### Final D-cluster (Metronome/Tempo round-trip)

After mass cleanup, 13 failures remained, all metronome/tempo. Re-triaged as D1–D4:

- **D3 — `MetronomeAttributes` missing `halign`/`justify` defaults** (`LeftCenterRight::center`):
  trivial `ATTR_DEFAULT_OVERRIDE` entries.
- **D1 — choice classes called `toStream` for empty-`streamName` containers** (BeatUnitPer,
  NoteRelationNote produced literal `<>...</>`): added `is_container` field to
  `TreeChoiceBranch`; emit `streamContents` instead of `toStream` for container branches in
  `generate_tree_choice_cpp`.
- **D2 — `DirectionWriter.cpp:370` throws on non-BPM tempos**: bisect confirmed pre-revgen
  `a0500803` did pass these tests, so revgen introduced the regression in the reader/writer
  path. Subsumed by D4.
- **D4 — Metronome container-branch `fromXElementImpl` missing**: `generate_tree_parent_cpp`
  emitted no dispatch for `is_container=True` branches. Synthetic containers have no XML
  tag, so the reader silently skipped all metronome children. Fix: added `trigger_names`
  field to `TreeChoiceBranch` (computed from first child element name); emit
  `importContainer<X>` declarations and bodies in the parent; dispatch in
  `fromXElementImpl` keyed on trigger names. Generated `Metronome::fromXElementImpl` matches
  HEAD exactly. 9 → 0 failed. **M2 complete.**

### Lessons / invariants captured

- `git checkout -- src/private/mx/core/` preserves mtimes; incremental cmake then links
  partly-old `.o`s, giving stale test counts. Use `make clean && make test-all` for any
  authoritative measurement.
- `make test-all` must run with generated files present (Issue A's `UpDownNone`
  hand-backport in HEAD is incompatible with the schema-faithful regen, so a reset-first
  build fails to compile).
- When removing a previously-emitted byte from a shared template, survey the whole HEAD
  population that template emits, not just one or two representative files (R2's
  DirectionType regression).
- Bespoke handlers should still read the parsed XSD model — pattern is "custom algorithm,
  schema-driven data" (Direction's element-name tables derived from
  `model.complex_types["direction-type"].content_tree.branches`).
