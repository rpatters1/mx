# mx

@./README.md

`mx` is a MusicXML C++ class library. The product surface is `mx::api`, a simplified structural
representation of MusicXML backed by the strongly-typed `mx::core` model. The repository also
contains the code generator (`gen/`) that emits `mx::core` (and secondary-language test targets)
from the MusicXML XSD specification.

## Phase state (read this first)

The integration plan `docs/ai/design/newgen-integration-plan.md` is the binding decision record
for the current state of the tree:

- **Phase 1 (DONE, this tree):** the old hand-written `mx::core` (and `ezxml`) were replaced by
  the generated core; the generator pipeline, its targets, and all of its gates are in place and
  green. `mx::api`, `mx::impl`, `mx::utility`, the examples, and their test suites are preserved
  in-tree but **excluded from the build** behind the CMake option `MX_API` (default `OFF`). They
  are not expected to compile yet.
- **Phase 2 (IN PROGRESS):** reimplement `mx::impl` against the new core,
  rewrite the `DocumentManager` seam on pugixml/`mx::core::parse`, bring the preserved
  `mxtest/api` + `mxtest/impl` suites back to green, and flip `MX_API` on by default.
  Work-order steps 0-2 are DONE (pinned baseline, the DocumentManager/Result seam, the
  Converter+Note vertical slice); the step-3 mechanical wave is underway. **Live status:
  `docs/ai/design/mx-impl-port-plan.md` §9.1 + the Appendix A checklist** (ported TUs
  compile via the `MX_IMPL_SLICE` CMake scratch target; corert stays green throughout).
  **The BINDING design doc is `docs/ai/design/mx-impl-port-plan.md`** (port strategy, the
  DocumentManager Result contract, version semantics, test fates, the corpus api-roundtrip
  gate with its pre-port baseline, work order, CI plan, adjudication log). The kickoff
  brief `docs/ai/design/mx-impl-port-brief.md` is superseded by it; scope authority remains
  plan §8 as amended by the design doc.
- **Phase 3 (theoretical):** iOS/macOS distribution (Xcode project, xcframework) reinstated with
  fresh eyes.

**CARDINAL RULE: the generator is language agnostic.** Adding a new language target must not
require edits to the generator's Python files; all language knowledge lives in the target's own
directory as `config.toml` data and Mustache templates. The rule HOLDS and is enforced
structurally by `gen/tests/test_agnosticism.py` (the generator's Python is a closed set; no
module is named after a language; targets contain no Python). Design and decision record:
`docs/ai/design/generator-agnosticism.md`. The proof: the JSON Schema target (`gen/schema/`)
was added without touching a single `.py` file.

## Repository layout

```
mx/
  AGENTS.md             <- you are here
  Makefile              <- top-level build driver (mx-sdk Docker-gated targets)
  Dockerfile            <- mx-sdk toolchain image (Ubuntu 24.04, GCC 14, Go, libxml2,
                           Python 3, gcovr, pinned quality-venv analyzers)
  CMakeLists.txt        <- C++ project: pugixml + mx_core + test binaries + MX_API stub
  data/                 <- MusicXML test corpus (~1,347 files, see data/README.md)
  docs/ai/design/       <- design docs (newgen-integration-plan.md: the phase plan;
                           plates.md: the Plates; generator-agnosticism.md: the cardinal
                           rule; mx-core-plan.md: the BINDING plan for the C++ mx::core
                           target, IMPLEMENTED)
  src/include/mx/api/   <- the PUBLIC mx::api headers (Phase 2; excluded from build)
  src/private/          <- C++ source
    pugixml/            <- vendored pugixml 1.15 (the XML layer mx::core consumes directly)
    mx/core/            <- mx::core's HAND-WRITTEN runtime: Decimal, Result, Error, Lexical,
                           Token, NameToken, OneOrMore, Xml (no generated banner)
    mx/core/generated/  <- the GENERATED C++ typed model (committed; do not edit; the
                           directory IS the generated/hand-written boundary)
    mx/api/             <- mx::api implementation        [Phase 2; excluded from build]
    mx/impl/            <- the api<->core translation    [Phase 2; excluded from build]
    mx/utility/         <- helpers used by impl          [Phase 2; excluded from build]
    mx/examples/        <- api example programs          [Phase 2; excluded from build]
    mxtest/core/        <- mx::core unit tests (values, shapes, document/rejection suite)
    mxtest/corert/      <- C++ core roundtrip test harness (Catch2, dynamic registration)
    mxtest/import/      <- normalization (Normalize.cpp: whitespace/decimals/attr order);
                           the OLD api-import harness files were deleted in Phase 2,
                           superseded by corert (mx-impl-port-plan.md §7.3)
    mxtest/probe/       <- Gate-4 must-NOT-compile probes (`make probe-cpp`)
    mxtest/validate/    <- Gate-2 tool: serialize corpus, xmllint outputs (`make validate-cpp`)
    mxtest/api/         <- the preserved api suite = the Phase-2 acceptance gate [excluded]
    mxtest/impl/        <- the preserved impl unit specs [Phase 2; excluded from build]
    mxtest/file/        <- api-test file repository infra + PathRoot.h (CMake-generated,
                           gitignored)
    mxtest/control/     <- api-test compile-control header [excluded from build]
    cpul/               <- vendored Catch2 test runner
  gen/                  <- code generator system (see gen/README.md)
    __main__.py         <- CLI: analyze | ir | plates | render | <config.toml>
    README.md           <- architecture, IR glossary, XSD analysis
    quality.py          <- design-quality scorer (`make gen-quality`; floor in Makefile)
    .pylintrc           <- pylint config (`make gen-lint`)
    xsd/                <- XSD parser + structural analysis
    ir/                 <- resolved intermediate representation (IR)
    plates/             <- the per-target projection (casings, idents, policy data,
                           the grammar-preserving content/group projection)
    press/              <- the Mustache engine, context builder, manifest renderer, writer
    cpp/                <- C++ target: config.toml + templates/ -> src/private/mx/core/generated
    schema/             <- JSON Schema target: config.toml + templates/ + out/ (committed)
    test/go/            <- Go corert test target
      config.toml       <- Go target configuration incl. the [render] manifest
      templates/        <- the Go templates (all Go knowledge lives here + config.toml)
      go.mod, go.sum    <- Go module (etree dependency, vendored)
      vendor/           <- vendored Go deps
      corert/           <- test package (discover, fixer, normalize, roundtrip, test)
      mx/               <- the GENERATED Go model (committed; do not edit)
    test/c/             <- C corert test target
      config.toml       <- C target configuration incl. the [render] manifest
      templates/        <- the C templates (all C knowledge lives here + config.toml)
      CMakeLists.txt    <- CMake project using libxml2
      src/              <- C source (main, discover, fixer, normalize, compare, roundtrip)
      mx/               <- the GENERATED C model (committed; do not edit)
```

## Build system

### Docker (mx-sdk)

All Docker-gated targets auto-build the `mx-sdk` image on first use. The workspace is bind-mounted
at `/workspace`. A named Docker volume `mx-build` persists CMake/ccache state across runs.

The `MX_RUNNING_IN_DOCKER` env var switches the Makefile between in-container (direct tool
invocation) and outside-container (docker run wrapper) behavior. Setting it on a dev machine (or
the macOS CI runner, which has no Docker) runs the same recipes natively with the host toolchain.

### Makefile targets

Run `make help` for the full, current target list (C++ core gates, generator, Go/C test targets,
quality gates, housekeeping). No Makefile target builds the `MX_API` sources yet -- the old
`lib`/`dev`/`test`/`examples-run` targets return in Phase 2 when they can actually build.

## The corert (core roundtrip) test

The corert test is the primary correctness gate. It exercises the generated parser by round-tripping
every eligible XML file in `data/` through the typed model and comparing the output to a normalized
form of the input. Both sides of a comparison get their root `version` attribute pinned to one
baseline so the attribute itself never produces a mismatch; the baseline is a constant in each
harness's normalize module (`musicXMLVersion` in `gen/test/go/corert/normalize.go`,
`MUSICXML_VERSION` in `gen/test/c/src/normalize.c`, `kMusicXmlVersionBaseline` in
`src/private/mxtest/corert/Compare.h` -- currently `3.0`), a harness choice, not a
property of the corpus or the architecture.

Vocabulary note: "corert" is the suite that round-trips through `mx::core` only. The OLD
api-level roundtrip ("api import", `mxtest/import/ImportTest*` against `data/expected/`) was
deleted in Phase 2, superseded by corert plus the corpus-wide api roundtrip gate
(mx-impl-port-plan.md §7.3/§8). Always say which one you mean.

### Flow (same in all three languages)

1. **Discover** eligible `.xml`/`.musicxml` files under `data/`, excluding directories `expected`,
   `testOutput`, `generalxml`, `smufl`, and files matching `*.fixup.xml` or having a `.invalid`
   sibling marker.
2. For each file:
   a. Load the XML into a DOM.
   b. Set the root `version` attribute to the harness baseline (see above).
   c. **Parse** into the typed model via the generated parse entry point.
   d. **Serialize** back to XML.
   e. **Normalize** the actual output (see Normalization pipeline below).
   f. Load a fresh expected document from disk, apply the same normalization.
   g. Apply **fixups** from `.fixup.xml` sidecars to the expected document.
   h. **Compare** the two DOMs depth-first: element names, text content, attributes (with numeric
      equivalence for ints/floats).
3. Report pass/fail per file.

### Current state

All three suites are GREEN. The C++ suite (`make test-core-dev`) passes **829/0/0**
(pass/fail/skip) with a pinned-counts case that makes count drift a failure. Go and C pass
777/0/52 (the 52 declare MusicXML 4.0; those targets generate from the 3.1 schema, and while
MusicXML is backward compatible, a newer document may use types an older model cannot
represent -- the harnesses gate on the root's declared version; the C++ target generates from
4.0, so nothing in the corpus skips for it).

Beyond corert, the C++ target has four more standing gates (mx-core-plan.md §5):
`make validate-cpp` (every parsed corpus document is serialized and the OUTPUT is
xmllint-validated against the 4.0 XSD -- the permanent mechanical proof that value-clamp
leniency emits only valid XML), `make test-cpp-unit` (construction-safety, shape, and
rejection suites), `make probe-cpp` (must-NOT-compile probes), and the generator hygiene
gates (`make gen-check` = plates --check, `make test-gen` incl. test_agnosticism,
regen-then-`git diff --exit-code`, `make gen-quality` + `make gen-lint` floors).

### Data directory conventions

- `data/README.md` - documents marker file conventions.
- `*.xml.invalid` - sibling marker meaning the file is not valid MusicXML; skip it.
- `*.fixup.xml` - sidecar describing value substitutions for the expected document. Used when mx
  clamps out-of-bounds values on import (e.g. MIDI channel 0 -> 1). Format:
  ```xml
  <fixups>
    <replace>
      <type>element</type>          <!-- "element" or "attribute" -->
      <name>midi-channel</name>
      <value>0</value>
      <replacement-value>1</replacement-value>
    </replace>
  </fixups>
  ```
- `data/testOutput/corert/` - debug output directory for failure diffs (gitignored).

### Normalization pipeline

Applied to both expected and actual documents before comparison:

1. Set XML declaration: `<?xml version="1.0" encoding="UTF-8" standalone="no"?>`.
2. Set DOCTYPE based on root element name (`score-timewise` vs `score-partwise`).
3. Set the root `version` attribute to the harness baseline version.
4. Strip whitespace-only text nodes from every element (pretty-printing indentation is not
   content; MusicXML has no mixed content, and the rule is applied to both sides, so it stays
   symmetric).
5. Strip trailing zeros from decimal fields (the list lives in `DecimalFields.h`).
6. Sort attributes alphabetically by QUALIFIED name (`xlink:href`, not `href`; must be last).

Comparison details that took debugging to get right (the C++ harness applies the same rules):
compare each element's DIRECT text only, never the subtree concatenation
(a numerically-equivalent leaf reformat would otherwise fail at every ancestor); compare
attributes by qualified name with entity-resolved values (a parsed `xlink:href` is (ns, href)
while a serialized one may be the literal name); the Go loader transcodes UTF-16 and ISO-8859-1
to UTF-8 (libxml2 and pugixml auto-detect these; Go's encoding/xml does not). Documents whose
root declares a version newer than the target's generated `SupportedMusicXMLVersion` /
`MX_SUPPORTED_MUSICXML_VERSION` constant are skipped, not failed.

### Numeric equivalence

Text comparisons use numeric equivalence: if both strings parse as integers (or floats), compare
their values instead of their string representations. Float comparison uses epsilon `< 0.00000001`.

## Generator architecture

The generator (`gen/`) is a Python program structured as a pipeline: parse the MusicXML XSD into a
model (`gen/xsd/`), lower that into a resolved intermediate representation (`gen/ir/`), project the
IR onto a target as the Plates (`gen/plates/`), then render the target's own Mustache templates
through the press (`gen/press/`) per its `[render]` manifest. The generator has no concept of any
language; a target IS a directory of config and templates. The IR data model preserves the schema's named structure
(model groups, attribute groups, inheritance edges); `gen/ir/resolve.py` collapses it on demand into
the flattened view an emitter consumes (attribute groups expanded, group refs spliced into content),
so that splicing-and-deduping reasoning lives once rather than once per language. See `gen/README.md`
for the architecture, IR glossary, the resolution layer, and a structural analysis of the schema.

Vocabulary: a **plate** is the per-type metadata object handed to a template -- one per emitted
type, carrying the target's identifier casings, type mappings, emit strategy, and file assignment.
The **Plates** is the full collection projected for one target; it is where config.toml meets the
IR, so templates stay dumb renderers. Specified in `docs/ai/design/plates.md`.

Commands:
- `python3 -m gen analyze [xsd]` - print a structural analysis of the XSD.
- `python3 -m gen ir [--type NAME] [--resolve] [--config C] [xsd]` - lower the XSD to the IR and
  print it as JSON; `--resolve` prints the collapsed (group-spliced, attribute-flattened) view of
  complex types; `--config` applies a target config's companion patches (the sounds.xml fold) first.
- `python3 -m gen plates --config C [--type NAME] [--check]` - project the IR onto the target the
  config describes and print the Plates as JSON; `--check` validates renames and detects identifier
  collisions (a CI gate, like analyze).
- `python3 -m gen render --config C --type NAME` - render one type through the target's templates
  to stdout (template debugging).
- `python3 -m gen <config.toml>` - emit the target: project the plates, render the [render]
  manifest's templates, run the optional format hook, write with marker-gated pruning.

Each target has a `config.toml` specifying the MusicXML XSD it generates from (`[input] xsd`), the
output directory (`[output] dir`, relative to the config file), an optional `[sounds] xml` companion
file (see below), and, eventually, language-specific settings. Each path is relative to the config
file. The three targets deliberately span the matrix: C++ is 4.0 with sounds, C is 3.1 with sounds,
and Go is 3.1 without sounds (the C/Go pair differ only by the companion fold).

### Companion data

`instrument-sound` is `xs:string` in the XSD; the standard sound identifiers live only in the
separately versioned `sounds.xml` (vendored as `docs/sounds-<version>.xml`). When a target's
`config.toml` sets `[sounds] xml`, `gen/ir/sounds.py` folds them into the IR as a `sound-id` enum
unioned with an open string (element `instrument-sound` retyped from `string` to that union). This is
the only place the IR depends on an input beyond the XSD; it is opt-in per target, so the base IR
stays a pure function of the schema.

**Status.** The parse, IR, analysis, Plates, and press stages exist; the generator is fully
language agnostic. All four targets (C++, Go, C, JSON Schema) render from their own templates;
all three corert suites are green. Generated output is committed (`src/private/mx/core/generated/`,
`gen/test/go/mx/`, `gen/test/c/mx/`, `gen/schema/out/`).

The Plates also carry the grammar-preserving content projection (mx-core-plan.md §2.9, all
neutral schema facts): `ComplexPlate.fields` is the ordered field view of the content tree
(trivial single-element groups spliced inline, occurrence bounds multiplied through), and
`Plates.groups` holds the shared content types -- structural named schema groups
(`full-note-group`, `music-data-choice`, ...) plus the types synthesized from anonymous nested
particles (`{owner}-choice` / `{owner}-group` wires, renameable via `[rename.type]`). Fields
and choice alternatives carry first-name dispatch sets, nullability, bounded-max caps, and
presence flags, so a strict in-order parser is renderable from data alone. Targets that read
only the flat member view never render these (the press includes group plates only when a
manifest engages a group strategy or wire).

### Code generation - historical context

`mx::core` was originally "hand-generated" by human brute-force from the MusicXML 3.0-era XSD
(Ruby scripts, then an abandoned Rust attempt), which is why the library was stuck near MusicXML
3.0: the types could not be reliably regenerated from a newer spec. The `gen/` pipeline is the
working replacement: `mx::core` is now a regenerable artifact of the 4.0 XSD, and human design
choices live in `config.toml` (renames, import defaults, bespoke type rows) instead of in
hand-edited generated files. Code generation remains bespoke: the goal is to produce what the
`mx` library needs from the MusicXML specification, not to compile arbitrary XSD.

## Language targets

### C++ (primary, `gen/cpp/`)

MusicXML 4.0 with the sounds companion. The product target, IMPLEMENTED per
`docs/ai/design/mx-core-plan.md` (the binding decision record, which supersedes the two earlier
proposals `mx-core-gpt-5.0.md` / `mx-core-opus-4.7.md` wherever they conflict). Generated code
lands in `src/private/mx/core/generated/` (committed; its own directory, so generator-owned
code is recognizable at a glance), built as the `mx_core` static library (C++20) together with
the hand-written runtime files at `src/private/mx/core/`.

The shape, as built: valid-by-construction API -- enum wrappers whose named factories are the
only constructors; clamp-on-construct number wrappers; structural bespoke value types (color as
RGB(A) bytes, yyyy-mm-dd as a calendar-clamped date, comma-separated lists as item vectors,
SMuFL glyph names storing only the suffix); composites with named fields in schema order (the
serializer walks declaration order, so wrong order is unrepresentable); `std::variant` choice
classes with positional Kind/index dispatch (measure music-data boxes its alternatives via a
manifest types-row); `OneOrMore<T>` for minOccurs>=1 repeats; Result-returning bounded appends
(beam <= 8). Errors exist in exactly two places: `mx::core::parse(const pugi::xml_document&)
-> Result<Document>` (strict names/structure, lenient values, version gating, configured
required-attribute defaults via `[import] attribute-defaults`) and the bounded `add...`
methods. Generated code consumes pugixml directly (`src/private/pugixml/`, 1.15); ezxml is
retired and deleted.

Gates (all green; see `make help`): corert 829/0/0 pinned, `validate-cpp`, `test-cpp-unit`,
`probe-cpp`, plates/agnosticism/regen hygiene, and the plan's adversarial audit.

Identity tokens: `xs:ID`/`xs:IDREF`/`xs:NMTOKEN` attributes are repairing wrapper types
(`Token` = NCName, `NameToken` = NMTOKEN), so a lexically invalid identity value is
unrepresentable through the public API (the IR preserves `id`/`idref`/`nmtoken` as distinct
primitives; non-validating targets map them to their string spelling). The one residual
XSD-validity property the type system cannot enforce locally -- document-wide `xs:ID`
uniqueness and `xs:IDREF` referential existence -- is a documented limitation (plan §7); it is
the standard referential-integrity carve-out, and `xmllint --schema` does not check IDREF
existence either.

### Go (test target, `gen/test/go/`)

MusicXML 3.1 *without* the sounds companion. Uses `github.com/beevik/etree` (vendored) for DOM-style
XML. The generated code lives in `gen/test/go/mx/` (committed). Test runner uses Go's `testing`
package with subtests; documents declaring a newer MusicXML than 3.1 are skipped.

**Anemic API warning (owner-mandated):** this target is a non-validating round-trip harness
binding. It does not enforce sequence grammar or choice exclusivity, and its API can construct
invalid documents (a note with two pitches is representable). The owner has no use for APIs that
can construct an invalid document; this target's only legitimate role is keeping the generator
honest about language-agnosticism. A retrofit to the grammar-enforcing contract is an approved
follow-up in `docs/ai/design/mx-core-plan.md` §6.

### C (test target, `gen/test/c/`)

MusicXML 3.1 *with* the sounds companion -- same schema as Go, so the two outputs differ only by the
fold. Uses libxml2 (apt package in Docker). The generated code lives in `gen/test/c/mx/`
(committed), built as the `mx-c` static library plus a `values-smoke` test binary. Test runner is a
simple `main()` that prints pass/fail/skip per file and a summary.

**Anemic API warning (owner-mandated):** same status as the Go target above -- a non-validating
harness binding that can represent invalid documents; not a product surface. See
`docs/ai/design/mx-core-plan.md` §2.8 and §6.

## Quality gates

Always run `make fmt` after modifying code under `src/`; `make check` is the fmt gate.

- C++ core changes: `make test-core-dev` (corert, pinned 829/0/0), `make test-cpp-unit`,
  `make validate-cpp`, `make probe-cpp`, `make check-core-dev` (warning-free build).
- Generator changes: `make test-gen`, `make gen-check`, regenerate (`make gen`) and confirm
  `git diff --exit-code` (committed output must match), `make gen-quality`, `make gen-lint`.
- Go/C target changes: `make test-go`, `make test-c` (777/0/52 each).
- Coverage (advisory): `make coverage-core-dev` -> `data/testOutput/coverage/`.

CI (`.github/workflows/ci.yaml`) runs exactly these through the mx-sdk image, plus a native
macOS (AppleClang) build of the core suites.

## Key files to understand

- `gen/cpp/config.toml` - the C++ target: naming, renames for synthesized types, import
  defaults, the render manifest (which template renders which shape, bespoke rows, boxing)
- `gen/cpp/templates/composite.cpp.tmpl` + `fields_parse.tmpl` - the strict in-order parser
  rendered from the plates' field projection
- `src/private/mx/core/generated/Document.h` - the generated public entry point (parse/serialize)
- `src/private/mxtest/corert/CoreRoundtripImpl.cpp` - the C++ roundtrip implementation
- `src/private/mxtest/import/Normalize.cpp` + `DecimalFields.h` - the normalization pipeline
- `gen/test/go/corert/roundtrip.go` - Go roundtrip implementation
- `gen/test/c/src/roundtrip.c` - C roundtrip implementation
- `src/include/mx/api/DocumentManager.h` + `src/private/mx/api/DocumentManager.cpp` - the
  api<->core seam Phase 2 rewrites (currently excluded from the build)
