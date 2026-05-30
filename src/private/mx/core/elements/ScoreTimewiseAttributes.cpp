// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/ScoreTimewiseAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
ScoreTimewiseAttributes::ScoreTimewiseAttributes() : version("3.0"), hasVersion(false)
{
}

bool ScoreTimewiseAttributes::hasValues() const
{
    return hasVersion || !xmlnsDeclarations.empty();
}

std::ostream &ScoreTimewiseAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, version, "version", hasVersion);
        for (const auto &ns : xmlnsDeclarations)
        {
            os << " " << ns.first << "=\"" << ns.second << "\"";
        }
    }
    return os;
}

bool ScoreTimewiseAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "ScoreTimewiseAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, version, hasVersion, "version"))
        {
            continue;
        }
        const auto attrName = it->getName();
        if (attrName == "xmlns" || (attrName.size() > 6 && attrName.substr(0, 6) == "xmlns:"))
        {
            xmlnsDeclarations.emplace_back(attrName, it->getValue());
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
