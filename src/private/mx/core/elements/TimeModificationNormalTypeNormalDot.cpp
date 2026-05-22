// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TimeModificationNormalTypeNormalDot.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/NormalDot.h"
#include "mx/core/elements/NormalType.h"
#include <iostream>

namespace mx
{
namespace core
{
TimeModificationNormalTypeNormalDot::TimeModificationNormalTypeNormalDot()
    : myNormalType(makeNormalType()), myNormalDotSet()
{
}

bool TimeModificationNormalTypeNormalDot::hasAttributes() const
{
    return false;
}

std::ostream &TimeModificationNormalTypeNormalDot::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &TimeModificationNormalTypeNormalDot::streamName(std::ostream &os) const
{
    return os;
}

bool TimeModificationNormalTypeNormalDot::hasContents() const
{
    return true;
}

std::ostream &TimeModificationNormalTypeNormalDot::streamContents(std::ostream &os, const int indentLevel,
                                                                  bool &isOneLineOnly) const
{
    bool isFirst = true;
    if (!isFirst)
        os << std::endl;
    myNormalType->toStream(os, indentLevel);
    isFirst = false;
    for (auto x : myNormalDotSet)
    {
        if (!isFirst)
            os << std::endl;
        x->toStream(os, indentLevel);
        isFirst = false;
    }
    isOneLineOnly = !hasContents();
    return os;
}

NormalTypePtr TimeModificationNormalTypeNormalDot::getNormalType() const
{
    return myNormalType;
}

void TimeModificationNormalTypeNormalDot::setNormalType(const NormalTypePtr &value)
{
    if (value)
    {
        myNormalType = value;
    }
}

const NormalDotSet &TimeModificationNormalTypeNormalDot::getNormalDotSet() const
{
    return myNormalDotSet;
}

void TimeModificationNormalTypeNormalDot::removeNormalDot(const NormalDotSetIterConst &value)
{
    if (value != myNormalDotSet.cend())
    {
        myNormalDotSet.erase(value);
    }
}

void TimeModificationNormalTypeNormalDot::addNormalDot(const NormalDotPtr &value)
{
    if (value)
    {
        myNormalDotSet.push_back(value);
    }
}

void TimeModificationNormalTypeNormalDot::clearNormalDotSet()
{
    myNormalDotSet.clear();
}

NormalDotPtr TimeModificationNormalTypeNormalDot::getNormalDot(const NormalDotSetIterConst &setIterator) const
{
    if (setIterator != myNormalDotSet.cend())
    {
        return *setIterator;
    }
    return NormalDotPtr();
}

bool TimeModificationNormalTypeNormalDot::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    bool isNormalTypeFound = false;
    bool isFirstNormalDotAdded = false;
    for (auto it = xelement.begin(); it != xelement.end(); ++it)
    {
        const std::string elementName = it->getName();

        if (elementName == "normal-type")
        {
            isNormalTypeFound = true;
            isSuccess &= myNormalType->fromXElement(message, *it);
        }
        else if (elementName == "normal-dot")
        {
            auto normalDot = makeNormalDot();
            isSuccess &= normalDot->fromXElement(message, *it);

            if (!isFirstNormalDotAdded && myNormalDotSet.size() == 1)
            {
                *(myNormalDotSet.begin()) = normalDot;
                isFirstNormalDotAdded = true;
            }
            else
            {
                myNormalDotSet.push_back(normalDot);
                isFirstNormalDotAdded = true;
            }
        }
        else
        {
            if (!isNormalTypeFound)
            {
                message << "TimeModificationNormalTypeNormalDot: a 'normal-type' element is required but was not found"
                        << std::endl;
                return false;
            }
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
