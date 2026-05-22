// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/BeatUnitPerOrNoteRelationNoteChoice.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/BeatUnitPer.h"
#include "mx/core/elements/NoteRelationNote.h"
#include <iostream>

namespace mx
{
namespace core
{
BeatUnitPerOrNoteRelationNoteChoice::BeatUnitPerOrNoteRelationNoteChoice()
    : myChoice(Choice::beatUnitPer), myBeatUnitPer(makeBeatUnitPer()), myNoteRelationNote(makeNoteRelationNote())
{
}

bool BeatUnitPerOrNoteRelationNoteChoice::hasAttributes() const
{
    return false;
}

std::ostream &BeatUnitPerOrNoteRelationNoteChoice::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &BeatUnitPerOrNoteRelationNoteChoice::streamName(std::ostream &os) const
{
    return os;
}

bool BeatUnitPerOrNoteRelationNoteChoice::hasContents() const
{
    return true;
}

std::ostream &BeatUnitPerOrNoteRelationNoteChoice::streamContents(std::ostream &os, const int indentLevel,
                                                                  bool &isOneLineOnly) const
{
    if (myChoice == Choice::beatUnitPer)
    {
        myBeatUnitPer->streamContents(os, indentLevel, isOneLineOnly);
    }
    if (myChoice == Choice::noteRelationNote)
    {
        myNoteRelationNote->streamContents(os, indentLevel, isOneLineOnly);
    }
    isOneLineOnly = false;
    return os;
}

BeatUnitPerOrNoteRelationNoteChoice::Choice BeatUnitPerOrNoteRelationNoteChoice::getChoice() const
{
    return myChoice;
}

void BeatUnitPerOrNoteRelationNoteChoice::setChoice(const Choice value)
{
    myChoice = value;
}

BeatUnitPerPtr BeatUnitPerOrNoteRelationNoteChoice::getBeatUnitPer() const
{
    return myBeatUnitPer;
}

void BeatUnitPerOrNoteRelationNoteChoice::setBeatUnitPer(const BeatUnitPerPtr &value)
{
    if (value)
    {
        myBeatUnitPer = value;
    }
}

NoteRelationNotePtr BeatUnitPerOrNoteRelationNoteChoice::getNoteRelationNote() const
{
    return myNoteRelationNote;
}

void BeatUnitPerOrNoteRelationNoteChoice::setNoteRelationNote(const NoteRelationNotePtr &value)
{
    if (value)
    {
        myNoteRelationNote = value;
    }
}

MX_FROM_XELEMENT_UNUSED(BeatUnitPerOrNoteRelationNoteChoice);

} // namespace core
} // namespace mx
