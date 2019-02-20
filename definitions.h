//
// A header for global definitions
//
#ifndef CCOMPILER_DEFINITIONS_H
#define CCOMPILER_DEFINITIONS_H

#include <map>
#include <vector>
#include <string>

typedef std::map<std::string, std::vector<std::string>> ProductionRule;
typedef std::map<std::string, std::vector<std::string>*> ProductionRulePtr;

// character used as the placeholder in the closure/goto algorithm.
const static std::string c_phStr = "@";
const static char c_phCh = '@';

#endif //CCOMPILER_DEFINITIONS_H
