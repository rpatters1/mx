// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mxtest/api/TestHelpers.h"

#include <string>

using namespace mx::api;

namespace mxtest::api::clef_data_test
{
ScoreData makeScoreWithClef(Bool printObject)
{
    ScoreData score;
    score.parts.emplace_back();
    auto &part = score.parts.back();
    part.measures.emplace_back();
    auto &measure = part.measures.back();
    measure.staves.emplace_back();
    auto &staff = measure.staves.back();

    staff.clefs.emplace_back();
    auto &clef = staff.clefs.back();
    clef.printObject = printObject;

    staff.voices[0].notes.emplace_back();
    return score;
}
} // namespace mxtest::api::clef_data_test

TEST(clefPrintObjectNoRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::no));
    CHECK(xml.find("print-object=\"no\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::no == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

TEST(clefPrintObjectYesRoundTrip, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::yes));
    CHECK(xml.find("print-object=\"yes\"") != std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::yes == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

TEST(clefPrintObjectUnspecifiedIsOmitted, ClefData)
{
    const auto xml = mxtest::toXml(mxtest::api::clef_data_test::makeScoreWithClef(Bool::unspecified));
    CHECK(xml.find("print-object=") == std::string::npos);

    const auto out = mxtest::fromXml(xml);
    CHECK_EQUAL(1, out.parts.size());
    CHECK_EQUAL(1, out.parts.front().measures.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.size());
    CHECK_EQUAL(1, out.parts.front().measures.front().staves.front().clefs.size());
    CHECK(Bool::unspecified == out.parts.front().measures.front().staves.front().clefs.front().printObject);
}

T_END;

#endif
