#include "wpower.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM {UNIFORMINT, UNIFORMREAL};
class Random {
public:
    Random(int min, int max, RANDOM type) {
        m_randGenerator = std::mt19937(10); //fixed seed of 10

        if (type == UNIFORMINT) {
            m_uniIntDist = std::uniform_int_distribution<>(min, max);
        } else {
            m_uniRealDist = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandInt() {
        //generate a random integer between the min and max values
        return m_uniIntDist(m_randGenerator);
    }

    double getRandReal(){
        //generate a random real number between the min and max values, rounded up to two decimal points
        double num = m_uniRealDist(m_randGenerator);
        num = std::ceil(num*100.0)/100.0;
        return num;
    }

private:
    std::uniform_int_distribution<> m_uniIntDist;
    std::uniform_real_distribution<double> m_uniRealDist;
    std::mt19937 m_randGenerator;
};

class Tester {
public:
    bool testAVLBalancedAfterInsert();

    bool testBSTPropertyAfterInsertBST();
    bool testBSTPropertyAfterInsertAVL();
    bool testBSTPropertyAfterInsertSPLAY();

    bool testSplaying();

    bool testSPLAYHeight();

    bool testBSTRemoveNormal();
    bool testBSTRemoveEdge();

    //Test whether the AVL tree is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the AVL property.
    bool testAVLBalancedAfterRemove();

    //Test whether the BST property is preserved after multiple removals from a BST tree and an AVL tree.
    bool testBSTPropertyAfterRemove();

    //Test whether the height values are correct in a BST tree after multiple removals.
    bool testBSTHeightAfterRemove();

    //Test the assignment operator for a normal case.
    bool testAssignmentNormal();

    //test the assignment operator for an error case, e.g. assigning an empty object to an empty object.
    bool testAssignmentError();

private:
    void insertRandMultiple(WirelessPower& powerSystem);
    bool postOrderBalanceCheck(Customer* node);
    int findBalanceFactor(Customer* node);
    int findHeight(Customer* node);
    int maxVal(int num1, int num2);

    bool postOrderBSTCheck(Customer* node);
    bool postOrderHeightCheck(Customer* node);
    bool postOrderCheckKeyAbsent(Customer* node, int key);
};

bool Tester::testAVLBalancedAfterInsert() {
    //test whether the AVL tree is balanced after a large number of insertions
    WirelessPower powerSystem(AVL);
    insertRandMultiple(powerSystem);

    return postOrderBalanceCheck(powerSystem.m_root);
}

void Tester::insertRandMultiple(WirelessPower& powerSystem) {
    Random randIDObject(MINID, MAXID, UNIFORMINT);
    Random randLatObject(MINLAT, MAXLAT, UNIFORMREAL);
    Random randLongObject(MINLONG, MAXLONG, UNIFORMREAL);

    int customerSize = 300;

    for (int i = 0; i < customerSize; i++){
        int randID = randIDObject.getRandInt();
        double randLat = randLatObject.getRandReal();
        double randLong = randLongObject.getRandReal();

        Customer customer(randID, randLat, randLong);
        powerSystem.insert(customer);
    }
}

bool Tester::postOrderBalanceCheck(Customer *node) {
    if (node != nullptr) {
        //visit all nodes and check if the balance factors are valid
        postOrderBalanceCheck(node->m_left);
        postOrderBalanceCheck(node->m_right);

        int balanceFactor = findBalanceFactor(node);
        if (balanceFactor < -1 || balanceFactor > 1) {
            return false;
        }
    }
    return true;
}

int Tester::findBalanceFactor(Customer* node) {
    if (node == nullptr) {
        return 0;
    }
    return findHeight(node->m_left) - findHeight(node->m_right);
}

int Tester::findHeight(Customer* node) {
    if (node == nullptr) {
        //empty tree has a height of -1 by convention
        return -1;
    } else {
        //standard formula to compute height of a node
        return maxVal(findHeight(node->m_left), findHeight(node->m_right)) + 1;
    }
}

int Tester::maxVal(int num1, int num2) {
    //simple function to determine the greater of two integers
    if (num1 > num2) {
        return num1;
    }
    return num2;
}

bool Tester::testBSTPropertyAfterInsertBST() {
    WirelessPower powerSystem(BST);
    insertRandMultiple(powerSystem);

    return postOrderBSTCheck(powerSystem.m_root);
}

bool Tester::testBSTPropertyAfterInsertAVL() {
    WirelessPower powerSystem(AVL);
    insertRandMultiple(powerSystem);

    return postOrderBSTCheck(powerSystem.m_root);
}

bool Tester::testBSTPropertyAfterInsertSPLAY() {
    WirelessPower powerSystem(SPLAY);
    insertRandMultiple(powerSystem);

    return postOrderBSTCheck(powerSystem.m_root);
}

bool Tester::postOrderBSTCheck(Customer *node) {
    if (node != nullptr) {
        //visit all nodes and compare key values
        postOrderBSTCheck(node->m_left);
        postOrderBSTCheck(node->m_right);

        if (node->m_left == nullptr && node->m_right == nullptr) {
            //node has no children
            return true;
        } else if (node->m_right == nullptr) {
            //node has one left child
            if (node->m_left->m_id < node->m_id) {
                return true;
            }
        } else if (node->m_left == nullptr) {
            //node has one right child
            if (node->m_right->m_id > node->m_id) {
                return true;
            }
        } else if (node->m_left != nullptr && node->m_right != nullptr) {
            //node has two children
            if (node->m_left->m_id < node->m_id && node->m_right->m_id > node->m_id) {
                return true;
            }
        }
        //test fails if any nodes violate BST property
        return false;
    }
    //test passes if tree is empty
    return true;
}

bool Tester::testSplaying() {
    WirelessPower powerSystem(SPLAY);

    Random randIDObject(MINID, MAXID, UNIFORMINT);
    Random randLatObject(MINLAT, MAXLAT, UNIFORMREAL);
    Random randLongObject(MINLONG, MAXLONG, UNIFORMREAL);

    int customerSize = 300;

    for (int i = 0; i < customerSize; i++){
        int randID = randIDObject.getRandInt();
        double randLat = randLatObject.getRandReal();
        double randLong = randLongObject.getRandReal();

        Customer customer(randID, randLat, randLong);
        powerSystem.insert(customer);

        //after every insertion, check whether the inserted node is at root
        if (powerSystem.m_root->m_id != customer.m_id) {
            return false;
        }
    }
    //test passes if all inserted nodes were splayed
    return true;
}

bool Tester::testSPLAYHeight() {
    //test whether the height values are correct after a large number of insetions in a SPLAY tree
    WirelessPower powerSystem(SPLAY);
    insertRandMultiple(powerSystem);

    return postOrderHeightCheck(powerSystem.m_root);
}

bool Tester::postOrderHeightCheck(Customer *node) {
    if (node != nullptr) {
        //visit all nodes and check if the balance factors are valid
        postOrderHeightCheck(node->m_left);
        postOrderHeightCheck(node->m_right);

        if (node->m_height != findHeight(node)) {
            return false;
        }
    }
    return true;
}

bool Tester::testBSTRemoveNormal() {
    //insert a large number of nodes
    WirelessPower powerSystem(BST);
    insertRandMultiple(powerSystem);

    //remove root
    int oldRootKey = powerSystem.m_root->m_id;
    powerSystem.remove(oldRootKey);

    return postOrderCheckKeyAbsent(powerSystem.m_root, oldRootKey);
}

bool Tester::postOrderCheckKeyAbsent(Customer *node, int key) {
    if (node != nullptr) {
        //visit all nodes and check if key is present
        postOrderBalanceCheck(node->m_left);
        postOrderBalanceCheck(node->m_right);

        if (node->m_id == key) {
            return false;
        }
    }
    return true;
}

bool Tester::testBSTRemoveEdge() {
    WirelessPower powerSystem(BST);

    Random randIDObject(MINID, MAXID, UNIFORMINT);
    Random randLatObject(MINLAT, MAXLAT, UNIFORMREAL);
    Random randLongObject(MINLONG, MAXLONG, UNIFORMREAL);

    int randID = randIDObject.getRandInt();
    double randLat = randLatObject.getRandReal();
    double randLong = randLongObject.getRandReal();

    //insert one node
    Customer customer(randID, randLat, randLong);
    powerSystem.insert(customer);

    //remove the inserted node
    powerSystem.remove(randID);

    if (powerSystem.m_root != nullptr) {
        //test fails if tree is not empty
        return false;
    }
    return true;
}

int main() {
    Tester tester;

    cout << "\nTesting InsertAVL - check balance factors:" << endl;
    if (tester.testAVLBalancedAfterInsert()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }

    cout << "\nTesting insertBST - check whether BST property is preserved:" << endl;
    if (tester.testBSTPropertyAfterInsertBST()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }
    cout << "Testing insertAVL - check whether BST property is preserved:" << endl;
    if (tester.testBSTPropertyAfterInsertAVL()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }
    cout << "Testing insertSPLAY - check whether BST property is preserved:" << endl;
    if (tester.testBSTPropertyAfterInsertSPLAY()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }

    cout << "\nTesting insertSPLAY - check whether splaying happens correctly:" << endl;
    if (tester.testSplaying()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }

    cout << "\nTesting insertSPLAY - check all heights:" << endl;
    if (tester.testSPLAYHeight()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }

    cout << "\nTesting removeBST (normal) - removes one node from normal tree successfully:" << endl;
    if (tester.testBSTRemoveNormal()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }
    cout << "Testing removeBST (edge) - removes only node from single-node tree successfully:" << endl;
    if (tester.testBSTRemoveEdge()) {
        cout << "\tTest passed!" << endl;
    } else {
        cout << "\t***Test failed!***" << endl;
    }
}