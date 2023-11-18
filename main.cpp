
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
        Begin = clock();
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
        End = clock();
        duration = double(End - Begin) / CLK_TCK;

        cout << "Sorting data:" << duration << "ms"<<endl;
    }
    void reset() {
        list.clear();
        fileID.clear();
    }

public:
    JobList() {
        reset();
    }
    ~JobList() {
        reset();
    }

    bool getAll() {
        string fileNo;
        cout << "Input a file number: ";
        cin >> fileNo;
        ifstream in;
        fileID = fileNo;
        string fileName = "input" + fileNo + ".txt";
        Begin = clock();
        in.open(fileName.c_str());

        if (!in.is_open()) {
            // can't find
            cout << "Can't open the file." << endl ;
            return false;
        }


        string temp;
        jobType aJob;
        getline(in, temp);
        char ch;
        while (in >> aJob.OID >> aJob.arrival >> aJob.duration >> aJob.timeout) {
            list.push_back(aJob);
        }

        in.close();
        End = clock();
        duration = double(End - Begin) / CLK_TCK;

        cout << "Reading data:" << duration << "ms"<<endl;
        return true;
    }


    bool getSorted() {
        if (!getAll())
            return false;
        sortByArrival();
        ofstream outFile;
        string fileName = "sorted" + fileID + ".txt";
        // write file
        Begin = clock();
        outFile.open(fileName.c_str());
        if (!outFile.is_open())
            return false;
        else {
            outFile << "OID\tArrival\tDuration\tTimeOut" << endl;
            for (int i = 0; i < list.size(); i++) {
                outFile << list[i].OID << "\t" << list[i].arrival << "\t";
                outFile << list[i].duration << "\t" << list[i].timeout << endl;
            }
            outFile.close();
            End = clock();
            duration = double(End - Begin) / CLK_TCK;

            cout << "Writing data:" << duration << "ms"<<endl;
            return true;
        }
    }
    void show() {
        cout << "OID\tArrival\tDuration\tTimeOut\n";
        for ( int i = 0; i < list.size(); i++ ) {
            cout << list[i].OID << "\t" << list[i].arrival << "\t";
            cout << list[i].duration << "\t" << list[i].timeout << endl;
        }
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
    JobQueue(int size):maxSize(size), front(0), curSize(0) {
        arrQueue = new jobType[maxSize];
        back = maxSize - 1;
    }
    ~JobQueue() {
        clear();
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

int main() {
    int cmd = -1;
    cout << "**** Simulate FIFO Queues by SQF *****" << endl;
    cout << "* 0. Quit                            *" << endl;
    cout << "* 1. Sort a file                     *" << endl;
    cout << "* 2. Simulate one FIFO queue         *" << endl;
    cout << "**************************************" << endl;


    JobList aList;
    do {
        cout << "Input a command(0, 1, 2): ";
        cin >> cmd;

        if ( cmd == 1 ) {

            if ( aList.getSorted() )
                // cout << "success\n";
                cout << "\n" ;
            else {
                continue;
            }

        }
        else if ( cmd == 2 ) {
            aList.getAll();

        }
        else
            cout << "Command does not exist!" << endl;

    } while ( cmd != 0 ) ;

}