// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/GroupNameDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
namespace core
{
GroupNameDisplay::GroupNameDisplay()
    : myAttributes(std::make_shared<GroupNameDisplayAttributes>()), myDisplayTextOrAccidentalTextSet()
{
}

bool GroupNameDisplay::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &GroupNameDisplay::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &GroupNameDisplay::streamName(std::ostream &os) const
{
    os << "group-name-display";
    return os;
}

bool GroupNameDisplay::hasContents() const
{
    return myDisplayTextOrAccidentalTextSet.size() > 0;
}

std::ostream &GroupNameDisplay::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
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

GroupNameDisplayAttributesPtr GroupNameDisplay::getAttributes() const
{
    return myAttributes;
}

void GroupNameDisplay::setAttributes(const GroupNameDisplayAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

const DisplayTextOrAccidentalTextSet &GroupNameDisplay::getDisplayTextOrAccidentalTextSet() const
{
    return myDisplayTextOrAccidentalTextSet;
}

void GroupNameDisplay::addDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextPtr &value)
{
    if (value)
    {
        myDisplayTextOrAccidentalTextSet.push_back(value);
    }
}

void GroupNameDisplay::removeDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextSetIterConst &value)
{
    if (value != myDisplayTextOrAccidentalTextSet.cend())
    {
        myDisplayTextOrAccidentalTextSet.erase(value);
    }
}

void GroupNameDisplay::clearDisplayTextOrAccidentalTextSet()
{
    myDisplayTextOrAccidentalTextSet.clear();
}

DisplayTextOrAccidentalTextPtr GroupNameDisplay::getDisplayTextOrAccidentalText(
    const DisplayTextOrAccidentalTextSetIterConst &setIterator) const
{
    if (setIterator != myDisplayTextOrAccidentalTextSet.cend())
    {
        return *setIterator;
    }
    return DisplayTextOrAccidentalTextPtr();
}

bool GroupNameDisplay::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
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
