// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/NoteheadText.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/AccidentalText.h"
#include "mx/core/elements/DisplayText.h"
#include "mx/core/elements/NoteheadTextChoice.h"
#include <iostream>

namespace mx
{
namespace core
{
NoteheadText::NoteheadText() : myNoteheadTextChoiceSet()
{
    myNoteheadTextChoiceSet.push_back(makeNoteheadTextChoice());
}

bool NoteheadText::hasAttributes() const
{
    return false;
}

std::ostream &NoteheadText::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &NoteheadText::streamName(std::ostream &os) const
{
    os << "notehead-text";
    return os;
}

bool NoteheadText::hasContents() const
{
    return true;
}

std::ostream &NoteheadText::streamContents(std::ostream &os, const int indentLevel, bool &isOneLineOnly) const
{
    if (myNoteheadTextChoiceSet.size() > 0)
    {
        for (auto x : myNoteheadTextChoiceSet)
        {
            os << std::endl;
            x->streamContents(os, indentLevel + 1, isOneLineOnly);
        }
        isOneLineOnly = false;
        os << std::endl;
    }
    else
    {
        isOneLineOnly = true;
    }
    return os;
}

const NoteheadTextChoiceSet &NoteheadText::getNoteheadTextChoiceSet() const
{
    return myNoteheadTextChoiceSet;
}

void NoteheadText::addNoteheadTextChoice(const NoteheadTextChoicePtr &value)
{
    if (value)
    {
        myNoteheadTextChoiceSet.push_back(value);
    }
}

void NoteheadText::removeNoteheadTextChoice(const NoteheadTextChoiceSetIterConst &value)
{
    if (value != myNoteheadTextChoiceSet.cend())
    {
        myNoteheadTextChoiceSet.erase(value);
    }
}

void NoteheadText::clearNoteheadTextChoiceSet()
{
    myNoteheadTextChoiceSet.clear();
    myNoteheadTextChoiceSet.push_back(makeNoteheadTextChoice());
}

NoteheadTextChoicePtr NoteheadText::getNoteheadTextChoice(const NoteheadTextChoiceSetIterConst &setIterator) const
{
    if (setIterator != myNoteheadTextChoiceSet.cend())
    {
        return *setIterator;
    }
    return NoteheadTextChoicePtr();
}

bool NoteheadText::fromXElementImpl(std::ostream &message, ::ezxml::XElement &xelement)
{
    bool isSuccess = true;
    bool isFirstItemAdded = false;

    auto endIter = xelement.end();
    for (auto it = xelement.begin(); it != endIter; ++it)
    {
        if (it->getName() == "display-text")
        {
            auto choice = makeNoteheadTextChoice();
            choice->setChoice(NoteheadTextChoice::Choice::displayText);
            isSuccess &= choice->getDisplayText()->fromXElement(message, *it);
            if (!isFirstItemAdded && myNoteheadTextChoiceSet.size() == 1)
            {
                *myNoteheadTextChoiceSet.begin() = choice;
                isFirstItemAdded = true;
            }
            else
            {
                myNoteheadTextChoiceSet.push_back(choice);
                isFirstItemAdded = true;
            }
            continue;
        }
        if (it->getName() == "accidental-text")
        {
            auto choice = makeNoteheadTextChoice();
            choice->setChoice(NoteheadTextChoice::Choice::accidentalText);
            isSuccess &= choice->getAccidentalText()->fromXElement(message, *it);
            if (!isFirstItemAdded && myNoteheadTextChoiceSet.size() == 1)
            {
                *myNoteheadTextChoiceSet.begin() = choice;
                isFirstItemAdded = true;
            }
            else
            {
                myNoteheadTextChoiceSet.push_back(choice);
                isFirstItemAdded = true;
            }
            continue;
        }
    }

    MX_RETURN_IS_SUCCESS;
}

} // namespace core
} // namespace mx
