#ifndef APRIORI_H
#define APRIORI_H

#include <set>
#include <vector>
#include <map>
#include <string>

// shared data types
typedef std::set<char> itemset;
typedef std::vector<itemset> transactions;
typedef std::vector<std::map<itemset, int>> frequentLvl;

struct rule {
    itemset x;
    itemset y;
    int supp;
    double conf;
    double lift;
};

// load data
transactions readFile(const std::string& file);

// apriori algo functions
int supportCount(const itemset& candidate, const transactions& tr);

std::map<itemset, int> generateL1(
    const transactions& trans, int minSup
);

std::set<itemset> generateCandidates(
    const std::map<itemset, int>& prev
);

frequentLvl generateFreq(
    const transactions& tr, int minSup
);

// generate rules
std::vector<itemset> subsets(const itemset& itm);

std::vector<std::vector<rule>> generateRules(
    const frequentLvl& lvl, int n, double minConf
);

#endif
