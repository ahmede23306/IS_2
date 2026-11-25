#include "apriori.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cctype>

using namespace std;

// convert from excel to csv
// + load data 
transactions readFile(const string& file){
    transactions trans;
    string f = "Horizontal_Format.csv";

    string cmd = "py -3 -c \"import pandas as pd; "
                 "pd.read_excel(r'" + file + "').to_csv('" + f + "', index=False)\"";

    if (system(cmd.c_str())!=0){
        cerr << "excel conversion failed"<<endl;
        return trans;
    }
    
    ifstream in(f);
    if(!in.is_open()){
        cerr << "cannot open: "<<f<<endl;
        return trans;
    }

    string row;
    getline(in,row);

    while(getline(in,row)){
        itemset itm;
        for(char c : row) if(isalpha(c)) itm.insert(c);
        if(itm.size())trans.push_back(itm);
    }

    return trans;
}