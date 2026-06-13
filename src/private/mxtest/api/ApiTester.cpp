
#include "mxtest/api/ApiTester.h"
#include "mx/api/DocumentManager.h"
#include "mxtest/file/StupidFileFunctions.h"
#include "pugixml/pugixml.hpp"

#include <sstream>

namespace mxtest
{
ApiTester::ApiTester(ScoreDataCreatorPtr &&inScoreDataCreator, MxFile inFile, std::string cppFileName,
                     int cppFileLineNumber)
    : MxFileTest{inFile, "ApiRoundTripTest", cppFileName, cppFileLineNumber},
      myScoreDataCreator{std::move(inScoreDataCreator)}, myIntermediateFilePath{}, myScrubbedFilePath{},
      myExpectedFilePath{}, myFinalFilePath{}
{
    std::stringstream ss;
    ss << "./apitester." << testFileNamePart() << ".intermediate.xml";
    myIntermediateFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".input.xml";
    myScrubbedFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".expected.xml";
    myExpectedFilePath = ss.str();

    ss.str("");
    ss << "./apitester." << testFileNamePart() << ".final.xml";
    myFinalFilePath = ss.str();

    deleteFiles();
}

void ApiTester::runTestCode()
{
    using namespace mx::api;
    const auto expectedScoreData = myScoreDataCreator->createScoreData();

    // load the data from disk
    auto &docMgr = DocumentManager::getInstance();
    const auto initialLoadDocIdResult = docMgr.createFromFile(testFilePath());
    if (!initialLoadDocIdResult.ok())
    {
        setIsSuccess(false);
        setFailureMessage("createFromFile failed: " + initialLoadDocIdResult.error().message);
        return;
    }
    const int initialLoadDocId = initialLoadDocIdResult.value();
    const auto initialLoadScoreDataResult = docMgr.getData(initialLoadDocId);
    if (!initialLoadScoreDataResult.ok())
    {
        docMgr.destroyDocument(initialLoadDocId);
        setIsSuccess(false);
        setFailureMessage("getData failed: " + initialLoadScoreDataResult.error().message);
        return;
    }
    const auto initialLoadScoreData = initialLoadScoreDataResult.value();

    // save what we loaded back to disk
    const auto initialScoreDataDocIdResult = docMgr.createFromScore(initialLoadScoreData);
    if (!initialScoreDataDocIdResult.ok())
    {
        docMgr.destroyDocument(initialLoadDocId);
        setIsSuccess(false);
        setFailureMessage("createFromScore failed");
        return;
    }
    const int initialScoreDataDocId = initialScoreDataDocIdResult.value();

    // save the 'intermediate' ScoreData
    docMgr.writeToFile(initialScoreDataDocId, myIntermediateFilePath);

    // load what what we just saved back up into memory
    const auto intermediateFileLoadDocIdResult = docMgr.createFromFile(myIntermediateFilePath);
    if (!intermediateFileLoadDocIdResult.ok())
    {
        docMgr.destroyDocument(initialLoadDocId);
        docMgr.destroyDocument(initialScoreDataDocId);
        setIsSuccess(false);
        setFailureMessage("createFromFile(intermediate) failed");
        return;
    }
    const int intermediateFileLoadDocId = intermediateFileLoadDocIdResult.value();
    const auto actualScoreDataResult = docMgr.getData(intermediateFileLoadDocId);
    if (!actualScoreDataResult.ok())
    {
        docMgr.destroyDocument(initialLoadDocId);
        docMgr.destroyDocument(initialScoreDataDocId);
        docMgr.destroyDocument(intermediateFileLoadDocId);
        setIsSuccess(false);
        setFailureMessage("getData(intermediate) failed");
        return;
    }
    const auto actualScoreData = actualScoreDataResult.value();

    const bool areEqual = (expectedScoreData == actualScoreData);
    if (areEqual)
    {
        // test was successful, return without registering a failure
        setIsSuccess(true);
        docMgr.destroyDocument(initialLoadDocId);
        docMgr.destroyDocument(initialScoreDataDocId);
        docMgr.destroyDocument(intermediateFileLoadDocId);
        deleteFiles();
        return;
    }

    // test was not successful
    setIsSuccess(false);
    setFailureMessage("after round-trips to disk the ScoreData objects were not equal");

    // save a "scrubbed" version of the input via pugixml round-trip
    pugi::xml_document pDoc;
    if (pDoc.load_file(testFilePath().c_str()))
    {
        pDoc.save_file(myScrubbedFilePath.c_str());
    }

    // save the 'expected' ScoreData
    const auto expectedScoreDataDocIdResult = docMgr.createFromScore(expectedScoreData);
    if (expectedScoreDataDocIdResult.ok())
    {
        const int expectedScoreDataDocId = expectedScoreDataDocIdResult.value();
        docMgr.writeToFile(expectedScoreDataDocId, myExpectedFilePath);
        docMgr.destroyDocument(expectedScoreDataDocId);
    }

    // save the 'actual' ScoreData
    const auto finalDocIdResult = docMgr.createFromScore(actualScoreData);
    if (finalDocIdResult.ok())
    {
        const int finalDocId = finalDocIdResult.value();
        docMgr.writeToFile(finalDocId, myFinalFilePath);
        docMgr.destroyDocument(finalDocId);
    }

    docMgr.destroyDocument(initialLoadDocId);
    docMgr.destroyDocument(initialScoreDataDocId);
    docMgr.destroyDocument(intermediateFileLoadDocId);
}

void ApiTester::deleteFiles() const
{
    deleteFileNoThrow(myIntermediateFilePath);
    deleteFileNoThrow(myScrubbedFilePath);
    deleteFileNoThrow(myExpectedFilePath);
    deleteFileNoThrow(myFinalFilePath);
}
} // namespace mxtest
