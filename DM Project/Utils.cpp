#include "Utils.h"

string itemsetToString(const Itemset& items) {
    string res = "{";

    for (auto it = items.begin(); it != items.end(); ++it) {
        res += *it;

        if (next(it) != items.end()) {
            res += ", ";
        }
    }

    res += "}";

    return res;
}
