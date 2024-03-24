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
        //If the tree type is AVL, after an insertion, we should update the height of each node in the insertion path as
        // well as check for an imbalance at each node in this path.
        insertAVL(customer, m_root); //pass tree by reference
    } else {
        //If the tree type is SPLAY, after an insertion, we need to splay the inserted node and bring it to the root of
        // the tree while the tree preserves the BST property as well as updating the node heights.
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
        } else {
            //duplicate id is not allowed, return without inserting
            return;
        }
    }
}

void WirelessPower::insertAVL(const Customer& customer, Customer*& curr) {
    //TODO
}

void WirelessPower::insertSPLAY(const Customer &customer, Customer *&curr) {
    //TODO
}

int WirelessPower::findHeight(Customer* customer) {
    if (customer == nullptr) {
        //empty tree has a height of -1 by convention
        return -1;
    } else {
        //standard formula to compute height of a node
        return maxVal(findHeight(customer->m_left), findHeight(customer->m_right)) + 1;
    }
}

int WirelessPower::maxVal(int num1, int num2) {
    //simple function to determine the greater of two integers
    if (num1 > num2) {
        return num1;
    }
    return num2;
}

void WirelessPower::remove(int id){
    if (m_type == BST) {
        //remove and update heights
        removeBST();
    } else if (m_type == AVL) {
        //remove, update heights, and balance
        removeAVL();
    }
    //if the tree type is SPLAY, the remove function does not remove the node
}

void WirelessPower::removeBST() {
    //TODO
}

void WirelessPower::removeAVL() {
    //TODO
}

TREETYPE WirelessPower::getType() const{
    return m_type;
}

void WirelessPower::setType(TREETYPE type){
    m_type = type;

    //TODO
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