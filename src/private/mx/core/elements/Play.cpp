// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/Play.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Ipa.h"
#include "mx/core/elements/Mute.h"
#include "mx/core/elements/OtherPlay.h"
#include "mx/core/elements/SemiPitched.h"
#include <iostream>

namespace mx
{
namespace core
{
Play::Play()
    : myChoice(Choice::ipa), myAttributes(std::make_shared<PlayAttributes>()), myIpa(makeIpa()), myMute(makeMute()),
      mySemiPitched(makeSemiPitched()), myOtherPlay(makeOtherPlay())
{
}

bool Play::hasAttributes() const
{
    return myAttributes->hasValues();
}

std::ostream &Play::streamAttributes(std::ostream &os) const
{
    return myAttributes->toStream(os);
}

std::ostream &Play::streamName(std::ostream &os) const
{
    os << "play";
    return os;
}

bool Play::hasContents() const
{
    return true;
}

std::ostream &Play::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    switch (myChoice)
    {
    case Choice::ipa: {
        os << std::endl;
        myIpa->toStream(os, indentLevel + 1);
        os << std::endl;
        isOneLineOnly = false;
    }
    break;
    case Choice::mute: {
        os << std::endl;
        myMute->toStream(os, indentLevel + 1);
        os << std::endl;
        isOneLineOnly = false;
    }
    break;
    case Choice::semiPitched: {
        os << std::endl;
        mySemiPitched->toStream(os, indentLevel + 1);
        os << std::endl;
        isOneLineOnly = false;
    }
    break;
    case Choice::otherPlay: {
        os << std::endl;
        myOtherPlay->toStream(os, indentLevel + 1);
        os << std::endl;
        isOneLineOnly = false;
    }
    break;
    default:
        break;
    }
    return os;
}

PlayAttributesPtr Play::getAttributes() const
{
    return myAttributes;
}

void Play::setAttributes(const PlayAttributesPtr &value)
{
    if (value)
    {
        myAttributes = value;
    }
}

Play::Choice Play::getChoice() const
{
    return myChoice;
}

void Play::setChoice(const Play::Choice value)
{
    myChoice = value;
}

IpaPtr Play::getIpa() const
{
    return myIpa;
}

void Play::setIpa(const IpaPtr &value)
{
    if (value)
    {
        myIpa = value;
    }
}

MutePtr Play::getMute() const
{
    return myMute;
}

void Play::setMute(const MutePtr &value)
{
    if (value)
    {
        myMute = value;
    }
}

SemiPitchedPtr Play::getSemiPitched() const
{
    return mySemiPitched;
}

void Play::setSemiPitched(const SemiPitchedPtr &value)
{
    if (value)
    {
        mySemiPitched = value;
    }
}

OtherPlayPtr Play::getOtherPlay() const
{
    return myOtherPlay;
}

void Play::setOtherPlay(const OtherPlayPtr &value)
{
    if (value)
    {
        myOtherPlay = value;
    }
}

bool Play::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    isSuccess &= myAttributes->fromXElement(message, xelement);

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (it->getName() == "ipa")
        {
            myChoice = Choice::ipa;
            isSuccess &= myIpa->fromXElement(message, *it);
            continue;
        }
        if (it->getName() == "mute")
        {
            myChoice = Choice::mute;
            isSuccess &= myMute->fromXElement(message, *it);
            continue;
        }
        if (it->getName() == "semi-pitched")
        {
            myChoice = Choice::semiPitched;
            isSuccess &= mySemiPitched->fromXElement(message, *it);
            continue;
        }
        if (it->getName() == "other-play")
        {
            myChoice = Choice::otherPlay;
            isSuccess &= myOtherPlay->fromXElement(message, *it);
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
