//NAME: Brent Harris 1231407
//COSC 3360
//Due: Feb 16, 2018



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
vector< vector<pair<string, string> > > table;
vector<pair<string, string> > row;
vector<string> processTracker;

//function declarations:
void scheduler();
void tester();
void tableMaker();
void arrival();
class Process //Used to contain important information for the priority queue, which sorts items by time.
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

class CompareTime //Used to explicitly compare process times in the priority queue. Orders into min-heap.
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


int main() {
    tableMaker();
    //tester();
    arrival();

    return 0;
}


void tableMaker() { //Makes a table of the data based on it's process, in order to allow for backtracking when needed.
    while (cin >> a >> b) {
        for (unsigned int i = 0; i < a.size(); i++) {
            a.at(i) = static_cast<char>(toupper(a.at(i)));
        }
        if(a == "NCORES") {
            //cout << "# of cores: " << b << endl;
            NCORES = stoi(b);
        } else if( a == "NEW") {
            //cout << "New process.\n";
            totalProcs++;
            processTracker.push_back("OPEN");
            if(row.empty()) {
                //cout << "Pushed it back, since first process.\n";
                row.push_back(make_pair(a, b));
            } else {
                table.push_back(row);
                row.clear();
                row.push_back(make_pair(a,b));
            }
        } else if( a == "CORE") {
            //cout << "Core process.\n";
            row.push_back(make_pair(a, b));
        } else if( a == "SSD") {
            //cout << "SSD process.\n";
            row.push_back(make_pair(a, b));
        } else if( a == "INPUT") {
            //cout << "Input process.\n";
            row.push_back(make_pair(a, b));
        } else if(a == "break") {
            break;
        } else {
            //cout << "Input failed. Moving to next input.\n";
        }
    }
    table.push_back(row);
    row.clear();

}

void tester() { //merely tests to make sure the process table works correctly. Not necessary to call when finished, but neat to have
                //for documentation I guess.
    for( int i = 0; i < table.size(); i++) {
        for( int j = 0; j < table[i].size(); j++) {
            cout << "Process " << i << ": " << table[i][j].first << " " << table[i][j].second << endl;
        }
    }
}


void arrival() {//new processes are the most important, since they get looked at first, and start the ball
                //rolling. Looks for a new process to start if nothing is scheduled before the new process.
                //Also calls scheduler which finishes processes, so that they can call arrival again.
                //Note that this is me using a long-winded recursion, rather than a while loop.
                //This means I have to end it by breaking the recursion when there are no processes left anywhere.
    int seqNum = 0;
    if (currentProc < totalProcs) { //if there's still a process left, check arrival, else, go to scheduler. if arrival starts before next process, check arrival, else, go to scheduler.
        int ms = stoi(table[currentProc][seqNum].second);
        if(pq.empty() || ms < pq.top().getTime()) {
            if (NCORES > 0) {
                NCORES--;
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
                processTracker[currentProc] = "RUNNING";
                cout << endl;


                //cout << "Core process scheduled to finish at t = " << finishTime << " ms" << endl;
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

                //cout << "P" <<p.getProcNum() << ": " << p.getProcType() << " request enters Ready Queue at t = " << ms << " ms"<<  endl;
                currentProc++;
                arrival(); //fixed this
            }
        } else {
            scheduler();
        }

    } else { //after each process, checks arrival to see if the next arrival is before the next completion.
        scheduler();
    }

}

void scheduler() {
    // checks next process in pq. Completes the process, calls for next request based on last process finished.

    Process p = pq.top();
    pq.pop();
    int finishTime;
    //cout << "TYPE: " << p.getProcType()<<endl;

    if(p.getProcType() == "CoreCompletion") {
        //cout << "P" << p.getProcNum()<<": Core finishes at t = " << p.getTime() << " ms" << endl;
        if(rq.empty()) {
            NCORES++;
        } else {
            Process Q = rq.front();
            rq.pop();
            finishTime = Q.getTime() + p.getTime();
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
            processTracker[Q.getProcNum()] = "RUNNING";
        }
        request(p);
    } else if (p.getProcType() == "SSDCompletion") {
        //cout <<"P" << p.getProcNum()<<":SSD finishes at t = " << p.getTime() << " ms" << endl;
        SSDTimes += p.getTime();
        if(sq.empty()) {
            SSDFree = true;
        } else {
            Process Q = sq.front();
            sq.pop();
            finishTime = Q.getTime() + p.getTime();
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
            processTracker[Q.getProcNum()] = "BLOCKED";
        }
        request(p);
    } else if (p.getProcType() == "InputCompletion") {
        //cout << "P" << p.getProcNum()<<":Input finishes at t = " << p.getTime() << " ms" << endl;
        if(iq.empty()) {
            INPUTFree = true;
        } else {
            Process Q = iq.front();
            iq.pop();
            finishTime = Q.getTime() + p.getTime();
            //cout << "P" << Q.getProcNum()<<": Input starts at t = " << p.getTime() << " ms, Finishes at t = " << finishTime << endl;
            pq.push(Process(Q.getProcNum(),Q.getSeqNum(),Q.getProcType(), finishTime));
            processTracker[Q.getProcNum()] = "BLOCKED";
        }
        request(p);
    } else {
        //cout << "An error has occurred at line 191 or before.";
    }

}

void  request(const Process& p ) { // checks if program is finished, exits the recursion if so, else it schedules the next event.
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
            processTracker[p.getProcNum()] = "BLOCKED";
            if(INPUTFree) {
                INPUTFree = false;
                //cout <<"P" << p.getProcNum()<<": Input request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << "ms at t = " << p.getTime() << endl;
                //cout <<"P" << p.getProcNum()<<": Input finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "InputCompletion", finishTime));
            } else {
                //cout <<"P"<<p.getProcNum()<<": S"<<p.getSeqNum()<<": Time: "<< p.getTime()<< " Input must wait in Input queue"<<endl;
                iq.push(Process(p.getProcNum(), p.getSeqNum()+1, "InputCompletion", requestTime));
            }
        } else if (requestType == "CORE") {
            if(NCORES > 0) {
                NCORES--;
                //cout <<"P" << p.getProcNum()<<":Core request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << " ms at t = " << p.getTime() << endl;
                totalCORERequestTime += stoi(table[p.getProcNum()][p.getSeqNum()+1].second);
                //cout <<"P" << p.getProcNum()<<":Core finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "CoreCompletion", finishTime));
                processTracker[p.getProcNum()] = "RUNNING";
            } else {
                //cout <<"P"<<p.getProcNum()<<" S"<<p.getSeqNum()<<" Time: "<< p.getTime()<< " CORE must wait in Ready queue"<<endl;
                rq.push(Process(p.getProcNum(), p.getSeqNum()+1, "CoreCompletion", requestTime));
                totalCORERequestTime += requestTime;
                processTracker[p.getProcNum()] = "READY";
            }
        } else if (requestType == "SSD") {
            totalSSDsAccessed++;
            SSDTimes -= p.getTime();
            SSDTotal += requestTime;
            processTracker[p.getProcNum()] = "BLOCKED";
            if(SSDFree) {
                SSDFree = false;
                //cout <<"P" << p.getProcNum()<<":SSD request time = " << table[p.getProcNum()][p.getSeqNum()+1].second << "ms at t = " << p.getTime() << endl;
                //cout <<"P" << p.getProcNum()<<":SSD finish time = " << finishTime << "ms" << endl;
                pq.push(Process(p.getProcNum(), p.getSeqNum()+1, "SSDCompletion", finishTime));

            } else {
                //cout <<"P"<<p.getProcNum()<<" S"<<p.getSeqNum()<<" Time: "<< p.getTime()<< " SSD must wait in SSD queue"<<endl;
                sq.push(Process(p.getProcNum(), p.getSeqNum()+1, "SSDCompletion", requestTime));
            }
        } else {
            //cout << "Mistakes were made at line 273 or before it maybe" <<endl;
        }

    }
    arrival();
}
