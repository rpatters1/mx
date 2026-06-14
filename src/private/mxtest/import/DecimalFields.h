// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <set>
#include <string>

// Some of the test files (particularly MuseScore and Recordare) have
// trailing zeros after decimal points. mx does not write these (its Decimal
// serializes the shortest round-trip form when constructed numerically), so
// the corert normalization strips them from BOTH sides of the comparison.
// The list names every element/attribute the rule applies to.

namespace mxtest
{

const std::set<std::string> decimalFields = {
    "top-system-distance", "dynamics",  "left-margin", "right-margin", "staff-distance",
    "system-distance",     "default-y", "default-x",   "tenths",       "width"};

// Number of trailing characters to strip from a decimal string: the
// trailing zeros after the decimal point, plus the orphaned point itself.
inline int trailingCharsToStrip(const std::string &value)
{
    bool isDecimalFound = false;
    bool isNonZeroFound = false;
    int zeroCount = 0;
    for (auto it = value.crbegin(); it != value.crend(); ++it)
    {
        if (*it == '0')
        {
            if (!isDecimalFound && !isNonZeroFound)
            {
                ++zeroCount;
            }
        }
        else if (*it == '.')
        {
            isDecimalFound = true;
            break;
        }
        else
        {
            isNonZeroFound = true;
        }
    }

    if (!isDecimalFound)
    {
        return 0;
    }

    if (!isNonZeroFound)
    {
        // add 1 to strip the decimal
        return zeroCount + 1;
    }

    return zeroCount;
}

inline std::string stripZeros(const std::string &value)
{
    if (value.empty())
    {
        return value;
    }
    const int chars = trailingCharsToStrip(value);
    std::string out = chars ? value.substr(0, value.size() - chars) : value;
    if (out == "-0")
    {
        out = "0";
    }
    return out;
}

} // namespace mxtest
