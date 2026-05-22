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

MX_FORWARD_DECLARE_ELEMENT(Solo)
MX_FORWARD_DECLARE_ELEMENT(Ensemble)
MX_FORWARD_DECLARE_ELEMENT(SoloOrEnsembleChoice)

inline SoloOrEnsembleChoicePtr makeSoloOrEnsembleChoice()
{
    return std::make_shared<SoloOrEnsembleChoice>();
}

class SoloOrEnsembleChoice : public ElementInterface
{
  public:
    enum class Choice
    {
        solo = 0,
        ensemble = 1
    };
    SoloOrEnsembleChoice();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    Choice getChoice() const;
    void setChoice(const Choice value);
    SoloPtr getSolo() const;
    void setSolo(const SoloPtr &value);
    EnsemblePtr getEnsemble() const;
    void setEnsemble(const EnsemblePtr &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    Choice myChoice;
    SoloPtr mySolo;
    EnsemblePtr myEnsemble;
};
} // namespace core
} // namespace mx
