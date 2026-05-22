// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/String.h"
#include "mx/core/FromXElement.h"
#include <iostream>

namespace mx
{
namespace core
{
String::String() : myValue(), myAttributes(std::make_shared<StringAttributes>())
{
}

String::String(const StringNumber &value) : myValue(value), myAttributes(std::make_shared<StringAttributes>())
{
}

bool String::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &String::streamAttributes(std::ostream &os) const
{
    if (myAttributes)
    {
        myAttributes->toStream(os);
    }
    return os;
}

std::ostream &String::streamName(std::ostream &os) const
{
    os << "string";
    return os;
}

bool String::hasContents() const
{
    return true;
}

std::ostream &String::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    MX_UNUSED(indentLevel);
    isOneLineOnly = true;
    os << myValue;
    return os;
}

StringAttributesPtr String::getAttributes() const
{
    return myAttributes;
}

void String::setAttributes(const StringAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

StringNumber String::getValue() const
{
    return myValue;
}

void String::setValue(const StringNumber &value)
{
    myValue = value;
}

bool String::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);
    myValue.parse(xelement.getValue());
    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
