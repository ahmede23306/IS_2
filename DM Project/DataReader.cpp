
#include "DataReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n\"'");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n\"'");
    return str.substr(first, (last - first + 1));
}

Transactions loadTransactions(const string& filename) {
    Transactions transactions;
    string csvFile = filename;

    if (filename.find(".xlsx") != string::npos) {
        csvFile = "temp_data.csv";
        string cmd =
            "py -3 -c \"import pandas as pd; "
            "pd.read_excel(r'" + filename +
            "').to_csv(r'" + csvFile +
            "', index=False)\"";
        cout << "Executing: " << cmd << endl;
        int status = system(cmd.c_str());
        cout << "Python exit status: " << status << endl;
        if (status != 0) {
            cout << "Excel conversion failed!\n";
            return transactions;
        }
    }

    ifstream file(csvFile);

    if (!file.is_open()) {
        cerr << "Error: Cannot open file "
            << csvFile << endl;
        exit(1);
    }

    string line;
    bool isHeader = true;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (isHeader) {
            isHeader = false;
            string lowerLine = line;
            transform(lowerLine.begin(), lowerLine.end(),  lowerLine.begin(),::tolower);

            if (lowerLine.find("tid") != string::npos || lowerLine.find("item") != string::npos || lowerLine.find("trans") != string::npos ) {
                continue;
            }
        }

        Itemset currentTransaction;
        stringstream ss(line);
        string cell;
        int colIndex = 0;

        while (getline(ss, cell, ',')) {
            cell = trim(cell);
            if (cell.empty()) {
                continue;
            }

            if (colIndex == 0) {
                colIndex++;
                continue;
            }

            stringstream itemStream(cell);
            string item;

            while (getline(itemStream, item, ';')) {
                item = trim(item);
                if (!item.empty()) {
                    currentTransaction.insert(item);
                }
            }
            colIndex++;
        }

        if (!currentTransaction.empty()) {
            transactions.push_back(currentTransaction);
        }
    }

    file.close();

    return transactions;
}
