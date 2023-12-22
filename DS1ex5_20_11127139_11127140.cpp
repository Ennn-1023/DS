// 11127139 陳恩
// 11127140 姜義新
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

typedef struct pT {
    string rawR; // raw data of one record
    int no; // pokemon number
    string name; // pokemon name
    string tp1; // 1st type
    int tot; // total sum
    int hp; // health point
    int atk; // attack
    int def; // defense
} pokemonType;

class SchoolList {

private:
    vector<pokemonType> pSet;
    string fileID;
public:
    void reset() {
        this->pSet.clear();
        this->fileID.clear();
    }

    SchoolList() { reset(); }
    ~SchoolList() { reset(); }


    bool readFile(){
        fstream inFile;
        string fileName;
        this->reset();
        cout << endl << "Input a file number: ";
        cin >> this->fileID;
        fileName = "input" + fileID + ".txt";
        inFile.open(fileName.c_str(), fstream::in);
        if (!inFile.is_open()){
            cout << endl << "### " << fileName << " does not exist! ###" << endl;
            return false;
        } //end if
        else{
            char cstr[255];
            int fNo, pre, pos;
            inFile.getline(cstr, 255, '\n');
            inFile.getline(cstr, 255, '\n');
            inFile.getline(cstr, 255, '\n');
            while (inFile.getline(cstr, 255, '\n')) {
                pT oneR;
                string buf, cut;
                fNo = 0;
                pre = 0;
                buf.assign(cstr);
                do {
                    pos = buf.find_first_of('\t', pre);
                    cut = buf.substr(pre, pos - pre);
                    switch (++fNo) {
                        case 2: oneR.sname = cut;break;
                        case 4: oneR.dname = cut; break;
                        case 5: oneR.type = cut; break;
                        case 6: oneR.level = cut; break;
                        case 7: oneR.nstud = atoi(cut.c_str()); break;
                        case 8: oneR.nprof = atoi(cut.c_str()); break;
                        case 9: oneR.ngrad = atoi(cut.c_str()); break;
                        default: break; //end switch
                    }
                    pre = ++pos;
                } while ((pos > 0) && (fNo < 10));
                this->pSet.push_back(oneR); } //end outer-while inFile.close();
        } // end else
        if (!this->pSet.size()){
            cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
            return false;
        } // end if
        return true;
    } // end readFile

    void print() {
        cout << "School Information:" << endl;
        cout << setw(15) << "School Name" << setw(15) << "Department" << setw(10) << "Type"
             << setw(10) << "Level" << setw(10) << "Students" << setw(10) << "Professors"
             << setw(10) << "Graduates" << endl;
        for (const auto& record : pSet) {
            cout << setw(15) << record.sname << setw(15) << record.dname << setw(10)
                 << record.type << setw(10) << record.level << setw(10) << record.nstud
                 << setw(10) << record.nprof << setw(10) << record.ngrad << endl;
        }
    }

    vector<pT> returnVector() {
        return pSet ;
    }
};