// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/DisplayStepOctaveGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/DisplayOctave.h"
#include "mx/core/elements/DisplayStep.h"
#include <iostream>

namespace mx
{
namespace core
{
DisplayStepOctaveGroup::DisplayStepOctaveGroup()
    : myDisplayStep(makeDisplayStep()), myDisplayOctave(makeDisplayOctave())
{
}

bool DisplayStepOctaveGroup::hasAttributes() const
{
    return false;
}

std::ostream &DisplayStepOctaveGroup::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &DisplayStepOctaveGroup::streamName(std::ostream &os) const
{
    return os;
}

bool DisplayStepOctaveGroup::hasContents() const
{
    return true;
}

std::ostream &DisplayStepOctaveGroup::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    bool isFirst = true;
    if (!isFirst)
        os << std::endl;
    myDisplayStep->toStream(os, indentLevel);
    isFirst = false;
    if (!isFirst)
        os << std::endl;
    myDisplayOctave->toStream(os, indentLevel);
    isFirst = false;
    isOneLineOnly = !hasContents();
    return os;
}

DisplayStepPtr DisplayStepOctaveGroup::getDisplayStep() const
{
    return myDisplayStep;
}

void DisplayStepOctaveGroup::setDisplayStep(const DisplayStepPtr &value)
{
    if (value)
    {
        myDisplayStep = value;
    }
}

DisplayOctavePtr DisplayStepOctaveGroup::getDisplayOctave() const
{
    return myDisplayOctave;
}

void DisplayStepOctaveGroup::setDisplayOctave(const DisplayOctavePtr &value)
{
    if (value)
    {
        myDisplayOctave = value;
    }
}

MX_FROM_XELEMENT_UNUSED(DisplayStepOctaveGroup);

} // namespace core
} // namespace mx
