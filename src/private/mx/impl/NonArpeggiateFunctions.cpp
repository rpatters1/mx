// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/NonArpeggiateFunctions.h"
#include "mx/core/generated/NonArpeggiate.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

NonArpeggiateFunctions::NonArpeggiateFunctions(const core::NonArpeggiate &inNonArpeggiate, impl::Cursor inCursor)
    : myNonArpeggiate{inNonArpeggiate}, myCursor{inCursor}
{
}

api::MarkData NonArpeggiateFunctions::parseNonArpeggiate() const
{
    api::MarkData markData{api::MarkType::nonArpeggiate};
    impl::parseMarkDataAttributes(myNonArpeggiate, markData);
    markData.tickTimePosition = myCursor.tickTimePosition;
    return markData;
}
} // namespace impl
} // namespace mx
