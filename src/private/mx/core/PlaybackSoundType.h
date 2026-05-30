// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/PlaybackSound.h"

#include <iostream>
#include <optional>
#include <string>

namespace mx
{
namespace core
{

std::optional<PlaybackSound> tryParsePlaybackSound(const std::string &value);

class PlaybackSoundType
{
  public:
    explicit PlaybackSoundType(const PlaybackSound value);
    explicit PlaybackSoundType(const std::string &value);
    PlaybackSoundType();
    PlaybackSound getValue() const;
    std::string getValueString() const;
    void setValue(const PlaybackSound value);
    void setValue(const std::string &value);

  private:
    PlaybackSound myEnum;
    std::string myCustomValue;
};

PlaybackSoundType parsePlaybackSoundType(const std::string &value);
std::string toString(const PlaybackSoundType &value);
std::ostream &toStream(std::ostream &os, const PlaybackSoundType &value);
std::ostream &operator<<(std::ostream &os, const PlaybackSoundType &value);

} // namespace core
} // namespace mx
