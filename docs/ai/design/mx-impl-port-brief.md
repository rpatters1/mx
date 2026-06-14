# Phase 2 Kickoff Brief: the `mx::impl` Port

Status: SUPERSEDED. The design session happened; every §5 question is settled in
`mx-impl-port-plan.md`, the BINDING Phase-2 decision record. This brief remains as the
evidence snapshot it was. Scope authority remains plan §8 as amended by the design doc.

## 1. Goal and acceptance (fixed by plan §8)

**Goal:** `mx::api` works again, implemented via `mx::impl` against the new generated
`mx::core`; api changes minimally invasive (test-harness fallout only).

**Acceptance:**
- preserved `mxtest/api` + `mxtest/impl` suites ALL GREEN;
- `MX_API` default ON;
- `lib`/`dev`/`test`/`examples-run` Makefile targets restored;
- coverage job naturally grows to cover api/impl.

**Out of scope:** xcode/xcframework/iOS/macOS distribution (Phase 3).

## 2. Where Phase 1 left the tree

- Branch `newgen-integration` (all Phase-1 gates green; see plan §7). The new core lives at
  `src/private/mx/core/` (runtime) + `src/private/mx/core/generated/` (model); entry points are
  `mx::core::parse(const pugi::xml_document&) -> Result<Document>` and
  `mx::core::serialize(const Document&, pugi::xml_document&)` in `generated/Document.h`.
- The whole product stack is restored in-tree behind `option(MX_API OFF)` in CMakeLists.txt:
  the `mx` static library (api + impl + utility, linking `mx_core`), the `mxtest` binary
  (api/control/file/impl/import suites + cpul), and the three example programs. It is wired but
  has never compiled against the new core. There is no Makefile target for it yet (deliberate:
  no advertised target may be broken).
- The old api-import harness files sit in `src/private/mxtest/import/` next to newgen's
  `Normalize.{h,cpp}`/`DecimalFields.h`, as rewrite reference.

## 3. Measured inventory (run on this tree, commit e713bfc's successor)

- Old-core includes (`#include "mx/core/..."` paths that no longer exist): **46 of 77** impl
  files, **4 of 13** api .cpp files, **7 of 8** `mxtest/impl` files — **348 distinct old-core
  headers** referenced in total (mostly `mx/core/elements/*.h`, plus `mx/core/Enums.h`).
- ezxml references (the library is deleted): `src/private/mx/api/DocumentManager.cpp`,
  `mxtest/api/{ApiTester,PitchDataTest,SlurTest}.cpp`, and the entire old import harness
  (`ImportTest*`, `ExpectedFiles*`, `ChangeValues*`, `SortAttributes*`).
- `DocumentManager` public surface (`src/include/mx/api/DocumentManager.h`): `createFromFile`,
  `createFromStream`, `createFromScore`, `writeToFile`, `writeToStream`, `getData`,
  `destroyDocument` — an int-handle registry around the old `mx::core::Document` shared
  pointer. This is the api<->core seam and the natural first rewrite.
- Suites: `mxtest/api` has 30 test .cpp files (plus ScoreData headers); `mxtest/impl` has 8
  files (CurveFunctions, DirectionReader, DirectionWriter, MagicTemplates, MeasureWriter,
  PositionFunctions, PrintFunctions, ScoreWriter) — these assert old-core types directly.
- Examples (`mx/examples/`): api-only consumers; no direct core or ezxml use.

## 4. Old core -> new core: the deltas impl must absorb

| Old core (what impl was written against) | New core (what exists now) |
|---|---|
| `std::shared_ptr` element graph (`makeNote()`, `NotePtr`) | value-semantics classes; `std::optional<T>` members |
| `setHasFoo(true)` presence flags | `std::optional` presence (set the optional) |
| `Choice` enum + all alternatives co-allocated | `std::variant` with positional Kind/index dispatch |
| order-free child setters | named fields in schema order; serializer walks declaration order |
| `fromXDoc(std::ostream&, XDoc&)` per element, bool return | parse exists ONLY at the document boundary: `parse(...) -> Result<Document>` |
| ezxml `XDoc`/`XElement` DOM | pugixml directly |
| invalid values representable; clamping ad hoc on import | clamp-on-construct wrappers; invalid unrepresentable; bounded `add...` returns `Result` |
| required-with-unbounded: default element pre-pushed into the set | `OneOrMore<T>` (a first element always exists, by type) |
| MusicXML 3.0-era element set | MusicXML 4.0 model + version gating on parse |

Semantic fallout to adjudicate per-test (plan §8): api tests that relied on old-core leniency,
error behavior, or representational slack need deliberate decisions, not mechanical porting.

## 5. Decisions the design session must settle (the grill agenda)

1. **Port strategy for impl:** mechanical file-by-file translation of the 77 impl files vs.
   reimplementation around the new core's shape (the old reader/writer split may not map; the
   new core's variants and schema-order fields remove whole classes of old impl bookkeeping).
2. **The `DocumentManager` rewrite:** keeps its int-handle/singleton contract (minimally
   invasive) — but what does it hold now that `Document` is a value type? What happens to
   `mx::core` parse failures that the old api silently tolerated (`Result` carries errors;
   the old `createFromStream` had no failure channel)?
3. **api surface changes:** plan says "test-harness fallout only." Is the version field
   (`ScoreData::musicXmlVersion`) now 4.0? What does `mx::api` emit as the root `version`?
4. **`mxtest/impl` per-file fate:** the 8 files assert old-core types directly; port or retire
   each (e.g. MagicTemplatesTest may be meaningless against the new core).
5. **The api-level corpus roundtrip:** rebuild ImportTest on pugixml, preferring newgen's
   `Normalize.cpp`; do `SortAttributes`/`ChangeValues`/`ExpectedFiles` (and `data/expected/`)
   survive at all?
6. **`mx::utility` (9 headers, 15 impl files reference it):** keep as-is, shrink, or absorb.
7. **Order of work and gating:** which suite comes green first (impl unit vs api vs import)?
   When does `MX_API` flip ON in CI (a new CI job, or extend linux-core)? When do the
   `lib`/`dev`/`test`/`examples-run` Makefile targets return?
8. **Old-core idioms with no new equivalent:** e.g. impl code that built partially-invalid
   intermediate states before fixing them up — the new core forbids those states; the impl
   algorithms that depended on them need redesign, and they should be found early (the
   `MX_API=ON` error wall is the cheapest census: configure with `-DMX_API=on` and catalog).

## 6. Suggested first moves (cheap, evidence-producing)

1. `cmake -S . -B build/api-wall -DMX_API=on && cmake --build build/api-wall` — capture and
   classify the error wall (it is the true, compiler-verified inventory of §3).
2. Rewrite `DocumentManager.cpp` first; everything in `mxtest/api` funnels through it.
3. Pick one thin vertical slice (e.g. PitchData: api struct -> impl reader/writer -> one api
   test) to validate the chosen port strategy before committing to all 77 files.
