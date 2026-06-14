// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/TimeReader.h"
#include "mx/core/generated/Attributes.h"
#include "mx/core/generated/Time.h"
#include "mx/core/generated/TimeChoice.h"
#include "mx/core/generated/TimeChoiceGroup.h"
#include "mx/core/generated/TimeSignatureGroup.h"
#include "mx/utility/Throw.h"

#include <cmath>
#include <set>
#include <string>

namespace mx
{
namespace impl
{

TimeReader::TimeReader(std::span<const core::MusicDataChoice> inMusicDataChoices)
    : myMusicDataChoiceSet{inMusicDataChoices}, myTime{nullptr}, myIsTimeFound{false}, myTimeSignatureData{}
{
    myIsTimeFound = initialize();
}

bool TimeReader::getIsTimeFound() const
{
    return myIsTimeFound;
}

mx::api::TimeSignatureData TimeReader::getTimeSignatureData() const
{
    return myTimeSignatureData;
}

bool TimeReader::initialize()
{
    for (const auto &mdc : myMusicDataChoiceSet)
    {
        if (mdc.kind() == core::MusicDataChoice::Kind::attributes)
        {
            const auto &props = mdc.asAttributes();
            if (props.time().size() > 0)
            {
                myTime = &props.time().front();
                return parseTime();
            }
        }
    }
    return false;
}

bool TimeReader::parseTime()
{
    const auto &timeChoice = myTime->choice();

    if (timeChoice.kind() == core::TimeChoice::Kind::group)
    {
        const auto sigGroupSet = timeChoice.asGroup().timeSignature();
        MX_ASSERT(sigGroupSet.size() > 0);
        return parseTimeSignatureGroup(sigGroupSet.front());
    }
    else
    {
        return false;
        // MX_THROW( "TODO - other time signature stuff" );
    }
}

bool TimeReader::parseTimeSignatureGroup(const core::TimeSignatureGroup &timeSig)
{
    myTimeSignatureData.beats = timeSig.beats();
    myTimeSignatureData.beatType = timeSig.beatType();

    if (myTime->symbol().has_value())
    {
        if (*myTime->symbol() == core::TimeSymbol::common())
        {
            myTimeSignatureData.symbol = api::TimeSignatureSymbol::common;
        }
        else if (*myTime->symbol() == core::TimeSymbol::cut())
        {
            myTimeSignatureData.symbol = api::TimeSignatureSymbol::cut;
        }
        else if (*myTime->symbol() == core::TimeSymbol::singleNumber())
        {
            myTimeSignatureData.symbol = api::TimeSignatureSymbol::singleNumber;
        }
    }
    else
    {
        myTimeSignatureData.symbol = api::TimeSignatureSymbol::unspecified;
    }

    myTimeSignatureData.display = api::Bool::unspecified;
    if (myTime->printObject().has_value())
    {
        bool isPrint = *myTime->printObject() == core::YesNo::yes();

        if (isPrint)
        {
            myTimeSignatureData.display = api::Bool::yes;
        }
        else
        {
            myTimeSignatureData.display = api::Bool::no;
        }
    }

    myTimeSignatureData.isImplicit = false;

    return true;
}
} // namespace impl
} // namespace mx
