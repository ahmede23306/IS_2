#include "apriori.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


int main() {
    transactions t = readFile("Horizontal_Format.xlsx");
    cout << t.size() << " transactions\n\n";
    
    double minConf = 0.7;
    int minsup = 2;
    
    cout <<"Enter minumum support: ";
    cin >> minsup;
    cout <<"Enter minumum confidence: ";
    cin >>minConf;
    

    frequentLvl levels = generateFreq(t, minsup);

    vector<vector<rule>> rules = generateRules(levels, t.size(), minConf);

    for(int i = 0; i < rules.size(); i++){
        cout << "Level " << i + 2 << ":\n";

        for(auto r : rules[i]){
            cout << "{";

            for(char c : r.x)
              cout << c;

            cout << "} -> {";

            for(char c : r.y)
                cout << c;

            cout << "}\n";
            cout << "Support count: " << r.supp << '\n';
            cout << "Confidence: " << r.conf << '\n';
            cout << "Lift: " << r.lift << "\n\n"; //  
        }
    }
     
    ofstream fout("support_data.csv");

    fout << "Level,Itemset,SupportCount,Support\n";

    for(int i = 0; i < levels.size(); i++){

    cout << "\n========== L" << i + 1 << " Support ==========\n";

    for(const auto& item : levels[i]){
        string name = "";
        for(char c : item.first) name += c;
        double support = (double)item.second / t.size();

        // export
        fout << i + 1 << ","<< name << ","<< item.second << ","<< support << "\n";


        cout << name << " | ";

        int bars = support * 40;
        for(int j = 0; j < bars; j++) cout << "#";
        cout << " " << support << '\n';

        }
    }

    fout.close();

    system("py visualize.py");

    system("pause");
    return 0;
}