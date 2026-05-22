// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TraditionalKey.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Cancel.h"
#include "mx/core/elements/Fifths.h"
#include "mx/core/elements/Mode.h"
#include <iostream>

namespace mx
{
namespace core
{
TraditionalKey::TraditionalKey()
    : myCancel(makeCancel()), myHasCancel(false), myFifths(makeFifths()), myMode(makeMode()), myHasMode(false)
{
}

bool TraditionalKey::hasAttributes() const
{
    return false;
}

std::ostream &TraditionalKey::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &TraditionalKey::streamName(std::ostream &os) const
{
    return os;
}

bool TraditionalKey::hasContents() const
{
    return true;
}

std::ostream &TraditionalKey::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    isOneLineOnly = false;
    bool isFirst = true;
    if (myHasCancel)
    {
        if (!isFirst)
            os << std::endl;
        myCancel->toStream(os, indentLevel);
        isFirst = false;
    }
    if (!isFirst)
        os << std::endl;
    myFifths->toStream(os, indentLevel);
    isFirst = false;
    if (myHasMode)
    {
        if (!isFirst)
            os << std::endl;
        myMode->toStream(os, indentLevel);
        isFirst = false;
    }
    return os;
}

CancelPtr TraditionalKey::getCancel() const
{
    return myCancel;
}

void TraditionalKey::setCancel(const CancelPtr &value)
{
    if (value)
    {
        myCancel = value;
    }
}

bool TraditionalKey::getHasCancel() const
{
    return myHasCancel;
}

void TraditionalKey::setHasCancel(const bool value)
{
    myHasCancel = value;
}

FifthsPtr TraditionalKey::getFifths() const
{
    return myFifths;
}

void TraditionalKey::setFifths(const FifthsPtr &value)
{
    if (value)
    {
        myFifths = value;
    }
}

ModePtr TraditionalKey::getMode() const
{
    return myMode;
}

void TraditionalKey::setMode(const ModePtr &value)
{
    if (value)
    {
        myMode = value;
    }
}

bool TraditionalKey::getHasMode() const
{
    return myHasMode;
}

void TraditionalKey::setHasMode(const bool value)
{
    myHasMode = value;
}

MX_FROM_XELEMENT_UNUSED(TraditionalKey);

} // namespace core
} // namespace mx
