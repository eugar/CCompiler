//
// Created by mmalett on 3/4/19.
//
#ifndef CCOMPILER_PARSETABLES_H
#define CCOMPILER_PARSETABLES_H

#include <map>
#include <set>
#include <vector>
#include "definitions.h"


class StateTables
{
public:

    explicit StateTables(std::string goalSymbol)
    :
    m_goalSymbol(goalSymbol)
    {}
    bool addEntry(Lr1Item item, int fromState, int toState);
    void addEntry(Lr1Item item);

    typedef struct Action
    {
        enum ACTION {SHIFT, REDUCE, ACCEPT};
        typedef std::map<size_t, std::map<std::string, std::pair<ACTION, size_t>>> ActionTable;

        ActionTable table;

        void insert(const size_t &currentState, const int &newState, std::string token, ACTION action)
        {
            table[currentState][token] = std::make_pair(action, newState);
        }

    } Action;

    typedef struct Goto
    {
        typedef std::map<size_t, std::map<std::string, size_t>> GotoTable;

        GotoTable table;

        void insert(const size_t &fromState, const size_t &toState, const std::string &token)
        {
            table[fromState][token] = toState;
        }

    } Goto;

    std::string m_goalSymbol;
    StateTables() = default;
    void generateTables(std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates,
                        std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccCurrentStates,
                        Action &action,
                        Goto &go);

    ~StateTables() = default;

    Action m_action;
    Goto m_goto;
    std::vector<Lr1Item> gotoQueue;
private:
    Lr1Item gotoItem(Lr1Item item
                    /*, std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates*/);
};

#endif //CCOMPILER_PARSETABLES_H
