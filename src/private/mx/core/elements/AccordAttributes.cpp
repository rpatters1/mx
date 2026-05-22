// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/AccordAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
AccordAttributes::AccordAttributes() : string_(), hasString(false)
{
}

bool AccordAttributes::hasValues() const
{
    return hasString;
}

std::ostream &AccordAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, string_, "string", hasString);
    }
    return os;
}

bool AccordAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "AccordAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, string_, hasString, "string"))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
