// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/NoteData.h"
#include "mx/core/generated/Note.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

#include <mutex>

namespace mx
{
namespace impl
{
class NoteReader;

class NoteFunctions
{
  public:
    NoteFunctions(const core::Note &inMxNote, impl::Cursor cursor);
    ~NoteFunctions() = default;
    api::NoteData parseNote() const;

  private:
    const core::Note &myNote;
    const impl::Cursor myCursor;
    mutable api::NoteData myOutNoteData;
    mutable std::mutex myMutex;

  private:
    api::DurationName deriveNoteTypeFromDurationValue(const NoteReader &reader) const;
    void parseNotations() const;
    void parseMiscData() const;
};
} // namespace impl
} // namespace mx
