// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PartAbbreviationDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
namespace core
{
PartAbbreviationDisplay::PartAbbreviationDisplay()
    : myAttributes(std::make_shared<PartAbbreviationDisplayAttributes>()), myDisplayTextOrAccidentalTextSet()
{
}

bool PartAbbreviationDisplay::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &PartAbbreviationDisplay::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &PartAbbreviationDisplay::streamName(std::ostream &os) const
{
    os << "part-abbreviation-display";
    return os;
}

bool PartAbbreviationDisplay::hasContents() const
{
    return myDisplayTextOrAccidentalTextSet.size() > 0;
}

std::ostream &PartAbbreviationDisplay::streamContents(std::ostream &os, const int indentLevel,
                                                      bool &isOneLineOnly) const
{
    for (auto x : myDisplayTextOrAccidentalTextSet)
    {
        os << std::endl;
        x->streamContents(os, indentLevel + 1, isOneLineOnly);
    }
    isOneLineOnly = false;
    os << std::endl;
    return os;
}

PartAbbreviationDisplayAttributesPtr PartAbbreviationDisplay::getAttributes() const
{
    return myAttributes;
}

void PartAbbreviationDisplay::setAttributes(const PartAbbreviationDisplayAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

const DisplayTextOrAccidentalTextSet &PartAbbreviationDisplay::getDisplayTextOrAccidentalTextSet() const
{
    return myDisplayTextOrAccidentalTextSet;
}

void PartAbbreviationDisplay::addDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextPtr &value)
{
    if (value)
    {
        myDisplayTextOrAccidentalTextSet.push_back(value);
    }
}

void PartAbbreviationDisplay::removeDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextSetIterConst &value)
{
    if (value != myDisplayTextOrAccidentalTextSet.cend())
    {
        myDisplayTextOrAccidentalTextSet.erase(value);
    }
}

void PartAbbreviationDisplay::clearDisplayTextOrAccidentalTextSet()
{
    myDisplayTextOrAccidentalTextSet.clear();
}

DisplayTextOrAccidentalTextPtr PartAbbreviationDisplay::getDisplayTextOrAccidentalText(
    const DisplayTextOrAccidentalTextSetIterConst &setIterator) const
{
    if (setIterator != myDisplayTextOrAccidentalTextSet.cend())
    {
        return *setIterator;
    }
    return DisplayTextOrAccidentalTextPtr();
}

bool PartAbbreviationDisplay::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (it->getName() == "display-text")
        {
            auto choice = makeDisplayTextOrAccidentalText();
            choice->setChoice(DisplayTextOrAccidentalText::Choice::displayText);
            isSuccess &= choice->getDisplayText()->fromXElement(message, *it);
            myDisplayTextOrAccidentalTextSet.push_back(choice);
            continue;
        }
        if (it->getName() == "accidental-text")
        {
            auto choice = makeDisplayTextOrAccidentalText();
            choice->setChoice(DisplayTextOrAccidentalText::Choice::accidentalText);
            isSuccess &= choice->getAccidentalText()->fromXElement(message, *it);
            myDisplayTextOrAccidentalTextSet.push_back(choice);
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
