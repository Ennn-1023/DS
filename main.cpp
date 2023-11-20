
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<ctime>

using namespace std;

clock_t  Begin, End;
double duration;

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
            cout << "Error\n";
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
        Begin = clock();
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
        End = clock();
        duration = double(End - Begin) / CLK_TCK;

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

        system("PAUSE");

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
        fileID.clear();
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
        for ( int i = 0; i < curSize; i++ ) {
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
    };
    struct doneType {
        int OID;
        int Departure;
        int Delay;
    };

    vector<abortType> abortList;
    vector<doneType> doneList;
    float avgDelay;
    float successRate;
    int totalDelay;
    void computeStats() {
        int totalJob = abortList.size() + doneList.size();
        avgDelay = float(totalDelay) / float(totalJob);
        successRate = float ( doneList.size() ) / float(totalJob);
    }

public:
    AnsList():avgDelay(0.0), successRate(0.0), totalDelay(0), abortList(0), doneList(0) {
        cout << endl << "abortList: " << abortList.max_size();
        cout << endl << "donetList: " << doneList.max_size();
    }
    ~AnsList() {
        clear();
    }
    void clear() {
        abortList.clear();
        doneList.clear();
    }
    // void showAll();

    void addAbortJob( int OID, int abort, int delay ) {
        abortType newJob{ OID, abort, delay };
        cout << endl << abortList.max_size();
        abortList.push_back( newJob );
        totalDelay += delay;
    }



    void addDoneJob( int OID, int departure, int delay ) {
        doneType newJob{ OID, departure, delay };
        doneList.push_back(newJob);
        totalDelay += delay;
    }

    void putAll( string fileName ) {
        ofstream outFile;
        outFile.open( fileName );
        if ( outFile.is_open() ) {
            // abortList
            outFile << endl << "\t[Abort Jobs]";
            outFile << endl << "\tOID\tAbort\tDelay";
            for ( int i = 0; i < abortList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << abortList[i].OID;
                outFile << "\t" << abortList[i].Abort << "\t" << abortList[i].Delay;
            }
            // doneList
            outFile << endl << "\t[Jobs Done]";
            outFile << endl << "\tOID\tDeparture\tDelay";
            for ( int i = 0; i < doneList.size(); i++ ) {
                outFile << endl << "[" << i+1 << "]\t" << doneList[i].OID;
                outFile << "\t" << doneList[i].Departure << "\t" << doneList[i].Delay;
            }
            outFile << endl << "[Average Delay]\t" << avgDelay << " ms";
            outFile << endl << "[Success Rate]\t" << successRate << " %";

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
    void updateQueue( int time ) {
        // update each cpu to be the status at input time;

        for (int i = 0; i < numOfCPU; i++ ) {

            // if the executing job is finished when time = input time, update CPUType status
            // only if the cpu have job to do.
            while ( time >= nStatOfCPU[i].leavingTime && !nQueue[i].isEmpty() ) {

                // determine the job is done or aborted when there is job in cpu

                if (!nStatOfCPU[i].isFree){ // the CPUType has a job to output
                    if (nStatOfCPU[i].done) // done
                        ansList.addDoneJob(nStatOfCPU[i].OID, nStatOfCPU[i].leavingTime, nStatOfCPU[i].delay);
                    else // abort
                        ansList.addAbortJob(nStatOfCPU[i].OID, nStatOfCPU[i].leavingTime, nStatOfCPU[i].delay);
                    // current job is done
                    nStatOfCPU[i].isFree = true;
                }

                // get a waiting job to process or there is no job to do
                if ( !processNextJob( i ) )
                    nStatOfCPU[i].isFree = true;


            }
        } // end of for, has updated each CPUType
    }
    bool processNextJob( int nthCPU ) {
        // return true when set the CPUType status by a new job successfully
        // otherwise, return false when there is no job in queue to do

        // the waiting job in queue front
        jobType aJob;
        while ( !nQueue[nthCPU].isEmpty() ) {
            nQueue[nthCPU].getFront( aJob );
            // delete the front job from queue
            nQueue[nthCPU].deQueue();
            // timeout when pop the job from queue, abort the job
            if ( aJob.timeout <= nStatOfCPU[nthCPU].leavingTime ) {
                ansList.addAbortJob(aJob.OID, nStatOfCPU[nthCPU].leavingTime, nStatOfCPU[nthCPU].delay );
            }
            else { // process the waiting job
                setCPU(nthCPU, aJob);
                return true; // successfully changed the CPUType status
            }
            // delete the processing job from queue

        }
        // there is no job to be push into CPUType;
        return false;
    }
    void setCPU(int nthCPU, const jobType& newJob ) {
        // set the executing job information, include it will be done or aborted

        nStatOfCPU[nthCPU].OID = newJob.OID;
        nStatOfCPU[nthCPU].isFree = false;

        // set the start time
        if ( nStatOfCPU[nthCPU].leavingTime < newJob.arrival ) // no delay
            nStatOfCPU[nthCPU].startTime = newJob.arrival;
        else // start when previous job is done
            nStatOfCPU[nthCPU].startTime = nStatOfCPU[nthCPU].leavingTime;


        // check the job will be done or aborted when leaving
        nStatOfCPU[nthCPU].leavingTime = nStatOfCPU[nthCPU].startTime + newJob.duration;

        // if the done time exceed timeout, set the leaving time to be the abort time
        if (nStatOfCPU[nthCPU].leavingTime > newJob.timeout) {
            nStatOfCPU[nthCPU].leavingTime = newJob.timeout;
            nStatOfCPU[nthCPU].done = false;
            nStatOfCPU[nthCPU].delay = nStatOfCPU[nthCPU].leavingTime - newJob.arrival;
        }
        else { // done
            nStatOfCPU[nthCPU].done = true;
            nStatOfCPU[nthCPU].delay = nStatOfCPU[nthCPU].leavingTime - (newJob.arrival + newJob.duration);
        }

    }
    bool checkFinishOrNot() {
        // check if there is any job in CPUType
        for ( int i = 0; i < numOfCPU; i++ ) {
            if ( !nStatOfCPU[i].isFree ) {
                return false;
            }
        }
        // all CPUType is free
        return true;
    }
    void finishQueue() {
        // finish all the jobs in queues
        bool done = checkFinishOrNot();
        int time = -1;
        // deal with the remaining jobs in queue

        while ( !done ) {
            // update time
            // choose the fastest time
            for ( int i = 0; i < numOfCPU && time == -1; i++ ) {
                if ( !nStatOfCPU[i].isFree )
                    time = nStatOfCPU[i].leavingTime;
            }
            for (int i = 0; i < numOfCPU; i++) {
                if ( !nStatOfCPU[i].isFree && time < nStatOfCPU[i].leavingTime )
                    time = nStatOfCPU[i].leavingTime;
            }
            // update by the time
            updateQueue( time );

            time = -1;
            done = checkFinishOrNot();
        }

    }

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

    void simulate( AnsList& answer ) {

        int arrival; // the job arriving time
        jobType nextJob; // the job to push into queue.
        bool processed; // whether the job has been processed
        // keep processing the jobs in jobList until it is empty
        while ( !jobList.isEmpty() ) {
            // processed = false;
            jobList.getNextJob( nextJob );
            jobList.delOneJob();
            // according to the next job, which should be processed, to update all CPUType

            updateQueue( nextJob.arrival );

            int nthCpu = chooseOneCPU(nextJob.arrival);
            // choose one queue to push nextJob in.
            // otherwise, if there is no queue available, abort this job
            if ( nthCpu != -1 )
                nQueue[nthCpu].enQueue(nextJob);
            else // abort the job immediately when it arrived
                ansList.addAbortJob(nextJob.OID, nextJob.timeout, 0);
        }

        // finish the job in queues
        // time begin at the next job arrived
        finishQueue();

        answer = ansList;

    }
    int chooseOneCPU(int arrivalTime ) {

        // the first idle CPUType will have a highest priority.
        for (int i = 0; i < numOfCPU; i++ ) {
            if ( nStatOfCPU[i].isFree ) {
                return i;
            }
        }
        // no idle CPUType
        // choose the shortest one
        int shortest = 0; // assume the shortest one is shortest
        for (int i = 1; i < numOfCPU; i++ ) {
            if ( nStatOfCPU[i].isFree ) {
                if ( nQueue[i].length() < nQueue[i].length() ) {
                    shortest = i;
                }
            }
        }
        // the shortest is full
        if ( nQueue[shortest].isFull() )
            return -1;
        else // the shortest CPUType is available
            return shortest;
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
        cout << endl << "Input a command(0, 1, 2): ";
        cin >> cmd;
        string fileName;
        if ( cmd == 1 ) {
            fileName.clear();
            aList.setID();  // input fileID
            fileName = "input" + aList.getID() +".txt"; // format: inputXXX.txt
            if ( aList.getSorted( fileName ) )
                cout << "\n" ;
            else {
                cout << endl << "### " + fileName + " does not exist! ###";
                continue;
            }

        }
        else if ( cmd == 2 ) {
            if ( aList.getID().empty() )
                aList.setID();
            fileName = "sorted" + aList.getID() + ".txt"; // format: sortedXXX.txt
            if ( !aList.getAll( fileName ) ) {
                cout << endl << "### " + fileName + " does not exist! ###";
            }
            // simulate
            else {
                cout << endl << "Simulating...";
                AnsList answer;
                Simulation simulation(aList, 1, 3); // jobList, numOfCPU, queueSize
                simulation.simulate( answer );
                fileName = "output" + aList.getID() + ".txt";
                answer.putAll( fileName );
            }
        }
        else if ( cmd != 0 )
            cout << endl << "Command does not exist!";

    } while ( cmd != 0 ) ;
}