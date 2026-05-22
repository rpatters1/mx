// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/SoloOrEnsembleChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/Ensemble.h"
#include "mx/core/elements/Solo.h"
#include <iostream>

namespace mx
{
namespace core
{
SoloOrEnsembleChoice::SoloOrEnsembleChoice() : myChoice(Choice::solo), mySolo(makeSolo()), myEnsemble(makeEnsemble())
{
}

bool SoloOrEnsembleChoice::hasAttributes() const
{
    return false;
}

std::ostream &SoloOrEnsembleChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &SoloOrEnsembleChoice::streamName(std::ostream &os) const
{
    return os;
}

bool SoloOrEnsembleChoice::hasContents() const
{
    return true;
}

std::ostream &SoloOrEnsembleChoice::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myChoice == Choice::solo)
    {
        mySolo->toStream(os, indentLevel);
    }
    if (myChoice == Choice::ensemble)
    {
        myEnsemble->toStream(os, indentLevel);
    }
    isOneLineOnly = false;
    return os;
}

SoloOrEnsembleChoice::Choice SoloOrEnsembleChoice::getChoice() const
{
    return myChoice;
}

void SoloOrEnsembleChoice::setChoice(const Choice value)
{
    myChoice = value;
}

SoloPtr SoloOrEnsembleChoice::getSolo() const
{
    return mySolo;
}

void SoloOrEnsembleChoice::setSolo(const SoloPtr &value)
{
    if (value)
    {
        mySolo = value;
    }
}

EnsemblePtr SoloOrEnsembleChoice::getEnsemble() const
{
    return myEnsemble;
}

void SoloOrEnsembleChoice::setEnsemble(const EnsemblePtr &value)
{
    if (value)
    {
        myEnsemble = value;
    }
}

MX_FROM_XELEMENT_UNUSED(SoloOrEnsembleChoice);

} // namespace core
} // namespace mx
