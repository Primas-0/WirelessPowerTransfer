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

    void setSeed(int seed){
        m_randGenerator = std::mt19937(seed);
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

    //Test whether the BST property is preserved after all insertions. (Note: this requires visiting all nodes and comparing key values.)
    bool testBSTPropertyAfterInsert();

    //Test whether the Splay tree performs the splay operations. For example, we can insert multiple nodes in the splay tree and after every insertion we check whether the inserted node is at root and the tree preserves the BST property.
    bool testSPLAY();

    //Test whether the height values are correct after multiple insetions in a Splay tree.
    bool testSPLAYHeight();

    //Test the remove function for a normal case in the BST tree. Trying to remove a node from a tree results in a tree without the node.
    bool testBSTRemoveNormal();

    //Test the remove function for an edge case in the BST tree. In this case the tree has only one node and we remove the node.
    bool testBSTRemoveEdge();

    //Test whether the AVL tree is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the AVL property.
    bool testBalancedAfterRemoveMultiple();

    //Test whether the BST property is preserved after multiple removals from a BST tree and an AVL tree.
    bool testBSTPropertyAfterRemoveMultiple();

    //Test whether the height values are correct in a BST tree after multiple removals.
    bool testBSTHeight();

    //Test the assignment operator for a normal case.
    bool testAssignmentNormal();

    //test the assignment operator for an error case, e.g. assigning an empty object to an empty object.
    bool testAssignmentError();

private:
    bool postOrderHeightCheck(Customer* node);
    int findHeight(Customer* node);
    int maxVal(int num1, int num2);
};

bool Tester::testAVLBalancedAfterInsert() {
    //Test whether the AVL tree is balanced after a decent number of insertions, e.g. 300 insertions. (Note: this
    // requires visiting all nodes and checking the height values are correct.)

    WirelessPower powerSystem(AVL);

    Random randIDObject(MINID, MAXID, UNIFORMINT);
    Random randLatObject(MINLAT, MAXLAT, UNIFORMREAL);
    Random randLongObject(MINLONG, MAXLONG, UNIFORMREAL);

    int customerSize = 300;

    for (int i = 0; i < customerSize; i++){
        int randID = randIDObject.getRandInt();
        int randLat = randLatObject.getRandReal();
        int randLong = randLongObject.getRandReal();

        Customer customer(randID, randLat, randLong);
        powerSystem.insert(customer);
    }

    return postOrderHeightCheck(powerSystem.m_root);
}

bool Tester::postOrderHeightCheck(Customer *node) {
    if (node != nullptr) {
        postOrderHeightCheck(node->m_left);
        postOrderHeightCheck(node->m_right);

        if (node->m_height != findHeight(node)) {
            return false;
        }
        return true;
    }
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

int main() {
    Tester tester;

    cout << "Testing InsertAVL - balancing:" << endl;
    if (tester.testAVLBalancedAfterInsert()) {
        cout << "\tConstructor passed!" << endl;
    } else {
        cout << "\t***Constructor failed!***" << endl;
    }
}