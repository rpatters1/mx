// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/KindAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
KindAttributes::KindAttributes()
    : useSymbols(YesNo::no), text(), stackDegrees(YesNo::no), parenthesesDegrees(YesNo::no), bracketDegrees(YesNo::no),
      defaultX(), defaultY(), relativeX(), relativeY(), fontFamily(), fontStyle(FontStyle::normal),
      fontSize(CssFontSize::medium), fontWeight(FontWeight::normal), color(), halign(LeftCenterRight::left),
      valign(Valign::bottom), hasUseSymbols(false), hasText(false), hasStackDegrees(false),
      hasParenthesesDegrees(false), hasBracketDegrees(false), hasDefaultX(false), hasDefaultY(false),
      hasRelativeX(false), hasRelativeY(false), hasFontFamily(false), hasFontStyle(false), hasFontSize(false),
      hasFontWeight(false), hasColor(false), hasHalign(false), hasValign(false)
{
}

bool KindAttributes::hasValues() const
{
    return hasUseSymbols || hasText || hasStackDegrees || hasParenthesesDegrees || hasBracketDegrees || hasDefaultX ||
           hasDefaultY || hasRelativeX || hasRelativeY || hasFontFamily || hasFontStyle || hasFontSize ||
           hasFontWeight || hasColor || hasHalign || hasValign;
}

std::ostream &KindAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, useSymbols, "use-symbols", hasUseSymbols);
        streamAttribute(os, text, "text", hasText);
        streamAttribute(os, stackDegrees, "stack-degrees", hasStackDegrees);
        streamAttribute(os, parenthesesDegrees, "parentheses-degrees", hasParenthesesDegrees);
        streamAttribute(os, bracketDegrees, "bracket-degrees", hasBracketDegrees);
        streamAttribute(os, defaultX, "default-x", hasDefaultX);
        streamAttribute(os, defaultY, "default-y", hasDefaultY);
        streamAttribute(os, relativeX, "relative-x", hasRelativeX);
        streamAttribute(os, relativeY, "relative-y", hasRelativeY);
        streamAttribute(os, fontFamily, "font-family", hasFontFamily);
        streamAttribute(os, fontStyle, "font-style", hasFontStyle);
        streamAttribute(os, fontSize, "font-size", hasFontSize);
        streamAttribute(os, fontWeight, "font-weight", hasFontWeight);
        streamAttribute(os, color, "color", hasColor);
        streamAttribute(os, halign, "halign", hasHalign);
        streamAttribute(os, valign, "valign", hasValign);
    }
    return os;
}

bool KindAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "KindAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, useSymbols, hasUseSymbols, "use-symbols", &parseYesNo))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, text, hasText, "text"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, stackDegrees, hasStackDegrees, "stack-degrees",
                           &parseYesNo))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, parenthesesDegrees, hasParenthesesDegrees,
                           "parentheses-degrees", &parseYesNo))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, bracketDegrees, hasBracketDegrees, "bracket-degrees",
                           &parseYesNo))
        {
            continue;
        }
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
        if (parseAttribute(message, it, className, isSuccess, fontFamily, hasFontFamily, "font-family"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, fontStyle, hasFontStyle, "font-style", &parseFontStyle))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, fontSize, hasFontSize, "font-size"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, fontWeight, hasFontWeight, "font-weight",
                           &parseFontWeight))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, color, hasColor, "color"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, halign, hasHalign, "halign", &parseLeftCenterRight))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, valign, hasValign, "valign", &parseValign))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
