// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileRepository.h"
#include <sstream>

TEST(Mxl, TemporaryNoCrashTest)
{
    const auto filepath = mxtest::MxFileRepository::getFullPath("Dichterliebe01.mxl");
    auto &docMgr = mx::api::DocumentManager::getInstance();

    // The new API does not throw; it returns Result<int> with an error code
    const auto result = docMgr.createFromFile(filepath);
    CHECK(!result.ok());
    if (!result.ok())
    {
        const auto &err = result.error();
        const auto msg = err.message;
        const std::string expectedMessage =
            "it looks like you are trying to parse a compressed musicxml file, which is currently unsupported.";
        const auto isFound = msg.find(expectedMessage) != std::string::npos;
        if (!isFound)
        {
            std::stringstream failure;
            failure << "expected to find the string '" << expectedMessage << "' in the error message '" << msg
                    << "' but did not.";
            FAIL(failure.str());
        }
    }
}

T_END

#endif
