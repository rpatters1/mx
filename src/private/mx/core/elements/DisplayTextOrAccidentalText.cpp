// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include <iostream>

namespace mx
{
namespace core
{
DisplayTextOrAccidentalText::DisplayTextOrAccidentalText()
    : myChoice(Choice::displayText), myDisplayText(makeDisplayText()), myAccidentalText(makeAccidentalText())
{
}

bool DisplayTextOrAccidentalText::hasAttributes() const
{
    return false;
}

std::ostream &DisplayTextOrAccidentalText::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &DisplayTextOrAccidentalText::streamName(std::ostream &os) const
{
    return os;
}

bool DisplayTextOrAccidentalText::hasContents() const
{
    return true;
}

std::ostream &DisplayTextOrAccidentalText::streamContents(std::ostream &os, const int indentLevel,
                                                          bool &isOneLineOnly) const
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

DisplayTextOrAccidentalText::Choice DisplayTextOrAccidentalText::getChoice() const
{
    return myChoice;
}

void DisplayTextOrAccidentalText::setChoice(const Choice value)
{
    myChoice = value;
}

DisplayTextPtr DisplayTextOrAccidentalText::getDisplayText() const
{
    return myDisplayText;
}

void DisplayTextOrAccidentalText::setDisplayText(const DisplayTextPtr &value)
{
    if (value)
    {
        myDisplayText = value;
    }
}

AccidentalTextPtr DisplayTextOrAccidentalText::getAccidentalText() const
{
    return myAccidentalText;
}

void DisplayTextOrAccidentalText::setAccidentalText(const AccidentalTextPtr &value)
{
    if (value)
    {
        myAccidentalText = value;
    }
}

MX_FROM_XELEMENT_UNUSED(DisplayTextOrAccidentalText);

} // namespace core
} // namespace mx
