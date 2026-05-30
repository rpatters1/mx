// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "ezxml/XDoc.h"

#include <string>
#include <vector>

namespace mxtest
{
namespace corert
{

// One substitution rule deserialized from a `<replace>` child in a `.fixup.xml` sidecar.
// A rule matches when both `name` and `value` are exact-equal to the node being visited.
struct Fixup
{
    enum class Type
    {
        Element,
        Attribute,
    };

    Type type = Type::Element;
    std::string name;
    std::string value;
    std::string replacementValue;
};

// Loads a `.fixup.xml` sidecar (if any) for a given input file and applies its
// substitution rules to the *expected* XDoc before the round-trip comparison.
//
// ## Why this exists
//
// mx silently clamps out-of-bounds values on import (e.g. MIDI channel 0 -> 1,
// dynamics -1.11 -> 0). When a corpus file contains such a value the round-trip
// comparison fails even though mx handled it correctly. Rather than marking the
// file `.invalid` (it is a valid file that mx processes correctly), a sidecar
// patches the expected tree to match mx's clamped output.
//
// ## Sidecar path
//
// `data/foo/example.xml` -> `data/foo/example.fixup.xml`
// (the `.xml` / `.musicxml` extension is replaced by `.fixup.xml`)
//
// ## Sidecar format
//
//   <fixups>
//     <replace>
//       <type>element</type>          <!-- "element" or "attribute" -->
//       <name>midi-channel</name>
//       <value>0</value>              <!-- exact string to match -->
//       <replacement-value>1</replacement-value>
//     </replace>
//     ...
//   </fixups>
//
// A file may contain any number of `<replace>` entries. Matching is exact string
// equality on both name and value so the same element name with different
// out-of-bounds values can be fixed independently.
//
// ## Invariants
//
// - Fixups apply to the expected tree only; the actual (round-tripped) tree is
//   never modified.
// - Files without a `.fixup.xml` sibling are unaffected; a Fixer with no fixups
//   is a no-op.
// - Fixups do not add or remove nodes; they only replace scalar values.
// - Empty-element leaves (`XElementType::empty`, e.g. `<fret/>`) are matched
//   only when the fixup's own `value` is also empty, to avoid spuriously
//   rewriting intentionally-empty marker elements (e.g. `<fermata/>`) that share
//   a name with a clamped text leaf.
class Fixer
{
  public:
    explicit Fixer(const std::string &inputFilePath);

    bool hasFixups() const;
    void applyToExpected(const ::ezxml::XDocPtr &expectedXDoc) const;

  private:
    std::vector<Fixup> myFixups;
};

} // namespace corert
} // namespace mxtest
