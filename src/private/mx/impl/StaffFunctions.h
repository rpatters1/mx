// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/PartData.h"
#include "mx/api/StaffData.h"
#include "mx/core/generated/PartwiseMeasure.h"

#include <span>

namespace mx
{
namespace impl
{

void createStavesFromMx(int inGlobalTicksPerQuarter, std::span<const core::PartwiseMeasure> inMxMeasures,
                        std::vector<api::StaffData> &outStaves);
}
} // namespace mx
