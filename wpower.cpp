// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"

WirelessPower::WirelessPower(TREETYPE type){
    m_type = type;
    m_root = nullptr;
}

void WirelessPower::destroyTree(Customer* customer) {
    //if node is not nullptr
    if (customer) {
        //recursively deallocate memory for left and right subtrees
        destroyTree(customer->m_left);
        destroyTree(customer->m_right);
        delete customer;
    }
}

WirelessPower::~WirelessPower(){
    clear();
}

void WirelessPower::clear(){
    destroyTree(m_root); //deallocate all memory
    m_root = nullptr; //re-initialize empty tree
}

void WirelessPower::insert(const Customer& customer){
    if (m_type == BST) {
        insertBST(customer, m_root); //pass tree by reference
    } else if (m_type == AVL) {
        insertAVL(customer, m_root); //pass tree by reference
    } else {
        insertSPLAY(customer, m_root); //pass tree by reference
    }
}

void WirelessPower::insertBST(const Customer& customer, Customer*& curr){
    if (curr == nullptr) {
        //if past a leaf, allocate memory for new node and insert
        Customer* newCustomer = new Customer(customer);
        curr = newCustomer;
        curr->m_left = curr->m_right = nullptr;
    } else {
        if (customer.m_id < curr->m_id) {
            //traverse left subtree, updating heights along the way
            insertBST(customer, curr->m_left);
            curr->m_height = findHeight(curr);
        } else if (customer.m_id > curr->m_id) {
            //traverse right subtree, updating heights along the way
            insertBST(customer, curr->m_right);
            curr->m_height = findHeight(curr);
        }
    }
}

void WirelessPower::insertAVL(const Customer& customer, Customer*& curr) {
    if (curr == nullptr) {
        //if past a leaf, allocate memory for new node and insert
        Customer* newCustomer = new Customer(customer);
        curr = newCustomer;
        curr->m_left = curr->m_right = nullptr;
    } else {
        if (customer.m_id < curr->m_id) {
            //traverse left subtree, updating heights and balancing along the way
            insertAVL(customer, curr->m_left);
            curr->m_height = findHeight(curr);
            balanceTree(curr);
        } else if (customer.m_id > curr->m_id) {
            //traverse right subtree, updating heights and balancing along the way
            insertAVL(customer, curr->m_right);
            curr->m_height = findHeight(curr);
            balanceTree(curr);
        }
    }
}

void WirelessPower::insertSPLAY(const Customer &customer, Customer *&curr) {
    //If the tree type is SPLAY, after an insertion, we need to splay the inserted node and bring it to the root of
    // the tree while the tree preserves the BST property as well as updating the node heights.

    //TODO
}

int WirelessPower::findHeight(Customer* node) {
    if (node == nullptr) {
        //empty tree has a height of -1 by convention
        return -1;
    } else {
        //standard formula to compute height of a node
        return maxVal(findHeight(node->m_left), findHeight(node->m_right)) + 1;
    }
}

int WirelessPower::maxVal(int num1, int num2) {
    //simple function to determine the greater of two integers
    if (num1 > num2) {
        return num1;
    }
    return num2;
}

int WirelessPower::findBalanceFactor(Customer* node) {
    if (node == nullptr) {
        return 0;
    }
    return findHeight(node->m_left) - findHeight(node->m_right);
}

void WirelessPower::balanceTree(Customer*& curr) {
    int balanceFactor = findBalanceFactor(curr);

    //if balance factor is greater than 1, tree is left-heavy
    if (balanceFactor > 1) {
        if (findBalanceFactor(curr->m_left) >= 0) {
            //if left child's balance factor is also positive or 0, a single right rotation is enough
            rotateRight(curr);
        } else {
            //otherwise, do double rotation (left-right)
            rotateLeft(curr->m_left);
            rotateRight(curr);
        }
    } else if (balanceFactor < -1) {
        //if balance factor is less than -1, tree is right-heavy
        if (findBalanceFactor(curr->m_right) <= 0) {
            //if right child's balance factor is also negative or 0, a single left rotation is enough
            rotateLeft(curr);
        } else {
            //otherwise, do double rotation (right-left)
            rotateRight(curr->m_right);
            rotateLeft(curr);
        }
    }
}

void WirelessPower::rotateLeft(Customer*& curr) {
    Customer* rightChild = curr->m_right;
    Customer* leftOfRightChild = rightChild->m_left;

    //rotate
    rightChild->m_left = curr;
    curr->m_right = leftOfRightChild;

    //update heights
    curr->m_height = findHeight(curr);
    rightChild->m_height = findHeight(rightChild);

    //update tree structure
    curr = rightChild;
}

void WirelessPower::rotateRight(Customer*& curr) {
    Customer* leftChild = curr->m_left;
    Customer* rightOfLeftChild = leftChild->m_right;

    //rotate
    leftChild->m_right = curr;
    curr->m_left = rightOfLeftChild;

    //update heights
    curr->m_height = findHeight(curr);
    leftChild->m_height = findHeight(leftChild);

    //update tree structure
    curr = leftChild;
}

void WirelessPower::remove(int id){
    if (m_type == BST) {
        removeBST(id, m_root);
    } else if (m_type == AVL) {
        removeAVL(id, m_root);
    }
    //if the tree type is SPLAY, the remove function does nothing
}

void WirelessPower::removeBST(int id, Customer*& curr) {
    if (id < curr->m_id) {
        removeBST(id, curr->m_left);

        //update heights
        curr->m_height = findHeight(curr);

    } else if (id > curr->m_id) {
        removeBST(id, curr->m_right);

        //update heights
        curr->m_height = findHeight(curr);

    } else {
        //target node found
        if (curr->m_left == nullptr && curr->m_right == nullptr) {
            //case 1: no children (leaf node)
            delete curr;
            curr = nullptr;
        } else if (curr->m_left == nullptr) {
            //case 2: one right child
            Customer* temp = curr;
            curr = curr->m_right;
            delete temp;
        } else if (curr->m_right == nullptr) {
            //case 3: one left child
            Customer* temp = curr;
            curr = curr->m_left;
            delete temp;
        } else {
            //case 4: two children
            Customer* temp = findMinNode(curr->m_right);

            //transfer all data from minimum node of right subtree to target node
            curr->m_id = temp->m_id;
            curr->m_latitude = temp->m_latitude;
            curr->m_longitude = temp->m_longitude;

            //delete the minimum node of right subtree
            removeBST(temp->m_id, curr->m_right);
        }
    }
}

void WirelessPower::removeAVL(int id, Customer*& curr) {
    //TODO
    if (id < curr->m_id) {
        removeAVL(id, curr->m_left);

        //update heights and balance if necessary
        curr->m_height = findHeight(curr);
        balanceTree(curr);

    } else if (id > curr->m_id) {
        removeAVL(id, curr->m_right);

        //update heights and balance if necessary
        curr->m_height = findHeight(curr);
        balanceTree(curr);

    } else {
        //target node found
        if (curr->m_left == nullptr && curr->m_right == nullptr) {
            //case 1: no children (leaf node)
            delete curr;
            curr = nullptr;
        } else if (curr->m_left == nullptr) {
            //case 2: one right child
            Customer* temp = curr;
            curr = curr->m_right;
            delete temp;
        } else if (curr->m_right == nullptr) {
            //case 3: one left child
            Customer* temp = curr;
            curr = curr->m_left;
            delete temp;
        } else {
            //case 4: two children
            Customer* temp = findMinNode(curr->m_right);

            //transfer all data from minimum node of right subtree to target node
            curr->m_id = temp->m_id;
            curr->m_latitude = temp->m_latitude;
            curr->m_longitude = temp->m_longitude;

            //delete the minimum node of right subtree
            removeAVL(temp->m_id, curr->m_right);
        }
    }
}

Customer* WirelessPower::findMinNode(Customer* node) {
    //returns the minimum node in a tree or subtree
    if (node->m_left == nullptr) {
        return node;
    }
    return findMinNode(node->m_left);
}

TREETYPE WirelessPower::getType() const{
    return m_type;
}

void WirelessPower::setType(TREETYPE type){
    m_type = type;

    if (type == AVL) {
        inOrderBalance(m_root);
    }
}

void WirelessPower::inOrderBalance(Customer*& curr) {
    if (curr != nullptr) {
        inOrderBalance(curr->m_left);
        inOrderBalance(curr->m_right);
        balanceTree(curr);
    }
}

const WirelessPower & WirelessPower::operator=(const WirelessPower & rhs){
    //TODO
}

void WirelessPower::dumpTree() const {dump(m_root);}
void WirelessPower::dump(Customer* customer) const{
    if (customer != nullptr){
        cout << "(";
        dump(customer->m_left); //first visit the left child
        cout << customer->m_id << ":" << customer->m_height;//second visit the node itself
        dump(customer->m_right);//third visit the right child
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Customer & x ) {
    sout << x.m_id << " (" << x.getLatStr() << ", " << x.getLongStr() << ")" << endl;;
    return sout;
}