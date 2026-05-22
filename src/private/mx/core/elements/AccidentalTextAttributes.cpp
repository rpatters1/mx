// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/AccidentalTextAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
AccidentalTextAttributes::AccidentalTextAttributes()
    : justify(LeftCenterRight::left), defaultX(), defaultY(), relativeX(), relativeY(), fontFamily(),
      fontStyle(FontStyle::normal), fontSize(CssFontSize::medium), fontWeight(FontWeight::normal), color(),
      halign(LeftCenterRight::left), valign(Valign::bottom), underline(), overline(), lineThrough(), rotation(),
      letterSpacing(), lineHeight(), lang("it"), space(), dir(), enclosure(EnclosureShape::none), hasJustify(false),
      hasDefaultX(false), hasDefaultY(false), hasRelativeX(false), hasRelativeY(false), hasFontFamily(false),
      hasFontStyle(false), hasFontSize(false), hasFontWeight(false), hasColor(false), hasHalign(false),
      hasValign(false), hasUnderline(false), hasOverline(false), hasLineThrough(false), hasRotation(false),
      hasLetterSpacing(false), hasLineHeight(false), hasLang(false), hasSpace(false), hasDir(false), hasEnclosure(false)
{
}

bool AccidentalTextAttributes::hasValues() const
{
    return hasJustify || hasDefaultX || hasDefaultY || hasRelativeX || hasRelativeY || hasFontFamily || hasFontStyle ||
           hasFontSize || hasFontWeight || hasColor || hasHalign || hasValign || hasUnderline || hasOverline ||
           hasLineThrough || hasRotation || hasLetterSpacing || hasLineHeight || hasLang || hasSpace || hasDir ||
           hasEnclosure;
}

std::ostream &AccidentalTextAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, justify, "justify", hasJustify);
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
        streamAttribute(os, underline, "underline", hasUnderline);
        streamAttribute(os, overline, "overline", hasOverline);
        streamAttribute(os, lineThrough, "line-through", hasLineThrough);
        streamAttribute(os, rotation, "rotation", hasRotation);
        streamAttribute(os, letterSpacing, "letter-spacing", hasLetterSpacing);
        streamAttribute(os, lineHeight, "line-height", hasLineHeight);
        streamAttribute(os, lang, "xml:lang", hasLang);
        streamAttribute(os, space, "xml:space", hasSpace);
        streamAttribute(os, dir, "dir", hasDir);
        streamAttribute(os, enclosure, "enclosure", hasEnclosure);
    }
    return os;
}

bool AccidentalTextAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "AccidentalTextAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
        if (parseAttribute(message, it, className, isSuccess, justify, hasJustify, "justify", &parseLeftCenterRight))
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
        if (parseAttribute(message, it, className, isSuccess, underline, hasUnderline, "underline"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, overline, hasOverline, "overline"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, lineThrough, hasLineThrough, "line-through"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, rotation, hasRotation, "rotation"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, letterSpacing, hasLetterSpacing, "letter-spacing"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, lineHeight, hasLineHeight, "line-height"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, lang, hasLang, "xml:lang"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, space, hasSpace, "xml:space", &parseXmlSpace))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, dir, hasDir, "dir", &parseTextDirection))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, enclosure, hasEnclosure, "enclosure",
                           &parseEnclosureShape))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
