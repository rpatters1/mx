// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/ElementInterface.h"
#include "mx/core/ForwardDeclare.h"

#include <iosfwd>
#include <memory>
#include <vector>

namespace mx
{
namespace core
{

MX_FORWARD_DECLARE_ELEMENT(BasePitch)
MX_FORWARD_DECLARE_ELEMENT(TouchingPitch)
MX_FORWARD_DECLARE_ELEMENT(SoundingPitch)
MX_FORWARD_DECLARE_ELEMENT(HarmonicInfoChoice)

inline HarmonicInfoChoicePtr makeHarmonicInfoChoice()
{
    return std::make_shared<HarmonicInfoChoice>();
}

class HarmonicInfoChoice : public ElementInterface
{
  public:
    enum class Choice
    {
        basePitch = 0,
        touchingPitch = 1,
        soundingPitch = 2
    };
    HarmonicInfoChoice();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    Choice getChoice() const;
    void setChoice(const Choice value);
    BasePitchPtr getBasePitch() const;
    void setBasePitch(const BasePitchPtr &value);
    TouchingPitchPtr getTouchingPitch() const;
    void setTouchingPitch(const TouchingPitchPtr &value);
    SoundingPitchPtr getSoundingPitch() const;
    void setSoundingPitch(const SoundingPitchPtr &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    Choice myChoice;
    BasePitchPtr myBasePitch;
    TouchingPitchPtr myTouchingPitch;
    SoundingPitchPtr mySoundingPitch;
};
} // namespace core
} // namespace mx
