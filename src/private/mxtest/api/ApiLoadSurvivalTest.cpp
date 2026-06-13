// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/control/CompileControl.h"
#ifdef MX_COMPILE_API_TESTS

#include "cpul/cpulTestHarness.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/MxFileTest.h"
#include "mxtest/file/MxFileTestGroup.h"

namespace mxtest
{
class ApiLoadSurvivalTest : public mxtest::MxFileTest
{
  public:
    ApiLoadSurvivalTest(mxtest::MxFile inTestFile, std::string inTestName, std::string inTestCppFileName,
                        int inTestCppFileLineNumber)
        : mxtest::MxFileTest(inTestFile, inTestName, inTestCppFileName, inTestCppFileLineNumber)
    {
    }

    inline void runTestCode()
    {
        if (isLoadFailureExpected())
        {
            setIsSuccess(true);
            return;
        }
        auto &docMgr = mx::api::DocumentManager::getInstance();
        const auto docIdResult = docMgr.createFromFile(testFilePath());
        if (!docIdResult.ok())
        {
            setIsSuccess(false);
            setFailureMessage("docMgr.createFromFile failed: " + docIdResult.error().message);
            return;
        }
        const int docId = docIdResult.value();
        const auto scoreDataResult = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        if (!scoreDataResult.ok())
        {
            setIsSuccess(false);
            setFailureMessage("docMgr.getData failed: " + scoreDataResult.error().message);
            return;
        }
        bool isSuccess = scoreDataResult.value().parts.size() > 0;
        setIsSuccess(isSuccess);
        setFailureMessage("scoreData has no parts");
    }
};

MxFileTestGroup<ApiLoadSurvivalTest> instance{MX_COMPILE_MAX_FILE_SIZE_BYTES, // maxFileSizeBytes
                                              "Api Load Survival Test", __FILE__, __LINE__};
} // namespace mxtest

#endif
