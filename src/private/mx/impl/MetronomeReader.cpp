// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/MetronomeReader.h"
#include "mx/core/generated/BeatUnitGroup.h"
#include "mx/core/generated/Metronome.h"
#include "mx/core/generated/MetronomeChoice.h"
#include "mx/core/generated/MetronomeChoiceGroup.h"
#include "mx/core/generated/MetronomeChoiceGroupChoice.h"
#include "mx/core/generated/PerMinute.h"
#include "mx/impl/Converter.h"
#include "mx/utility/StringToInt.h"
#include "mx/utility/Throw.h"

namespace mx
{
namespace impl
{
MetronomeReader::MetronomeReader(MetronomeReaderParameters &&params)
    : myMutex{}, myOutTempoData{}, myMetronome{params.metronome},
      myPreviousTempoData{std::move(params.previousTempoData)}, myCursor{std::move(params.cursor)},
      myBeatUnitPerOrNoteRelationNoteChoice{myMetronome.choice()}
{
}

api::TempoData MetronomeReader::getTempoData() const
{
    std::lock_guard<std::mutex> lock{myMutex};
    myOutTempoData = api::TempoData{};
    // the old core's beatUnitPer is the new core's group; the old core's
    // noteRelationNote (metronome-note based) is group2
    using FirstChoice = core::MetronomeChoice::Kind;
    const auto firstChoice = myBeatUnitPerOrNoteRelationNoteChoice.kind();

    switch (firstChoice)
    {
    case FirstChoice::group: {
        parseBeatUnitPer();
        break;
    }
    case FirstChoice::group2: {
        parseNoteRelationNote();
        break;
    }
    default:
        break;
    }

    api::TempoData temp{std::move(myOutTempoData)};
    myOutTempoData = api::TempoData{};
    return temp;
}

void MetronomeReader::parseBeatUnitPer() const
{
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    const auto choice = beatUnitPer.choice().kind();

    switch (choice)
    {
    case core::MetronomeChoiceGroupChoice::Kind::perMinute: {
        parseBeatsPerMinute();
        break;
    }
    case core::MetronomeChoiceGroupChoice::Kind::group: {
        parseMetronomeModulation();
        break;
    }
    default:
        break;
    }
}

void MetronomeReader::parseNoteRelationNote() const
{
    MX_THROW("wtf is this");
}

void MetronomeReader::parseBeatsPerMinute() const
{
    myOutTempoData.tempoType = api::TempoType::beatsPerMinute;
    const auto &beatUnitPer = myBeatUnitPerOrNoteRelationNoteChoice.asGroup();
    const auto &grp = beatUnitPer.beatUnit();
    Converter converter;
    myOutTempoData.beatsPerMinute.durationName = converter.convert(grp.beatUnit());
    myOutTempoData.beatsPerMinute.dots = static_cast<int>(grp.beatUnitDot().size());
    const auto bpmStr = beatUnitPer.choice().asPerMinute().value();
    int bpm = -1;
    bool isNumeric = utility::stringToInt(bpmStr, bpm);
    if (!isNumeric)
    {
        myOutTempoData.tempoType = api::TempoType::tempoText;
        myOutTempoData.tempoText.tempoText = bpmStr;
    }
    else
    {
        myOutTempoData.beatsPerMinute.beatsPerMinute = bpm;
    }
}

void MetronomeReader::parseMetronomeModulation() const
{
}
} // namespace impl
} // namespace mx
