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

    StateTables() = default;
    void generateTables(std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates,
                        std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccCurrentStates,
                        Action &action,
                        Goto &go);

    ~StateTables() = default;

private:
    Lr1Item gotoItem(Lr1Item item,
                     std::map<size_t, std::set<Lr1Item, Lr1Compare>> &ccPrevStates);
};

#endif //CCOMPILER_PARSETABLES_H
