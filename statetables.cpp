//
// Created by mmalett on 3/4/19.
//

#include "statetables.h"

bool StateTables::addEntry(Lr1Item item, int from, int to)
{
    std::string token = item.tokenAfterPh();
    auto actState = m_action.table.find(from);
    auto gotoState = m_goto.table.find(from);

    if (item.phPos > 0)
    {
        if (gotoState != m_goto.table.end())
        {
            auto go = gotoState->second.find(item.tokenBeforePh());
            if (go != gotoState->second.end())
            {
                from = go->second;
                gotoState = m_goto.table.find(from);
            }
        }
        if (actState != m_action.table.end())
        {
            auto act = actState->second.find(item.tokenBeforePh());
            if (act != actState->second.end())
            {
                from = act->second.second;
                actState = m_action.table.find(from);
            }
        }
        // check goto table for the entry that was "found"
    }
    if (item.phAtEnd())
    {

        if (actState != m_action.table.end() )
        {
            if (actState->second.find(token) != actState->second.end())
            {
                return false;
            }
        }

        if (item.lookAhead == "EOF" && item.lhs == m_goalSymbol)
        {
            m_action.insert(from, to, item.lookAhead, Action::ACCEPT);
        }
        else
        {
            m_action.insert(item.state, to, item.lookAhead, Action::REDUCE);
        }
        // the state doesn't need to be incrememnted
        return false;
    }
        // terminal -- action: shift
    else if (isupper(token[0]))
    {

        if (actState != m_action.table.end() )
        {
            if (actState->second.find(token) != actState->second.end())
            {
                return false;
            }
        }
        m_action.insert(from, to, token, Action::SHIFT);
    }
        // non-terminal -- goto entry
    else
    {

        if (gotoState != m_goto.table.end())
        {
            if (gotoState->second.find(token) != gotoState->second.end())
            {
                return false;
            }
        }
        m_goto.insert(from, to, token);
    }
    return true;

}

// Manages entries into the action and goto tables as the canonical collection is being constructed.
void StateTables::addEntry(Lr1Item item)
{
    // The queue should only contain items with the ph after a terminal or non terminal.
    for (auto prevItem = gotoQueue.begin(); prevItem != gotoQueue.end(); prevItem++)
    {
        std::string token = item.tokenBeforePh();
        if (item.equals(*prevItem))
        {
            if (item.phAtEnd())
            {
                if (item.lookAhead == "EOF")
                {
                    m_action.insert(item.state, item.state, item.lookAhead, Action::ACCEPT);
                } else
                {
                    m_action.insert(item.state, item.state, item.lookAhead, Action::REDUCE);
                }
            }
            // terminal -- action: shift
            else if (isupper(token[0]))
            {
                m_action.insert(prevItem->state, item.state, token, Action::SHIFT);
            }
            // non-terminal -- goto entry
            else
            {
                m_goto.insert(prevItem->state, item.state, token);
            }
            // Erase an item once we've found its successor.
            // note: This should have zero items at the start of each goto (createStates) loop,
            // with the exception of the first one.
            gotoQueue.erase(prevItem);
            break;
        }
    }
    if (!item.phAtEnd())
    {
        // move ph, keep state info the same
        // store item to find its goto state later.
        Lr1Item i = gotoItem(item);
        if (!i.empty())
        {
            gotoQueue.push_back(i);
        }
    }
}

void StateTables::generateTables(std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccCurrentStates,
                                 std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates,
                                 Action &action,
                                 Goto &go)
{

    for (auto cci = ccCurrentStates.begin(); cci != ccCurrentStates.end(); cci++)
    {
        for (auto& item : cci->second)
        {
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

                Lr1Item nextItem = gotoItem(item/*, ccPrevStates*/);
                action.insert(item.state, nextItem.state, item.tokenAfterPh(), Action::SHIFT);
            }
            // if non terminal is after ph
            else
            {
                Lr1Item nextItem = gotoItem(item/*, ccPrevStates*/);
                // change state only i.e., Goto entry
                if (item.state != nextItem.state)
                {
                    go.insert(item.state, nextItem.state, item.tokenAfterPh());
                }
            }
        }
    }
}

Lr1Item StateTables::gotoItem(Lr1Item item
                              /*,std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates*/)
{
    // next is terminal
    bool atEnd, atStart = false;
    std::string token = item.tokenAfterPh(atEnd, atStart);
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
        newItem.phPos += token.size();
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
    //auto nextItemSet = ccPrevStates[item.state];
    //Lr1Item lastItem = *nextItemSet.find(newItem);
    //return lastItem;
    return newItem;

}

