#include "stdafx.h"
#include "Grammar.h"

namespace
{
const std::string LINE_SPLIT_PATTERN = "->|( +)";
const std::string GUIDE_SET_SYMBOL = "/";
} // namespace

CGrammar::CGrammar()
	: m_lineSplitRegex(boost::regex(LINE_SPLIT_PATTERN))
{
}

void CGrammar::ReadAndParseGrammar(std::istream& input)
{
	std::string line;
	std::string leftGrammarPart;

	std::vector<std::string> parsedLine;
	while (getline(input, line))
	{
		ParseLine(line, parsedLine);
		leftGrammarPart = parsedLine.at(0);
		parsedLine.erase(parsedLine.begin());

		Rule::RightPart::Items items;
		Rule::RightPart::Guides guides;

		bool isItems = true;
		for (const auto& parsedElement : parsedLine)
		{
			if (parsedElement == GUIDE_SET_SYMBOL)
			{
				isItems = false;
				continue;
			}

			if (isItems)
			{
				items.emplace_back(parsedElement);
			}
			else
			{
				guides.emplace(parsedElement);
			}
		}

		Rule::RightPart rightPart;
		rightPart.items = items;
		rightPart.guides = guides;

		size_t pos = GetRuleIndex(leftGrammarPart);
		if (pos == -1)
		{
			Rule newRule;
			newRule.leftPart = leftGrammarPart;
			newRule.rightParts = Rule::RightParts(1, rightPart);

			m_grammar.push_back(newRule);
		}
		else
		{
			m_grammar.at(pos).rightParts.emplace_back(rightPart);
		}
	}
}

CGrammar::Grammar CGrammar::GetGrammar() const
{
	return m_grammar;
}

void CGrammar::ParseLine(const std::string& line, std::vector<std::string>& splitResult) const
{
	boost::algorithm::split_regex(splitResult, line, m_lineSplitRegex);
}

/* return index of first rule with leftGrammarPart or -1 if rule not exist */
long CGrammar::GetRuleIndex(const std::string& leftGrammarPart) const
{
	auto comparator = [&leftGrammarPart](const Rule& rule) {
		return rule.leftPart == leftGrammarPart;
	};
	auto result = std::find_if(m_grammar.begin(), m_grammar.end(), comparator);

	return (result != m_grammar.end()) ? std::distance(m_grammar.begin(), result) : -1;
}

