
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
    void reset() {
        list.clear();
        fileID.clear();
    }
    void writeFile( string fileName ) {
        ofstream outFile;
        outFile.open(fileName.c_str());
        if (!outFile.is_open())
            cout << "Error\n";
        else {
            outFile << "OID\tArrival\tDuration\tTimeOut" << endl;

            for (int i = 0; i < list.size(); i++) {
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
        cout << endl << "Reading data: " << readTime << " clocks (" << readTime << ".00 ms).";
        cout << endl << "Sorting data: " << sortTime << " clocks (" <<sortTime << ".00 ms).";
        cout << endl << "Writing data: " << writeTime << " clocks (" << writeTime << ".00 ms).";
        cout << endl;
        cout << endl << "see " + fileName << endl;
        return true;
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
            if ( aList.getID().size() == 0 )
                aList.setID();
            fileName = "sorted" + aList.getID() + ".txt"; // format: sortedXXX.txt
            if ( !aList.getAll( fileName ) ) {
                cout << endl << "### " + fileName + " does not exist! ###";
            }

        }
        else if ( cmd != 0 )
            cout << endl << "Command does not exist!";

    } while ( cmd != 0 ) ;

}