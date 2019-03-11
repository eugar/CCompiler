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
            // if
            if (item.phAtEnd())
            {
                if (item.lookAhead == "EOF")
                {
                    action.insert(item.state, 0, "EOF", Action::ACCEPT);
                }
                else
                {
                    action.insert(item.state, -1, item.lookAhead, Action::REDUCE);
                }
            }
            // if terminal is after ph
            else if (isupper(item.tokenAfterPh()[0]))
            {

                Lr1Item nextItem = gotoItem(item, ccPrevStates);
                action.insert(item.state, nextItem.state, item.tokenAfterPh(), Action::SHIFT);
            }
            // if non terminal is after ph
            else
            {
                Lr1Item nextItem = gotoItem(item, ccPrevStates);
                // change state only i.e., Goto entry
                if (item.state != nextItem.state)
                {
                    go.insert(item.state, nextItem.state, item.tokenAfterPh());
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
    // next is terminal token, i.e., action item

/*    if (isupper(token[0]))
    {
        if (item.phAtEnd())
        {

        }
        else
        {

        }
    }
    // next is non terminal, i.e., goto item
    else
  */  {
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
        Lr1Item lastItem = *nextItemSet.find(newItem);
        return lastItem;
    }
}