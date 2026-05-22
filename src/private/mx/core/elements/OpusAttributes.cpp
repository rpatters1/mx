// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/OpusAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
OpusAttributes::OpusAttributes()
    : href(), type(), role(), title(), show(), actuate(), hasHref(true), hasType(false), hasRole(false),
      hasTitle(false), hasShow(false), hasActuate(false)
{
}

bool OpusAttributes::hasValues() const
{
    return hasHref || hasType || hasRole || hasTitle || hasShow || hasActuate;
}

std::ostream &OpusAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, href, "xlink:href", hasHref);
        streamAttribute(os, type, "xlink:type", hasType);
        streamAttribute(os, role, "xlink:role", hasRole);
        streamAttribute(os, title, "xlink:title", hasTitle);
        streamAttribute(os, show, "xlink:show", hasShow);
        streamAttribute(os, actuate, "xlink:actuate", hasActuate);
    }
    return os;
}

bool OpusAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "OpusAttributes";
    bool isSuccess = true;
    bool isHrefFound = false;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, href, isHrefFound, "xlink:href"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, type, hasType, "xlink:type", &parseXlinkType))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, role, hasRole, "xlink:role"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, title, hasTitle, "xlink:title"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, show, hasShow, "xlink:show", &parseXlinkShow))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, actuate, hasActuate, "xlink:actuate", &parseXlinkActuate))
        {
            continue;
        }
    }

    if (!isHrefFound)
    {
        isSuccess = false;
        message << className << ": 'xlink:href' is a required attribute but was not found" << std::endl;
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
