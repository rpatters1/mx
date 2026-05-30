// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/corert/Fixer.h"

#include "ezxml/XAttribute.h"
#include "ezxml/XAttributeIterator.h"
#include "ezxml/XElement.h"
#include "ezxml/XElementIterator.h"
#include "ezxml/XFactory.h"

#include <filesystem>

namespace mxtest
{
namespace corert
{

namespace
{

constexpr const char *const kFixupSuffix = ".fixup.xml";
// The extension passed to `path::replace_extension`. It accepts the leading
// dot but treats the rest as the literal extension; `.fixup.xml` is
// effectively a multi-part extension that the function appends as-is.
constexpr const char *const kRootName = "fixups";
constexpr const char *const kReplaceName = "replace";
constexpr const char *const kTypeName = "type";
constexpr const char *const kNameName = "name";
constexpr const char *const kValueName = "value";
constexpr const char *const kReplacementValueName = "replacement-value";
constexpr const char *const kTypeElement = "element";
constexpr const char *const kTypeAttribute = "attribute";

bool parseReplace(const ::ezxml::XElement &replace, Fixup &out)
{
    bool gotType = false;
    bool gotName = false;
    bool gotValue = false;
    bool gotReplacement = false;
    for (auto it = replace.begin(); it != replace.end(); ++it)
    {
        const std::string &childName = it->getName();
        const std::string childValue = it->getValue();
        if (childName == kTypeName)
        {
            if (childValue == kTypeElement)
            {
                out.type = Fixup::Type::Element;
                gotType = true;
            }
            else if (childValue == kTypeAttribute)
            {
                out.type = Fixup::Type::Attribute;
                gotType = true;
            }
        }
        else if (childName == kNameName)
        {
            out.name = childValue;
            gotName = true;
        }
        else if (childName == kValueName)
        {
            out.value = childValue;
            gotValue = true;
        }
        else if (childName == kReplacementValueName)
        {
            out.replacementValue = childValue;
            gotReplacement = true;
        }
    }
    return gotType && gotName && gotValue && gotReplacement;
}

void applyToElement(::ezxml::XElement &element, const std::vector<Fixup> &fixups)
{
    const ::ezxml::XElementType etype = element.getType();

    if (etype == ::ezxml::XElementType::text)
    {
        const std::string name = element.getName();
        const std::string value = element.getValue();
        for (const auto &fix : fixups)
        {
            if (fix.type == Fixup::Type::Element && fix.name == name && fix.value == value)
            {
                element.setValue(fix.replacementValue);
                break;
            }
        }
    }
    else if (etype == ::ezxml::XElementType::empty)
    {
        // Empty leaves (no text, no children) are treated as text with value
        // "". Only match Element fixups whose own `value` is empty; matching
        // arbitrary fixup values would spuriously rewrite intentionally-empty
        // marker elements (e.g. <fermata/>) that happen to share a name.
        const std::string name = element.getName();
        for (const auto &fix : fixups)
        {
            if (fix.type == Fixup::Type::Element && fix.name == name && fix.value.empty())
            {
                element.setValue(fix.replacementValue);
                break;
            }
        }
    }

    for (auto attrIt = element.attributesBegin(); attrIt != element.attributesEnd(); ++attrIt)
    {
        const std::string aName = attrIt->getName();
        const std::string aValue = attrIt->getValue();
        for (const auto &fix : fixups)
        {
            if (fix.type == Fixup::Type::Attribute && fix.name == aName && fix.value == aValue)
            {
                attrIt->setValue(fix.replacementValue);
                break;
            }
        }
    }

    if (etype == ::ezxml::XElementType::element)
    {
        for (auto childIt = element.begin(); childIt != element.end(); ++childIt)
        {
            applyToElement(*childIt, fixups);
        }
    }
}

} // namespace

Fixer::Fixer(const std::string &inputFilePath)
{
    // Sidecar lives next to the input with the `.xml` or `.musicxml`
    // extension replaced by `.fixup.xml`. Example:
    // `data/foo/example.xml` -> `data/foo/example.fixup.xml`.
    std::filesystem::path inputPath(inputFilePath);
    const std::filesystem::path fixupPath = inputPath.replace_extension(kFixupSuffix);
    std::error_code ec;
    if (!std::filesystem::exists(fixupPath, ec))
    {
        return;
    }

    auto xdoc = ::ezxml::XFactory::makeXDoc();
    xdoc->loadFile(fixupPath.string());
    const auto root = xdoc->getRoot();
    if (!root || root->getName() != kRootName)
    {
        return;
    }

    for (auto it = root->begin(); it != root->end(); ++it)
    {
        if (it->getName() != kReplaceName)
        {
            continue;
        }
        Fixup fix;
        if (parseReplace(*it, fix))
        {
            myFixups.push_back(std::move(fix));
        }
    }
}

bool Fixer::hasFixups() const
{
    return !myFixups.empty();
}

void Fixer::applyToExpected(const ::ezxml::XDocPtr &expectedXDoc) const
{
    if (myFixups.empty() || !expectedXDoc)
    {
        return;
    }
    const auto root = expectedXDoc->getRoot();
    if (!root)
    {
        return;
    }
    applyToElement(*root, myFixups);
}

} // namespace corert
} // namespace mxtest
