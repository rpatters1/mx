// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/StemAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
StemAttributes::StemAttributes()
    : defaultX(), defaultY(), relativeX(), relativeY(), color(), hasDefaultX(false), hasDefaultY(false),
      hasRelativeX(false), hasRelativeY(false), hasColor(false)
{
}

bool StemAttributes::hasValues() const
{
    return hasDefaultX || hasDefaultY || hasRelativeX || hasRelativeY || hasColor;
}

std::ostream &StemAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, defaultX, "default-x", hasDefaultX);
        streamAttribute(os, defaultY, "default-y", hasDefaultY);
        streamAttribute(os, relativeX, "relative-x", hasRelativeX);
        streamAttribute(os, relativeY, "relative-y", hasRelativeY);
        streamAttribute(os, color, "color", hasColor);
    }
    return os;
}

bool StemAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "StemAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, defaultX, hasDefaultX, "default-x"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, defaultY, hasDefaultY, "default-y"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, relativeX, hasRelativeX, "relative-x"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, relativeY, hasRelativeY, "relative-y"))
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
