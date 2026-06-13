// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/core/generated/TechnicalChoice.h"
#include "mx/impl/Cursor.h"

#include <span>

// TODO - import fingering
// TODO - import pluck
// TODO - import hammerOn
// TODO - import pullOff
// TODO - import bend
// TODO - import tap
// TODO - import hole
// TODO - import arrow
// TODO - import handbell

namespace mx
{
namespace impl
{
class TechnicalFunctions
{

  public:
    TechnicalFunctions(std::span<const core::TechnicalChoice> inTechincalChoiceSet, Cursor inCursor);

    void parseTechnicalMarks(std::vector<api::MarkData> &outMarks) const;

  private:
    std::span<const core::TechnicalChoice> myTechincalChoiceSet;
    const Cursor myCursor;

  private:
    bool parseTechicalMark(const core::TechnicalChoice &techicalChoice, api::MarkData &outMarkData) const;
};
} // namespace impl
} // namespace mx
