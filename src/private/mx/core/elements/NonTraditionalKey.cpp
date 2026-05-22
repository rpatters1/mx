// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NonTraditionalKey.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/KeyAccidental.h"
#include "mx/core/elements/KeyAlter.h"
#include "mx/core/elements/KeyStep.h"
#include <iostream>

namespace mx
{
namespace core
{
NonTraditionalKey::NonTraditionalKey()
    : myKeyStep(makeKeyStep()), myKeyAlter(makeKeyAlter()), myKeyAccidental(makeKeyAccidental()),
      myHasKeyAccidental(false)
{
}

bool NonTraditionalKey::hasAttributes() const
{
    return false;
}

std::ostream &NonTraditionalKey::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &NonTraditionalKey::streamName(std::ostream &os) const
{
    return os;
}

bool NonTraditionalKey::hasContents() const
{
    return true;
}

std::ostream &NonTraditionalKey::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    isOneLineOnly = false;
    bool isFirst = true;
    if (!isFirst)
        os << std::endl;
    myKeyStep->toStream(os, indentLevel);
    isFirst = false;
    if (!isFirst)
        os << std::endl;
    myKeyAlter->toStream(os, indentLevel);
    isFirst = false;
    if (myHasKeyAccidental)
    {
        if (!isFirst)
            os << std::endl;
        myKeyAccidental->toStream(os, indentLevel);
        isFirst = false;
    }
    return os;
}

KeyStepPtr NonTraditionalKey::getKeyStep() const
{
    return myKeyStep;
}

void NonTraditionalKey::setKeyStep(const KeyStepPtr &value)
{
    if (value)
    {
        myKeyStep = value;
    }
}

KeyAlterPtr NonTraditionalKey::getKeyAlter() const
{
    return myKeyAlter;
}

void NonTraditionalKey::setKeyAlter(const KeyAlterPtr &value)
{
    if (value)
    {
        myKeyAlter = value;
    }
}

KeyAccidentalPtr NonTraditionalKey::getKeyAccidental() const
{
    return myKeyAccidental;
}

void NonTraditionalKey::setKeyAccidental(const KeyAccidentalPtr &value)
{
    if (value)
    {
        myKeyAccidental = value;
    }
}

bool NonTraditionalKey::getHasKeyAccidental() const
{
    return myHasKeyAccidental;
}

void NonTraditionalKey::setHasKeyAccidental(const bool value)
{
    myHasKeyAccidental = value;
}

MX_FROM_XELEMENT_UNUSED(NonTraditionalKey);

} // namespace core
} // namespace mx
