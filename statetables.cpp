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
        Action::ACTION act;
        for (auto& item : cci->second)
        {
            if (item.phAtEnd())
            {
                act = Action::ACCEPT;
            }
            else if (isupper(item.tokenAfterPh()[0]))
            {
                //Lr1Item nextItem = gotoItem(item, ccPrevStates);
                act = Action::REDUCE;
            }
            else // non terminal
            {
                Lr1Item nextItem = gotoItem(item, ccPrevStates);
                act = Action::SHIFT;
                // Goto entry
                if (item.state != item.nextState)
                {
                    go.insert(item.state, item.nextState, item.tokenAfterPh());
                }
            }
        }
    }
}

Lr1Item StateTables::gotoItem(Lr1Item item,
                              std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates)
{
    // next is terminal
    bool atEnd, atStart = false;
    std::string token = item.tokenAfterPh(atEnd, atStart);
    if (isupper(token[0]))
    {

    }
    // next is non terminal
    else
    {
        Lr1Item newItem(item);
        // The ph should be on the last character in the rhs string.
        if (atEnd) // This is also the case when both are true, i.e., only one token on RHS
        {
            newItem.phPos = newItem.rhs.size() - 1;
        }
            // The ph should line up on the last character in token, but we
            // started on the first character.
        else if (atStart)
        {
            newItem.phPos += token.size() - 1;
        }
            // The ph should line up on a space, AFTER the token, and we
            // started on a space BEFORE the token
        else
        {
            newItem.phPos += token.size() + 1;
        }
        // Now we have duplicate LR1 item with the ph in the next position.
        // Find it in the prevStates map. Get the proper set with the
        // current item's state.
        auto nextItemSet = ccPrevStates[item.state];
        auto iter = *nextItemSet.find(newItem);
        return iter;
    }
}

void StateTables::Action::insert(const size_t &currentState,
                                 const size_t &prevState,
                                 const Lr1Item &item,
                                 bool atEnd)
{
    std::string token = "";
    if (atEnd)
    {

    }
    else
    {
        token = item.tokenAfterPh();
    }
    //table[currentState].insert(std::make_pair(token, std::make_pair(action, prevState)));
}
