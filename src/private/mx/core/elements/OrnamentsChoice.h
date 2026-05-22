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

MX_FORWARD_DECLARE_ELEMENT(TrillMark)
MX_FORWARD_DECLARE_ELEMENT(Turn)
MX_FORWARD_DECLARE_ELEMENT(DelayedTurn)
MX_FORWARD_DECLARE_ELEMENT(InvertedTurn)
MX_FORWARD_DECLARE_ELEMENT(DelayedInvertedTurn)
MX_FORWARD_DECLARE_ELEMENT(VerticalTurn)
MX_FORWARD_DECLARE_ELEMENT(Shake)
MX_FORWARD_DECLARE_ELEMENT(WavyLine)
MX_FORWARD_DECLARE_ELEMENT(Mordent)
MX_FORWARD_DECLARE_ELEMENT(InvertedMordent)
MX_FORWARD_DECLARE_ELEMENT(Schleifer)
MX_FORWARD_DECLARE_ELEMENT(Tremolo)
MX_FORWARD_DECLARE_ELEMENT(OtherOrnament)
MX_FORWARD_DECLARE_ELEMENT(OrnamentsChoice)

inline OrnamentsChoicePtr makeOrnamentsChoice()
{
    return std::make_shared<OrnamentsChoice>();
}

class OrnamentsChoice : public ElementInterface
{
  public:
    enum class Choice
    {
        trillMark = 0,
        turn = 1,
        delayedTurn = 2,
        invertedTurn = 3,
        delayedInvertedTurn = 4,
        verticalTurn = 5,
        shake = 6,
        wavyLine = 7,
        mordent = 8,
        invertedMordent = 9,
        schleifer = 10,
        tremolo = 11,
        otherOrnament = 12
    };
    OrnamentsChoice();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    Choice getChoice() const;
    void setChoice(const Choice value);
    TrillMarkPtr getTrillMark() const;
    void setTrillMark(const TrillMarkPtr &value);
    TurnPtr getTurn() const;
    void setTurn(const TurnPtr &value);
    DelayedTurnPtr getDelayedTurn() const;
    void setDelayedTurn(const DelayedTurnPtr &value);
    InvertedTurnPtr getInvertedTurn() const;
    void setInvertedTurn(const InvertedTurnPtr &value);
    DelayedInvertedTurnPtr getDelayedInvertedTurn() const;
    void setDelayedInvertedTurn(const DelayedInvertedTurnPtr &value);
    VerticalTurnPtr getVerticalTurn() const;
    void setVerticalTurn(const VerticalTurnPtr &value);
    ShakePtr getShake() const;
    void setShake(const ShakePtr &value);
    WavyLinePtr getWavyLine() const;
    void setWavyLine(const WavyLinePtr &value);
    MordentPtr getMordent() const;
    void setMordent(const MordentPtr &value);
    InvertedMordentPtr getInvertedMordent() const;
    void setInvertedMordent(const InvertedMordentPtr &value);
    SchleiferPtr getSchleifer() const;
    void setSchleifer(const SchleiferPtr &value);
    TremoloPtr getTremolo() const;
    void setTremolo(const TremoloPtr &value);
    OtherOrnamentPtr getOtherOrnament() const;
    void setOtherOrnament(const OtherOrnamentPtr &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    Choice myChoice;
    TrillMarkPtr myTrillMark;
    TurnPtr myTurn;
    DelayedTurnPtr myDelayedTurn;
    InvertedTurnPtr myInvertedTurn;
    DelayedInvertedTurnPtr myDelayedInvertedTurn;
    VerticalTurnPtr myVerticalTurn;
    ShakePtr myShake;
    WavyLinePtr myWavyLine;
    MordentPtr myMordent;
    InvertedMordentPtr myInvertedMordent;
    SchleiferPtr mySchleifer;
    TremoloPtr myTremolo;
    OtherOrnamentPtr myOtherOrnament;
};
} // namespace core
} // namespace mx
