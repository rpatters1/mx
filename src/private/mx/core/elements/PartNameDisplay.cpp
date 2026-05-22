// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/PartNameDisplay.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/DisplayTextOrAccidentalText.h"
#include <iostream>

namespace mx
{
namespace core
{
PartNameDisplay::PartNameDisplay()
    : myAttributes(std::make_shared<PartNameDisplayAttributes>()), myDisplayTextOrAccidentalTextSet()
{
}

bool PartNameDisplay::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &PartNameDisplay::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &PartNameDisplay::streamName(std::ostream &os) const
{
    os << "part-name-display";
    return os;
}

bool PartNameDisplay::hasContents() const
{
    return true;
}

std::ostream &PartNameDisplay::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myDisplayTextOrAccidentalTextSet.size() > 0)
    {
        for (auto x : myDisplayTextOrAccidentalTextSet)
        {
            os << std::endl;
            x->streamContents(os, indentLevel + 1, isOneLineOnly);
        }
        isOneLineOnly = false;
        os << std::endl;
    }
    else
    {
        isOneLineOnly = true;
    }
    return os;
}

PartNameDisplayAttributesPtr PartNameDisplay::getAttributes() const
{
    return myAttributes;
}

void PartNameDisplay::setAttributes(const PartNameDisplayAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

const DisplayTextOrAccidentalTextSet &PartNameDisplay::getDisplayTextOrAccidentalTextSet() const
{
    return myDisplayTextOrAccidentalTextSet;
}

void PartNameDisplay::addDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextPtr &value)
{
    if (value)
    {
        myDisplayTextOrAccidentalTextSet.push_back(value);
    }
}

void PartNameDisplay::removeDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextSetIterConst &value)
{
    if (value != myDisplayTextOrAccidentalTextSet.cend())
    {
        myDisplayTextOrAccidentalTextSet.erase(value);
    }
}

void PartNameDisplay::clearDisplayTextOrAccidentalTextSet()
{
    myDisplayTextOrAccidentalTextSet.clear();
}

DisplayTextOrAccidentalTextPtr PartNameDisplay::getDisplayTextOrAccidentalText(
    const DisplayTextOrAccidentalTextSetIterConst &setIterator) const
{
    if (setIterator != myDisplayTextOrAccidentalTextSet.cend())
    {
        return *setIterator;
    }
    return DisplayTextOrAccidentalTextPtr();
}

bool PartNameDisplay::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
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
