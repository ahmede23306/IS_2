#include "apriori.h"

#include <algorithm>
#include <iterator>

using namespace std;

vector<itemset> subsets(const itemset& itm){
    vector<itemset>subs;
    vector<char>item(itm.begin(), itm.end());

    auto generateSubsets = [&](auto&& self, int idx, itemset curr) -> void{
        if(idx == itm.size()) {
            if(curr.size()>0 && curr.size()<itm.size()){
                subs.push_back(curr);
            } return;
        }
        // leave
        self(self, idx + 1, curr);

        // take
        curr.insert(item[idx]);
        self(self, idx + 1, curr);
    };

    generateSubsets(generateSubsets, 0, {});

    return subs;
}

vector<vector<rule>> generateRules(const frequentLvl& lvl, int n, double minConf){
    vector<vector<rule>> rl;
    
    // save time 
    map<itemset, int> lookup;
    for(const auto& level: lvl){
        for(const auto& l : level){
            lookup[l.first] = l.second;
        }
    }

    for(int i=1;i<lvl.size();i++){
        vector<rule> r = {};

        for(const auto& l : lvl[i]){
            itemset full = l.first;
            vector<itemset>subst = subsets(full);

            // loop over all subsets
            for(auto eachSub : subst){
                itemset first = eachSub;
                itemset second = {};

                set_difference(full.begin(), full.end(),
                    first.begin(), first.end(),
                    inserter(second,second.begin())
                );

                // calculate confidence
                double conf = (double)lookup[full] / (double)lookup[first];

                // calculate lift
                double lift = (double)(conf * n) / (double)lookup[second];

                if(conf >= minConf){
                    rule ruule = {first, second, lookup[full], conf, lift};
                    r.push_back(ruule);
                }
            }
        } rl.push_back(r);
    }
    return rl;
}