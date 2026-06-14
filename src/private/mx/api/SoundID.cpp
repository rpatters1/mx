// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/SoundID.h"
#include "mx/core/generated/SoundID.h"
#include "mx/impl/Converter.h"

namespace mx
{
namespace api
{
std::string SoundIDToString(SoundID inSoundID)
{
    if (inSoundID == SoundID::unspecified)
    {
        return "unspecified";
    }
    else if (inSoundID == SoundID::errorBadString)
    {
        return "error.bad.string";
    }

    impl::Converter c;
    return std::string{c.convert(inSoundID).toString()};
}

SoundID SoundIDFromString(const std::string &inString)
{
    core::SoundID parsed;
    if (!core::SoundID::tryParse(inString, parsed))
    {
        // the old core's FromString threw on an unrecognized string
        return SoundID::errorBadString;
    }

    impl::Converter c;
    return c.convert(parsed);
}
} // namespace api
} // namespace mx
