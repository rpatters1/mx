// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/TempoData.h"
#include "mx/impl/Cursor.h"

#include <mutex>

namespace mx
{
namespace core
{
class Metronome;
class MetronomeChoice;
} // namespace core

namespace impl
{
struct MetronomeReaderParameters
{
    const core::Metronome &metronome;
    api::TempoData previousTempoData;
    Cursor cursor;

    MetronomeReaderParameters(const core::Metronome &inMetronome)
        : metronome{inMetronome}, previousTempoData{}, cursor{1, 1}
    {
    }
};

class MetronomeReader
{
  public:
    explicit MetronomeReader(MetronomeReaderParameters &&params);
    ~MetronomeReader() = default;
    MetronomeReader(const MetronomeReader &) = delete;
    MetronomeReader(MetronomeReader &&) = delete;
    MetronomeReader &operator=(const MetronomeReader &) = delete;
    MetronomeReader &operator=(MetronomeReader &&) = delete;

    api::TempoData getTempoData() const;

  private:
    mutable std::mutex myMutex;
    mutable api::TempoData myOutTempoData;
    const core::Metronome &myMetronome;
    const api::TempoData myPreviousTempoData;
    const Cursor myCursor;
    // the old core's BeatUnitPerOrNoteRelationNoteChoice
    const core::MetronomeChoice &myBeatUnitPerOrNoteRelationNoteChoice;

  private:
    void parseBeatUnitPer() const;
    void parseNoteRelationNote() const;
    void parseBeatsPerMinute() const;
    void parseMetronomeModulation() const;
};
} // namespace impl
} // namespace mx
