// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Ornaments.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalMark.h"
#include "mx/core/elements/DelayedInvertedTurn.h"
#include "mx/core/elements/DelayedTurn.h"
#include "mx/core/elements/InvertedMordent.h"
#include "mx/core/elements/InvertedTurn.h"
#include "mx/core/elements/Mordent.h"
#include "mx/core/elements/OrnamentsChoice.h"
#include "mx/core/elements/OtherOrnament.h"
#include "mx/core/elements/Schleifer.h"
#include "mx/core/elements/Shake.h"
#include "mx/core/elements/Tremolo.h"
#include "mx/core/elements/TrillMark.h"
#include "mx/core/elements/Turn.h"
#include "mx/core/elements/VerticalTurn.h"
#include "mx/core/elements/WavyLine.h"
#include <iostream>

namespace mx
{
namespace core
{
Ornaments::Ornaments() : myOrnamentsChoiceSet(), myAccidentalMarkSet()
{
}

bool Ornaments::hasAttributes() const
{
    return false;
}

std::ostream &Ornaments::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &Ornaments::streamName(std::ostream &os) const
{
    os << "ornaments";
    return os;
}

bool Ornaments::hasContents() const
{
    return myOrnamentsChoiceSet.size() > 0 || myAccidentalMarkSet.size() > 0;
}

std::ostream &Ornaments::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    for (auto x : myOrnamentsChoiceSet)
    {
        os << std::endl;
        x->streamContents(os, indentLevel + 1, isOneLineOnly);
    }
    for (auto x : myAccidentalMarkSet)
    {
        os << std::endl;
        x->toStream(os, indentLevel + 1);
    }
    isOneLineOnly = false;
    os << std::endl;
    return os;
}

const OrnamentsChoiceSet &Ornaments::getOrnamentsChoiceSet() const
{
    return myOrnamentsChoiceSet;
}

void Ornaments::addOrnamentsChoice(const OrnamentsChoicePtr &value)
{
    if (value)
    {
        myOrnamentsChoiceSet.push_back(value);
    }
}

void Ornaments::removeOrnamentsChoice(const OrnamentsChoiceSetIterConst &value)
{
    if (value != myOrnamentsChoiceSet.cend())
    {
        myOrnamentsChoiceSet.erase(value);
    }
}

void Ornaments::clearOrnamentsChoiceSet()
{
    myOrnamentsChoiceSet.clear();
}

OrnamentsChoicePtr Ornaments::getOrnamentsChoice(const OrnamentsChoiceSetIterConst &setIterator) const
{
    if (setIterator != myOrnamentsChoiceSet.cend())
    {
        return *setIterator;
    }
    return OrnamentsChoicePtr();
}

const AccidentalMarkSet &Ornaments::getAccidentalMarkSet() const
{
    return myAccidentalMarkSet;
}

void Ornaments::addAccidentalMark(const AccidentalMarkPtr &value)
{
    if (value)
    {
        myAccidentalMarkSet.push_back(value);
    }
}

void Ornaments::removeAccidentalMark(const AccidentalMarkSetIterConst &value)
{
    if (value != myAccidentalMarkSet.cend())
    {
        myAccidentalMarkSet.erase(value);
    }
}

void Ornaments::clearAccidentalMarkSet()
{
    myAccidentalMarkSet.clear();
}

AccidentalMarkPtr Ornaments::getAccidentalMark(const AccidentalMarkSetIterConst &setIterator) const
{
    if (setIterator != myAccidentalMarkSet.cend())
    {
        return *setIterator;
    }
    return AccidentalMarkPtr();
}

bool Ornaments::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (it->getName() == "trill-mark")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::trillMark);
            isSuccess &= choice->getTrillMark()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "turn")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::turn);
            isSuccess &= choice->getTurn()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "delayed-turn")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::delayedTurn);
            isSuccess &= choice->getDelayedTurn()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "inverted-turn")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::invertedTurn);
            isSuccess &= choice->getInvertedTurn()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "delayed-inverted-turn")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::delayedInvertedTurn);
            isSuccess &= choice->getDelayedInvertedTurn()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "vertical-turn")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::verticalTurn);
            isSuccess &= choice->getVerticalTurn()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "shake")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::shake);
            isSuccess &= choice->getShake()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "wavy-line")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::wavyLine);
            isSuccess &= choice->getWavyLine()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "mordent")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::mordent);
            isSuccess &= choice->getMordent()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "inverted-mordent")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::invertedMordent);
            isSuccess &= choice->getInvertedMordent()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "schleifer")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::schleifer);
            isSuccess &= choice->getSchleifer()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "tremolo")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::tremolo);
            isSuccess &= choice->getTremolo()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (it->getName() == "other-ornament")
        {
            auto choice = makeOrnamentsChoice();
            choice->setChoice(OrnamentsChoice::Choice::otherOrnament);
            isSuccess &= choice->getOtherOrnament()->fromXElement(message, *it);
            myOrnamentsChoiceSet.push_back(choice);
            continue;
        }
        if (importElementSet(message, it, endIter, isSuccess, "accidental-mark", myAccidentalMarkSet))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
