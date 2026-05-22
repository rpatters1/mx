// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/HarmonicTypeChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Artificial.h"
#include "mx/core/elements/Natural.h"
#include <iostream>

namespace mx
{
namespace core
{
HarmonicTypeChoice::HarmonicTypeChoice()
    : myChoice(Choice::natural), myNatural(makeNatural()), myArtificial(makeArtificial())
{
}

bool HarmonicTypeChoice::hasAttributes() const
{
    return false;
}

std::ostream &HarmonicTypeChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &HarmonicTypeChoice::streamName(std::ostream &os) const
{
    return os;
}

bool HarmonicTypeChoice::hasContents() const
{
    return true;
}

std::ostream &HarmonicTypeChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myChoice == Choice::natural)
    {
        myNatural->toStream(os, indentLevel);
    }
    if (myChoice == Choice::artificial)
    {
        myArtificial->toStream(os, indentLevel);
    }
    isOneLineOnly = false;
    return os;
}

HarmonicTypeChoice::Choice HarmonicTypeChoice::getChoice() const
{
    return myChoice;
}

void HarmonicTypeChoice::setChoice(const Choice value)
{
    myChoice = value;
}

NaturalPtr HarmonicTypeChoice::getNatural() const
{
    return myNatural;
}

void HarmonicTypeChoice::setNatural(const NaturalPtr &value)
{
    if (value)
    {
        myNatural = value;
    }
}

ArtificialPtr HarmonicTypeChoice::getArtificial() const
{
    return myArtificial;
}

void HarmonicTypeChoice::setArtificial(const ArtificialPtr &value)
{
    if (value)
    {
        myArtificial = value;
    }
}

MX_FROM_XELEMENT_UNUSED(HarmonicTypeChoice);

} // namespace core
} // namespace mx
