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
    string rawR; // raw key of one record
    int no; // pokemon number
    string name; // pokemon name
    string tp1; // 1st type
    int tot; // total sum
    int hp; // health point
    int atk; // attack
    int def; // defense
} pokemonType;
class BSTree {
private:
    struct TreeNode {
        int key; // hp
        int index; // key index in list
        TreeNode* left;
        TreeNode* right;

        TreeNode(int value, int idx) : key(value), index(idx), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;



    // Recursive helper function for inserting nodes
    TreeNode* insertRecursive(TreeNode* root, int value, int idx) {
        if (root == nullptr) {
            return new TreeNode(value, idx);
        }

        if (value < root->key) {
            root->left = insertRecursive(root->left, value, idx);
        } else if (value >= root->key) {
            root->right = insertRecursive(root->right, value, idx);
        }

        return root;
    }


    int calculateHeight(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }

        int leftHeight = calculateHeight(root->left);
        int rightHeight = calculateHeight(root->right);


        return 1 + max(leftHeight, rightHeight);
    }

public:
    // Constructor
    BSTree() : root(nullptr) {}
    ~BSTree() {
        TreeNode* root = this->root;
        clear(root);
    }
    void reset() {
        TreeNode* root = this->root;
        clear(root);
        this->root = nullptr;
    }
    void clear( TreeNode* root ) {
        if ( root == nullptr )
            return;
        clear(root->right);
        clear(root->left);
        delete root;
    }
    // Public interface for inserting nodes
    void insert(int value, int idx) {
        root = insertRecursive(root, value, idx);
    }

    int getHeight() {
        return calculateHeight(root);
    }
    int findLeftMost() {
        TreeNode* node = this->root;
        if ( node == nullptr )
            return -1;
        while ( node->left != nullptr ) {
            node = node->left;
        }
        return node->index;
    }
    int findRightMost() {
        TreeNode* node = this->root;
        if ( node == nullptr )
            return -1;
        while ( node->right != nullptr ) {
            node = node->right;
        }
        return node->index;
    }

};

class pokemonList {

private:
    vector<pokemonType> pSet;
    string fileID;
    BSTree aBST;
public:
    void reset() {
        this->pSet.clear();
        this->fileID.clear();
        aBST.reset();
    }

    pokemonList() { reset(); }
    ~pokemonList() { reset(); }

    void buildBST() {
        for ( int i = 0; i < pSet.size(); i++ ) {
            aBST.insert(pSet[i].hp, i);
        }
    }
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
        int leftIdx = aBST.findLeftMost(), rightIdx = aBST.findRightMost();
        cout << "\nLeftmost node:";
        cout << "\n[" << leftIdx+1 << "]\t";
        cout << pSet[leftIdx].no << "\t" << pSet[leftIdx].name << "\t" << pSet[leftIdx].tp1 << "\t";
        cout << pSet[leftIdx].hp << "\t" << pSet[leftIdx].atk << "\t" << pSet[leftIdx].def;
        cout << "\nRightmost node:";
        cout << "\n[" << rightIdx+1 << "]\t";
        cout << pSet[rightIdx].no << "\t" << pSet[rightIdx].name << "\t" << pSet[rightIdx].tp1 << "\t";
        cout << pSet[rightIdx].hp << "\t" << pSet[rightIdx].atk << "\t" << pSet[rightIdx].def;
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
                aList.buildBST();
                aList.print();
                break;
            default:
                cout << "\nCommand does not exist!";
        }

    } while ( cmd != 0);

    return 0;
}