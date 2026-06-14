// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Decimal.h"

#include "mx/core/Lexical.h"

namespace mx::core
{

std::string Decimal::toString() const
{
    if (!m_text.empty())
    {
        return m_text;
    }
    return formatDouble(m_value);
}

bool Decimal::tryParse(std::string_view text, Decimal &out)
{
    double value = 0.0;
    if (!tryParseDouble(text, value))
    {
        return false;
    }
    out.m_value = value;
    out.m_text.assign(trimWhitespace(text));
    return true;
}

Decimal Decimal::parse(std::string_view text)
{
    Decimal d;
    if (!tryParse(text, d))
    {
        d = Decimal{};
    }
    return d;
}

} // namespace mx::core
