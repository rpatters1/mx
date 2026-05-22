// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NoteheadTextChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include <iostream>

namespace mx
{
namespace core
{
NoteheadTextChoice::NoteheadTextChoice()
    : myChoice(Choice::displayText), myDisplayText(makeDisplayText()), myAccidentalText(makeAccidentalText())
{
}

bool NoteheadTextChoice::hasAttributes() const
{
    return false;
}

std::ostream &NoteheadTextChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &NoteheadTextChoice::streamName(std::ostream &os) const
{
    return os;
}

bool NoteheadTextChoice::hasContents() const
{
    return true;
}

std::ostream &NoteheadTextChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myChoice == Choice::displayText)
    {
        myDisplayText->toStream(os, indentLevel);
    }
    if (myChoice == Choice::accidentalText)
    {
        myAccidentalText->toStream(os, indentLevel);
    }
    isOneLineOnly = false;
    return os;
}

NoteheadTextChoice::Choice NoteheadTextChoice::getChoice() const
{
    return myChoice;
}

void NoteheadTextChoice::setChoice(const Choice value)
{
    myChoice = value;
}

DisplayTextPtr NoteheadTextChoice::getDisplayText() const
{
    return myDisplayText;
}

void NoteheadTextChoice::setDisplayText(const DisplayTextPtr &value)
{
    if (value)
    {
        myDisplayText = value;
    }
}

AccidentalTextPtr NoteheadTextChoice::getAccidentalText() const
{
    return myAccidentalText;
}

void NoteheadTextChoice::setAccidentalText(const AccidentalTextPtr &value)
{
    if (value)
    {
        myAccidentalText = value;
    }
}

MX_FROM_XELEMENT_UNUSED(NoteheadTextChoice);

} // namespace core
} // namespace mx
