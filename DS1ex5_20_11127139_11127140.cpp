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

class pokemonList {

private:
    vector<pokemonType> pSet;
    string fileID;

public:
    void reset() {
        this->pSet.clear();
        this->fileID.clear();
    }

    pokemonList() { reset(); }
    ~pokemonList() { reset(); }


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
                        case 1: oneR.no = atoi(cut.c_str());break;
                        case 2: oneR.name = cut; break;
                        case 3: oneR.tp1 = cut; break;
                        case 6: oneR.hp = atoi(cut.c_str()); break;
                        case 7: oneR.atk = atoi(cut.c_str()); break;
                        case 8: oneR.def = atoi(cut.c_str()); break;
                        default: break; //end switch
                    }
                    pre = ++pos;
                } while ((pos > 0) && (fNo < 9));
                this->pSet.push_back(oneR); } //end outer-while inFile.close();
        } // end else
        if (!this->pSet.size()){
            cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
            return false;
        } // end if
        return true;
    } // end readFile

    void print() {
        cout << "\n#\tName\tType1\tHP\tAttack\tDefence" << endl;
        for ( int i = 0; i < pSet.size(); i++ ) {
            cout << "\n[" << i+1 << "]\t";
            cout << pSet[i].no << "\t" << pSet[i].name << "\t" << pSet[i].tp1 << "\t";
            cout << pSet[i].hp << "\t" << pSet[i].atk << "\t" << pSet[i].def;
        }
    }
};

int main() {
    int cmd = -1;
    string fileNo;
    pokemonList aList;
    do {
        cout << "\n***** Pokemon Tree and Heap *****"
                "\n* 0. QUIT                       *"
                "\n* 1. Read a file to build BST   *"
                "\n* 2. Transform it into Max Heap *"
                "\n*********************************"
                "\nInput a choice(0, 1, 2): ";
        cin >> cmd;

        switch (cmd) {
            case 0:
                break;
            case 1:
                aList.readFile();
                aList.print();
                break;
            default:
                cout << "\nCommand does not exist!";
        }

    } while ( cmd != 0);

    return 0;
}