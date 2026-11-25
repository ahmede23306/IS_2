#pragma once

#include "Transaction.h"

void getSubsets(
    const vector<string>& items,
    int index,
    Itemset& current,
    vector<Itemset>& result
);

vector<AssociationRule> generateAssociationRules(
    const FrequentLevels& allFrequentItemsets,
    double minConfidence,
    vector<AssociationRule>& strongRulesOut
);
