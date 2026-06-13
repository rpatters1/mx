// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include "mx/api/DocumentManager.h"
#include "mx/utility/Throw.h"
#include <sstream>

namespace mxtest
{
inline std::string toXml(const mx::api::ScoreData &inScoreData)
{
    using namespace mx::api;
    auto &docMgr = DocumentManager::getInstance();
    const auto docIdResult = docMgr.createFromScore(inScoreData);
    if (!docIdResult.ok())
        return {};
    const int docId = docIdResult.value();
    std::stringstream ss;
    docMgr.writeToStream(docId, ss);
    docMgr.destroyDocument(docId);
    const auto xml = ss.str();
    return xml;
}

inline mx::api::ScoreData fromXml(const std::string &inXml)
{
    using namespace mx::api;
    auto &docMgr = DocumentManager::getInstance();
    std::istringstream iss{inXml};
    const auto docIdResult = docMgr.createFromStream(iss);
    if (!docIdResult.ok())
        return {};
    const int docId = docIdResult.value();
    const auto scoreResult = docMgr.getData(docId);
    docMgr.destroyDocument(docId);
    if (!scoreResult.ok())
        return {};
    return scoreResult.value();
}
} // namespace mxtest
