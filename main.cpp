#include <iostream>
#include <algorithm>//sort
#include <vector>	//vector
#include <queue> //queue
using namespace std;

string a;
string b;
int NCORES;
vector< vector<pair<string, string>> > table;
vector<pair<string, string> > row;


void tester();
void reader();
void tableMaker();
bool checkProcess();
class Process
{
    int processNum;
    int sequenceNum;
    string processType;
    int time;
public:
    Process(int _processNum, int _sequenceNum, string _processType, int _time)
    {
        processNum = _processNum;
        sequenceNum = _sequenceNum;
        processType = _processType;
        time = _time;
    }
    int getProcNum() const { return processNum; }
    int getSeqNum() const { return sequenceNum; }
    string getProcType() const { return processType;}
    int getTime() const { return time; }
};

class CompareTime
{
public:
    bool operator()( const Process& p1, const Process& p2) {
        return p1.getTime() > p2.getTime();
    }
};

priority_queue<Process, vector<Process>, CompareTime> pq;

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
    reader();

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
    int i = 0;
    int j = 0;
    int ms;
    switch (hashit(table[i][j].first)) {
        case eNEW:
            ms = stoi(table[i][j].second);
            cout << "New process starts at t = " << ms << "ms" << endl;
            if (NCORES > 0) {
                int seqNum = 0;
                NCORES--;
                pq.push(Process(i, seqNum, "NewProcess", ms ));
                j++;
                ms = stoi(table[i][j].second);
                Process p = pq.top();
                int finishTime = p.getTime() + ms;
                cout << "P" << p.getProcNum() << ": Core process starts at t = " << p.getTime() << " ms" << endl;
                cout <<  "Core process ends at t = " << finishTime << " ms" << endl;
                seqNum++;
                pq.pop();
                pq.push(Process(i, seqNum, "CoreProcess", finishTime));
                bool pass = checkProcess();
            }
            break;
        default:
            cout << "Input failed. Moving to next input.\n";
    }
}

bool checkProcess() {
    Process p = pq.top();
    int nextProc = p.getProcNum() + 1;
    cout << "Last process completes at t = " << p.getTime() << endl;
    if(stoi(table[nextProc][0].second) < p.getTime()) {

    }
}