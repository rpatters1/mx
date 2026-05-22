// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/core/elements/MidiDeviceInstrumentGroup.h"
#include "mx/core/FromXElement.h"
#include "mx/core/elements/MidiDevice.h"
#include "mx/core/elements/MidiInstrument.h"
#include <iostream>

namespace mx
{
namespace core
{
MidiDeviceInstrumentGroup::MidiDeviceInstrumentGroup()
    : myMidiDevice(makeMidiDevice()), myHasMidiDevice(false), myMidiInstrument(makeMidiInstrument()),
      myHasMidiInstrument(false)
{
}

bool MidiDeviceInstrumentGroup::hasAttributes() const
{
    return false;
}

std::ostream &MidiDeviceInstrumentGroup::streamAttributes(std::ostream &os) const
{
    return os;
}

std::ostream &MidiDeviceInstrumentGroup::streamName(std::ostream &os) const
{
    return os;
}

bool MidiDeviceInstrumentGroup::hasContents() const
{
    return myHasMidiDevice || myHasMidiInstrument;
}

std::ostream &MidiDeviceInstrumentGroup::streamContents(std::ostream &os, const int indentLevel,
                                                        bool &isOneLineOnly) const
{
    bool firstItem = true;
    isOneLineOnly = true;
    if (myHasMidiDevice)
    {
        if (!firstItem)
            os << std::endl;
        myMidiDevice->toStream(os, indentLevel);
        firstItem = false;
    }
    if (myHasMidiInstrument)
    {
        if (!firstItem)
            os << std::endl;
        myMidiInstrument->toStream(os, indentLevel);
        firstItem = false;
    }
    isOneLineOnly = !hasContents();
    return os;
}

MidiDevicePtr MidiDeviceInstrumentGroup::getMidiDevice() const
{
    return myMidiDevice;
}

void MidiDeviceInstrumentGroup::setMidiDevice(const MidiDevicePtr &value)
{
    if (value)
    {
        myMidiDevice = value;
    }
}

bool MidiDeviceInstrumentGroup::getHasMidiDevice() const
{
    return myHasMidiDevice;
}

void MidiDeviceInstrumentGroup::setHasMidiDevice(const bool value)
{
    myHasMidiDevice = value;
}

MidiInstrumentPtr MidiDeviceInstrumentGroup::getMidiInstrument() const
{
    return myMidiInstrument;
}

void MidiDeviceInstrumentGroup::setMidiInstrument(const MidiInstrumentPtr &value)
{
    if (value)
    {
        myMidiInstrument = value;
    }
}

bool MidiDeviceInstrumentGroup::getHasMidiInstrument() const
{
    return myHasMidiInstrument;
}

void MidiDeviceInstrumentGroup::setHasMidiInstrument(const bool value)
{
    myHasMidiInstrument = value;
}

MX_FROM_XELEMENT_UNUSED(MidiDeviceInstrumentGroup);

} // namespace core
} // namespace mx
