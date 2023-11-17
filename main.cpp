
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
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
class Joblist {
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

public:
    bool getAll() {
        string fileNo;
        cout << "Input a file number: ";
        cin >> fileNo;
        ifstream in;
        fileID = fileNo;
        string fileName = "input" + fileNo + ".txt";
        in.open(fileName.c_str());
        if ( !in.is_open() )
            return false;

        string temp;
        jobType aJob;
        getline(in, temp);
        char ch;
        while (in >> aJob.OID >> aJob.arrival >> aJob.duration >> aJob.timeout) {
            list.push_back(aJob);
        }

        in.close();
        return true;
    }
    bool getSorted() {
        getAll();
        sortByArrival();
        ofstream outFile;
        string fileName = "sorted" + fileID + ".txt";
        // write file
        outFile.open(fileName.c_str());
        if (!outFile.is_open())
            return false;
        else {
            outFile << "OID\tArrival\tDuration\tTimeOut" << endl;
            for (int i = 0; i < list.size(); i++) {
                outFile << list[i].OID << "\t" << list[i].arrival << "\t";
                outFile << list[i].duration << "\t" << list[i].timeout << endl;
            }
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

int main() {
    int cmd = -1;
    cout << "**** Simulate FIFO Queues by SQF *****" << endl;
    cout << "* 0. Quit                            *" << endl;
    cout << "* 1. Sort a file                     *" << endl;
    cout << "* 2. Simulate one FIFO queue         *" << endl;
    cout << "**************************************" << endl;
    cout << "Input a command(0, 1, 2): ";
    cin >> cmd;
    Joblist aList;
    while ( cmd != 0 ) {
        if ( cmd == 1 ) {

            if ( aList.getSorted() )
                cout << "success\n";
        }
        else if ( cmd == 2 ) {
            aList.getAll();

        }
        else
            cout << "Command does not exist!" << endl;

        cin >> cmd;
    }

}