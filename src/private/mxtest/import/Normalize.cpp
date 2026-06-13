// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mxtest/import/Normalize.h"

#include "mxtest/import/DecimalFields.h"

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace mxtest
{

namespace
{

bool isBlankText(pugi::xml_node node)
{
    if (node.type() != pugi::node_pcdata && node.type() != pugi::node_cdata)
    {
        return false;
    }
    const std::string_view text{node.value()};
    return text.find_first_not_of(" \t\r\n") == std::string_view::npos;
}

void stripWhitespaceRecursive(pugi::xml_node el)
{
    pugi::xml_node child = el.first_child();
    while (child)
    {
        pugi::xml_node next = child.next_sibling();
        if (isBlankText(child))
        {
            el.remove_child(child);
        }
        else if (child.type() == pugi::node_element)
        {
            stripWhitespaceRecursive(child);
        }
        child = next;
    }
}

void stripZerosRecursive(pugi::xml_node el)
{
    bool hasElementChildren = false;
    for (pugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == pugi::node_element)
        {
            hasElementChildren = true;
            break;
        }
    }
    if (!hasElementChildren && decimalFields.count(el.name()))
    {
        const std::string text = el.text().get();
        if (!text.empty())
        {
            el.text().set(stripZeros(text).c_str());
        }
    }
    for (pugi::xml_attribute a : el.attributes())
    {
        if (decimalFields.count(a.name()))
        {
            const std::string value = a.value();
            if (!value.empty())
            {
                a.set_value(stripZeros(value).c_str());
            }
        }
    }
    for (pugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == pugi::node_element)
        {
            stripZerosRecursive(c);
        }
    }
}

void sortAttributesRecursive(pugi::xml_node el)
{
    std::vector<std::pair<std::string, std::string>> attrs;
    for (pugi::xml_attribute a : el.attributes())
    {
        attrs.emplace_back(a.name(), a.value());
    }
    if (attrs.size() > 1)
    {
        std::sort(attrs.begin(), attrs.end());
        while (el.first_attribute())
        {
            el.remove_attribute(el.first_attribute());
        }
        for (const auto &[name, value] : attrs)
        {
            el.append_attribute(name.c_str()).set_value(value.c_str());
        }
    }
    for (pugi::xml_node c = el.first_child(); c; c = c.next_sibling())
    {
        if (c.type() == pugi::node_element)
        {
            sortAttributesRecursive(c);
        }
    }
}

} // namespace

void setRootMusicXmlVersion(pugi::xml_document &doc, const char *version)
{
    pugi::xml_node root = doc.document_element();
    if (!root)
    {
        return;
    }
    pugi::xml_attribute attr = root.attribute("version");
    if (!attr)
    {
        attr = root.append_attribute("version");
    }
    attr.set_value(version);
}

void stripInterElementWhitespace(pugi::xml_document &doc)
{
    if (doc.document_element())
    {
        stripWhitespaceRecursive(doc.document_element());
    }
}

void stripZerosFromDecimalFields(pugi::xml_document &doc)
{
    if (doc.document_element())
    {
        stripZerosRecursive(doc.document_element());
    }
}

void sortAttributes(pugi::xml_document &doc)
{
    if (doc.document_element())
    {
        sortAttributesRecursive(doc.document_element());
    }
}

} // namespace mxtest
