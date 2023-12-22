// 11127139 陳恩
// 11127140 姜義新
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath>

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

    vector<pokemonType> returnVector(){
        return pSet ;
    }
};

class pokemonHeap {

private:
    vector<pokemonType> pSet;

    int calculateHeight(int rootIndex, int size) {
        if (rootIndex >= size) {
            return 0;
        }

        int leftChildIndex = 2 * rootIndex + 1;
        int rightChildIndex = 2 * rootIndex + 2;

        int leftSubtreeHeight = calculateHeight(leftChildIndex, size);
        int rightSubtreeHeight = calculateHeight(rightChildIndex, size);

        return max(leftSubtreeHeight, rightSubtreeHeight) + 1;
    }

    // Helper function to maintain max heap property
    void heapify(int rootIndex, int size) {
        int maxIndex = rootIndex;
        int leftChild = 2 * rootIndex + 1;
        int rightChild = 2 * rootIndex + 2;

        if (leftChild < size &&
            (pSet[leftChild].hp > pSet[maxIndex].hp ||
             (pSet[leftChild].hp == pSet[maxIndex].hp && leftChild < maxIndex))) {
            maxIndex = leftChild;
        }

        if (rightChild < size &&
            (pSet[rightChild].hp > pSet[maxIndex].hp ||
             (pSet[rightChild].hp == pSet[maxIndex].hp && rightChild < maxIndex))) {
            maxIndex = rightChild;
        }

        if (maxIndex != rootIndex) {
            swap(pSet[rootIndex], pSet[maxIndex]);
            heapify(maxIndex, size);
        }
    }


public:
    void reset() {
        this->pSet.clear();
    }

    pokemonHeap() { reset(); }
    ~pokemonHeap() { reset(); }

    void inputPSet( vector<pokemonType> input ) {
        pSet = input ;
    }


    void print() {
        cout << "\n#\tName\tType1\tHP\tAttack\tDefence" << endl;
        for (int i = 0; i < pSet.size(); i++) {
            cout << "\n[" << i + 1 << "]\t";
            cout << pSet[i].no << "\t" << pSet[i].name << "\t" << pSet[i].tp1 << "\t";
            cout << pSet[i].hp << "\t" << pSet[i].atk << "\t" << pSet[i].def;
        }
    }

    // Function to build max heap based on hp
    void buildMaxHeap() {
        int size = pSet.size();

        for (int i = size / 2 - 1; i >= 0; i--) {
            heapify(i, size);
        }
    }

    // Function to print the leftmost node at the bottom of the max heap
    void printLeftmostBottomNode() {
        if (pSet.empty()) {
            cout << "\n### The heap is empty! ###" << endl;
            return;
        }


        for ( int i = 0 ; i < pSet.size(); i++ ) {
            cout << pSet[i].no << "\t" << pSet[i].name << "\t" << pSet[i].tp1 << "\t";
            cout << pSet[i].hp << "\t" << pSet[i].atk << "\t" << pSet[i].def << endl;
        }

        cout << "\nLeftmost Node at the Bottom: ";
        int h = calculateHeight(0, pSet.size()) ;

        cout << pSet[pow(2,h-1) - 1].no << "\t" << pSet[pow(2,h-1) - 1].name << "\t" << pSet[pow(2,h-1) - 1].tp1 << "\t";
        cout << pSet[pow(2,h-1) - 1].hp << "\t" << pSet[pow(2,h-1) - 1].atk << "\t" << pSet[pow(2,h-1) - 1].def << endl;

        cout << pSet[pSet.size()-1].no << "\t" << pSet[pSet.size()-1].name << "\t" << pSet[pSet.size()-1].tp1 << "\t";
        cout << pSet[pSet.size()-1].hp << "\t" << pSet[pSet.size()-1].atk << "\t" << pSet[pSet.size()-1].def << endl;
    }

    int getHeight() {
        cout << calculateHeight(0, pSet.size()) << endl ;
    }
};

int main() {
    int cmd = -1;
    string fileNo;
    pokemonList aList;
    pokemonHeap aheap;
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
            case 2:
                aheap.inputPSet(aList.returnVector()) ;
                aheap.buildMaxHeap();
                aheap.getHeight();
                aheap.printLeftmostBottomNode() ;
                break;
            default:
                cout << "\nCommand does not exist!";
        }

    } while ( cmd != 0);

    return 0;
}