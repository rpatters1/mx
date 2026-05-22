// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/TextAttributes.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
TextAttributes::TextAttributes()
    : fontFamily(), fontStyle(FontStyle::normal), fontSize(CssFontSize::medium), fontWeight(FontWeight::normal),
      color(), underline(), overline(), lineThrough(), rotation(), letterSpacing(), lang("it"), dir(),
      hasFontFamily(false), hasFontStyle(false), hasFontSize(false), hasFontWeight(false), hasColor(false),
      hasUnderline(false), hasOverline(false), hasLineThrough(false), hasRotation(false), hasLetterSpacing(false),
      hasLang(false), hasDir(false)
{
}

bool TextAttributes::hasValues() const
{
    return hasFontFamily || hasFontStyle || hasFontSize || hasFontWeight || hasColor || hasUnderline || hasOverline ||
           hasLineThrough || hasRotation || hasLetterSpacing || hasLang || hasDir;
}

std::ostream &TextAttributes::toStream(std::ostream &os) const
{
    if (hasValues())
    {
        streamAttribute(os, fontFamily, "font-family", hasFontFamily);
        streamAttribute(os, fontStyle, "font-style", hasFontStyle);
        streamAttribute(os, fontSize, "font-size", hasFontSize);
        streamAttribute(os, fontWeight, "font-weight", hasFontWeight);
        streamAttribute(os, color, "color", hasColor);
        streamAttribute(os, underline, "underline", hasUnderline);
        streamAttribute(os, overline, "overline", hasOverline);
        streamAttribute(os, lineThrough, "line-through", hasLineThrough);
        streamAttribute(os, rotation, "rotation", hasRotation);
        streamAttribute(os, letterSpacing, "letter-spacing", hasLetterSpacing);
        streamAttribute(os, lang, "xml:lang", hasLang);
        streamAttribute(os, dir, "dir", hasDir);
    }
    return os;
}

bool TextAttributes::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    const char *const className = "TextAttributes";
    bool isSuccess = true;

    auto it = xelement.attributesBegin();
    auto endIter = xelement.attributesEnd();

    for (; it != endIter; ++it)
    {
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
        if (parseAttribute(message, it, className, isSuccess, lang, hasLang, "xml:lang"))
        {
            continue;
        }
        if (parseAttribute(message, it, className, isSuccess, dir, hasDir, "dir", &parseTextDirection))
        {
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
