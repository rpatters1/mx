// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeamAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
BeamAttributes::BeamAttributes()
    : number(), repeater(YesNo::no), fan(), color(), hasNumber(false), hasRepeater(false), hasFan(false),
      hasColor(false)
{
}

bool BeamAttributes::hasValues() const
{
    return hasNumber || hasRepeater || hasFan || hasColor;
}

std::ostream &BeamAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, number, "number", hasNumber);
        streamAttribute(os, repeater, "repeater", hasRepeater);
        streamAttribute(os, fan, "fan", hasFan);
        streamAttribute(os, color, "color", hasColor);
    }
    return os;
}

bool BeamAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "BeamAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, number, hasNumber, "number"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, repeater, hasRepeater, "repeater", &parseYesNo))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, fan, hasFan, "fan", &parseFan))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, color, hasColor, "color"))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
