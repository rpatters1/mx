// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/DynamicsReader.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/DynamicsChoice.h"
#include "mx/impl/Converter.h"
#include "mx/impl/MarkDataFunctions.h"

namespace mx
{
namespace impl
{

DynamicsReader::DynamicsReader(const core::Dynamics &inDynamic, impl::Cursor inCursor)
    : myDynamic{inDynamic}, myCursor{inCursor}
{
}

void DynamicsReader::parseDynamics(std::vector<api::MarkData> &outMarks) const
{
    const auto &choices = myDynamic.choice();
    if (choices.empty())
    {
        return;
    }

    const auto &firstChoice = choices.front();
    const auto kind = firstChoice.kind();
    Converter converter;
    const auto markType = converter.convertDynamic(kind);

    auto markData = api::MarkData{};
    markData.markType = markType;
    markData.tickTimePosition = myCursor.tickTimePosition;

    if (kind == core::DynamicsChoice::Kind::otherDynamics)
    {
        markData.name = firstChoice.asOtherDynamics().value();
    }
    else
    {
        markData.name = dynamicsKindToName(kind);
    }

    markData.positionData = impl::getPositionData(myDynamic);
    outMarks.emplace_back(std::move(markData));
}
} // namespace impl
} // namespace mx
