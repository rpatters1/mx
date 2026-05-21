// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "ezxml/XDoc.h"
#include "ezxml/XElement.h"

#include <string>
#include <vector>

namespace mxtest
{
namespace corert
{

// Result of running the core roundtrip flow on a single file.
struct CoreRoundtripResult
{
    bool ok = false;
    std::string message;
    // Populated on failure (and only on failure) so the caller can diff. Each
    // is the corresponding XDoc serialized to a string after the full
    // normalization pipeline has been applied.
    std::string expectedXml;
    std::string actualXml;
};

// Discover all eligible MusicXML inputs under `data/`.
//
// Scans `MX_REPO_ROOT_PATH "/data"` recursively, collects regular files with
// extension `.xml` or `.musicxml`, and excludes any path containing a
// directory segment named `expected`, `testOutput`, `generalxml`, or `smufl`.
// Returns absolute paths.
//
// Not used by the Phase B hardcoded test case; declared here so Phase C can
// pick it up without further header changes.
std::vector<std::string> discoverInputFiles();

// Convert an absolute path under `data/` into the relative test name used as
// the Catch2 case name (e.g., `lysuite/ly01a_Pitches_Pitches.xml`). If the
// path is not under the data root the absolute path is returned unchanged.
std::string toTestName(const std::string &absolutePath);

// Run the per-file core roundtrip flow described in design §3.
//
// Steps: load via ezxml; set version attribute on root; fromXDoc into a
// mx::core::Document; toXDoc back out; normalize both the actual and a
// fresh-loaded expected; depth-first compare. Result is a structured value;
// the caller is responsible for emitting Catch2 FAIL / passing assertions and
// writing debug files. The function does not throw — any exception from the
// pipeline is caught and reported via `CoreRoundtripResult::message`.
CoreRoundtripResult runCoreRoundtrip(const std::string &absoluteInputPath);

// Write a pair of `.expected.xml` / `.actual.xml` debug files to
// `data/testOutput/corert/`. Creates the directory if it does not exist.
// `testName` is the path relative to `data/`; directory separators are
// replaced with `_` to flatten into a single directory. Both strings are the
// already-normalized XML payloads.
void writeFailureFiles(const std::string &testName, const std::string &expectedXml, const std::string &actualXml);

} // namespace corert
} // namespace mxtest
