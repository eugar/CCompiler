//
// A header for global definitions
//
#ifndef CCOMPILER_DEFINITIONS_H
#define CCOMPILER_DEFINITIONS_H

#include <map>
#include <vector>
#include <string>
typedef struct Lr1Item
{
    Lr1Item() = default;
    Lr1Item(std::string lhs, std::string rhs, std::string lookAhead = "", size_t state = states, size_t phPos = 0, size_t fromState = 0, std::string gotoToken = "")
    {
        this->lhs = lhs;
        this->rhs = rhs;
        this->lookAhead = lookAhead;
        this->phPos = phPos;
        this->state = state;
        this->fromState = fromState;
        this->gotoToken = gotoToken;
    }
    // new follow constructor
    Lr1Item(Lr1Item *item, std::string lookAhead)
    {
        lhs = item->lhs;
        rhs = item->rhs;
        phPos = item->phPos;
        state = item->state;
        fromState = item->fromState;
        gotoToken = item->gotoToken;
        this->lookAhead = lookAhead;
    }

    Lr1Item *getByLhs(std::string lhs)
    {
        if (this->lhs == lhs)
        {
            return this;
        }
        return nullptr;
    }

    std::string handleFollow() const
    {
        size_t afterHandlePos = rhs.find(' ', phPos + 1);
        if (afterHandlePos != std::string::npos )
        {
            size_t end = rhs.find(' ', afterHandlePos + 1);
            if (end == std::string::npos)
            {
                end = rhs.size();
            }
            return rhs.substr(afterHandlePos + 1, end);
        }
    }

    // Returns true if the lhs (the lr1item in usage) is empty.
    bool empty()
    {
        return lhs.size() <= 0;
    }

    // Returns true if the ph is at the end of the rhs string.
    const bool phAtEnd() const
    {
        return phPos >= (rhs.size() - 1);
    }

    // Returns a new Lr1Item with the ph advanced to the next position
    // if possible, returns an empty Lr1Item otherwise
    Lr1Item advancePh(std::map<std::pair<std::string, size_t >, size_t> &stateMap)
    {
        Lr1Item newLr1Item;
        if (phAtEnd())
        {
            // return empty lr1item
            return Lr1Item();
        }

        size_t newPhPos = 0;
        size_t newPh;
        if ((newPhPos = rhs.find(' ', phPos + 1)) == std::string::npos)
        {
            newPhPos = rhs.size() - 1;
            newPh = newPhPos + 1;
        }
        else
        {
            newPh = newPhPos;
        }
        size_t afterPh = phPos;
        if (phPos > 0)
        {
            afterPh++;
        }



        std::string token = rhs.substr(afterPh, newPh - afterPh);
        size_t nState;
        auto st = stateMap.find(make_pair(token, state));
        if (st == stateMap.end())
        {
            stateMap[make_pair(token, state)] = ++states;

        }
         return Lr1Item(lhs, rhs, lookAhead, stateMap[make_pair(token, state)], newPhPos, state, token);
    }

    std::string lhs;
    std::string rhs;
    std::string lookAhead;
    size_t phPos;
    size_t fromState;
    size_t state;
    std::string gotoToken;
    static size_t states;

}Lr1Item;

struct Lr1Compare
{
    bool operator() (const Lr1Item& a, const Lr1Item& b) const
    {
        //bool com = (a.lhs != b.lhs) || (a.rhs != b.rhs) || (a.lookAhead != b.lookAhead) || (a.phPos != b.phPos);
        bool com = std::tie(a.lhs, a.rhs, a.lookAhead, a.phPos) < std::tie(b.lhs, b.rhs, b.lookAhead, b.phPos);
        return com;
    }
};
typedef std::map<std::string, std::vector<std::string>> ProductionRule;
typedef std::map<std::string, std::vector<std::string>*> ProductionRulePtr;
// character used as the placeholder in the closure/goto algorithm.
const static std::string c_phStr = "@";
const static char c_phCh = '@';

#endif //CCOMPILER_DEFINITIONS_H
