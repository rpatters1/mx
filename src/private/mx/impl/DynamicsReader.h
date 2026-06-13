// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/impl/Cursor.h"

#include <string>

namespace mx
{
namespace core
{
class Dynamics;
} // namespace core

namespace impl
{

/// Returns the MusicXML element-name string for a dynamics kind
/// (e.g. Kind::mf → "mf", Kind::otherDynamics → "other-dynamics").
/// Exposed here so test-data helpers can use it without duplicating
/// the mapping.
inline std::string dynamicsKindToName(core::DynamicsChoice::Kind kind)
{
    using K = core::DynamicsChoice::Kind;
    switch (kind)
    {
    case K::p:
        return "p";
    case K::pp:
        return "pp";
    case K::ppp:
        return "ppp";
    case K::pppp:
        return "pppp";
    case K::ppppp:
        return "ppppp";
    case K::pppppp:
        return "pppppp";
    case K::f:
        return "f";
    case K::ff:
        return "ff";
    case K::fff:
        return "fff";
    case K::ffff:
        return "ffff";
    case K::fffff:
        return "fffff";
    case K::ffffff:
        return "ffffff";
    case K::mp:
        return "mp";
    case K::mf:
        return "mf";
    case K::sf:
        return "sf";
    case K::sfp:
        return "sfp";
    case K::sfpp:
        return "sfpp";
    case K::fp:
        return "fp";
    case K::rf:
        return "rf";
    case K::rfz:
        return "rfz";
    case K::sfz:
        return "sfz";
    case K::sffz:
        return "sffz";
    case K::fz:
        return "fz";
    case K::n:
        return "n";
    case K::pf:
        return "pf";
    case K::sfzp:
        return "sfzp";
    case K::otherDynamics:
        return "other-dynamics";
    default:
        return "p";
    }
}

class DynamicsReader
{
  public:
    DynamicsReader(const core::Dynamics &inDynamic, impl::Cursor inCursor);
    ~DynamicsReader() = default;
    DynamicsReader(const DynamicsReader &) = delete;
    DynamicsReader(DynamicsReader &&) = delete;
    DynamicsReader &operator=(const DynamicsReader &) = delete;
    DynamicsReader &operator=(DynamicsReader &&) = delete;

    void parseDynamics(std::vector<api::MarkData> &outMarks) const;

  private:
    const core::Dynamics &myDynamic;
    const impl::Cursor myCursor;
};
} // namespace impl
} // namespace mx
