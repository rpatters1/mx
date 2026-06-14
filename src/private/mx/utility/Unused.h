// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// The old core's UnusedParameter.h macro, rehomed to mx::utility for the
// impl layer (the old header died with the old core).

#pragma once

#ifndef MX_UNUSED
#define MX_UNUSED(expr)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        (void)(expr);                                                                                                  \
    } while (0)
#endif
