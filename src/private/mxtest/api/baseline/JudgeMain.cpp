// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Phase-2 Step-0 baseline judge (docs/ai/design/mx-impl-port-plan.md §8).
//
// Judges the OLD library's api-roundtrip outputs (written by
// CaptureDriver.cpp in a 71fc402 worktree) against their inputs with the
// NEW normalize-and-compare machinery -- the exact comparator the Phase-2
// corpus api-roundtrip gate uses (mxtest/corert/Compare + Fixer sidecars).
// The files judged PASS form the pinned baseline manifest
// (src/private/mxtest/api/roundtrip-baseline.txt).
//
// Build: cmake -DMX_BASELINE_JUDGE=on; target mxtest-baseline-judge.
//
// usage: mxtest-baseline-judge <dataRoot> <capturedDir> <statusFile>
//
// <statusFile> is the capture driver's stdout (status<TAB>relpath<TAB>msg).
// For every OK line, loads <dataRoot>/<relpath> as expected and
// <capturedDir>/<flattened relpath> as actual, normalizes both, applies the
// input's .fixup.xml sidecar to the expected side, compares, and prints one
// line per file: PASS|FAIL <tab> relpath <tab> detail. Non-OK capture rows
// are echoed as SKIP rows. Exit 0 (even with FAILs; the caller selects the
// PASS set).

#include "mxtest/corert/Compare.h"
#include "mxtest/corert/Fixer.h"

#include "pugixml/pugixml.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{

std::string flatten(const std::string &relPath)
{
    std::string flat = relPath;
    std::replace(flat.begin(), flat.end(), '/', '_');
    std::replace(flat.begin(), flat.end(), '\\', '_');
    return flat;
}

std::vector<std::string> splitTabs(const std::string &line)
{
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, '\t'))
    {
        out.push_back(field);
    }
    return out;
}

} // namespace

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "usage: mxtest-baseline-judge <dataRoot> <capturedDir> <statusFile>" << std::endl;
        return 2;
    }
    const std::string dataRoot = argv[1];
    const std::string capturedDir = argv[2];
    std::ifstream status(argv[3]);
    if (!status.is_open())
    {
        std::cerr << "cannot open status file: " << argv[3] << std::endl;
        return 2;
    }

    std::string line;
    while (std::getline(status, line))
    {
        if (line.empty())
        {
            continue;
        }
        const auto fields = splitTabs(line);
        if (fields.size() < 2)
        {
            continue;
        }
        const std::string &captureStatus = fields.at(0);
        const std::string &rel = fields.at(1);
        if (captureStatus != "OK")
        {
            std::cout << "SKIP\t" << rel << "\tcapture status " << captureStatus << std::endl;
            continue;
        }

        const std::string inputPath = dataRoot + "/" + rel;
        const std::string actualPath = capturedDir + "/" + flatten(rel);

        pugi::xml_document expectedDoc;
        if (!expectedDoc.load_file(inputPath.c_str(), pugi::parse_default | pugi::parse_doctype))
        {
            std::cout << "FAIL\t" << rel << "\tfailed to load expected input" << std::endl;
            continue;
        }
        pugi::xml_document actualDoc;
        if (!actualDoc.load_file(actualPath.c_str(), pugi::parse_default | pugi::parse_doctype))
        {
            std::cout << "FAIL\t" << rel << "\tfailed to load captured output" << std::endl;
            continue;
        }

        mxtest::corert::normalizeForComparison(expectedDoc);
        mxtest::corert::normalizeForComparison(actualDoc);
        mxtest::corert::Fixer fixer(inputPath);
        fixer.applyToExpected(expectedDoc);

        const auto fail = mxtest::corert::compareElements(expectedDoc.document_element(), actualDoc.document_element());
        if (fail.isFailure)
        {
            std::cout << "FAIL\t" << rel << "\t" << fail.detail << std::endl;
        }
        else
        {
            std::cout << "PASS\t" << rel << "\t" << std::endl;
        }
    }
    return 0;
}
