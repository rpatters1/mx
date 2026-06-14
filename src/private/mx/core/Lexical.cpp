// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/Lexical.h"

#include <charconv>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>

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
    // std::from_chars(double) is unimplemented in AppleClang's libc++ (only the
    // integral overloads exist), so doubles go through strtod. strtod needs a
    // NUL-terminated string and honors the active locale's decimal point; XML
    // decimals always use '.', so under a locale whose separator differs (e.g.
    // ',') translate before parsing. strtod also accepts a leading '+', which
    // from_chars rejected and the old code stripped by hand -- no longer needed.
    const char decimalPoint = *std::localeconv()->decimal_point;
    std::string buffer(t);
    if (decimalPoint != '.')
    {
        for (char &c : buffer)
        {
            if (c == '.')
            {
                c = decimalPoint;
            }
        }
    }
    // Reject hex floats ("0x1p4") and strtod's "nan(...)" spellings: xs:decimal
    // and xs:float never use them, and from_chars(general) rejected them too.
    if (buffer.find_first_of("xX") != std::string::npos)
    {
        return false;
    }
    const char *const begin = buffer.c_str();
    char *end = nullptr;
    const double value = std::strtod(begin, &end);
    // Lexically strict: some text must parse, and all of it must be consumed.
    if (end == begin || end != begin + buffer.size())
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
    // Shortest fixed-notation string that round-trips; never exponent notation
    // (xs:decimal forbids it). std::to_chars(double) is unavailable on
    // AppleClang's libc++ below macOS 13.3 / iOS 16.3, and mx targets older, so
    // reproduce its contract with snprintf: grow the fractional precision until
    // the printed text parses back to exactly the input. snprintf and strtod
    // share the active locale's decimal point, so the round-trip check is
    // locale-agnostic; only the returned spelling is normalized back to '.'.
    // Large magnitudes round-trip at low precision (their ULP exceeds 1), so the
    // 340-digit cap is only ever approached by sub-unit values whose fixed form
    // stays well inside the buffer; MusicXML decimals never reach it.
    const char decimalPoint = *std::localeconv()->decimal_point;
    char buf[400];
    for (int precision = 0; precision <= 340; ++precision)
    {
        const int n = std::snprintf(buf, sizeof(buf), "%.*f", precision, value);
        if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(buf))
        {
            continue;
        }
        if (std::strtod(buf, nullptr) == value)
        {
            std::string result(buf, static_cast<std::size_t>(n));
            if (decimalPoint != '.')
            {
                for (char &c : result)
                {
                    if (c == decimalPoint)
                    {
                        c = '.';
                    }
                }
            }
            return result;
        }
    }
    return "0";
}

} // namespace mx::core
