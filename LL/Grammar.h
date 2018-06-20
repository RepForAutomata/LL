#pragma once
#include "GrammarRule.h"

class CGrammar
{
public:
	using Grammar = std::vector<Rule>;

	CGrammar();

	void ReadAndParseGrammar(std::istream& input);
	Grammar GetGrammar() const;

private:
	void ParseLine(const std::string& line, std::vector<std::string>& splitResult) const;
	long GetRuleIndex(const std::string& leftGrammarPart) const;

	Grammar m_grammar;
	boost::regex m_lineSplitRegex;
};
