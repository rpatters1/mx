// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Directive.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
Directive::Directive() : myValue(), myAttributes(std::make_shared<DirectiveAttributes>())
{
}

Directive::Directive(const XsString &value) : myValue(value), myAttributes(std::make_shared<DirectiveAttributes>())
{
}

bool Directive::hasAttributes() const
{
    return myAttributes->hasValues();
}

bool Directive::hasContents() const
{
    return true;
}

std::ostream &Directive::streamAttributes(std::ostream &os) const
{
    if (myAttributes)
    {
        myAttributes->toStream(os);
    }
    return os;
}

std::ostream &Directive::streamName(std::ostream &os) const
{
    os << "directive";
    return os;
}

std::ostream &Directive::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    MX_UNUSED(indentLevel);
    isOneLineOnly = true;
    os << myValue;
    return os;
}

DirectiveAttributesPtr Directive::getAttributes() const
{
    return myAttributes;
}

void Directive::setAttributes(const DirectiveAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

XsString Directive::getValue() const
{
    return myValue;
}

void Directive::setValue(const XsString &value)
{
    myValue = value;
}

bool Directive::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);
    myValue.setValue(xelement.getValue());
    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
