#include <iostream>
#include <algorithm>//sort
#include <vector>	//vector
#include <queue> //queue
using namespace std;

string a;
string b;
int NCORES;
vector< vector<pair<string, string> > > table;
vector<pair<string, string> > row;

void tester();
void reader();
void tableMaker();
enum
string_code {
    eNCORES,
    eNEW,
    eCORE,
    eSSD,
    eINPUT,
    eErr
};
string_code hashit(string inString) {
    for (unsigned int i = 0; i < inString.size(); i++) {
        inString.at(i) = static_cast<char>(toupper(inString.at(i)));
    }
    if (inString == "NCORES") return eNCORES;
    if (inString == "NEW") return eNEW;
    if (inString == "CORE") return eCORE;
    if (inString == "SSD") return eSSD;
    if (inString == "INPUT") return eINPUT;
    return eErr;
}

int main() {
    tableMaker();
    tester();
    //reader();

    return 0;
}

void tableMaker() {
    while (cin >> a >> b) {
        switch (hashit(a)) {
        case eNCORES:
            cout << "# of cores: " << b << endl;
            NCORES = stoi(b);
            break;
        case eNEW:
            cout << "New process.\n";
            if(row.empty()) {
                cout << "Pushed it back, since first process.\n";
                row.push_back(make_pair(a, b));
            } else {
                table.push_back(row);
                row.clear();
                row.push_back(make_pair(a,b));
            }
            break;
        case eCORE:
            cout << "Core process.\n";
            row.push_back(make_pair(a, b));
            break;
        case eSSD:
            cout << "SSD process.\n";
            row.push_back(make_pair(a, b));
            break;
        case eINPUT:
            cout << "Input process.\n";
                row.push_back(make_pair(a, b));
            break;
        case eErr:
            cout << "Input failed. Moving to next input.\n";
            break;
        default:
            cout << "Input failed. Moving to next input.\n";
            break;
        }

    }
    table.push_back(row);
    row.clear();

}

void tester() {
    for( int i = 0; i < table.size(); i++) {
        for( int j = 0; j < table[i].size(); j++) {
            cout << "Process " << i << ": " << table[i][j].first << " " << table[i][j].second << endl;
        }
    }
}


void reader() {

    switch (hashit(a)) {
        case eNEW:
            cout << "New process.\n";

            break;
        case eCORE: cout << "Core process.\n";

            break;
        case eSSD: cout << "SSD process.\n";

            break;
        case eINPUT: cout << "Input process.\n";

            break;
        case eErr: cout << "Input failed. Moving to next input.\n";
            break;
        default:
            cout << "Input failed. Moving to next input.\n";
    }
}