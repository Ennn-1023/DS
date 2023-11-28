// 11127139 陳恩
// 11127140 姜義新

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

clock_t  Begin, End;

struct jobType {
    int OID;
    int arrival;
    int duration;
    int timeout;
};
bool cmp( const jobType& leftOne, const jobType& rightOne ) {
    if ( leftOne.arrival > rightOne.arrival )
        return true;
        // arrival time are equivalent, then compare with OID
    else if ( leftOne.arrival == rightOne.arrival )
        return leftOne.OID > rightOne.OID;
    else
        return false;
}
class JobList {
private:
    vector<jobType> list;
    string fileID;
    void sortByArrival() {
        int n = list.size();
        for ( int stepLength = n/2; stepLength > 0;stepLength /= 2 ) {
            for ( int unsorted = stepLength; unsorted < n; unsorted++ ) {
                int loc = unsorted;
                jobType nextJob = list[unsorted];
                // left > right, shift
                for ( loc = loc - stepLength; loc >= 0 && cmp(list[loc], nextJob); loc -= stepLength ) {
                    list[loc + stepLength] = list[loc];
                }
                list[loc+stepLength] = nextJob;
            }
        }
    }

    void writeFile( string fileName ) {
        ofstream outFile;
        outFile.open(fileName.c_str());
        if (!outFile.is_open())
            cerr << "Error\n";
        else {
            outFile << "OID\tArrival\tDuration\tTimeOut" << endl;

            for ( int i = 0; i < list.size(); i++ ) {
                outFile << list[i].OID << "\t" << list[i].arrival << "\t";
                outFile << list[i].duration << "\t" << list[i].timeout << endl;
            }

            outFile.close();
        }
    }

public:
    JobList() {
        reset();
    }
    ~JobList() {
        reset();
    }
    bool isEmpty() const {
        return list.empty();
    }
    void getNextJob( jobType& firstJob) {
        // get the first job in list but not delete it.
        if ( !list.empty() ) {
            firstJob = list[0];

        }
        else {
            cerr << "Get from empty jobList!" << endl;
        }
    }
    int getArrivalTime() {
        if ( !list.empty() )
            return list[0].arrival;
        else
            return -1;
    }
    void delOneJob() {
        /*
         * delete the first item in list
         */
        if ( !list.empty() )
            list.erase(list.begin());
        else
            cerr << "Pop from empty jobList!" << endl;
    }

    bool getAll( string fileName ) {

        ifstream in;
        in.open(fileName.c_str());

        if (!in.is_open()) {
            // can't find
            return false;
        }


        string temp;
        jobType aJob;
        getline(in, temp);
        while (in >> aJob.OID >> aJob.arrival >> aJob.duration >> aJob.timeout) {
            list.push_back(aJob);
        }

        in.close();


        return true;
    }


    bool getSorted( string fileName ) {
        int readTime, sortTime, writeTime;
        Begin = clock();
        if (!getAll(fileName))
            return false;
        End = clock();
        show();
        readTime = End - Begin;

        // sorting
        Begin = clock();
        sortByArrival();
        End = clock();
        sortTime = End - Begin;

        fileName = "sorted" + fileID + ".txt";
        // write file
        Begin = clock();
        writeFile( fileName );
        End = clock();
        writeTime = End - Begin;

        // system("PAUSE");

        // show time
        cout << endl << "Reading data: " << readTime << " clocks (" << readTime << ".00 ms).";
        cout << endl << "Sorting data: " << sortTime << " clocks (" <<sortTime << ".00 ms).";
        cout << endl << "Writing data: " << writeTime << " clocks (" << writeTime << ".00 ms).";
        cout << endl;
        cout << endl << "see " + fileName << endl;
        return true;
    }
    void reset() {
        list.clear();
    }
    void show() {
        cout << "\tOID\tArrival\tDuration\tTimeOut";
        for ( int i = 0; i < list.size(); i++ ) {
            cout << endl << "(" << i+1 << ")\t";
            cout << list[i].OID << "\t" << list[i].arrival << "\t";
            cout << list[i].duration << "\t" << list[i].timeout;
        }
        cout << endl;
    }
    string getID() {
        return fileID;
    }
    int getLength() {
        return list.size();;
    }
    void setID() {
        cout << endl << "Input a file number: ";
        cin >> fileID;
    }
};

class JobQueue {
private:
    jobType* arrQueue;
    int front;
    int back;
    int curSize;
    int maxSize;
public:
    JobQueue():arrQueue(NULL), front(0), back(0), curSize(0), maxSize(0) {
    }
    JobQueue( const JobQueue& aQueue ):front(aQueue.front), back(aQueue.back)
            ,curSize(aQueue.curSize), maxSize(aQueue.maxSize) {
        arrQueue = new jobType[maxSize];
        for ( int i = 0; i < maxSize; i++ ) {
            arrQueue[i] = aQueue.arrQueue[i];
        }
    }
    JobQueue(int size):maxSize(size), front(0), curSize(0) {
        arrQueue = new jobType[maxSize];
        back = maxSize - 1;
    }
    ~JobQueue() {
        clear();
    }
    int getTimeOut( int index ) {
        if ( index >= 0 && index < length() ) {
            // exact index in queue
            index = (front + index) % maxSize;
            return arrQueue[index].timeout;
        }
        else {
            cerr << "Index error on Queue!" << endl;
        }
    }
    int length() {
        return curSize;
    }
    bool isEmpty() {
        return curSize == 0;
    }
    bool isFull() {
        return curSize == maxSize;
    }
    void enQueue( const jobType& newJob ) {
        if ( !isFull() ) {
            back = ( back + 1 ) % maxSize;
            arrQueue[back] = newJob;
            curSize++;
        }
        else {
            cerr << "Failed to enQueue one item on full queue." << endl;
        }
    }
    void getFront( jobType& firstJob ) {
        firstJob = arrQueue[front];
    }
    void deQueue() {
        if ( !isEmpty() ) {
            front = (front + 1) % maxSize;
            curSize--;
        }
        else {
            cerr << "Failed to deQueue one item on empty queue." << endl;
        }
    }
    void clear() {
        delete [] arrQueue;
        arrQueue = NULL;
        maxSize = 0;
        curSize = 0;
        front = 0;
        back = 0;
    }

};

class AnsList {
private:
    struct abortType {
        int OID;
        int Abort;
        int Delay;
        int CID;
    };
    struct doneType {
        int OID;
        int Departure;
        int Delay;
        int CID;
    };

    vector<abortType> abortList;
    vector<doneType> doneList;
    float avgDelay;
    float successRate;
    int totalDelay;

public:
    AnsList():avgDelay(0.0), successRate(0.0), totalDelay(0), abortList(0), doneList(0) {
    }
    ~AnsList() {
        clear();
    }
    void clear() {
        abortList.clear();
        doneList.clear();
    }
    // void showAll();
    void addAbortJob( int OID, int abort, int delay, int CID ) {
        abortType newJob = { OID, abort, delay, CID };

        abortList.push_back( newJob );
        totalDelay += delay;
    }
    int getNumOfDone() {
        return abortList.size() + doneList.size();
    }

    void addDoneJob( int OID, int departure, int delay, int CID ) {
        doneType newJob = { OID, departure, delay, CID };
        doneList.push_back(newJob);
        totalDelay += delay;
    }
    void computeStats() {
        int totalJob = abortList.size() + doneList.size();
        avgDelay = float(totalDelay) / float(totalJob);
        successRate = (float ( doneList.size() ) / float(totalJob)) * 100;

    }
    void putAll3( string fileName ) {
        ofstream outFile;
        outFile.open( fileName );
        if ( outFile.is_open() ) {
            // abortList
            outFile << "\t[Abort Jobs]";
            outFile << endl << "\tOID\tCID\tAbort\tDelay";
            for ( int i = 0; i < abortList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << abortList[i].OID << "\t" << abortList[i].CID;
                outFile << "\t" << abortList[i].Abort << "\t" << abortList[i].Delay << "\t";
            }
            // doneList
            outFile << endl << "\t[Jobs Done]";
            outFile << endl << "\tOID\tCID\tDeparture\tDelay";
            for ( int i = 0; i < doneList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << doneList[i].OID << "\t" << doneList[i].CID;
                outFile << "\t" << doneList[i].Departure << "\t" << doneList[i].Delay << "\t";
            }

            char delay[7], rate[7];
            sprintf( delay, "%.2f", avgDelay);
            sprintf(rate, "%.2f", successRate);
            outFile << endl << "[Average Delay]\t" << delay << " ms";
            outFile << endl << "[Success Rate]\t" << rate << " %\n";
            outFile.close();
        }
        else {
            cerr << "Open output file error!" << endl;
        }
    }
    void putAll2( string fileName ) {
        ofstream outFile;
        outFile.open( fileName );
        if ( outFile.is_open() ) {
            // abortList
            outFile << "\t[Abort Jobs]";
            outFile << endl << "\tOID\tCID\tAbort\tDelay";
            for ( int i = 0; i < abortList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << abortList[i].OID << "\t";
                outFile << "\t" << abortList[i].Abort << "\t" << abortList[i].Delay << "\t";
            }
            // doneList
            outFile << endl << "\t[Jobs Done]";
            outFile << endl << "\tOID\tDeparture\tDelay";
            for ( int i = 0; i < doneList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << doneList[i].OID;
                outFile << "\t" << doneList[i].Departure << "\t" << doneList[i].Delay << "\t";
            }

            char delay[7], rate[7];
            sprintf( delay, "%.2f", avgDelay);
            sprintf(rate, "%.2f", successRate);
            outFile << endl << "[Average Delay]\t" << delay << " ms";
            outFile << endl << "[Success Rate]\t" << rate << " %\n";
            outFile.close();
        }
        else {
            cerr << "Open output file error!" << endl;
        }
    }
};

class Simulation {
private:
    struct CPUType {
        int OID;
        int delay;
        int leavingTime; // the time when current job will leave or previous job was leaving
        int startTime; // the time current job was started to be processed
        bool done; // the job will be done or aborted
        bool isFree; // if the CPUType is busy
    };
    JobList jobList; // the jobs
    AnsList ansList; // output stats
    vector<JobQueue> nQueue; // waiting job queues for each cpu
    CPUType* nStatOfCPU; // an array for n cpu
    int numOfCPU; // number of total cpu
    int queueSize;



public:
    Simulation( const JobList& jobs, int numOfQueue, int sizeOfQueue )
            : jobList(jobs), numOfCPU(numOfQueue), queueSize( sizeOfQueue) {

        JobQueue temp( queueSize );

        nStatOfCPU = new CPUType[numOfCPU];
        // create n CPUType work space
        for (int i = 0; i < numOfCPU; i++ ) {
            nQueue.push_back( temp );
            nStatOfCPU[i].OID = 0;
            nStatOfCPU[i].leavingTime = 0;
            nStatOfCPU[i].startTime = 0;
            nStatOfCPU[i].isFree = true;
            nStatOfCPU[i].done = true;
        }

    }
    ~Simulation() {
        clear();
    }
    void clear() {
        jobList.reset();
        ansList.clear();
        nQueue.clear();
        delete[] nStatOfCPU;
    }
    int getAvailableTime( int time ) {
        int nextTime = time;
        for (int i = 0; i < numOfCPU; i++) {
            if ( nStatOfCPU[i].leavingTime > time ) {
                nextTime = nStatOfCPU[i].leavingTime;
                break;
            }
        }
        for ( int i = 0; i < numOfCPU; i++ ) {
            if ( nStatOfCPU[i].leavingTime > time && nextTime > nStatOfCPU[i].leavingTime )
                nextTime = nStatOfCPU[i].leavingTime;
        }


        if ( nextTime <= time )
            nextTime = jobList.getArrivalTime();


        return nextTime;
    }

    bool allFull( int time ) {
        for ( int i = 0; i < numOfCPU; i++ ) {
            if ( !nQueue[i].isFull() )
                return false;
        }

        return true;
    }
    int chooseACPU( int arrival) {
        int nth = 0;
        for ( int i = 0; i < numOfCPU; i++ ) {
            if ( nQueue[i].isEmpty() && nStatOfCPU[i].leavingTime < arrival )
                return i;
        }

        for ( int i = 1; i < numOfCPU; i++ ) {
            if ( nQueue[nth].length() > nQueue[i].length() )
                nth = i;
        }

        return nth;
    }
    void updateCurrent( int time ) {
        // go into CPU
        jobType aJob;
        for (int nth = 0; nth < numOfCPU; nth++ ) {
            while (nStatOfCPU[nth].leavingTime <= time && !nQueue[nth].isEmpty() ) {
                nQueue[nth].getFront(aJob ); // get the job from queue and put into CPU
                nQueue[nth].deQueue();
                if ( aJob.timeout <= nStatOfCPU[nth].leavingTime )
                    ansList.addAbortJob(aJob.OID, time, nStatOfCPU[nth].leavingTime - aJob.arrival, nth + 1 );
                else if ( aJob.arrival <= nStatOfCPU[nth].leavingTime ){
                    nStatOfCPU[nth].OID = aJob.OID;
                    nStatOfCPU[nth].startTime = nStatOfCPU[nth].leavingTime;
                    if (nStatOfCPU[nth].leavingTime + aJob.duration <= aJob.timeout) {
                        nStatOfCPU[nth].leavingTime = nStatOfCPU[nth].leavingTime + aJob.duration;
                        ansList.addDoneJob(aJob.OID, nStatOfCPU[nth].leavingTime , nStatOfCPU[nth].startTime - aJob.arrival, nth + 1 );
                    }
                    else {
                        nStatOfCPU[nth].leavingTime = aJob.timeout;
                        ansList.addAbortJob(aJob.OID, aJob.timeout, nStatOfCPU[nth].leavingTime - aJob.arrival, nth + 1 );
                    }
                }
                else { // aJob.arrival > leavingTime
                    nStatOfCPU[nth].OID = aJob.OID;
                    nStatOfCPU[nth].startTime = time;
                    if ( aJob.arrival + aJob.duration <= aJob.timeout) {
                        nStatOfCPU[nth].leavingTime = nStatOfCPU[nth].startTime + aJob.duration;
                        ansList.addDoneJob(aJob.OID, nStatOfCPU[nth].leavingTime , nStatOfCPU[nth].startTime - aJob.arrival, nth + 1 );
                    }
                    else {
                        nStatOfCPU[nth].leavingTime = aJob.timeout;
                        ansList.addAbortJob(aJob.OID, aJob.timeout, nStatOfCPU[nth].leavingTime - aJob.arrival, nth + 1 );
                    }
                }
            }
        }
    }
    void processArrived( int time ) {
        /*
         * First, deal the arrival happened before the time ( enqueue or abort )
         * Second, deal the cpu status at the time ( nStatOfCPU and nQueue )
         * third, deal the arriving job at the time ( arrival == time )
         */

        // enqueue the arrived job before the time
        jobType aJob;
        while ( jobList.getArrivalTime() != -1 && jobList.getArrivalTime() < time ) {
            jobList.getNextJob( aJob ) ;
            jobList.delOneJob();
            updateCurrent( aJob.arrival );
            if ( allFull( time ) ) {
                updateCurrent( aJob.arrival );
                ansList.addAbortJob(aJob.OID, aJob.arrival, 0, 0);
            }
            // choose one enqueue
            else {
                int n = chooseACPU( aJob.arrival );
                nQueue[n].enQueue( aJob );
                // if the job can be process immediately
                // updateCurrent( aJob.arrival );
                /*
                if ( nStatOfCPU[n].leavingTime <= aJob.arrival ) {
                    nQueue[n].deQueue();
                    nStatOfCPU[n].startTime = aJob.arrival;
                    nStatOfCPU[n].leavingTime = aJob.arrival + aJob.duration;
                    ansList.addDoneJob( aJob.OID, nStatOfCPU[n].leavingTime, 0 );
                }
                */
            }
        }

        // update current
        updateCurrent( time );

        while ( jobList.getArrivalTime() == time ) {
            jobList.getNextJob( aJob );
            jobList.delOneJob();

            if (allFull(time)) // all full
                ansList.addAbortJob( aJob.OID, aJob.arrival, 0, 0 );
                // choose one enqueue
            else { // 丟job進queue或CPU?
                int n = chooseACPU(aJob.arrival);
                nQueue[n].enQueue(aJob);
                // if the job can be process immediately
                updateCurrent( aJob.arrival );
            }
        }
        // again check

    }
    void simulate( AnsList& answer ) {
        int numOfJob = jobList.getLength(); // problem size
        int time = 0;
        while ( ansList.getNumOfDone() < numOfJob ) {
            time = getAvailableTime( time );
            // deal with the jobs arrived before the time
            processArrived( time );
            // if no job to do, get a new one

        }

        answer = ansList;

    }

};


int main() {
    int cmd = -1;

    JobList aList;
    do {
        cout << endl << "**** Simulate FIFO Queues by SQF *****";
        cout << endl << "* 0. Quit                            *";
        cout << endl << "* 1. Sort a file                     *";
        cout << endl << "* 2. Simulate one FIFO queue         *";
        cout << endl << "**************************************";
        cout << endl << "Input a command(0, 1, 2, 3): ";
        cin >> cmd;
        string fileName, id;
        if ( cmd == 1 ) {
            fileName.clear();
            aList.reset();
            aList.setID();  // input fileID
            fileName = "input" + aList.getID() +".txt"; // format: inputXXX.txt
            if ( aList.getSorted( fileName ) )
                cout << "\n" ;
            else {
                cout << endl << "### " + fileName + " does not exist! ###";
                continue;
            }

        }
        else if ( cmd == 2 || cmd == 3 ) {
            if ( aList.getID().empty() )
                aList.setID();
            fileName = "sorted" + aList.getID() + ".txt"; // format: sortedXXX.txt

            if ( !aList.getAll( fileName ) ) {
                cout << endl << "### " + fileName + " does not exist! ###";
            }
                // simulate
            else {
                aList.reset();
                aList.getAll( fileName );
                int numOfCpu, sizeOfQueue = 3;
                cout << endl << "The simulation is running...";
                if ( cmd == 2 )
                    numOfCpu = 1;
                else
                    numOfCpu = 2;

                AnsList answer;
                Simulation simulation(aList, numOfCpu, sizeOfQueue); // jobList, numOfCPU, queueSize
                simulation.simulate( answer );
                answer.computeStats();
                if( cmd == 2) {
                    fileName = "output" + aList.getID() + ".txt";
                    answer.putAll2(fileName);
                }
                else {
                    fileName = "double" + aList.getID() + ".txt";
                    answer.putAll3(fileName);
                }
                cout << endl << "See " + fileName;
            }
        }
        else if ( cmd != 0 )
            cout << endl << "Command does not exist!";

    } while ( cmd != 0 ) ;
}