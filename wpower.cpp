// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"
WirelessPower::WirelessPower(TREETYPE type){
    m_type = type;
    m_root = nullptr;
}
void WirelessPower::destroyTree(Customer* customer) {
    if (customer) {
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
    insertRecursive(customer, m_root);

    if (m_type == BST) {
        //If the tree type is BST, after an insertion, we should update the height for all nodes in the insertion path.

    } else if (m_type == AVL) {
        //If the tree type is AVL, after an insertion, we should update the height of each node in the insertion path as
        // well as check for an imbalance at each node in this path.

    } else {
        //If the tree type is SPLAY, after and insertion, we need to splay the inserted node and bring it to the root of
        // the tree while the tree preserves the BST property as well as updating the node heights.

    }
}
void WirelessPower::insertRecursive(const Customer& customer, Customer*& curr){
    if (curr == nullptr) {
        Customer* newCustomer = new Customer(customer);
        curr = newCustomer;
        curr->m_left = curr->m_right = nullptr;
    } else {
        if (customer.m_id < curr->m_id) {
            insertRecursive(customer, curr->m_left);
        } else if (customer.m_id > curr->m_id) {
            insertRecursive(customer, curr->m_right);
        }
    }
}
int WirelessPower::findHeight(Customer* curr) {
    if (curr == nullptr) {
        return -1;
    } else {
        int leftHeight = findHeight(curr->m_left);
        int rightHeight = findHeight(curr->m_right);
        return max(leftHeight, rightHeight) + 1;
    }
}
int max(int num1, int num2) {
    if (num1 > num2) {
        return num1;
    }
    return num2;
}
void WirelessPower::remove(int id){

}
TREETYPE WirelessPower::getType() const{

}
void WirelessPower::setType(TREETYPE type){

}
const WirelessPower & WirelessPower::operator=(const WirelessPower & rhs){

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