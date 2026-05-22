// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Metronome.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnit.h"
#include "mx/core/elements/BeatUnitDot.h"
#include "mx/core/elements/BeatUnitGroup.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/elements/MetronomeNote.h"
#include "mx/core/elements/MetronomeRelationGroup.h"
#include "mx/core/elements/NoteRelationNote.h"
#include "mx/core/elements/PerMinute.h"
#include "mx/core/elements/PerMinuteOrBeatUnitChoice.h"
#include <iostream>

namespace mx
{
namespace core
{
Metronome::Metronome()
    : myAttributes(std::make_shared<MetronomeAttributes>()),
      myBeatUnitPerOrNoteRelationNoteChoice(std::make_shared<BeatUnitPerOrNoteRelationNoteChoice>())
{
}

bool Metronome::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &Metronome::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &Metronome::streamName(std::ostream &os) const
{
    os << "metronome";
    return os;
}

bool Metronome::hasContents() const
{
    return myBeatUnitPerOrNoteRelationNoteChoice->hasContents();
}

std::ostream &Metronome::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myBeatUnitPerOrNoteRelationNoteChoice->hasContents())
    {
        os << std::endl;
        if (myBeatUnitPerOrNoteRelationNoteChoice)
        {
            myBeatUnitPerOrNoteRelationNoteChoice->streamContents(os, indentLevel + 1, isOneLineOnly);
        }
        os << std::endl;
    }
    if (myBeatUnitPerOrNoteRelationNoteChoice->hasContents())
    {
        isOneLineOnly = false;
    }
    return os;
}

MetronomeAttributesPtr Metronome::getAttributes() const
{
    return myAttributes;
}

void Metronome::setAttributes(const MetronomeAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

BeatUnitPerOrNoteRelationNoteChoicePtr Metronome::getBeatUnitPerOrNoteRelationNoteChoice() const
{
    return myBeatUnitPerOrNoteRelationNoteChoice;
}

void Metronome::setBeatUnitPerOrNoteRelationNoteChoice(const BeatUnitPerOrNoteRelationNoteChoicePtr &value)
{
    if (value)
    {
        myBeatUnitPerOrNoteRelationNoteChoice = value;
    }
}

bool Metronome::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (it->getName() == "beat-unit" || it->getName() == "beat-unit-dot")
        {
            if (importContainerBeatUnitPer(message, it, endIter, isSuccess))
            {
                continue;
            }
        }
        if (it->getName() == "metronome-note")
        {
            if (importContainerNoteRelationNote(message, it, endIter, isSuccess))
            {
                continue;
            }
        }
    }

    MX_RETURN_IS_SUCCESS;
}

bool Metronome::importContainerBeatUnitPer(std::ostream &message, ::ezxml::XElementIterator &it,
                                           ::ezxml::XElementIterator &endIter, bool &isSuccess)
{
    if (it == endIter)
    {
        return false;
    }

    if (it->getName() != "beat-unit" && it->getName() != "beat-unit-dot")
    {
        return false;
    }

    myBeatUnitPerOrNoteRelationNoteChoice->setChoice(BeatUnitPerOrNoteRelationNoteChoice::Choice::beatUnitPer);
    auto containerPtr = myBeatUnitPerOrNoteRelationNoteChoice->getBeatUnitPer();
    bool isIterIncremented = false;

    if (it != endIter)
    {
        auto groupPtr = containerPtr->getBeatUnitGroup();
        importGroup(message, it, endIter, isSuccess, groupPtr);
        isIterIncremented = true;
        ++it;
    }

    auto perminuteorbeatunitchoice = containerPtr->getPerMinuteOrBeatUnitChoice();
    if (it != endIter && it->getName() == "per-minute")
    {
        perminuteorbeatunitchoice->setChoice(PerMinuteOrBeatUnitChoice::Choice::perMinute);
        isSuccess &= perminuteorbeatunitchoice->getPerMinute()->fromXElement(message, *it);
        isIterIncremented = true;
        ++it;
    }
    else if (it != endIter && (it->getName() == "beat-unit" || it->getName() == "beat-unit-dot"))
    {
        perminuteorbeatunitchoice->setChoice(PerMinuteOrBeatUnitChoice::Choice::beatUnitGroup);
        auto groupPtr = perminuteorbeatunitchoice->getBeatUnitGroup();
        importGroup(message, it, endIter, isSuccess, groupPtr);
        isIterIncremented = true;
        ++it;
    }

    if (isIterIncremented)
    {
        --it;
    }

    return true;
}

bool Metronome::importContainerNoteRelationNote(std::ostream &message, ::ezxml::XElementIterator &it,
                                                ::ezxml::XElementIterator &endIter, bool &isSuccess)
{
    if (it == endIter)
    {
        return false;
    }

    if (it->getName() != "metronome-note")
    {
        return false;
    }

    myBeatUnitPerOrNoteRelationNoteChoice->setChoice(BeatUnitPerOrNoteRelationNoteChoice::Choice::noteRelationNote);
    auto containerPtr = myBeatUnitPerOrNoteRelationNoteChoice->getNoteRelationNote();
    bool isIterIncremented = false;

    while (it != endIter && it->getName() == "metronome-note")
    {
        auto item = makeMetronomeNote();
        isSuccess &= item->fromXElement(message, *it);
        const auto &items = containerPtr->getMetronomeNoteSet();
        if (items.size() == 1 && !isIterIncremented)
        {
            containerPtr->addMetronomeNote(item);
            containerPtr->removeMetronomeNote(items.cbegin());
        }
        else
        {
            containerPtr->addMetronomeNote(item);
        }
        isIterIncremented = true;
        ++it;
    }

    bool hasMetronomeRelationGroup = false;
    auto metronomerelationgroup = containerPtr->getMetronomeRelationGroup();
    importGroup(message, it, endIter, isSuccess, metronomerelationgroup, hasMetronomeRelationGroup);
    if (hasMetronomeRelationGroup)
    {
        containerPtr->setHasMetronomeRelationGroup(true);
        isIterIncremented = true;
        ++it;
    }

    if (isIterIncremented)
    {
        --it;
    }

    return true;
}

} // namespace core
} // namespace mx
