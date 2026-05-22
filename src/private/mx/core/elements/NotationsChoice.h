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

MX_FORWARD_DECLARE_ELEMENT(Tied)
MX_FORWARD_DECLARE_ELEMENT(Slur)
MX_FORWARD_DECLARE_ELEMENT(Tuplet)
MX_FORWARD_DECLARE_ELEMENT(Glissando)
MX_FORWARD_DECLARE_ELEMENT(Slide)
MX_FORWARD_DECLARE_ELEMENT(Ornaments)
MX_FORWARD_DECLARE_ELEMENT(Technical)
MX_FORWARD_DECLARE_ELEMENT(Articulations)
MX_FORWARD_DECLARE_ELEMENT(Dynamics)
MX_FORWARD_DECLARE_ELEMENT(Fermata)
MX_FORWARD_DECLARE_ELEMENT(Arpeggiate)
MX_FORWARD_DECLARE_ELEMENT(NonArpeggiate)
MX_FORWARD_DECLARE_ELEMENT(AccidentalMark)
MX_FORWARD_DECLARE_ELEMENT(OtherNotation)
MX_FORWARD_DECLARE_ELEMENT(NotationsChoice)

inline NotationsChoicePtr makeNotationsChoice()
{
    return std::make_shared<NotationsChoice>();
}

class NotationsChoice : public ElementInterface
{
  public:
    enum class Choice
    {
        tied = 0,
        slur = 1,
        tuplet = 2,
        glissando = 3,
        slide = 4,
        ornaments = 5,
        technical = 6,
        articulations = 7,
        dynamics = 8,
        fermata = 9,
        arpeggiate = 10,
        nonArpeggiate = 11,
        accidentalMark = 12,
        otherNotation = 13
    };
    NotationsChoice();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    Choice getChoice() const;
    void setChoice(const Choice value);
    TiedPtr getTied() const;
    void setTied(const TiedPtr &value);
    SlurPtr getSlur() const;
    void setSlur(const SlurPtr &value);
    TupletPtr getTuplet() const;
    void setTuplet(const TupletPtr &value);
    GlissandoPtr getGlissando() const;
    void setGlissando(const GlissandoPtr &value);
    SlidePtr getSlide() const;
    void setSlide(const SlidePtr &value);
    OrnamentsPtr getOrnaments() const;
    void setOrnaments(const OrnamentsPtr &value);
    TechnicalPtr getTechnical() const;
    void setTechnical(const TechnicalPtr &value);
    ArticulationsPtr getArticulations() const;
    void setArticulations(const ArticulationsPtr &value);
    DynamicsPtr getDynamics() const;
    void setDynamics(const DynamicsPtr &value);
    FermataPtr getFermata() const;
    void setFermata(const FermataPtr &value);
    ArpeggiatePtr getArpeggiate() const;
    void setArpeggiate(const ArpeggiatePtr &value);
    NonArpeggiatePtr getNonArpeggiate() const;
    void setNonArpeggiate(const NonArpeggiatePtr &value);
    AccidentalMarkPtr getAccidentalMark() const;
    void setAccidentalMark(const AccidentalMarkPtr &value);
    OtherNotationPtr getOtherNotation() const;
    void setOtherNotation(const OtherNotationPtr &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    Choice myChoice;
    TiedPtr myTied;
    SlurPtr mySlur;
    TupletPtr myTuplet;
    GlissandoPtr myGlissando;
    SlidePtr mySlide;
    OrnamentsPtr myOrnaments;
    TechnicalPtr myTechnical;
    ArticulationsPtr myArticulations;
    DynamicsPtr myDynamics;
    FermataPtr myFermata;
    ArpeggiatePtr myArpeggiate;
    NonArpeggiatePtr myNonArpeggiate;
    AccidentalMarkPtr myAccidentalMark;
    OtherNotationPtr myOtherNotation;
};
} // namespace core
} // namespace mx
