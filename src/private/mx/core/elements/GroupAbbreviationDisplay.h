// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ElementInterface.h"
#include "mx/core/ForwardDeclare.h"
#include "mx/core/elements/GroupAbbreviationDisplayAttributes.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
namespace core
{

MX_FORWARD_DECLARE_ATTRIBUTES(GroupAbbreviationDisplayAttributes)
MX_FORWARD_DECLARE_ELEMENT(DisplayTextOrAccidentalText)
MX_FORWARD_DECLARE_ELEMENT(GroupAbbreviationDisplay)

inline GroupAbbreviationDisplayPtr makeGroupAbbreviationDisplay()
{
    return std::make_shared<GroupAbbreviationDisplay>();
}

class GroupAbbreviationDisplay : public ElementInterface
{
  public:
    GroupAbbreviationDisplay();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    GroupAbbreviationDisplayAttributesPtr getAttributes() const;
    void setAttributes(const GroupAbbreviationDisplayAttributesPtr &value);

    /* _________ DisplayTextOrAccidentalText minOccurs = 0, maxOccurs = unbounded _________ */
    const DisplayTextOrAccidentalTextSet &getDisplayTextOrAccidentalTextSet() const;
    void addDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextPtr &value);
    void removeDisplayTextOrAccidentalText(const DisplayTextOrAccidentalTextSetIterConst &value);
    void clearDisplayTextOrAccidentalTextSet();
    DisplayTextOrAccidentalTextPtr getDisplayTextOrAccidentalText(
        const DisplayTextOrAccidentalTextSetIterConst &setIterator) const;

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    GroupAbbreviationDisplayAttributesPtr myAttributes;
    DisplayTextOrAccidentalTextSet myDisplayTextOrAccidentalTextSet;
};
} // namespace core
} // namespace mx
