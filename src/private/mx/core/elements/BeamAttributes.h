// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/AttributesInterface.h"
#include "mx/core/Color.h"
#include "mx/core/Enums.h"
#include "mx/core/ForwardDeclare.h"
#include "mx/core/Integers.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
namespace core
{

MX_FORWARD_DECLARE_ATTRIBUTES(BeamAttributes)

struct BeamAttributes : public AttributesInterface
{
  public:
    BeamAttributes();
    virtual bool hasValues() const;
    virtual std::ostream &toStream(std::ostream &os) const;
    BeamLevel number;
    YesNo repeater;
    Fan fan;
    Color color;
    bool hasNumber;
    bool hasRepeater;
    bool hasFan;
    bool hasColor;

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);
};
} // namespace core
} // namespace mx
