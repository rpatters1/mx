// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/AttributesInterface.h"
#include "mx/core/Color.h"
#include "mx/core/CommaSeparatedText.h"
#include "mx/core/Decimals.h"
#include "mx/core/Enums.h"
#include "mx/core/FontSize.h"
#include "mx/core/ForwardDeclare.h"
#include "mx/core/Integers.h"
#include "mx/core/NumberOrNormal.h"
#include "mx/core/XmlLang.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
namespace core
{

MX_FORWARD_DECLARE_ATTRIBUTES(TextAttributes)

struct TextAttributes : public AttributesInterface
{
  public:
    TextAttributes();
    virtual bool hasValues() const;
    virtual std::ostream &toStream(std::ostream &os) const;
    CommaSeparatedText fontFamily;
    FontStyle fontStyle;
    FontSize fontSize;
    FontWeight fontWeight;
    Color color;
    NumberOfLines underline;
    NumberOfLines overline;
    NumberOfLines lineThrough;
    RotationDegrees rotation;
    NumberOrNormal letterSpacing;
    XmlLang lang;
    TextDirection dir;
    bool hasFontFamily;
    bool hasFontStyle;
    bool hasFontSize;
    bool hasFontWeight;
    bool hasColor;
    bool hasUnderline;
    bool hasOverline;
    bool hasLineThrough;
    bool hasRotation;
    bool hasLetterSpacing;
    bool hasLang;
    bool hasDir;

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);
};
} // namespace core
} // namespace mx
