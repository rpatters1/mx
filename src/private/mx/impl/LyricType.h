// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/core/generated/Syllabic.h"

#include <string>

namespace mx
{
namespace impl
{
class LyricType
{

  public:
    explicit LyricType(const std::string &value, core::Syllabic syllabic);
    LyricType();

  private:
    std::string myText;
    core::Syllabic mySyllabic;
};
} // namespace impl
} // namespace mx
