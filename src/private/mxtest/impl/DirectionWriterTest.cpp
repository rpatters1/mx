// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/OttavaData.h"
#include "mx/core/generated/Direction.h"
#include "mx/core/generated/DirectionType.h"
#include "mx/core/generated/MusicDataChoice.h"
#include "mx/core/generated/OctaveShift.h"
#include "mx/impl/DirectionWriter.h"

#include <memory>

using namespace mx;
using namespace mx::impl;

TEST(ottavaStartStop, DirectionWriter)
{
    Cursor cursor{1, 100};
    cursor.isFirstMeasureInPart = false;
    api::DirectionData directionData;

    directionData.ottavaStops.emplace_back(api::SpannerStop{});
    auto &stop = directionData.ottavaStops.back();
    stop.numberLevel = 2;

    directionData.ottavaStarts.emplace_back(api::OttavaStart{});
    auto &start = directionData.ottavaStarts.back();
    start.ottavaType = api::OttavaType::o15mb;
    start.spannerStart.positionData.isDefaultXSpecified = true;
    start.spannerStart.positionData.defaultX = 100.0;

    DirectionWriter writer{directionData, cursor};
    const auto mdcSet = writer.getDirectionLikeThings();
    CHECK(mdcSet.front().isDirection());
    const auto &direction = mdcSet.front().asDirection();
    const auto &directionTypes = direction.directionType();

    CHECK_EQUAL(2, directionTypes.size());
}

T_END

#endif
