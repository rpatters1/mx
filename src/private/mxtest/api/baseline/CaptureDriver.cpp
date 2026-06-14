// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

// Phase-2 Step-0 baseline capture driver (docs/ai/design/mx-impl-port-plan.md §8).
//
// This program compiles against OLD master (71fc402) -- the pre-port mx
// library with its throwing DocumentManager -- NOT against this tree. It is
// committed here as the record of how src/private/mxtest/api/
// roundtrip-baseline.txt was captured (this directory is excluded from the
// MX_API build in CMakeLists.txt).
//
// Capture procedure (run inside a scratch worktree of 71fc402):
//
//   cmake -S . -B build/lib -G Ninja -DCMAKE_BUILD_TYPE=Release -DMX_BUILD_TESTS=OFF
//   ninja -C build/lib mx
//   g++ -std=c++17 -O2 -I src/include -o build/capture CaptureDriver.cpp \
//       build/lib/libmx.a
//   build/capture <dataRoot> <outDir> [startIndex]
//
// For every eligible corpus file (same discovery rules as corert: .xml or
// .musicxml under <dataRoot>, excluding the directories expected/testOutput/
// generalxml/smufl, *.fixup.xml sidecars, and files with a .invalid marker)
// it runs the old library's honest api roundtrip:
//
//   createFromFile -> getData -> createFromScore -> writeToFile
//
// writing the output document to <outDir>/<relative-path-with-slashes-as-_>
// and one status line per file to stdout:
//
//   OK|LOADFAIL|GETDATAFAIL|CREATEFAIL|WRITEFAIL <tab> relpath <tab> message
//
// The OK outputs are then judged against their inputs with the NEW tree's
// normalize-and-compare machinery (mxtest-apirt-judge); the files whose old
// output survives that judgment ARE the pinned baseline manifest.

#include "mx/api/DocumentManager.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace
{

namespace fs = std::filesystem;

bool isExcludedPath(const fs::path &relative)
{
    static const std::vector<std::string> excludedSegments = {"expected", "testOutput", "generalxml", "smufl"};
    for (const auto &part : relative)
    {
        const std::string seg = part.string();
        for (const auto &ex : excludedSegments)
        {
            if (seg == ex)
            {
                return true;
            }
        }
    }
    return false;
}

bool hasXmlExtension(const fs::path &p)
{
    const std::string ext = p.extension().string();
    return ext == ".xml" || ext == ".musicxml";
}

bool isFixupSidecar(const fs::path &p)
{
    const std::string filename = p.filename().string();
    const std::string suffix = ".fixup.xml";
    if (filename.size() < suffix.size())
    {
        return false;
    }
    return filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool hasInvalidMarker(const fs::path &p)
{
    fs::path marker = p;
    marker += ".invalid";
    std::error_code ec;
    return fs::exists(marker, ec);
}

std::vector<std::string> discoverInputFiles(const std::string &dataRoot)
{
    std::vector<std::string> result;
    const fs::path root(dataRoot);
    if (!fs::exists(root))
    {
        return result;
    }
    for (auto it = fs::recursive_directory_iterator(root); it != fs::recursive_directory_iterator(); ++it)
    {
        const auto &entry = *it;
        if (!entry.is_regular_file())
        {
            continue;
        }
        const fs::path &p = entry.path();
        const fs::path relative = fs::relative(p, root);
        if (isExcludedPath(relative) || !hasXmlExtension(p) || isFixupSidecar(p) || hasInvalidMarker(p))
        {
            continue;
        }
        result.push_back(p.string());
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::string flatten(const std::string &relPath)
{
    std::string flat = relPath;
    std::replace(flat.begin(), flat.end(), '/', '_');
    std::replace(flat.begin(), flat.end(), '\\', '_');
    return flat;
}

void report(const char *status, const std::string &relPath, const std::string &message = "")
{
    std::cout << status << "\t" << relPath << "\t" << message << std::endl;
}

} // namespace

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "usage: capture <dataRoot> <outDir> [startIndex]" << std::endl;
        return 2;
    }
    const std::string dataRoot = argv[1];
    const fs::path outDir = argv[2];
    const size_t startIndex = argc > 3 ? static_cast<size_t>(std::atol(argv[3])) : 0;

    fs::create_directories(outDir);
    const auto files = discoverInputFiles(dataRoot);
    auto &mgr = mx::api::DocumentManager::getInstance();

    for (size_t i = startIndex; i < files.size(); ++i)
    {
        const std::string &path = files.at(i);
        const std::string rel = fs::relative(fs::path(path), fs::path(dataRoot)).generic_string();
        std::cerr << "[" << i << "/" << files.size() << "] " << rel << std::endl;

        int loadId = -1;
        try
        {
            loadId = mgr.createFromFile(path);
        }
        catch (const std::exception &e)
        {
            report("LOADFAIL", rel, e.what());
            continue;
        }
        catch (...)
        {
            report("LOADFAIL", rel, "unknown exception");
            continue;
        }

        mx::api::ScoreData score;
        try
        {
            score = mgr.getData(loadId);
            mgr.destroyDocument(loadId);
        }
        catch (const std::exception &e)
        {
            mgr.destroyDocument(loadId);
            report("GETDATAFAIL", rel, e.what());
            continue;
        }
        catch (...)
        {
            mgr.destroyDocument(loadId);
            report("GETDATAFAIL", rel, "unknown exception");
            continue;
        }

        int outId = -1;
        try
        {
            outId = mgr.createFromScore(score);
        }
        catch (const std::exception &e)
        {
            report("CREATEFAIL", rel, e.what());
            continue;
        }
        catch (...)
        {
            report("CREATEFAIL", rel, "unknown exception");
            continue;
        }

        try
        {
            const fs::path outPath = outDir / flatten(rel);
            mgr.writeToFile(outId, outPath.string());
            mgr.destroyDocument(outId);
        }
        catch (const std::exception &e)
        {
            mgr.destroyDocument(outId);
            report("WRITEFAIL", rel, e.what());
            continue;
        }
        catch (...)
        {
            mgr.destroyDocument(outId);
            report("WRITEFAIL", rel, "unknown exception");
            continue;
        }

        report("OK", rel);
    }

    return 0;
}
