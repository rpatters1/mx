// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/MarkData.h"
#include "mx/core/generated/Dynamics.h"
#include "mx/impl/Converter.h"
#include "mx/impl/Cursor.h"

namespace mx
{
namespace impl
{
class DynamicsWriter
{
  public:
    DynamicsWriter(const api::MarkData &inMark, impl::Cursor inCursor);
    DynamicsWriter(const api::MarkData &inMark, impl::Cursor inCursor, api::Placement directionPlacement);
    core::Dynamics getDynamics() const;

  private:
    const api::MarkData &myMarkData;
    const impl::Cursor myCursor;
    const Converter myConverter;
};
} // namespace impl
} // namespace mx
