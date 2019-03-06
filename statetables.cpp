//
// Created by mmalett on 3/4/19.
//

#include "statetables.h"

void StateTables::generateTables(std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccCurrentStates,
                                 std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates,
                                 Action &action,
                                 Goto &go)
{
    for (auto cci = ccCurrentStates.begin(); cci != ccCurrentStates.end(); cci++)
    {
        for (auto& item : cci->second)
        {
            bool atEnd = false;
            if ((atEnd = item.phAtEnd()) || isupper(item.tokenAfterPh()[0]))
            {
                action.insert(item.state, item.nextState, item, atEnd);
            }
            else // non terminal
            {
                // Goto entry
                if (item.state != item.nextState)
                {
                    go.insert(item.state, item.nextState, item.tokenAfterPh());
                }
            }
        }
    }
}

Lr1Item gotoItem(Lr1Item &item, bool atEnd)
{

}

void StateTables::Action::insert(const size_t &currentState, const size_t &newState, const Lr1Item &item, bool atEnd)
{
    ACTION action;
    std::string token = "";
    if (atEnd)
    {
        token = item.lookAhead;
        if (item.lookAhead == "EOF")
        {
            action = ACCEPT;
        }
        else
        {
            action = REDUCE;
        }
    }
    else
    {
        action = SHIFT;
        token = item.tokenAfterPh();
    }
    table[currentState].insert(std::make_pair(token, std::make_pair(action, newState)));
}
