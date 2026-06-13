// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/core/generated/ArticulationsChoice.h"
#include "mx/impl/Cursor.h"

#include <span>

namespace mx
{
namespace impl
{
class ArticulationsFunctions
{
  public:
    ArticulationsFunctions(std::span<const core::ArticulationsChoice> inArticulations, impl::Cursor inCursor);
    ~ArticulationsFunctions() = default;
    ArticulationsFunctions(const ArticulationsFunctions &) = delete;
    ArticulationsFunctions(ArticulationsFunctions &&) = delete;
    ArticulationsFunctions &operator=(const ArticulationsFunctions &) = delete;
    ArticulationsFunctions &operator=(ArticulationsFunctions &&) = delete;

    void parseArticulations(std::vector<api::MarkData> &outMarks) const;

  private:
    std::span<const core::ArticulationsChoice> myArticulations;
    const impl::Cursor myCursor;

  private:
    void parseArticulation(const core::ArticulationsChoice &inArticulation, api::MarkData &outMark) const;
};
} // namespace impl
} // namespace mx
