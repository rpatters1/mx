// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TimeSignatureData.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/impl/Cursor.h"

#include <span>

namespace mx
{
namespace impl
{
class TimeReader
{
  public:
    // The constructor will do all of the parsing
    // after that you can query the discovered time
    // element properties using accessors on the
    // cached data
    TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices);
    bool getIsTimeFound() const;
    mx::api::TimeSignatureData getTimeSignatureData() const;

  private:
    std::span<const core::MusicDataChoice> myMusicDataChoiceSet;
    const core::Time *myTime;
    bool myIsTimeFound;
    mx::api::TimeSignatureData myTimeSignatureData;

  private:
    bool initialize();
    bool parseTime();
    bool parseTimeSignatureGroup(const core::TimeSignatureGroup &timeSig);
};
} // namespace impl
} // namespace mx
