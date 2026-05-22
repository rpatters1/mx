// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OrnamentsChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DelayedInvertedTurn.h"
#include "mx/core/elements/DelayedTurn.h"
#include "mx/core/elements/InvertedMordent.h"
#include "mx/core/elements/InvertedTurn.h"
#include "mx/core/elements/Mordent.h"
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
OrnamentsChoice::OrnamentsChoice()
    : myChoice(Choice::trillMark), myTrillMark(makeTrillMark()), myTurn(makeTurn()), myDelayedTurn(makeDelayedTurn()),
      myInvertedTurn(makeInvertedTurn()), myDelayedInvertedTurn(makeDelayedInvertedTurn()),
      myVerticalTurn(makeVerticalTurn()), myShake(makeShake()), myWavyLine(makeWavyLine()), myMordent(makeMordent()),
      myInvertedMordent(makeInvertedMordent()), mySchleifer(makeSchleifer()), myTremolo(makeTremolo()),
      myOtherOrnament(makeOtherOrnament())
{
}

bool OrnamentsChoice::hasAttributes() const
{
    return false;
}

std::ostream &OrnamentsChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &OrnamentsChoice::streamName(std::ostream &os) const
{
    return os;
}

bool OrnamentsChoice::hasContents() const
{
    return true;
}

std::ostream &OrnamentsChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myChoice == Choice::trillMark)
    {
        myTrillMark->toStream(os, indentLevel);
    }
    if (myChoice == Choice::turn)
    {
        myTurn->toStream(os, indentLevel);
    }
    if (myChoice == Choice::delayedTurn)
    {
        myDelayedTurn->toStream(os, indentLevel);
    }
    if (myChoice == Choice::invertedTurn)
    {
        myInvertedTurn->toStream(os, indentLevel);
    }
    if (myChoice == Choice::delayedInvertedTurn)
    {
        myDelayedInvertedTurn->toStream(os, indentLevel);
    }
    if (myChoice == Choice::verticalTurn)
    {
        myVerticalTurn->toStream(os, indentLevel);
    }
    if (myChoice == Choice::shake)
    {
        myShake->toStream(os, indentLevel);
    }
    if (myChoice == Choice::wavyLine)
    {
        myWavyLine->toStream(os, indentLevel);
    }
    if (myChoice == Choice::mordent)
    {
        myMordent->toStream(os, indentLevel);
    }
    if (myChoice == Choice::invertedMordent)
    {
        myInvertedMordent->toStream(os, indentLevel);
    }
    if (myChoice == Choice::schleifer)
    {
        mySchleifer->toStream(os, indentLevel);
    }
    if (myChoice == Choice::tremolo)
    {
        myTremolo->toStream(os, indentLevel);
    }
    if (myChoice == Choice::otherOrnament)
    {
        myOtherOrnament->toStream(os, indentLevel);
    }
    isOneLineOnly = false;
    return os;
}

OrnamentsChoice::Choice OrnamentsChoice::getChoice() const
{
    return myChoice;
}

void OrnamentsChoice::setChoice(const Choice value)
{
    myChoice = value;
}

TrillMarkPtr OrnamentsChoice::getTrillMark() const
{
    return myTrillMark;
}

void OrnamentsChoice::setTrillMark(const TrillMarkPtr &value)
{
    if (value)
    {
        myTrillMark = value;
    }
}

TurnPtr OrnamentsChoice::getTurn() const
{
    return myTurn;
}

void OrnamentsChoice::setTurn(const TurnPtr &value)
{
    if (value)
    {
        myTurn = value;
    }
}

DelayedTurnPtr OrnamentsChoice::getDelayedTurn() const
{
    return myDelayedTurn;
}

void OrnamentsChoice::setDelayedTurn(const DelayedTurnPtr &value)
{
    if (value)
    {
        myDelayedTurn = value;
    }
}

InvertedTurnPtr OrnamentsChoice::getInvertedTurn() const
{
    return myInvertedTurn;
}

void OrnamentsChoice::setInvertedTurn(const InvertedTurnPtr &value)
{
    if (value)
    {
        myInvertedTurn = value;
    }
}

DelayedInvertedTurnPtr OrnamentsChoice::getDelayedInvertedTurn() const
{
    return myDelayedInvertedTurn;
}

void OrnamentsChoice::setDelayedInvertedTurn(const DelayedInvertedTurnPtr &value)
{
    if (value)
    {
        myDelayedInvertedTurn = value;
    }
}

VerticalTurnPtr OrnamentsChoice::getVerticalTurn() const
{
    return myVerticalTurn;
}

void OrnamentsChoice::setVerticalTurn(const VerticalTurnPtr &value)
{
    if (value)
    {
        myVerticalTurn = value;
    }
}

ShakePtr OrnamentsChoice::getShake() const
{
    return myShake;
}

void OrnamentsChoice::setShake(const ShakePtr &value)
{
    if (value)
    {
        myShake = value;
    }
}

WavyLinePtr OrnamentsChoice::getWavyLine() const
{
    return myWavyLine;
}

void OrnamentsChoice::setWavyLine(const WavyLinePtr &value)
{
    if (value)
    {
        myWavyLine = value;
    }
}

MordentPtr OrnamentsChoice::getMordent() const
{
    return myMordent;
}

void OrnamentsChoice::setMordent(const MordentPtr &value)
{
    if (value)
    {
        myMordent = value;
    }
}

InvertedMordentPtr OrnamentsChoice::getInvertedMordent() const
{
    return myInvertedMordent;
}

void OrnamentsChoice::setInvertedMordent(const InvertedMordentPtr &value)
{
    if (value)
    {
        myInvertedMordent = value;
    }
}

SchleiferPtr OrnamentsChoice::getSchleifer() const
{
    return mySchleifer;
}

void OrnamentsChoice::setSchleifer(const SchleiferPtr &value)
{
    if (value)
    {
        mySchleifer = value;
    }
}

TremoloPtr OrnamentsChoice::getTremolo() const
{
    return myTremolo;
}

void OrnamentsChoice::setTremolo(const TremoloPtr &value)
{
    if (value)
    {
        myTremolo = value;
    }
}

OtherOrnamentPtr OrnamentsChoice::getOtherOrnament() const
{
    return myOtherOrnament;
}

void OrnamentsChoice::setOtherOrnament(const OtherOrnamentPtr &value)
{
    if (value)
    {
        myOtherOrnament = value;
    }
}

MX_FROM_XELEMENT_UNUSED(OrnamentsChoice);

} // namespace core
} // namespace mx
