// 11127139 ����
// 11127140 ���q�s
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

typedef struct sT
{
    string sname;
    string dname;
    string type;
    string level;
    int nstud;
    int nprof;
    int ngrad;
} schoolType;

class BinarySearchTreeSname {
private:
    struct TreeNode {
        schoolType data;
        TreeNode* left;
        TreeNode* right;

        TreeNode(const schoolType& value) : data(value), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;
    vector<schoolType> deleteRecords;
private:
    TreeNode* deleteNodeRecursive(TreeNode* root, const string& sname, int& count) {
        if (root == nullptr) {
            return nullptr;
        }

        if (sname.compare(root->data.sname) < 0 ) {
            root->left = deleteNodeRecursive(root->left, sname, count);
        } else if (sname.compare(root->data.sname) >= 0) {
            root->right = deleteNodeRecursive(root->right, sname, count);
        }
        if (sname.compare(root->data.sname) == 0) {
            count++;
            deleteRecords.push_back(root->data);
            // Node with the matching sname found, perform deletion
            if (root->left == nullptr) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children, get the biggest in the left subtree)
            schoolType temp = findMaxNode(root);

            // Copy the inorder successor's data to this node

            root->data = temp;
        }

        return root;
    }
    schoolType findMaxNode(TreeNode* node) {
        TreeNode* pNode = node;
        node = node->left;
        int count = 0;
        while (node->right != nullptr) {
            count++;
            pNode = node;
            node = node->right;
        }
        schoolType temp = node->data;
        if ( count == 0 )
            pNode->left = node->left;
        else
            pNode->right = node->left;
        delete node;
        return temp;
    }
    schoolType findMinNode(TreeNode* node) {
        TreeNode* pNode = node;
        node = node->right;
        int count = 0;
        while (node->left != nullptr) {
            count++;
            pNode = node;
            node = node->left;
        }
        schoolType temp = node->data;
        if ( count == 0 ) // special case, min node is the first node in the right subtree
            pNode->right = node->right;
        else
            pNode->left = node->right;
        delete node;
        return temp;
    }


    // Recursive helper function for inserting nodes
    TreeNode* insertRecursive(TreeNode* root, const schoolType& value) {
        if (root == nullptr) {
            return new TreeNode(value);
        }

        /*
        if ((value.sname.compare(root->data.sname) < 0)){
            root->left = insertRecursive(root->left, value);
        } else if ((value.sname.compare(root->data.sname) >= 0)) {
            root->right = insertRecursive(root->right, value);
        }
        */

        if (value.sname < root->data.sname){
            root->left = insertRecursive(root->left, value);
        } else if (value.sname >= root->data.sname) {
            root->right = insertRecursive(root->right, value);
        }

        return root;
    }

    // Recursive helper function for in-order traversal
    void inorderTraversalRecursive(TreeNode* root) {
        if (root != nullptr) {
            inorderTraversalRecursive(root->left);
            std::cout << root->data.sname << " ";
            inorderTraversalRecursive(root->right);
        }
    }

    void snamePrint(TreeNode* root, string sname) {
        int count = 0;
        while (root != nullptr) {
            if ( sname >= root->data.sname ){
                if ( sname == root->data.sname ){
                    count++;
                    cout << "\n[" << count << "]\t";
                    printItem(root->data);
                }
                root = root->right ;
            }
            else{
                root = root->left ;
            }
        }

        if ( count == 0 ) {
            cout << "\nThere is no match!";
        }

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
    BinarySearchTreeSname() : root(nullptr) {
        deleteRecords.clear();
    }
    ~BinarySearchTreeSname() {
        TreeNode* root = this->root;
        clear(root);
    }

    void deleteNode(const string& sname) {
        int count = 0;
        root = deleteNodeRecursive(root, sname, count);

        // print records
        if ( count == 0 )
            cout << "\nThere is no match!";
        else {
            cout << "\nDeleted records:";
            for ( int i = deleteRecords.size()-1; i > -1; i-- ) {
                cout << "\n[" << deleteRecords.size() - i << "]\t";
                printItem(deleteRecords[i]);
            }
        }
    }

    void reset() {
        TreeNode* root = this->root;
        clear(root);
        this->root = nullptr;
        this->deleteRecords.clear();
    }
    void clear( TreeNode* root ) {
        if ( root == nullptr )
            return;
        clear(root->right);
        clear(root->left);
        delete root;
    }
    // Public interface for inserting nodes
    void insert(const schoolType& value) {
        root = insertRecursive(root, value);
    }

    // Public interface for in-order traversal
    void inorderTraversal() {
        std::cout << "Inorder Traversal: ";
        inorderTraversalRecursive(root);
        std::cout << std::endl;
    }

    void printSname( string sname ) {
        snamePrint(root, sname) ;
    }

    int getHeight() {
        return calculateHeight(root);
    }

    void printItem( const schoolType& item ) {
        cout << item.sname << "\t" << item.dname << "\t" << item.type << "\t" << item.level << "\t";
        cout << item.nstud << "\t" << item.nprof << "\t" << item.ngrad;
    }


};

class BinarySearchTreeGrad {
private:
    struct TreeNode {
        schoolType data;
        TreeNode* left;
        TreeNode* right;

        TreeNode(const schoolType& value) : data(value), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;

    TreeNode* deleteNodeRecursive(TreeNode* root, const string& sname, int& count) {
        if (root == nullptr) {
            return nullptr;
        }

        root->left = deleteNodeRecursive(root->left, sname, count);
        root->right = deleteNodeRecursive(root->right, sname, count);

        if (sname.compare(root->data.sname) == 0) {

            // Node with the matching sname found, perform deletion
            if (root->left == nullptr) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children, get the inorder successor (smallest in the right subtree)
            schoolType temp = findMaxNode(root);

            // Copy the inorder successor's data to this node
            root->data = temp;
        }

        return root;
    }
    schoolType findMaxNode(TreeNode* node) {
        TreeNode* pNode = node;
        node = node->left;
        int count = 0;
        while (node->right != nullptr) {
            count++;
            pNode = node;
            node = node->right;
        }
        schoolType temp = node->data;
        if ( count == 0 ) // special case: max node is the first node in the left subtree
            pNode->left = node->left;
        else
            pNode->right = node->left;
        delete node;
        return temp;
    }

    schoolType findMinNode(TreeNode* node) {
        TreeNode* pNode = node;
        node = node->right;
        int count = 0;
        while (node->left != nullptr) {
            count++;
            pNode = node;
            node = node->left;
        }
        schoolType temp = node->data;
        if ( count == 0 ) // special case, first node in the right subtree
            pNode->right = node->right;
        else
            pNode->left = node->right;
        delete node;
        return temp;
    }

    // Recursive helper function for inserting nodes
    TreeNode* insertRecursive(TreeNode* root, const schoolType& value) {
        if (root == nullptr) {
            return new TreeNode(value);
        }

        if (value.ngrad < root->data.ngrad) {
            root->left = insertRecursive(root->left, value);
        } else if (value.ngrad >= root->data.ngrad) {
            root->right = insertRecursive(root->right, value);
        }

        return root;
    }

    // Recursive helper function for in-order traversal
    void inorderTraversalRecursive(TreeNode* root) {
        if (root != nullptr) {
            inorderTraversalRecursive(root->left);
            std::cout << root->data.ngrad << " ";
            inorderTraversalRecursive(root->right);
        }
    }


    void gradRecursive(TreeNode* root, int gradNum, int& count) {

        if (root != nullptr) {
            if (gradNum <= root->data.ngrad) {
                count++;
                cout << "\n[" << count << "]\t";
                printItem( root->data );
            }
            if ( gradNum < root->data.ngrad ){
                gradRecursive(root->left, gradNum, count);
            }
            gradRecursive(root->right, gradNum, count);
        }

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
    BinarySearchTreeGrad() : root(nullptr) {}
    ~BinarySearchTreeGrad() {
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
    void insert(const schoolType& value) {
        root = insertRecursive(root, value);
    }

    // Public interface for in-order traversal
    void inorderTraversal() {
        std::cout << "Inorder Traversal: ";
        inorderTraversalRecursive(root);
        std::cout << std::endl;
    }
    void deleteNodes(const string& sname) {
        int count = 0;
        root = deleteNodeRecursive(root, sname, count);
    }
    int getHeight() {
        return calculateHeight(root);
    }

    void gradMore( int gradNum ) {
        int count = 0;
        gradRecursive(root, gradNum, count) ;
        if (count == 0)
            cout << "\nThere is no match!";
    }
    void printItem( const schoolType& item ) {
        cout << item.sname << "\t" << item.dname << "\t" << item.type << "\t" << item.level << "\t";
        cout << item.nstud << "\t" << item.nprof << "\t" << item.ngrad;
    }
};

class SchoolList {

private:
    vector<schoolType>  allR;
    string fileID;
public:
    void reset() {
        this->allR.clear();
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
                schoolType oneR;
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
                this->allR.push_back(oneR); } //end outer-while inFile.close();
        } // end else
        if (!this->allR.size()){
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
        for (const auto& record : allR) {
            cout << setw(15) << record.sname << setw(15) << record.dname << setw(10)
                 << record.type << setw(10) << record.level << setw(10) << record.nstud
                 << setw(10) << record.nprof << setw(10) << record.ngrad << endl;
        }
    }

    vector<schoolType> returnVector() {
        return allR ;
    }
};



int main(){

    SchoolList schoolList;
    vector<schoolType> schoolVector ;


    BinarySearchTreeGrad schoolBSTGrad ;
    BinarySearchTreeSname schoolBSTSname ;
    int cmd = -1;
    string sName;
    int n;
    bool dataExist = false;
    do {
        cout << "\n*** University Graduate Information System ***"
                "\n* 0. Quit                                    *"
                "\n* 1. Create Two Binary Search Trees          *"
                "\n* 2. Search by Number of Graduates           *"
                "\n* 3. Search by School Name                   *"
                "\n**********************************************"
                "\nInput a command(0, 1-4): ";
        cin >> cmd;
        switch (cmd) {
            case 0:
                break;
            case 1:
                schoolList.reset();
                dataExist = schoolList.readFile();
                schoolBSTSname.reset();
                schoolBSTGrad.reset();
                if ( !dataExist ) {
                    cout << "\nThere is no data!";
                    break;
                }
                schoolVector = schoolList.returnVector() ;
                for ( int i = 0; i < schoolVector.size(); i++ ) {
                    schoolBSTSname.insert( schoolVector[i] ) ;
                }
                for ( int i = 0; i < schoolVector.size(); i++ ) {
                    schoolBSTGrad.insert( schoolVector[i] ) ;
                }
                cout << endl << "Tree height {School name} = " << schoolBSTSname.getHeight();
                cout << endl << "Tree height {Number of graduates} = " << schoolBSTGrad.getHeight();
                break;
            case 2:
                // search by graduates
                if ( !dataExist ) {
                    cout << "\nPlease choose command 1 first!";
                    break;
                }
                cout << "\nInput the number of graduates: ";
                cin >> n;
                schoolBSTGrad.gradMore(n);
                break;
            case 3:
                if ( !dataExist ) {
                    cout << "\nPlease choose command 1 first!";
                    break;
                }
                cout << "\nInput a school name: ";
                cin >> sName;
                schoolBSTSname.printSname(sName);
                break;
            case 4:
                if (!dataExist) {
                    cout << "\nPlease choose command 1 first!";
                    break;
                }
                cout << "\nInput a school name to delete: ";
                cin >> sName;
                schoolBSTSname.deleteNode(sName);
                schoolBSTGrad.deleteNodes(sName);
                // schoolBSTSname.inorderTraversal() ;
                cout << endl << "Tree height {School name} = " << schoolBSTSname.getHeight();
                cout << endl << "Tree height {Number of graduates} = " << schoolBSTGrad.getHeight();
                break;
            default:
                cout << "\nCommand does not exist!";
                break;
        }
    } while ( cmd != 0 );

}