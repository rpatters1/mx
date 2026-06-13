// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Lexical.h"

#include <charconv>
#include <cmath>

namespace mx::core
{

// XML whitespace is the only decoration a lexically-strict parse tolerates.
std::string_view trimWhitespace(std::string_view text)
{
    constexpr std::string_view space = " \t\r\n";
    const auto first = text.find_first_not_of(space);
    if (first == std::string_view::npos)
    {
        return {};
    }
    const auto last = text.find_last_not_of(space);
    return text.substr(first, last - first + 1);
}

bool tryParseInt(std::string_view text, int &out)
{
    const std::string_view t = trimWhitespace(text);
    if (t.empty())
    {
        return false;
    }
    // std::from_chars accepts a leading minus but not a plus; xs:integer
    // allows an explicit plus sign.
    const std::string_view digits = t.front() == '+' ? t.substr(1) : t;
    int value = 0;
    const auto [ptr, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), value);
    if (ec != std::errc{} || ptr != digits.data() + digits.size())
    {
        return false;
    }
    out = value;
    return true;
}

int parseInt(std::string_view text)
{
    int value = 0;
    return tryParseInt(text, value) ? value : 0;
}

bool tryParseDouble(std::string_view text, double &out)
{
    const std::string_view t = trimWhitespace(text);
    if (t.empty())
    {
        return false;
    }
    const std::string_view digits = t.front() == '+' ? t.substr(1) : t;
    double value = 0.0;
    const auto [ptr, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), value);
    if (ec != std::errc{} || ptr != digits.data() + digits.size())
    {
        return false;
    }
    if (std::isnan(value) || std::isinf(value))
    {
        return false;
    }
    out = value;
    return true;
}

double parseDouble(std::string_view text)
{
    double value = 0.0;
    return tryParseDouble(text, value) ? value : 0.0;
}

std::string formatInt(int value)
{
    char buf[16];
    const auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
    return std::string(buf, ptr);
}

namespace
{

void splitVersion(std::string_view v, int &major, int &minor)
{
    major = 0;
    minor = 0;
    const auto dot = v.find('.');
    if (dot == std::string_view::npos)
    {
        tryParseInt(v, major);
        return;
    }
    tryParseInt(v.substr(0, dot), major);
    tryParseInt(v.substr(dot + 1), minor);
}

} // namespace

bool musicXmlVersionExceeds(std::string_view declared, std::string_view supported)
{
    int dMajor = 0;
    int dMinor = 0;
    int sMajor = 0;
    int sMinor = 0;
    splitVersion(declared, dMajor, dMinor);
    splitVersion(supported, sMajor, sMinor);
    return dMajor > sMajor || (dMajor == sMajor && dMinor > sMinor);
}

std::string formatDouble(double value)
{
    // Shortest fixed-notation string that round-trips. to_chars(fixed) is
    // exactly that contract; never exponent notation (xs:decimal forbids it).
    char buf[352]; // worst case fixed notation for a double
    const auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value, std::chars_format::fixed);
    if (ec != std::errc{})
    {
        return "0";
    }
    return std::string(buf, ptr);
}

} // namespace mx::core
