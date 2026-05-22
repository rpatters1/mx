// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/PerMinute.h"
#include <iostream>

namespace mx
{
namespace core
{
PerMinuteOrBeatUnitChoice::PerMinuteOrBeatUnitChoice()
    : myChoice(Choice::perMinute), myPerMinute(makePerMinute()), myBeatUnitGroup(makeBeatUnitGroup())
{
}

bool PerMinuteOrBeatUnitChoice::hasAttributes() const
{
    return false;
}

std::ostream &PerMinuteOrBeatUnitChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &PerMinuteOrBeatUnitChoice::streamName(std::ostream &os) const
{
    return os;
}

bool PerMinuteOrBeatUnitChoice::hasContents() const
{
    return true;
}

std::ostream &PerMinuteOrBeatUnitChoice::streamContents(std::ostream &os, const int indentLevel,
                                                        bool &isOneLineOnly) const
{
    if (myChoice == Choice::perMinute)
    {
        myPerMinute->toStream(os, indentLevel);
    }
    if (myChoice == Choice::beatUnitGroup)
    {
        if (myBeatUnitGroup)
        {
            myBeatUnitGroup->streamContents(os, indentLevel, isOneLineOnly);
        }
    }
    isOneLineOnly = false;
    return os;
}

PerMinuteOrBeatUnitChoice::Choice PerMinuteOrBeatUnitChoice::getChoice() const
{
    return myChoice;
}

void PerMinuteOrBeatUnitChoice::setChoice(const Choice value)
{
    myChoice = value;
}

PerMinutePtr PerMinuteOrBeatUnitChoice::getPerMinute() const
{
    return myPerMinute;
}

void PerMinuteOrBeatUnitChoice::setPerMinute(const PerMinutePtr &value)
{
    if (value)
    {
        myPerMinute = value;
    }
}

BeatUnitGroupPtr PerMinuteOrBeatUnitChoice::getBeatUnitGroup() const
{
    return myBeatUnitGroup;
}

void PerMinuteOrBeatUnitChoice::setBeatUnitGroup(const BeatUnitGroupPtr &value)
{
    if (value)
    {
        myBeatUnitGroup = value;
    }
}

MX_FROM_XELEMENT_UNUSED(PerMinuteOrBeatUnitChoice);

} // namespace core
} // namespace mx
