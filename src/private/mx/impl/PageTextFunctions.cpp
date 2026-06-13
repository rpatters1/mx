// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/impl/PageTextFunctions.h"
#include "mx/core/generated/Credit.h"
#include "mx/core/generated/CreditChoice.h"
#include "mx/core/generated/CreditChoiceGroup.h"
#include "mx/core/generated/CreditChoiceGroupChoice.h"
#include "mx/core/generated/FormattedTextID.h"
#include "mx/core/generated/ScoreHeaderGroup.h"
#include "mx/impl/FontFunctions.h"
#include "mx/impl/PositionFunctions.h"

namespace mx
{
namespace impl
{
void createPageTextItems(const std::vector<api::PageTextData> &inPageTextItems, core::ScoreHeaderGroup &outHeader)
{
    for (const auto &p : inPageTextItems)
    {
        core::FormattedTextID words;
        words.setValue(p.text);
        impl::setAttributesFromFontData(p.fontData, words);
        impl::setAttributesFromPositionData(p.positionData, words);

        core::CreditChoiceGroupChoice groupChoice = core::CreditChoiceGroupChoice::creditWords(words);
        core::CreditChoiceGroup group;
        group.setChoice(groupChoice);

        core::Credit credit;
        credit.setChoice(core::CreditChoice::group(group));

        if (!p.description.empty())
        {
            credit.addCreditType(p.description);
        }

        if (p.pageNumber > 0)
        {
            credit.setPage(p.pageNumber);
        }

        outHeader.addCredit(credit);
    }
}

void createPageTextItems(const core::ScoreHeaderGroup &inHeader, std::vector<api::PageTextData> &outPageTextItems)
{
    for (const auto &c : inHeader.credit())
    {
        if (!c.choice().isGroup())
        {
            continue;
        }

        const auto &group = c.choice().asGroup();
        if (!group.choice().isCreditWords())
        {
            continue;
        }

        api::PageTextData pageText{};

        if (c.page().has_value())
        {
            pageText.pageNumber = *c.page();
        }

        const auto &words = group.choice().asCreditWords();
        pageText.text = words.value();
        pageText.positionData = impl::getPositionData(words);
        pageText.fontData = impl::getFontData(words);

        if (!c.creditType().empty())
        {
            pageText.description = c.creditType().front();
        }

        outPageTextItems.push_back(pageText);
    }
}
} // namespace impl
} // namespace mx
