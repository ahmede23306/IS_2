
#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

typedef set<string> Itemset;

typedef vector<Itemset> Transactions;

typedef vector<map<Itemset, double>> FrequentLevels;

struct AssociationRule {
    Itemset antecedent;
    Itemset consequent;

    double support;
    double confidence;
    double lift;
};
