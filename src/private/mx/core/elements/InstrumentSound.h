// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ElementInterface.h"
#include "mx/core/ForwardDeclare.h"
#include "mx/core/PlaybackSoundType.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
namespace core
{

MX_FORWARD_DECLARE_ELEMENT(InstrumentSound)

inline InstrumentSoundPtr makeInstrumentSound()
{
    return std::make_shared<InstrumentSound>();
}

inline InstrumentSoundPtr makeInstrumentSound(const PlaybackSoundType &value)
{
    return std::make_shared<InstrumentSound>(value);
}

inline InstrumentSoundPtr makeInstrumentSound(PlaybackSoundType &&value)
{
    return std::make_shared<InstrumentSound>(std::move(value));
}

class InstrumentSound : public ElementInterface
{
  public:
    InstrumentSound();
    InstrumentSound(const PlaybackSoundType &value);

    virtual bool hasAttributes() const;
    virtual bool hasContents() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    PlaybackSoundType getValue() const;
    void setValue(const PlaybackSoundType &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    PlaybackSoundType myValue;
};
} // namespace core
} // namespace mx
