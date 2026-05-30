// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/PlaybackSoundType.h"

namespace mx
{
namespace core
{

std::optional<PlaybackSound> tryParsePlaybackSound(const std::string &value)
{
    try
    {
        return PlaybackSoundFromString(value);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

PlaybackSoundType::PlaybackSoundType(const PlaybackSound value) : myEnum(value), myCustomValue("")
{
    setValue(value);
}

PlaybackSoundType::PlaybackSoundType(const std::string &value) : myEnum(PlaybackSound::other), myCustomValue(value)
{
    setValue(value);
}

PlaybackSoundType::PlaybackSoundType() : myEnum(PlaybackSound::keyboardPiano), myCustomValue("")
{
    setValue(PlaybackSound::keyboardPiano);
}

PlaybackSound PlaybackSoundType::getValue() const
{
    return myEnum;
}

std::string PlaybackSoundType::getValueString() const
{
    if (myEnum != PlaybackSound::other)
    {
        return PlaybackSoundToString(myEnum);
    }
    return myCustomValue;
}

void PlaybackSoundType::setValue(const PlaybackSound value)
{
    myEnum = value;
    if (value != PlaybackSound::other && !myCustomValue.empty())
    {
        myCustomValue = "";
    }
}

void PlaybackSoundType::setValue(const std::string &value)
{
    const auto temp = tryParsePlaybackSound(value);
    if (temp)
    {
        myEnum = temp.value();
        myCustomValue = "";
    }
    else
    {
        myEnum = PlaybackSound::other;
        myCustomValue = value;
    }
}

PlaybackSoundType parsePlaybackSoundType(const std::string &value)
{
    return PlaybackSoundType(value);
}

std::string toString(const PlaybackSoundType &value)
{
    return value.getValueString();
}

std::ostream &toStream(std::ostream &os, const PlaybackSoundType &value)
{
    return os << toString(value);
}

std::ostream &operator<<(std::ostream &os, const PlaybackSoundType &value)
{
    return toStream(os, value);
}

} // namespace core
} // namespace mx
