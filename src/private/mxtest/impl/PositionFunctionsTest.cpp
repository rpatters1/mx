// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/generated/Bracket.h"
#include "mx/core/generated/Direction.h"
#include "mx/impl/PositionFunctions.h"

using namespace mx;
using namespace mx::impl;

TEST(setAttributesFromPositionDataBracket, PositionFunctions)
{
    core::Bracket attr;
    api::PositionData positionData;

    positionData.isDefaultXSpecified = true;
    positionData.defaultX = 0.1;
    positionData.isDefaultYSpecified = true;
    positionData.defaultY = 0.2;

    positionData.isRelativeXSpecified = true;
    positionData.relativeX = 0.3;
    positionData.isRelativeYSpecified = true;
    positionData.relativeY = 0.4;

    impl::setAttributesFromPositionData(positionData, attr);

    CHECK(attr.defaultX().has_value());
    CHECK_DOUBLES_EQUAL(0.1, attr.defaultX()->value().value(), 0.0001);

    CHECK(attr.defaultY().has_value());
    CHECK_DOUBLES_EQUAL(0.2, attr.defaultY()->value().value(), 0.0001);

    CHECK(attr.relativeX().has_value());
    CHECK_DOUBLES_EQUAL(0.3, attr.relativeX()->value().value(), 0.0001);

    CHECK(attr.relativeY().has_value());
    CHECK_DOUBLES_EQUAL(0.4, attr.relativeY()->value().value(), 0.0001);
}

T_END

class TotallyBogusClassToTestSetAttributesFromPositionData
{
};

TEST(setAttributesFromPositionDataFakeClass, PositionFunctions)
{
    TotallyBogusClassToTestSetAttributesFromPositionData fake;
    api::PositionData positionData;
    impl::setAttributesFromPositionData(positionData, fake);

    // only check that it compiles
    CHECK(true);
}

T_END

TEST(setAttributesFromPositionDataDirectionAttributes, PositionFunctions)
{
    core::Direction attr;
    api::PositionData positionData;

    positionData.placement = api::Placement::below;

    impl::setAttributesFromPositionData(positionData, attr);

    CHECK(attr.placement().has_value());
    CHECK(attr.placement() == core::AboveBelow::below());
}

T_END

#endif
