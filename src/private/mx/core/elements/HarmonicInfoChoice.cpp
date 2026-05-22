// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HarmonicInfoChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BasePitch.h"
#include "mx/core/elements/SoundingPitch.h"
#include "mx/core/elements/TouchingPitch.h"
#include <iostream>

namespace mx
{
namespace core
{
HarmonicInfoChoice::HarmonicInfoChoice()
    : myChoice(Choice::basePitch), myBasePitch(makeBasePitch()), myTouchingPitch(makeTouchingPitch()),
      mySoundingPitch(makeSoundingPitch())
{
}

bool HarmonicInfoChoice::hasAttributes() const
{
    return false;
}

std::ostream &HarmonicInfoChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &HarmonicInfoChoice::streamName(std::ostream &os) const
{
    return os;
}

bool HarmonicInfoChoice::hasContents() const
{
    return true;
}

std::ostream &HarmonicInfoChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myChoice == Choice::basePitch)
    {
        myBasePitch->toStream(os, indentLevel);
    }
    if (myChoice == Choice::touchingPitch)
    {
        myTouchingPitch->toStream(os, indentLevel);
    }
    if (myChoice == Choice::soundingPitch)
    {
        mySoundingPitch->toStream(os, indentLevel);
    }
    isOneLineOnly = false;
    return os;
}

HarmonicInfoChoice::Choice HarmonicInfoChoice::getChoice() const
{
    return myChoice;
}

void HarmonicInfoChoice::setChoice(const Choice value)
{
    myChoice = value;
}

BasePitchPtr HarmonicInfoChoice::getBasePitch() const
{
    return myBasePitch;
}

void HarmonicInfoChoice::setBasePitch(const BasePitchPtr &value)
{
    if (value)
    {
        myBasePitch = value;
    }
}

TouchingPitchPtr HarmonicInfoChoice::getTouchingPitch() const
{
    return myTouchingPitch;
}

void HarmonicInfoChoice::setTouchingPitch(const TouchingPitchPtr &value)
{
    if (value)
    {
        myTouchingPitch = value;
    }
}

SoundingPitchPtr HarmonicInfoChoice::getSoundingPitch() const
{
    return mySoundingPitch;
}

void HarmonicInfoChoice::setSoundingPitch(const SoundingPitchPtr &value)
{
    if (value)
    {
        mySoundingPitch = value;
    }
}

MX_FROM_XELEMENT_UNUSED(HarmonicInfoChoice);

} // namespace core
} // namespace mx
