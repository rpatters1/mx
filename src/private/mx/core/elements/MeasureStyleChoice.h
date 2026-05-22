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

MX_FORWARD_DECLARE_ELEMENT(BeatRepeat)
MX_FORWARD_DECLARE_ELEMENT(MeasureRepeat)
MX_FORWARD_DECLARE_ELEMENT(MultipleRest)
MX_FORWARD_DECLARE_ELEMENT(Slash)
MX_FORWARD_DECLARE_ELEMENT(MeasureStyleChoice)

inline MeasureStyleChoicePtr makeMeasureStyleChoice()
{
    return std::make_shared<MeasureStyleChoice>();
}

class MeasureStyleChoice : public ElementInterface
{
  public:
    enum class Choice
    {
        multipleRest = 0,
        measureRepeat = 1,
        beatRepeat = 2,
        slash = 3
    };
    MeasureStyleChoice();

    virtual bool hasAttributes() const;
    virtual std::ostream &streamAttributes(std::ostream &os) const;
    virtual std::ostream &streamName(std::ostream &os) const;
    virtual bool hasContents() const;
    virtual std::ostream &streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const;
    Choice getChoice() const;
    void setChoice(const Choice value);

    /* _________ MultipleRest minOccurs = 1, maxOccurs = 1 _________ */
    MultipleRestPtr getMultipleRest() const;
    void setMultipleRest(const MultipleRestPtr &value);

    /* _________ MeasureRepeat minOccurs = 1, maxOccurs = 1 _________ */
    MeasureRepeatPtr getMeasureRepeat() const;
    void setMeasureRepeat(const MeasureRepeatPtr &value);

    /* _________ BeatRepeat minOccurs = 1, maxOccurs = 1 _________ */
    BeatRepeatPtr getBeatRepeat() const;
    void setBeatRepeat(const BeatRepeatPtr &value);

    /* _________ Slash minOccurs = 1, maxOccurs = 1 _________ */
    SlashPtr getSlash() const;
    void setSlash(const SlashPtr &value);

  private:
    virtual bool fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement);

  private:
    Choice myChoice;
    MultipleRestPtr myMultipleRest;
    MeasureRepeatPtr myMeasureRepeat;
    BeatRepeatPtr myBeatRepeat;
    SlashPtr mySlash;
};
} // namespace core
} // namespace mx
