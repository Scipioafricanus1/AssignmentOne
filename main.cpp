#include <iostream>
#include <algorithm>//sort
#include <vector>	//vector
#include <queue> //queue
using namespace std;

//Summary globals:
int totalSSDsAccessed = 0;
int SSDTimes = 0;
int SSDTotal = 0;
int totalCORERequestTime = 0;

//program globals:
string a;
string b;
int NCORES;
bool SSDFree = true;
bool INPUTFree = true;
int totalProcs = 0;
int currentProc = 0;
vector< vector<pair<string, string>> > table;
vector<pair<string, string> > row;
vector<string> processTracker;

//function declarations:
void scheduler();
void tester();
void tableMaker();
void arrival();
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
queue<Process> rq;
queue<Process> sq;
queue<Process> iq;
void request(const Process& p );

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
    if (inString == "CORE" || "CoreCompletion") return eCORE;
    if (inString == "SSD" || "SSDCompletion") return eSSD;
    if (inString == "INPUT" || "InputCompletion") return eINPUT;
    return eErr;
}

int main() {
    tableMaker();
    tester();
    arrival();

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
            totalProcs++;
            processTracker.push_back("OPEN");
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


void arrival() {
    int seqNum = 0;
    if (currentProc < totalProcs) { //if there's still a process left, check arrival, else, go to scheduler. if arrival starts before next process, check arrival, else, go to scheduler.
        int ms = stoi(table[currentProc][seqNum].second);
        if(pq.empty() || ms < pq.top().getTime()) {
            if (NCORES > 0) {
                NCORES--;
                processTracker[currentProc] = "RUNNING";
                seqNum++;
                int finishTime = ms + stoi(table[currentProc][seqNum].second);
                totalCORERequestTime += stoi(table[currentProc][seqNum].second);
                cout <<endl<< "Process "<<currentProc<<" starts at t = " << ms << "ms" << endl;
                for(int i = 0; i < processTracker.size(); i++)
                {
                    string temp = processTracker[i];
                    if(temp == "OPEN" || i == currentProc) {
                    } else {
                        cout << "Process " << i << " is " << temp << endl;
                    }
                }
                cout << endl;


                cout << "Core process scheduled to finish at t = " << finishTime << " ms" << endl;
                pq.push(Process(currentProc, seqNum, "CoreCompletion", finishTime));
                currentProc++;
                arrival();

            } else { //if CORES are busy, waits.
                cout <<endl<< "Process "<<currentProc<<" starts at t = " << ms << "ms" << endl;
                for(int i = 0; i < processTracker.size(); i++)
                {
                    string temp = processTracker[i];
                    if(temp == "OPEN" || i == currentProc) {
                    } else {
                        cout << "Process " << i << " is " << temp << endl;
                    }
                }
                cout << endl;
                seqNum++;
                Process p = Process(currentProc, seqNum, "CoreCompletion", stoi(table[currentProc][seqNum].second));
                totalCORERequestTime += stoi(table[currentProc][seqNum].second);
                rq.push(p);
                processTracker[currentProc] = "READY";

                cout << "P" <<p.getProcNum() << ": " << p.getProcType() << " request enters Ready Queue at t = " << ms << " ms"<<  endl;
                currentProc++;
                scheduler();
            }
        } else {
            scheduler();
        }

    } else { //after each process, checks arrival to see if the next arrival is before the next completion.
        scheduler();
    }

}

void scheduler() {
    // checks next process in pq. Completes the process, checks *

    Process p = pq.top();
    pq.pop();
    int finishTime;
    cout << "TYPE: " << p.getProcType()<<endl;

    if(p.getProcType() == "CoreCompletion") {
        cout << "Core finishes at t = " << p.getTime() << " ms" << endl;
        if(rq.empty()) {
            NCORES++;
        } else {
            Process Q = rq.front();
            rq.pop();
            finishTime = Q.getTime() + p.getTime();
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
        }
        request(p);
    } else if (p.getProcType() == "SSDCompletion") {
        cout << "SSD finishes at t = " << p.getTime() << " ms" << endl;
        SSDTimes += p.getTime();
        if(sq.empty()) {
            SSDFree = true;
        } else {
            Process Q = sq.front();
            sq.pop();
            finishTime = Q.getTime() + p.getTime();
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
        }
        request(p);
    } else if (p.getProcType() == "InputCompletion") {
        cout << "Input finishes at t = " << p.getTime() << " ms" << endl;
        if(iq.empty()) {
            INPUTFree = true;
        } else {
            Process Q = iq.front();
            iq.pop();
            finishTime = Q.getTime() + p.getTime();
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
        }
        request(p);
    } else {
        cout << "An error has occurred at line 191 or before.";
    }

}

void  request(const Process& p ) {
    if( table[p.getProcNum()].size() == (p.getSeqNum()+1) ) { //process is finished if this is true.
        cout << endl << "Process " << p.getProcNum() << " finishes at t = " << p.getTime() << "ms" << endl;
        processTracker[p.getProcNum()] = "TERMINATED";
        for(int i = 0; i < processTracker.size(); i++)
        {
            string temp = processTracker[i];
            if(temp == "OPEN") {
            } else {
                cout << "Process " << i << " is " << temp << endl;
            }
        }
        cout << endl;
        processTracker[p.getProcNum()] = "OPEN"; //finished with the process for good now.
        cout << endl;
        if(pq.empty() && rq.empty() && sq.empty() && iq.empty()) { //ENDS PROGRAM HERE rather than calling arrival recursively.
            cout << endl<< "Summary: "<<endl;
            cout << "Number of process that completed: " << totalProcs << endl;
            cout << "Total number of SSD accesses: "<< totalSSDsAccessed<<endl;
            double coreUtilization = (double)totalCORERequestTime/(double)p.getTime();
            double SSDAverage = (double)SSDTimes/(double)totalSSDsAccessed;
            double SSDUtilization = (double)SSDTotal/(double)p.getTime();

            cout << "Average SSD access time: " << SSDAverage<<" ms" << endl;
            cout << "Total elapsed time: " << p.getTime() << " ms" << endl;
            cout << "Core utilization: " << coreUtilization*100 << " percent" <<endl;
            cout << "SSD utilization: " << SSDUtilization*100 << " percent" << endl;
            return;
        }
    } else {
        string requestType = table[p.getProcNum()][p.getSeqNum()+1].first;
        int requestTime = stoi(table[p.getProcNum()][p.getSeqNum()+1].second);
        int finishTime = p.getTime() + requestTime;
        if(requestType == "INPUT") {

            if(INPUTFree) {
                INPUTFree = false;

                cout << "Input request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << "ms" << endl;
                cout << "Input finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "InputCompletion", finishTime));
            } else {
                iq.push(Process(p.getProcNum(), p.getSeqNum()+1, "InputCompletion", requestTime));
            }
        } else if (requestType == "CORE") {
            if(NCORES > 0) {
                NCORES--;
                cout << "Core request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << " ms" << endl;
                totalCORERequestTime += stoi(table[p.getProcNum()][p.getSeqNum()+1].second);
                cout << "Core finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "CoreCompletion", finishTime));
            } else {
                rq.push(Process(p.getProcNum(), p.getSeqNum()+1, "CoreCompletion", requestTime));
                totalCORERequestTime += requestTime;
            }
        } else if (requestType == "SSD") {
            totalSSDsAccessed++;
            SSDTimes -= p.getTime();
            SSDTotal += requestTime;
            if(SSDFree) {
                SSDFree = false;
                cout << "SSD request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << "ms" << endl;
                cout << "SSD finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "SSDCompletion", finishTime));

            } else {
                sq.push(Process(p.getProcNum(), p.getSeqNum()+1, "SSDCompletion", requestTime));
            }
        } else {
            cout << "Mistakes were made at line 273 or before it maybe" <<endl;
        }

    }
    arrival();


}
