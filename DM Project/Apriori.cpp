#include "apriori.h"

#include <algorithm>
#include <iterator>

using namespace std;

// given a candidate, calculte support count
// over all other transactions

int supportCount(const itemset& candidate, const transactions& tr){
    int cnt = 0;

    for(auto t : tr){
        bool allIn = true;
        for(auto c : candidate){
            if(!t.count(c)) allIn = false;
        }
        if(allIn) cnt++;
    }

    return cnt;
    //return ((double)cnt / (double)tr.size());
}

map<itemset, int> generateL1(const transactions& trans, int minSup){
    map<itemset,int> lvl1;

    set<char>distinct;

    // get distinct items
    // so we can loop over them later
    // and find which passes >= minSup
    for(const auto& tr : trans){
        for(auto itm: tr) distinct.insert(itm);
    }

    for(char c : distinct){
        int cnt = 0;

        for(auto tr : trans){
            if(tr.count(c)) cnt++;
        }

        if(cnt >= minSup) lvl1[{c}] = cnt;
    }

    return lvl1;
}

set<itemset> generateCandidates(const map<itemset, int>& prev){
    set<itemset> cand;
    if(prev.empty()) return cand;

    vector<itemset> v;
    for(const auto& p: prev) v.push_back(p.first);
    
    int k = v[0].size() + 1;

    for(int i=0 ; i<v.size();i++){
        for(int j=i+1;j<v.size();j++){
            const itemset& item1 = v[i];
            const itemset& item2 = v[j];

            if(!equal(item1.begin(), next(item1.begin(),k-2), item2.begin())){
                continue;
            }

            itemset u = item1;

            u.insert(*item2.rbegin());

            // check each k-1 is in prev
            bool prune = true;
            for(char x : u){
                itemset a = u;
                a.erase(x);
                if(!prev.count(a)){
                    prune = false; break;
                }
            }
            if(prune) cand.insert(u);
        }
    } return cand;
}

frequentLvl generateFreq(const transactions& tr, int minSup){
    frequentLvl lvls = {};

    map<itemset,int> L1 = generateL1(tr, minSup);
    if(L1.empty()) return lvls;

    lvls.push_back(L1);

    while(true){
        set<itemset> c = generateCandidates(lvls.back());

        map<itemset,int> lvl_k;
        for(const auto &cand : c){
            int cnt = supportCount(cand,tr);
            if(cnt >= minSup) lvl_k[cand] = cnt;
        }

        if(lvl_k.empty()) break;
        lvls.push_back(lvl_k);
    }

    return lvls;
}
