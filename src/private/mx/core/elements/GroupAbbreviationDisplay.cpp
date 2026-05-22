// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupAbbreviationDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
namespace core
{
GroupAbbreviationDisplay::GroupAbbreviationDisplay()
    : myAttributes(std::make_shared<GroupAbbreviationDisplayAttributes>()), myDisplayTextOrAccidentalTextSet()
{
}

bool GroupAbbreviationDisplay::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &GroupAbbreviationDisplay::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &GroupAbbreviationDisplay::streamName(std::ostream &os) const
{
    os << "group-abbreviation-display";
    return os;
}

bool GroupAbbreviationDisplay::hasContents() const
{
    return myDisplayTextOrAccidentalTextSet.size() > 0;
}

std::ostream &GroupAbbreviationDisplay::streamContents(std::ostream &os, const int indentLevel,
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

GroupAbbreviationDisplayAttributesPtr GroupAbbreviationDisplay::getAttributes() const
{
    return myAttributes;
}

void GroupAbbreviationDisplay::setAttributes(const GroupAbbreviationDisplayAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

const DisplayTextOrAccidentalTextSet &GroupAbbreviationDisplay::getDisplayTextOrAccidentalTextSet() const
{
    return myDisplayTextOrAccidentalTextSet;
}

void GroupAbbreviationDisplay::addDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextPtr &value)
{
    if (value)
    {
        myDisplayTextOrAccidentalTextSet.push_back(value);
    }
}

void GroupAbbreviationDisplay::removeDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextSetIterConst &value)
{
    if (value != myDisplayTextOrAccidentalTextSet.cend())
    {
        myDisplayTextOrAccidentalTextSet.erase(value);
    }
}

void GroupAbbreviationDisplay::clearDisplayTextOrAccidentalTextSet()
{
    myDisplayTextOrAccidentalTextSet.clear();
}

DisplayTextOrAccidentalTextPtr GroupAbbreviationDisplay::getDisplayTextOrAccidentalText(
    const DisplayTextOrAccidentalTextSetIterConst &setIterator) const
{
    if (setIterator != myDisplayTextOrAccidentalTextSet.cend())
    {
        return *setIterator;
    }
    return DisplayTextOrAccidentalTextPtr();
}

bool GroupAbbreviationDisplay::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
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
