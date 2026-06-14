// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"

#ifdef MX_COMPILE_IMPL_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/core/generated/Fermata.h"
#include "mx/impl/PrintFunctions.h"

using namespace mx;
using namespace mx::impl;

TEST(getColor, PrintFunctions)
{
    core::Fermata attr;
    attr.setColor(core::Color{1, 2, 3});
    auto printData = impl::getPrintData(attr);
    auto colorData = printData.color;
    CHECK(printData.isColorSpecified);
    CHECK(!colorData.isAlphaSpecified);
    CHECK_EQUAL(static_cast<int>(1), static_cast<int>(colorData.red));
    CHECK_EQUAL(static_cast<int>(2), static_cast<int>(colorData.green));
    CHECK_EQUAL(static_cast<int>(3), static_cast<int>(colorData.blue));
}

T_END

TEST(getColorAlpha, PrintFunctions)
{
    core::Fermata attr;
    // Color(alpha, red, green, blue)
    attr.setColor(core::Color{1, 4, 3, 2});
    auto printData = impl::getPrintData(attr);
    auto colorData = printData.color;
    CHECK(printData.isColorSpecified);
    CHECK(colorData.isAlphaSpecified);
    CHECK_EQUAL(static_cast<int>(4), static_cast<int>(colorData.red));
    CHECK_EQUAL(static_cast<int>(3), static_cast<int>(colorData.green));
    CHECK_EQUAL(static_cast<int>(2), static_cast<int>(colorData.blue));
    CHECK_EQUAL(static_cast<int>(1), static_cast<int>(colorData.alpha));
}

T_END

TEST(getColorFalse, PrintFunctions)
{
    core::Fermata attr;
    // color not set — has_value() == false
    auto printData = impl::getPrintData(attr);
    auto colorData = printData.color;
    CHECK(!printData.isColorSpecified);
    CHECK(!colorData.isAlphaSpecified);
    CHECK_EQUAL(static_cast<int>(255), static_cast<int>(colorData.red));
    CHECK_EQUAL(static_cast<int>(255), static_cast<int>(colorData.green));
    CHECK_EQUAL(static_cast<int>(255), static_cast<int>(colorData.blue));
    CHECK_EQUAL(static_cast<int>(255), static_cast<int>(colorData.alpha));
}

T_END

TEST(setColor, PrintFunctions)
{
    api::ColorData color;
    color.red = 1;
    color.green = 2;
    color.blue = 3;
    color.isAlphaSpecified = false;
    color.alpha = 4;
    api::PrintData printData;
    printData.isColorSpecified = true;
    printData.color = color;
    core::Fermata attr;
    impl::setAttributesFromPrintData(printData, attr);
    CHECK(attr.color().has_value());
    const auto &coreColor = *attr.color();
    CHECK_EQUAL(1, static_cast<int>(coreColor.red()));
    CHECK_EQUAL(2, static_cast<int>(coreColor.green()));
    CHECK_EQUAL(3, static_cast<int>(coreColor.blue()));
    CHECK(!coreColor.alpha().has_value());
    CHECK_EQUAL(255, static_cast<int>(255));
}

T_END

TEST(setColorAlpha, PrintFunctions)
{
    api::ColorData color;
    color.red = 4;
    color.green = 3;
    color.blue = 2;
    color.isAlphaSpecified = true;
    color.alpha = 1;
    api::PrintData printData;
    printData.isColorSpecified = true;
    printData.color = color;
    core::Fermata attr;
    impl::setAttributesFromPrintData(printData, attr);
    CHECK(attr.color().has_value());
    const auto &coreColor = *attr.color();
    CHECK_EQUAL(4, static_cast<int>(coreColor.red()));
    CHECK_EQUAL(3, static_cast<int>(coreColor.green()));
    CHECK_EQUAL(2, static_cast<int>(coreColor.blue()));
    CHECK(coreColor.alpha().has_value());
    CHECK_EQUAL(1, static_cast<int>(*coreColor.alpha()));
}

T_END

TEST(setColorFalse, PrintFunctions)
{
    api::ColorData color;
    color.red = 4;
    color.green = 3;
    color.blue = 2;
    color.isAlphaSpecified = true;
    color.alpha = 1;
    api::PrintData printData;
    printData.isColorSpecified = false;
    printData.color = color;
    core::Fermata attr;
    impl::setAttributesFromPrintData(printData, attr);
    CHECK(!attr.color().has_value());
}

T_END

TEST(everythingSet, PrintFunctions)
{
    api::PrintData p;
    p.fontData.lineThrough = 1;
    p.fontData.overline = 2;
    p.fontData.underline = 3;
    p.fontData.sizeType = api::FontSizeType::point;
    p.fontData.sizePoint = 2.0;
    p.fontData.style = api::FontStyle::italic;
    p.fontData.weight = api::FontWeight::bold;
    p.fontData.fontFamily.emplace_back("z");
    p.fontData.fontFamily.emplace_back("ABC");
    core::Dynamics attr;
    impl::setAttributesFromPrintData(p, attr);
    CHECK(attr.lineThrough().has_value());
    CHECK_EQUAL(1, attr.lineThrough()->value());
    CHECK(attr.overline().has_value());
    CHECK_EQUAL(2, attr.overline()->value());
    CHECK(attr.underline().has_value());
    CHECK_EQUAL(3, attr.underline()->value());
    CHECK(attr.fontSize().has_value());
    CHECK(attr.fontSize()->isDecimal());
    CHECK_DOUBLES_EQUAL(2.0, attr.fontSize()->asDecimal().value(), 0.01);
    CHECK(attr.fontStyle().has_value());
    CHECK(core::FontStyle::italic() == *attr.fontStyle());
    CHECK(attr.fontWeight().has_value());
    CHECK(core::FontWeight::bold() == *attr.fontWeight());
    CHECK(attr.fontFamily().has_value());
    CHECK_EQUAL(2, attr.fontFamily()->items().size());
    CHECK_EQUAL("z", attr.fontFamily()->items().front());
    CHECK_EQUAL("ABC", attr.fontFamily()->items().back());
}

T_END

TEST(everythingGet, PrintFunctions)
{
    core::Dynamics attr;
    attr.setLineThrough(core::NumberOfLines{1});
    attr.setOverline(core::NumberOfLines{2});
    attr.setUnderline(core::NumberOfLines{3});
    attr.setFontSize(core::FontSize::decimal(core::Decimal{2.0}));
    attr.setFontStyle(core::FontStyle::italic());
    attr.setFontWeight(core::FontWeight::bold());
    attr.setFontFamily(core::FontFamily{{"z", "ABC"}});
    auto p = impl::getPrintData(attr);
    CHECK_EQUAL(1, p.fontData.lineThrough);
    CHECK_EQUAL(2, p.fontData.overline);
    CHECK_EQUAL(3, p.fontData.underline);
    CHECK(api::FontSizeType::point == p.fontData.sizeType);
    CHECK_DOUBLES_EQUAL(2.0, p.fontData.sizePoint, 0.01);
    CHECK(api::FontStyle::italic == p.fontData.style);
    CHECK(api::FontWeight::bold == p.fontData.weight);
    CHECK_EQUAL(2, p.fontData.fontFamily.size());
    CHECK_EQUAL("z", p.fontData.fontFamily.front());
    CHECK_EQUAL("ABC", p.fontData.fontFamily.back());
}

T_END

#endif
