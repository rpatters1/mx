// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DocumentManager.h"
#include "mxtest/control/CompileControl.h"
#include "mxtest/file/MxFileRepository.h"
#include "mxtest/file/Path.h"
#include <sstream>

namespace mxtest
{
constexpr const char *const roundTripFileName = "k007a_Notations_Dynamics.xml";

inline void roundTrip()
{
    const std::string path{MxFileRepository::getFullPath(roundTripFileName)};
    auto &docMgr = mx::api::DocumentManager::getInstance();
    auto docIdResult = docMgr.createFromFile(path);
    if (!docIdResult.ok())
        return;
    auto docId = docIdResult.value();
    auto scoreDataResult = docMgr.getData(docId);
    docMgr.destroyDocument(docId);
    if (!scoreDataResult.ok())
        return;
    auto scoreData = scoreDataResult.value();
    auto docId2Result = docMgr.createFromScore(scoreData);
    if (!docId2Result.ok())
        return;
    auto docId2 = docId2Result.value();
    const std::string outputPath = getResourcesDirectoryPath() + "testOutput" + FILE_PATH_SEPARATOR + "output.xml";
    docMgr.writeToFile(docId2, outputPath);
    docMgr.destroyDocument(docId2);
}

inline mx::api::ScoreData roundTrip(const mx::api::ScoreData inScoreData)
{
    auto &docMgr = mx::api::DocumentManager::getInstance();
    auto docIdResult = docMgr.createFromScore(inScoreData);
    if (!docIdResult.ok())
        return {};
    auto docId = docIdResult.value();
    std::stringstream ss;
    docMgr.writeToStream(docId, ss);
    docMgr.destroyDocument(docId);
    auto xmlData = ss.str();
    std::istringstream iss{xmlData};
    auto docId2Result = docMgr.createFromStream(iss);
    if (!docId2Result.ok())
        return {};
    auto docId2 = docId2Result.value();
    auto outScoreDataResult = docMgr.getData(docId2);
    docMgr.destroyDocument(docId2);
    if (!outScoreDataResult.ok())
        return {};
    // std::cout << xmlData << std::endl;
    return outScoreDataResult.value();
}
} // namespace mxtest
