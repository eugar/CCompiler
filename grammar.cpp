//
// Created by mmalett on 2/17/19.
//
#include "grammar.h"
#include <iostream>
#include <fstream>

void grammar::reader(std::vector<std::string> &pRuleIndex, // [in/out] used to index into the production rule list by order of placement
            ProductionRule &pRule, // [in/out] mapping of LHS to RHS
            const std::string& filename) // filename to read the grammar from
{
    std::ifstream fs(filename);
    if (!fs.is_open())
        throw "unable to open file";
    std::string line;
    while(getline(fs, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        std::string LHS;
        int start = 0;
        int length = 0;
        while (!isspace(line[start + length]) && line[start + length] != '~')
        {
            length++;
        }
        LHS = line.substr(start, length);
        pRuleIndex.push_back(LHS);

        if (!nextRule(line, start, length))
        {
            throw "Grammar is missing the right hand side";
        }

        std::vector<std::string> RHS;
        while (!isspace(line[start + length]))
        {
            while (line[start + length] != '|')
            {
                length++;
            }
            std::string rule(line.substr(start, length));

            RHS.push_back(rule);

            if (!nextRule(line, start, length))
            {
                break;
            }
        }
        pRule[LHS] = RHS;

    }

#ifdef DEBUG
    for (auto lhs : m_pRuleIndex)
    {
        string RHS;
        auto  RHSvec = m_pRule[lhs];
        int i = 1;
        int j = 0;
        for (auto rhs : RHSvec)
        {
            string rulenum = to_string(i) + string(". ");
            RHS += rulenum + rhs;
            i++;
        }
        cout << "LHS : " << m_pRuleIndex[j] << "  --->  RHS : " << RHS << endl ;
        j++;
    }
#endif
}

// Gets the next rule from the RHS
// returns true if it found more rules, false otherwise.
bool grammar::nextRule(std::string line, // [in]line to parse
              int &start, // [in/out]current starting point in line
              int &length) // [in/out]length of next word
{
    start = start + length;
    length = 0;
    auto it = line.begin() + start;
    while (!isalpha(line[start]))
    {
        it++;
        if ( it == line.end())
            return false;
        start++;

    }
    return true;
}

