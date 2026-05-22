// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include <iostream>

namespace mx
{
namespace core
{
BeatUnitGroup::BeatUnitGroup() : myBeatUnit(makeBeatUnit()), myBeatUnitDotSet()
{
}

bool BeatUnitGroup::hasAttributes() const
{
    return false;
}

std::ostream &BeatUnitGroup::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &BeatUnitGroup::streamName(std::ostream &os) const
{
    return os;
}

bool BeatUnitGroup::hasContents() const
{
    return true;
}

std::ostream &BeatUnitGroup::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    bool isFirst = true;
    if (!isFirst)
        os << std::endl;
    myBeatUnit->toStream(os, indentLevel);
    isFirst = false;
    for (auto x : myBeatUnitDotSet)
    {
        if (!isFirst)
            os << std::endl;
        x->toStream(os, indentLevel);
        isFirst = false;
    }
    isOneLineOnly = !hasContents();
    return os;
}

BeatUnitPtr BeatUnitGroup::getBeatUnit() const
{
    return myBeatUnit;
}

void BeatUnitGroup::setBeatUnit(const BeatUnitPtr &value)
{
    if (value)
    {
        myBeatUnit = value;
    }
}

const BeatUnitDotSet &BeatUnitGroup::getBeatUnitDotSet() const
{
    return myBeatUnitDotSet;
}

void BeatUnitGroup::removeBeatUnitDot(const BeatUnitDotSetIterConst &value)
{
    if (value != myBeatUnitDotSet.cend())
    {
        myBeatUnitDotSet.erase(value);
    }
}

void BeatUnitGroup::addBeatUnitDot(const BeatUnitDotPtr &value)
{
    if (value)
    {
        myBeatUnitDotSet.push_back(value);
    }
}

void BeatUnitGroup::clearBeatUnitDotSet()
{
    myBeatUnitDotSet.clear();
}

BeatUnitDotPtr BeatUnitGroup::getBeatUnitDot(const BeatUnitDotSetIterConst &setIterator) const
{
    if (setIterator != myBeatUnitDotSet.cend())
    {
        return *setIterator;
    }
    return BeatUnitDotPtr();
}

MX_FROM_XELEMENT_UNUSED(BeatUnitGroup);

} // namespace core
} // namespace mx
