# mx generator (`gen/`)

A Python pipeline that reads a MusicXML XSD and emits typed
serialization/deserialization libraries. The C++ `mx::core` model
(`src/private/mx/core/generated/`) is its primary output. Secondary Go, C, and JSON Schema
targets validate language-agnosticism.

## How it works

```
XSD  ->  XSD model  ->  IR  ->  Plates  ->  press  ->  C++ / Go / C / JSON Schema
         (gen/xsd)      (gen/ir)  (gen/plates)  (gen/press)
```

Parse the XSD. Lower it to a language-agnostic IR. Project the IR onto one target as the
**Plates** (per-type metadata: identifiers in each casing, type mappings, emit strategies,
file assignment). Render the target's Mustache templates through the **press** per its
`[render]` manifest.

A **target** is a directory: `config.toml` plus `templates/`. All language knowledge lives
there. Adding a new language touches no generator Python.

## Running it

```
python3 -m gen analyze [xsd]                               # structural analysis report
python3 -m gen ir [--type NAME] [--resolve] [--config C]   # IR as JSON
python3 -m gen plates --config C [--type NAME] [--check]   # Plates as JSON; --check gates CI
python3 -m gen render --config C --type NAME               # render one type to stdout
python3 -m gen <config.toml>                               # emit the target (full run)
```

Full runs: `make gen-cpp` (C++ target), `make gen` (all targets). Generated output is
committed; `make test-gen` regenerates and asserts `git diff --exit-code`.

## For agents

Read `gen/AGENTS.md` before working on the generator. It has the hot path: commands, gates,
cardinal rules, and what must never break.

Read `gen/DESIGN.md` for the full design: goals, pipeline stage responsibilities, IR model and
glossary, Plates architecture (name model, override system, collision detection), the press
and Mustache spec, generator agnosticism rationale, and alternatives considered.
