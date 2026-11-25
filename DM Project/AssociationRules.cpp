
#include "AssociationRules.h"
#include <algorithm>
#include <iterator>

using namespace std;

void getSubsets( const vector<string>& items, int index, Itemset& current, vector<Itemset>& result) {
    if (index == (int)items.size()) {
        if (!current.empty() &&current.size() < items.size()) {
            result.push_back(current);
        }
        return;
    }
    current.insert(items[index]);
    getSubsets(items, index + 1, current, result);
    current.erase(items[index]);
    getSubsets(items,index + 1,current,result);
}

vector<AssociationRule> generateAssociationRules(const FrequentLevels& allFrequentItemsets,double minConfidence,vector<AssociationRule>& strongRulesOut) {
    vector<AssociationRule> allRules;
    strongRulesOut.clear();
    map<Itemset, double> supportLookup;
    for (const auto& level : allFrequentItemsets) {
        for (const auto& pair : level) {
            supportLookup[pair.first] = pair.second;
        }
    }
    for (size_t k = 1; k < allFrequentItemsets.size();++k) {
        for (const auto& pair : allFrequentItemsets[k]) {
            const Itemset& itemset = pair.first;
            double itemsetSup = pair.second;

            vector<string> itemsVec(itemset.begin(), itemset.end());
            vector<Itemset> antecedents;
            Itemset temp;
            getSubsets(itemsVec, 0,temp,antecedents);
            for (const auto& X : antecedents) {
                Itemset Y;
                set_difference(itemset.begin(),itemset.end(),X.begin(),X.end(),inserter(Y, Y.begin()));
                double supX = supportLookup[X];
                double supY = supportLookup[Y];
                double confidence = itemsetSup / supX;
                double lift = confidence / supY;
                AssociationRule rule = {X,Y,itemsetSup,confidence,lift };
                allRules.push_back(rule);
                if ( confidence >= minConfidence - 1e-9) {
                    strongRulesOut.push_back(rule);
                }
            }
        }
    }
    return allRules;
}
